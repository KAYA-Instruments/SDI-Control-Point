/******************************************************************************
 * Copyright (C) 2017 Dream Chip Technologies GmbH
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/
/**
 * @file    main.cpp
 *
 * @brief   Application main window
 *
 *****************************************************************************/
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QDockWidget>
#include <QDesktopWidget>
#include <QScrollBar>

#include <ProVideoDevice.h>
#include <infodialog.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <defines.h>

/******************************************************************************
 * Settings
 *****************************************************************************/
#define MAIN_SETTINGS_SECTION_NAME          ( "MAIN" )
#define MAIN_SETTINGS_SYSTEM_PLATFORM       ( "platform" )

/******************************************************************************
 * Ui Settings which are stored in .ini file
 *****************************************************************************/
#define UI_SETTINGS_SECTION_NAME            ( "UI" )
#define UI_SETTING_WIDGET_MODE              ( "widget_mode" )
#define UI_SETTING_SHOW_DEBUG_TERMINAL      ( "show_debug_terminal" )
#define UI_SETTING_ENABLE_CONNECTION_CHECK  ( "enable_connection_check" )

/******************************************************************************
 * MainWindow::MainWindow
 *****************************************************************************/
MainWindow::MainWindow( ConnectDialog * connectDialog, QWidget * parent )
    : QMainWindow( parent )
    , m_ui( new Ui::MainWindow )
    , m_ConnectDlg( nullptr )
    , m_SettingsDlg( nullptr )
    , m_DebugTerminal( nullptr )
    , m_cbxConnectedDevices( nullptr )
    , m_dev ( nullptr )
    , m_resizeTimer()
    , m_checkConnectionTimer()
    , m_ScrollbarsNeeded( false )
    , m_WidgetMode( DctWidgetBox::Normal )
    , m_ShowDebugTerminal( false )
    , m_EnableConnectionCheck( false )
{
    // Create ui
    m_ui->setupUi( this );

    // Setup the connect and setting dialogs
    setConnectDlg(connectDialog);
    setSettingsDlg(new SettingsDialog( this ));

    // Setup the debug terminal
    /* Note: This has to be done after setting the Settings Dialog, because the
     * debug terminal is connected to signals / slots of the settings dialog */
    setDebugTerminal(new DebugTerminal( this ));

    /* GUI has to be locked down during update procedure, also the reconnect timer
     * has to be disabled with the "BootIntoUpdateMode" event and re-enabled with
     * the "ReopenSerialConnection*/
    connect( m_ui->updBox, SIGNAL(LockCurrentTabPage(bool)), this, SLOT(onLockCurrentTabPage(bool)) );
    connect( m_ui->updBox, SIGNAL(BootIntoUpdateMode()), this, SLOT(onBootIntoUpdateMode()) );
    connect( m_ui->updBox, SIGNAL(ReopenSerialConnection()), this, SLOT(onReopenSerialConnection()) );
    
    // Connect toolbar actions
    connect( m_ui->actionConnect        , SIGNAL( triggered() ), this, SLOT( onConnectClicked() ) );
    connect( m_ui->actionSettings       , SIGNAL( triggered() ), this, SLOT( onSettingsClicked() ) );
    connect( m_ui->actionLoadSettings   , SIGNAL( triggered() ), this, SLOT( onLoadSettingsClicked() ) );
    connect( m_ui->actionSaveSettings   , SIGNAL( triggered() ), this, SLOT( onSaveSettingsClicked() ) );
    connect( m_ui->actionSelectSdi1     , SIGNAL( triggered() ), this, SLOT( onSelectSdi1Clicked() ) );
    connect( m_ui->actionSelectSdi2     , SIGNAL( triggered() ), this, SLOT( onSelectSdi2Clicked() ) );
    connect( m_ui->actionCopyChain1To2  , SIGNAL( triggered() ), this, SLOT( onCopyChain1To2Clicked() ) );
    connect( m_ui->actionCopyChain2To1  , SIGNAL( triggered() ), this, SLOT( onCopyChain2To1Clicked() ) );
    connect( m_ui->actionSplitScreen    , SIGNAL( triggered() ), this, SLOT( onSplitScreenClicked() ) );
    connect( m_ui->actionLoadFromFile   , SIGNAL( triggered() ), this, SLOT( onLoadFromFileClicked() ) );
    connect( m_ui->actionSaveToFile     , SIGNAL( triggered() ), this, SLOT( onSaveToFileClicked() ) );
    connect( m_ui->actionBroadcast      , SIGNAL( triggered() ), this, SLOT( onBroadcastClicked() ) );
    connect( m_ui->actionSync           , SIGNAL( triggered() ), this, SLOT( onSyncSettingsClicked()) );

    // Configure the resize timer
    m_resizeTimer.setSingleShot( true );
    connect( &m_resizeTimer, SIGNAL(timeout()), this, SLOT(onResizeMainWindow()) );

    // Configure the check connection timer
    /* The timer will be started as soon as a device has been connected, it
     * will be stopped when the connection is lost. */
    connect( &m_checkConnectionTimer, SIGNAL(timeout()), this, SLOT(onCheckConnection()) );

    // Try to load main window settings from file
    QString m_SettingsFile = QDir::homePath() + "/" + QString(SETTINGS_FILE_NAME);

    if ( fileExists( m_SettingsFile ))
    {
        QSettings settings( m_SettingsFile, QSettings::IniFormat );
        this->loadUiSettings( settings );
        qDebug() << "loaded ui settings from file";
    }
}

/******************************************************************************
 * MainWindow::~MainWindow
 *****************************************************************************/
MainWindow::~MainWindow()
{
    // Save Ui settings
    QString m_SettingsFile = QDir::homePath() + "/" + QString(SETTINGS_FILE_NAME);
    QSettings settings( m_SettingsFile, QSettings::IniFormat );
    saveUiSettings( settings );

    delete m_SettingsDlg;
    delete m_DebugTerminal;
    delete m_ui;
}

/******************************************************************************
 * MainWindow::closeEvent
 *****************************************************************************/
void MainWindow::closeEvent (QCloseEvent *event)
{
    // When the main window closes, disable broadcast mode (this disables the broadcast master)
    emit BroadcastChanged( false );

    event->accept();
}

/******************************************************************************
 * MainWindow::setupUI
 *****************************************************************************/
void MainWindow::setupUI(ProVideoDevice::features deviceFeatures)
{
    //////////////////////////
    // toolbar
    //////////////////////////

    // Remove all actions (including separators) from the toolbar
    foreach ( QAction* action, m_ui->toolBar->actions() )
    {
        m_ui->toolBar->removeAction(action);
    }

    // Add only those actions that are supported by the device
    m_ui->toolBar->addAction(m_ui->actionConnect);
    m_ui->toolBar->addAction(m_ui->actionSettings);

    if (deviceFeatures.hasSystemSaveLoad)
    {
        m_ui->toolBar->addSeparator();
        m_ui->toolBar->addAction(m_ui->actionSaveSettings);
        m_ui->toolBar->addAction(m_ui->actionLoadSettings);
    }
    if (deviceFeatures.hasChainSelection)
    {
        m_ui->toolBar->addSeparator();
        m_ui->toolBar->addAction(m_ui->actionSelectSdi1);
        m_ui->toolBar->addAction(m_ui->actionSelectSdi2);
    }
    if (deviceFeatures.hasSystemCopySettings)
    {
        m_ui->toolBar->addSeparator();
        m_ui->toolBar->addAction(m_ui->actionCopyChain1To2);
        m_ui->toolBar->addAction(m_ui->actionCopyChain2To1);
    }
    if (deviceFeatures.hasIspSplitScreen)
    {
        m_ui->toolBar->addSeparator();
        m_ui->toolBar->addAction(m_ui->actionSplitScreen);
    }

    m_ui->toolBar->addSeparator();
    m_ui->toolBar->addAction(m_ui->actionSaveToFile);
    m_ui->toolBar->addAction(m_ui->actionLoadFromFile);

    // Add a combo box for device selection if the connect dialog has a list
    QVector<ConnectDialog::detectedRS485Device> connectedRS485Devices = m_ConnectDlg->getDetectedRS485Devices();
    int currentRS485DeviceIndex = m_ConnectDlg->getCurrentRs485DeviceIndex();
    if ( !connectedRS485Devices.empty() )
    {
        // Create a new label
        QLabel * lblConnectedDevices = new QLabel( m_ui->toolBar );
        lblConnectedDevices->setText(" Select Device: ");

        // Create new combo box
        QComboBox * cbxConnectedDevices = new QComboBox( m_ui->toolBar );

        // Set size adjust policy to auto adjust to react to changing device names
        cbxConnectedDevices->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );

        // Add device names to the combo box
        for ( int i = 0; i < connectedRS485Devices.count(); i++ )
        {
            cbxConnectedDevices->addItem( QString(" %1 / %2: %3 ").arg(connectedRS485Devices[i].config.dev_addr)
                                                                  .arg(connectedRS485Devices[i].broadcastAddress)
                                                                  .arg(connectedRS485Devices[i].name), i );
        }

        // Set index of the combo box to the currently selected device
        cbxConnectedDevices->setCurrentIndex( currentRS485DeviceIndex );

        // Add seperator and combo box to toolbar
        m_ui->toolBar->addSeparator();
        m_ui->toolBar->addWidget(lblConnectedDevices);
        m_ui->toolBar->addWidget(cbxConnectedDevices);

        // Connect combo box with device changed event
        connect( cbxConnectedDevices, SIGNAL(currentIndexChanged(int)), this, SLOT(onDeviceSelectionChange(int)), Qt::UniqueConnection );

        // Store pointer to combo box in class variable
        m_cbxConnectedDevices = cbxConnectedDevices;
    }
    else
    {
        m_cbxConnectedDevices = nullptr;
    }

    // If the system supports broadcasting and there is at least one RS485 device connected show the broadcast toggle button
    if ( deviceFeatures.hasSystemBroadcast && !connectedRS485Devices.empty() )
    {
        m_ui->toolBar->addSeparator();
        m_ui->toolBar->addAction(m_ui->actionBroadcast);
        m_ui->toolBar->addAction(m_ui->actionSync);
    }

    //////////////////////////
    // widget tabs
    //////////////////////////

    // Remove all Tabs, clear active widget list
    m_activeWidgets.clear();
    int numTabs = m_ui->tabWidget->count();
    for ( int i = 0; i < numTabs; i++ )
    {
        m_ui->tabWidget->removeTab( 0 );
    }

    // Add only those tabs, which are supported by the device, add them to the active widget list
    m_activeWidgets.append(m_ui->inoutBox);
    m_ui->tabWidget->addTab(m_ui->tabInOut, QIcon(":/images/tab/inout.png"), "");
    if (deviceFeatures.hasLensItf)
    {
        m_activeWidgets.append(m_ui->lensDriverBox);
        m_ui->tabWidget->addTab(m_ui->lensDriverBox, QIcon(":/images/tab/lens.png"), "");
    }
    if (deviceFeatures.hasIspItf)
    {
        m_activeWidgets.append(m_ui->blackBox);
        m_ui->tabWidget->addTab(m_ui->tabBlack, QIcon(":/images/tab/black.png"), "");
    }
    if (deviceFeatures.hasIspAutoWhiteBalance || deviceFeatures.hasCprocItf || deviceFeatures.hasIspGain)
    {
        m_activeWidgets.append(m_ui->wbBox);
        m_ui->tabWidget->addTab(m_ui->tabWb, QIcon(":/images/tab/thermometer.png"), "");
    }
    if (deviceFeatures.hasIspFilter)
    {
        m_activeWidgets.append(m_ui->fltBox);
        m_ui->tabWidget->addTab(m_ui->tabFlt, QIcon(":/images/tab/filter.png"), "");
    }
    if (deviceFeatures.hasMccItf)
    {
        m_activeWidgets.append(m_ui->mccEqBox);
        m_ui->tabWidget->addTab(m_ui->tabMcc, QIcon(":/images/tab/piesegmentselect.png"), "");
    }
    if (deviceFeatures.hasKneeItf)
    {
        m_activeWidgets.append(m_ui->kneeBox);
        m_ui->tabWidget->addTab(m_ui->tabKnee, QIcon(":/images/tab/knee.png"), "");
    }
    if (deviceFeatures.hasLutItf)
    {
        m_activeWidgets.append(m_ui->lutBox);
        m_ui->tabWidget->addTab(m_ui->tabGamma, QIcon(":/images/tab/gamma.png"), "");
    }
    if (deviceFeatures.hasDpccItf)
    {
        m_activeWidgets.append(m_ui->dpccBox);
        m_ui->tabWidget->addTab(m_ui->tabDpcc, QIcon(":/images/tab/dpcc.png"), "");
    }
    if (deviceFeatures.hasIspConversion)
    {
        m_activeWidgets.append(m_ui->outBox);
        m_ui->tabWidget->addTab(m_ui->tabOut, QIcon(":/images/tab/out.png"), "");
    }
    m_activeWidgets.append(m_ui->infoBox);
    m_ui->tabWidget->addTab(m_ui->tabInfo, QIcon(":/images/tab/info.png"), "");

    // Only enable the update tab, if the device is connected over RS485 (Bootloader does not support RS232)
    if (deviceFeatures.hasSystemUpdate && m_ConnectDlg->getActiveInterface() == ConnectDialog::Rs485 )
    {
        m_activeWidgets.append(m_ui->updBox);
        m_ui->tabWidget->addTab(m_ui->tabUpdate, QIcon(":/images/tab/update.png"), "");
    }

    // when aec is disabled, exposure, gain and aperture have to be resynced for actual values
    if (deviceFeatures.hasIspItf && deviceFeatures.hasCamItf)
    {
        connect( m_ui->inoutBox, SIGNAL(ResyncRequest()), this, SLOT(onAecResyncRequest()), Qt::UniqueConnection );
    }


    // Enable / disable elements inside the tabs
    // InOut Tab
    m_ui->inoutBox->setCameraSettingsVisible(deviceFeatures.hasCamItf);
    m_ui->inoutBox->setAutoExposureSettingsVisible(deviceFeatures.hasAutoItf);
    m_ui->inoutBox->setLensChadingCorrectionSettingsVisible(deviceFeatures.hasIspLsc);
    m_ui->inoutBox->setApartureVisible(deviceFeatures.hasIrisItf);
    m_ui->inoutBox->setSdi2ModeVisible(deviceFeatures.hasChainSdi2Mode);
    m_ui->inoutBox->setDownscaleModeVisible(deviceFeatures.hasChainDownscale);
    m_ui->inoutBox->setGenLockVisible(deviceFeatures.hasChainGenLock);
    m_ui->inoutBox->setTimeCodeVisible(deviceFeatures.hasChainTimeCode, deviceFeatures.hasChainTimeCodeHold);
    m_ui->inoutBox->setFlipModeVisible(deviceFeatures.hasChainFlipVertical, deviceFeatures.hasChainFlipHorizontal);
    m_ui->inoutBox->setLogModeVisible(deviceFeatures.hasLutItf);
    m_ui->inoutBox->setTestPatternVisible(deviceFeatures.hasOsdTestPattern);
    m_ui->inoutBox->setAudioEnableVisible(deviceFeatures.hasChainAudio);

    // BlackBox Tab
    m_ui->blackBox->setFlareLevelVisible(deviceFeatures.hasIspFlare);
    m_ui->blackBox->setMasterBlackLevelVisible(deviceFeatures.hasIspMasterBlackLevel);

    // WbBox Tab
    m_ui->wbBox->setWhiteBalanceSettingsVisible(deviceFeatures.hasIspAutoWhiteBalance, deviceFeatures.hasIspGain, deviceFeatures.hasIspGreenGain);
    m_ui->wbBox->setColorProcessingSettingsVisible(deviceFeatures.hasCprocItfHue);

    // LutBox Tab
    m_ui->lutBox->setLutBitWidth(deviceFeatures.lutBitWidth);

    // Dpcc Tab
    m_ui->dpccBox->setFullDpccFeatureSetVisible(deviceFeatures.hasDpccFullFeautureSet);
     // Clear old table, otherwise user might see a "your table is not valid" message on device switch
    m_ui->dpccBox->clearDpccTable();

    // Out Tab
    m_ui->outBox->changeSdiStringToHdmi(deviceFeatures.hasChainHdmiSettings);
    m_ui->outBox->setRawModeVisible(deviceFeatures.hasChainRawMode);

    // Info Tab
    m_ui->infoBox->setRuntimeVisible(deviceFeatures.hasSystemRuntime);
    m_ui->infoBox->setFanSettingsVisible(deviceFeatures.hasSystemFan);

    // Settings Dialog
    m_SettingsDlg->setBroadcastSettingsVisible(deviceFeatures.hasRS232Interface);
    m_SettingsDlg->setRS232SettingsVisible(deviceFeatures.hasRS232Interface);

    if ( !m_ScrollbarsNeeded )
    {
        // Resize the scroll area widget (which holds the tab widget) to minimum size
        m_ui->scrollArea->widget()->adjustSize();
        m_ui->scrollArea->widget()->resize( m_ui->scrollArea->widget()->minimumSizeHint() );

        /* Try setting the minimum size of the scroll area to the minimum size of the scroll area
         * widget to avoid showing vertical and horizontal scroll bars */
        m_ui->scrollArea->setMinimumSize( m_ui->scrollArea->widget()->minimumSizeHint());

        /* Check if the screen is large enough to show the GUI, if it is not, set the
         * minimum width / height to 0 again to show scroll bars */
        if ( QApplication::desktop()->availableGeometry().height() < this->minimumSizeHint().height() )
        {
            m_ScrollbarsNeeded = true;
            m_ui->scrollArea->setMinimumHeight( 0 );

            // Set minimum width to include the vertical scroll bar
            m_ui->scrollArea->setMinimumWidth( m_ui->scrollArea->widget()->minimumSizeHint().width() + m_ui->scrollArea->verticalScrollBar()->width() );
        }
        if ( QApplication::desktop()->availableGeometry().width() < this->width() )
        {
            m_ScrollbarsNeeded = true;
            m_ui->scrollArea->setMinimumWidth( 0 );
        }
    }

    // Show or hide advanced settings
    onWidgetModeChange( m_WidgetMode );

    // Resize main window
    onResizeMainWindow( true );
}

/******************************************************************************
 * MainWindow::fileExists
 *****************************************************************************/
bool MainWindow::fileExists( QString & path )
{
    QFileInfo check_file( path );

    // check if file exists and if yes: Is it really a file and no directory?
    return ( check_file.exists() && check_file.isFile() );
}


/******************************************************************************
 * MainWindow::loadUiSettings
 *****************************************************************************/
void MainWindow::loadUiSettings( QSettings &s )
{
    // load Ui settings
    s.beginGroup( UI_SETTINGS_SECTION_NAME );

    // engineering mode
    m_WidgetMode = static_cast<DctWidgetBox::Mode>( s.value( UI_SETTING_WIDGET_MODE, m_WidgetMode ).toInt() );
    m_SettingsDlg->setEngineeringModeChecked( m_WidgetMode == DctWidgetBox::Advanced ? true : false );

    /* Note: Enginering mode enables advanced settings in all visible
     * tabs. This is set at the end of setupUI() by calling onWidgetModeChange(),
     * when the active widgets for the current device have been determined. */

    // debug terminal
    m_ShowDebugTerminal = s.value( UI_SETTING_SHOW_DEBUG_TERMINAL, m_ShowDebugTerminal ).toBool();
    emit setDockWidgetVisible( m_ShowDebugTerminal );

    // connection check
    m_EnableConnectionCheck = s.value( UI_SETTING_ENABLE_CONNECTION_CHECK, m_EnableConnectionCheck ).toBool();
    m_SettingsDlg->setConnectionCheckChecked( m_EnableConnectionCheck );

    /* Note connection check is started at the end of connectToDevice() */

    s.endGroup();
}

/******************************************************************************
 * MainWindow::saveUiSettings
 *****************************************************************************/
void MainWindow::saveUiSettings( QSettings &s )
{
    // save Ui settings
    s.beginGroup( UI_SETTINGS_SECTION_NAME );
    s.setValue( UI_SETTING_WIDGET_MODE, m_WidgetMode );
    s.setValue( UI_SETTING_SHOW_DEBUG_TERMINAL, m_ShowDebugTerminal );
    s.setValue( UI_SETTING_ENABLE_CONNECTION_CHECK, m_EnableConnectionCheck );
    s.endGroup();
}

/******************************************************************************
 * MainWindow::updateDeviceList
 *****************************************************************************/
void MainWindow::updateDeviceList()
{
    // Adjust the devices list
    QVector<ConnectDialog::detectedRS485Device> connectedRS485Devices = m_ConnectDlg->getDetectedRS485Devices();
    int currentRS485DeviceIndex = m_ConnectDlg->getCurrentRs485DeviceIndex();
    if ( !connectedRS485Devices.empty() && m_cbxConnectedDevices != nullptr )
    {
        // Block signals from combo box
        m_cbxConnectedDevices->blockSignals( true );

        // Clear combo box
        m_cbxConnectedDevices->clear();

        // Add entries to combo box
        for (int i = 0; i < connectedRS485Devices.count(); i++ )
        {
            QString broadcastMaster = "";
            if ( connectedRS485Devices[i].isBroadcastMaster )
            {
                broadcastMaster = QString("(Master) ");

            }
            m_cbxConnectedDevices->addItem( QString(" %1 / %2: %3 %4").arg(connectedRS485Devices[i].config.dev_addr)
                                                                      .arg(connectedRS485Devices[i].broadcastAddress)
                                                                      .arg(connectedRS485Devices[i].name)
                                                                      .arg(broadcastMaster), i) ;
        }

        // Set index of the combo box to the currently selected device
        m_cbxConnectedDevices->setCurrentIndex( currentRS485DeviceIndex );

        // Unblock signals
        m_cbxConnectedDevices->blockSignals( false );
    }
}

/******************************************************************************
 * MainWindow::connectToDevice
 *****************************************************************************/
void MainWindow::connectToDevice( ProVideoDevice * dev )
{
    // Show a message box to indicate connection is ongoing
    InfoDialog infoDlg( QString(":/icons/cog-64.png"), QString("Connect Dialog"), QString("Loading Device Settings..."), this->parentWidget() );
    infoDlg.show();

    // sleep for 100ms and process events, this ensures that the message box is correctly shown under linux
    QThread::msleep( 100 );
    QApplication::processEvents(QEventLoop::WaitForMoreEvents);

    // Get the features which are supported by this device
    m_dev = dev;
    ProVideoDevice::features deviceFeatures = dev->getSupportedFeatures();

    // Enable / disable UI elements, as they are supported by the device
    setupUI(deviceFeatures);

    // Connect all needed Slots and Signals according to the device features
    //////////////////////////
    // inout widget
    //////////////////////////
    if (deviceFeatures.hasCamItf)
    {
        // connect camera info
        connect( dev->GetCamItf(), SIGNAL(CameraInfoChanged(int,int,int,int,int)), m_ui->inoutBox, SLOT(onCameraInfoChange(int,int,int,int,int)) );

        // connect camera gain
        connect( dev->GetCamItf(), SIGNAL(CameraGainChanged(int)), m_ui->inoutBox, SLOT(onCameraGainChange(int)) );
        connect( m_ui->inoutBox, SIGNAL(CameraGainChanged(int)), dev->GetCamItf(), SLOT(onCameraGainChange(int)) );

        // connect camera exposure
        connect( dev->GetCamItf(), SIGNAL(CameraExposureChanged(int)), m_ui->inoutBox, SLOT(onCameraExposureChange(int)) );
        connect( m_ui->inoutBox, SIGNAL(CameraExposureChanged(int)), dev->GetCamItf(), SLOT(onCameraExposureChange(int)) );
    }


    if (deviceFeatures.hasIspItf)
    {
        if ( deviceFeatures.hasIspLsc )
        {
            // connect lens shading correction
            connect( dev->GetIspItf(), SIGNAL(LscChanged(QVector<uint>)), m_ui->inoutBox, SLOT(onLscChange(QVector<uint>)) );
            connect( m_ui->inoutBox, SIGNAL(LscChanged(QVector<uint>)), dev->GetIspItf(), SLOT(onLscChange(QVector<uint>)) );
        }
    }

    if (deviceFeatures.hasChainItf)
    {
        // connect video mode
        connect( dev->GetChainItf(), SIGNAL(ChainVideoModeChanged(int)), m_ui->inoutBox, SLOT(onChainVideoModeChange(int)) );
        connect( m_ui->inoutBox, SIGNAL(ChainVideoModeChanged(int)), dev->GetChainItf(), SLOT(onChainVideoModeChange(int)) );

        if (deviceFeatures.hasChainSdi2Mode)
        {
            connect( dev->GetChainItf(), SIGNAL(ChainSdi2ModeChanged(int)), m_ui->inoutBox, SLOT(onChainSdi2ModeChange(int)) );
            connect( m_ui->inoutBox, SIGNAL(ChainSdi2ModeChanged(int)), dev->GetChainItf(), SLOT(onChainSdi2ModeChange(int)) );
        }
        if (deviceFeatures.hasChainDownscale)
        {
            connect( dev->GetChainItf(), SIGNAL(ChainDownscaleModeChanged(int,bool,bool)), m_ui->inoutBox, SLOT(onChainDownscaleModeChange(int,bool,bool)) );
            connect( m_ui->inoutBox, SIGNAL(ChainDownscaleModeChanged(int,bool,bool)), dev->GetChainItf(), SLOT(onChainDownscaleModeChange(int,bool,bool)) );
        }
        if (deviceFeatures.hasChainFlipVertical || deviceFeatures.hasChainFlipHorizontal)
        {
            connect( dev->GetChainItf(), SIGNAL(ChainFlipModeChanged(int)), m_ui->inoutBox, SLOT(onChainFlipModeChange(int)) );
            connect( m_ui->inoutBox, SIGNAL(ChainFlipModeChanged(int)), dev->GetChainItf(), SLOT(onChainFlipModeChange(int)) );
        }
        if (deviceFeatures.hasChainGenLock)
        {
            // connect gen-lock mode
            connect( dev->GetChainItf(), SIGNAL(ChainGenlockModeChanged(int)), m_ui->inoutBox, SLOT(onChainGenlockModeChange(int)) );
            connect( m_ui->inoutBox, SIGNAL(ChainGenlockModeChanged(int)), dev->GetChainItf(), SLOT(onChainGenlockModeChange(int)) );

            connect( dev->GetChainItf(), SIGNAL(ChainGenlockCrosslockChanged(int, int)), m_ui->inoutBox, SLOT(onChainGenlockCrosslockChange(int, int)) );
            connect( m_ui->inoutBox, SIGNAL(ChainGenlockCrosslockChanged(int, int)), dev->GetChainItf(), SLOT(onChainGenlockCrosslockChange(int, int)) );

            connect( dev->GetChainItf(), SIGNAL(ChainGenlockOffsetChanged(int, int)), m_ui->inoutBox, SLOT(onChainGenlockOffsetChange(int, int)) );
            connect( m_ui->inoutBox, SIGNAL(ChainGenlockOffsetChanged(int, int)), dev->GetChainItf(), SLOT(onChainGenlockOffsetChange(int, int)) );

            connect( dev->GetChainItf(), SIGNAL(ChainGenlockTerminationChanged(int)), m_ui->inoutBox, SLOT(onChainGenlockTerminationChange(int)) );
            connect( m_ui->inoutBox, SIGNAL(ChainGenlockTerminationChanged(int)), dev->GetChainItf(), SLOT(onChainGenlockTerminationChange(int)) );
        }
        if (deviceFeatures.hasChainTimeCode)
        {
            // Timecode
            connect( dev->GetChainItf(), SIGNAL(ChainTimecodeChanged(QVector<int>)), m_ui->inoutBox, SLOT(onChainTimecodeChange(QVector<int>)) );
            connect( m_ui->inoutBox, SIGNAL(ChainTimecodeSetChanged(QVector<int>)), dev->GetChainItf(), SLOT(onChainTimecodeChange(QVector<int>)) );

            connect( m_ui->inoutBox, SIGNAL(ChainTimecodeGetRequested()), dev->GetChainItf(), SLOT(onChainTimecodeGetRequest()) );

            connect( dev->GetChainItf(), SIGNAL(ChainTimecodeHoldChanged(bool)), m_ui->inoutBox, SLOT(onChainTimecodeHoldChange(bool)) );
            connect( m_ui->inoutBox, SIGNAL(ChainTimecodeHoldChanged(bool)), dev->GetChainItf(), SLOT(onChainTimecodeHoldChange(bool)) );
        }
        if (deviceFeatures.hasChainAudio)
        {
            connect( dev->GetChainItf(), SIGNAL(ChainAudioEnableChanged(bool)), m_ui->inoutBox, SLOT(onChainAudioEnableChange(bool)) );
            connect( m_ui->inoutBox, SIGNAL(ChainAudioEnableChanged(bool)), dev->GetChainItf(), SLOT(onChainAudioEnableChange(bool)) );
        }
    }

    if (deviceFeatures.hasOsdItf)
    {
        if (deviceFeatures.hasOsdTestPattern)
        {
            connect( dev->GetOsdItf(), SIGNAL(TestPatternChanged(int)), m_ui->inoutBox, SLOT(onOsdTestPatternChange(int)) );
            connect( m_ui->inoutBox, SIGNAL(OsdTestPatternChanged(int)), dev->GetOsdItf(), SLOT(onTestPatternChange(int)) );
        }
    }

    if (deviceFeatures.hasAutoItf)
    {
        // Auto Exposure
        connect( dev->GetAutoItf(), SIGNAL(AecEnableChanged(int)),m_ui->inoutBox, SLOT(onAecEnableChange(int)) );
        connect( m_ui->inoutBox, SIGNAL(AecEnableChanged(int)), dev->GetAutoItf(), SLOT(onAecEnableChange(int)) );

        connect( dev->GetAutoItf(), SIGNAL(AecSetupChanged(QVector<int>)), m_ui->inoutBox, SLOT(onAecSetupChange(QVector<int>)) );
        connect( m_ui->inoutBox, SIGNAL(AecSetupChanged(QVector<int>)), dev->GetAutoItf(), SLOT(onAecSetupChange(QVector<int>)) );

        connect( dev->GetAutoItf(), SIGNAL(AecWeightsChanged(QVector<int>)), m_ui->inoutBox, SLOT(onAecWeightsChange(QVector<int>)) );
        connect( m_ui->inoutBox, SIGNAL(AecWeightChanged(int,int)), dev->GetAutoItf(), SLOT(onAecWeightChange(int,int)) );
    }

    if (deviceFeatures.hasIrisItf)
    {
        // Auto Iris
        connect( dev->GetIrisItf(), SIGNAL(IrisSetupChanged(QVector<int>)), m_ui->inoutBox, SLOT(onIrisSetupChange(QVector<int>)) );
        connect( dev->GetIrisItf(), SIGNAL(IrisAptChanged(int)), m_ui->inoutBox, SLOT(onIrisAptChange(int)) );
        connect( dev->GetIrisItf(), SIGNAL(IrisAptError()), m_ui->inoutBox, SLOT(onIrisAptError()) );

        connect( m_ui->inoutBox, SIGNAL(IrisAptChanged(int)), dev->GetIrisItf(), SLOT(onIrisAptChange(int)) );
    }

    //////////////////////////
    // lensdriver widget
    //////////////////////////

    if (deviceFeatures.hasLensItf)
    {
        // lens settings
        connect( dev->GetLensItf(), SIGNAL(LensSettingsChanged(QVector<int>)), m_ui->lensDriverBox, SLOT(onLensSettingsChange(QVector<int>)) );
        connect( m_ui->lensDriverBox, SIGNAL(LensSettingsChanged(QVector<int>)), dev->GetLensItf(), SLOT(onLensSettingsChange(QVector<int>)) );

        // lens active
        connect( dev->GetLensItf(), SIGNAL(LensActiveChanged(bool)), m_ui->lensDriverBox, SLOT(onLensActiveChange(bool)) );
        connect( m_ui->lensDriverBox, SIGNAL(LensActiveChanged(bool)), dev->GetLensItf(), SLOT(onLensActiveChange(bool)) );
        connect(m_ui->lensDriverBox,SIGNAL(SmallResyncRequest(void)),dev->GetLensItf(),SLOT(onSmallResyncRequest(void)) );

        // lens Focus position
        connect( dev->GetLensItf(), SIGNAL(LensFocusPositionChanged( int ) ), m_ui->lensDriverBox, SLOT( onLensFocusPositionChange( int )) );
        connect( m_ui->lensDriverBox, SIGNAL(LensFocusPositionChanged( int ) ), dev->GetLensItf(), SLOT( onLensFocusPositionChange( int )) );

        // lens Focus Settings
        connect( dev->GetLensItf(), SIGNAL(LensFocusSettingsChanged(QVector<int>)), m_ui->lensDriverBox, SLOT(onLensFocusSettingsChange(QVector<int>)) );
        connect( m_ui->lensDriverBox, SIGNAL(LensFocusSettingsChanged(QVector<int>)), dev->GetLensItf(), SLOT(onLensFocusSettingsChange(QVector<int>)) );

        // lens Zoom position
        connect( dev->GetLensItf(), SIGNAL(LensZoomPositionChanged( int ) ), m_ui->lensDriverBox, SLOT( onLensZoomPositionChange( int )) );
        connect( m_ui->lensDriverBox, SIGNAL(LensZoomPositionChanged( int ) ), dev->GetLensItf(), SLOT( onLensZoomPositionChange( int )) );

        // lens Zoom Settings
        connect( dev->GetLensItf(), SIGNAL(LensZoomSettingsChanged(QVector<int>)), m_ui->lensDriverBox, SLOT(onLensZoomSettingsChange(QVector<int>)) );
        connect( m_ui->lensDriverBox, SIGNAL(LensZoomSettingsChanged(QVector<int>)), dev->GetLensItf(), SLOT(onLensZoomSettingsChange(QVector<int>)) );

        // lens Iris position
        connect( dev->GetLensItf(), SIGNAL(LensIrisPositionChanged( int ) ), m_ui->lensDriverBox, SLOT( onLensIrisPositionChange( int )) );
        connect( m_ui->lensDriverBox, SIGNAL(LensIrisPositionChanged( int ) ), dev->GetLensItf(), SLOT( onLensIrisPositionChange( int )) );

        // lens Iris Settings
        connect( dev->GetLensItf(), SIGNAL(LensIrisSettingsChanged(QVector<int>)), m_ui->lensDriverBox, SLOT(onLensIrisSettingsChange(QVector<int>)) );
        connect( m_ui->lensDriverBox, SIGNAL(LensIrisSettingsChanged(QVector<int>)), dev->GetLensItf(), SLOT(onLensIrisSettingsChange(QVector<int>)) );

        // lens Filter position
        connect( dev->GetLensItf(), SIGNAL(LensFilterPositionChanged( int ) ), m_ui->lensDriverBox, SLOT( onLensFilterPositionChange( int )) );
        connect( m_ui->lensDriverBox, SIGNAL(LensFilterPositionChanged( int ) ), dev->GetLensItf(), SLOT( onLensFilterPositionChange( int )) );

        // lens Filter Settings
        connect( dev->GetLensItf(), SIGNAL(LensFilterSettingsChanged(QVector<int>)), m_ui->lensDriverBox, SLOT(onLensFilterSettingsChange(QVector<int>)) );
        connect( m_ui->lensDriverBox, SIGNAL(LensFilterSettingsChanged(QVector<int>)), dev->GetLensItf(), SLOT(onLensFilterSettingsChange(QVector<int>)) );



    }
    //////////////////////////
    // blacklevel widget
    //////////////////////////
    if (deviceFeatures.hasIspItf)
    {
        // When there is no CamItf, we do net get a bayer pattern input image
        if (deviceFeatures.hasCamItf)
        {
            // connect bayer pattern
            connect( m_ui->inoutBox, SIGNAL(BayerPatternChanged(int)), dev->GetIspItf(), SLOT(onBayerPatternChange(int)) );
            connect( dev->GetIspItf(), SIGNAL(BayerPatternChanged(int)), m_ui->inoutBox, SLOT(onBayerPatternChange(int)) );
        }

        // connect RED black level
        connect( m_ui->blackBox, SIGNAL(RedBlackLevelChanged(int)), dev->GetIspItf(), SLOT(onRedBlackLevelChange(int)) );
        connect( dev->GetIspItf(), SIGNAL(RedBlackLevelChanged(int)), m_ui->blackBox, SLOT(onRedBlackLevelChange(int)) );

        // connect GREEN black level
        connect( m_ui->blackBox, SIGNAL(GreenBlackLevelChanged(int)), dev->GetIspItf(), SLOT(onGreenBlackLevelChange(int)) );
        connect( dev->GetIspItf(), SIGNAL(GreenBlackLevelChanged(int)), m_ui->blackBox, SLOT(onGreenBlackLevelChange(int)) );

        // connect BLUE black level
        connect( m_ui->blackBox, SIGNAL(BlueBlackLevelChanged(int)), dev->GetIspItf(), SLOT(onBlueBlackLevelChange(int)) );
        connect( dev->GetIspItf(), SIGNAL(BlueBlackLevelChanged(int)), m_ui->blackBox, SLOT(onBlueBlackLevelChange(int)) );

        connect( dev->GetIspItf(), SIGNAL(BlackLevelChanged(int,int,int)), m_ui->blackBox, SLOT(onBlackLevelChange(int,int,int)) );

        if (deviceFeatures.hasIspFlare)
        {
            // Flare level
            connect( dev->GetIspItf(), SIGNAL(FlareLevelChanged(int,int,int)), m_ui->blackBox, SLOT(onFlareLevelChange(int,int,int)) );
            connect( m_ui->blackBox, SIGNAL(FlareLevelChanged(int,int,int)), dev->GetIspItf(), SLOT(onFlareLevelChange(int,int,int)) );
        }
        if (deviceFeatures.hasIspMasterBlackLevel)
        {
            // Master black level
            connect( dev->GetIspItf(), SIGNAL(MasterBlackLevelChanged(int,int,int)), m_ui->blackBox, SLOT(onMasterBlackLevelChange(int,int,int)) );
            connect( m_ui->blackBox, SIGNAL(MasterBlackLevelChanged(int,int,int)), dev->GetIspItf(), SLOT(onMasterBlackLevelChange(int,int,int)) );
        }
    }

    //////////////////////////
    // flt widget
    //////////////////////////
    if (deviceFeatures.hasIspFilter)
    {
        connect( dev->GetIspItf(), SIGNAL(FilterEnableChanged(int)), m_ui->fltBox, SLOT(onFilterEnableChange(int)) );
        connect( m_ui->fltBox, SIGNAL(FilterEnableChanged(int)), dev->GetIspItf(), SLOT(onFilterEnableChange(int)) );
        connect( dev->GetIspItf(), SIGNAL(FilterDetailLevelChanged(int)), m_ui->fltBox, SLOT(onFilterDetailLevelChange(int)) );
        connect( m_ui->fltBox, SIGNAL(FilterDetailLevelChanged(int)), dev->GetIspItf(), SLOT(onFilterDetailLevelChange(int)) );

        connect( dev->GetIspItf(), SIGNAL(FilterDenoiseLevelChanged(int)), m_ui->fltBox, SLOT(onFilterDenoiseLevelChange(int)) );
        connect( m_ui->fltBox, SIGNAL(FilterDenoiseLevelChanged(int)), dev->GetIspItf(), SLOT(onFilterDenoiseLevelChange(int)) );

        connect( dev->GetIspItf(), SIGNAL(FilterChanged(int,int,int)), m_ui->fltBox, SLOT(onFilterChange(int,int,int)) );
    }

    //////////////////////////
    // white blance widget
    //////////////////////////
    if (deviceFeatures.hasIspAutoWhiteBalance)
    {
        // connect Continous white balancing
        connect( m_ui->wbBox, SIGNAL(AwbEnableChanged(int)), dev->GetAutoItf(), SLOT(onAwbEnableChange(int)) );
        connect( dev->GetAutoItf(), SIGNAL(AwbEnableChanged(int)), m_ui->wbBox, SLOT(onAwbEnableChange(int)) );

        connect( m_ui->wbBox, SIGNAL(AwbSpeedChanged(int)), dev->GetAutoItf(), SLOT(onAwbSpeedChange(int)) );
        connect( dev->GetAutoItf(), SIGNAL(AwbSpeedChanged(int)), m_ui->wbBox, SLOT(onAwbSpeedChange(int)) );

        // connect Button-Array Box
        connect( dev->GetAutoItf(), SIGNAL(NoWbPresetsChanged(int)), m_ui->wbBox, SLOT(onNoWbPresetsChange(int)) );
        connect( dev->GetAutoItf(), SIGNAL(WbPresetsChanged(int,QString,int)), m_ui->wbBox, SLOT(onWbPresetsChange(int,QString,int)) );
        connect( dev->GetAutoItf(), SIGNAL(StatRGBChanged(int,int,int)), m_ui->wbBox, SLOT(onStatRGBChange(int,int,int)) );
        connect( dev->GetAutoItf(), SIGNAL(StatXYZChanged(int,int,int)), m_ui->wbBox, SLOT(onStatXYZChange(int,int,int)) );
        connect( dev->GetAutoItf(), SIGNAL(ColorXYZChanged(int,int,int,int,int,int,int,int,int)),
                 m_ui->wbBox, SLOT(onColorXYZChange(int,int,int,int,int,int,int,int,int)) );

        connect( m_ui->wbBox, SIGNAL(WbChanged()), dev->GetAutoItf(), SLOT(onWbChange()) );
        connect( m_ui->wbBox, SIGNAL(WbPresetChanged(int)), dev->GetAutoItf(), SLOT(onWbPresetChange(int)) );
        connect( m_ui->wbBox, SIGNAL(StatisticChanged()), dev->GetAutoItf(), SLOT(onNotifyWhiteBalanceUpdate()) );
    }

    if (deviceFeatures.hasIspGain)
    {
        // connect RED gain with Dialog box
        connect( m_ui->wbBox, SIGNAL(RedGainChanged(int)), dev->GetIspItf(), SLOT(onRedGainChange(int)) );
        connect( dev->GetIspItf(), SIGNAL(RedGainChanged(int)), m_ui->wbBox, SLOT(onRedGainChange(int)) );

        // Usually cameras don't have green gain, but some devices (like cameleon image enhancer) have it
        if (deviceFeatures.hasIspGreenGain)
        {
            // connect GREEN gain with Dialog box
            connect( m_ui->wbBox, SIGNAL(GreenGainChanged(int)), dev->GetIspItf(), SLOT(onGreenGainChange(int)) );
            connect( dev->GetIspItf(), SIGNAL(GreenGainChanged(int)), m_ui->wbBox, SLOT(onGreenGainChange(int)) );
        }

        // connect BLUE gain with Dialog box
        connect( m_ui->wbBox, SIGNAL(BlueGainChanged(int)), dev->GetIspItf(), SLOT(onBlueGainChange(int)) );
        connect( dev->GetIspItf(), SIGNAL(BlueGainChanged(int)), m_ui->wbBox, SLOT(onBlueGainChange(int)) );
    }

    if (deviceFeatures.hasCprocItf)
    {
        if ( deviceFeatures.hasCprocItfHue )
        {
            // connect Hue with Dialog box
            connect( m_ui->wbBox, SIGNAL(HueChanged(int)), dev->GetCprocItf(), SLOT(onHueChange(int)) );
            connect( dev->GetCprocItf(), SIGNAL(HueChanged(int)), m_ui->wbBox, SLOT(onHueChange(int)) );
        }

        // connect Saturation with Dialog box
        connect( m_ui->wbBox, SIGNAL(SaturationChanged(int)), dev->GetCprocItf(), SLOT(onSaturationChange(int)) );
        connect( dev->GetCprocItf(), SIGNAL(SaturationChanged(int)), m_ui->wbBox, SLOT(onSaturationChange(int)) );

        // connect Brightness with Dialog box
        connect( m_ui->wbBox, SIGNAL(BrightnessChanged(int)), dev->GetCprocItf(), SLOT(onBrightnessChange(int)) );
        connect( dev->GetCprocItf(), SIGNAL(BrightnessChanged(int)), m_ui->wbBox, SLOT(onBrightnessChange(int)) );

        // connect Contrast with Dialog box
        connect( m_ui->wbBox, SIGNAL(ContrastChanged(int)), dev->GetCprocItf(), SLOT(onContrastChange(int)) );
        connect( dev->GetCprocItf(), SIGNAL(ContrastChanged(int)), m_ui->wbBox, SLOT(onContrastChange(int)) );
    }

    //////////////////////////
    // multi color controller
    //////////////////////////
    if (deviceFeatures.hasMccItf)
    {
        // connect enable
        connect( dev->GetMccItf(), SIGNAL(MccEnableChanged(int)), m_ui->mccEqBox, SLOT(onMccEnableChange(int)) );
        connect( m_ui->mccEqBox, SIGNAL(MccEnableChanged(int)), dev->GetMccItf(), SLOT(onMccEnableChange(int)) );

        // connect operation-mode
        connect( dev->GetMccItf(), SIGNAL(MccOperationModeChanged(int,int)), m_ui->mccEqBox, SLOT(onMccOperationModeChange(int,int)) );
        connect( m_ui->mccEqBox, SIGNAL(MccOperationModeChanged(int,int)), dev->GetMccItf(), SLOT(onMccOperationModeChange(int,int)) );

        // connect phase changes
        connect( dev->GetMccItf(), SIGNAL(MccPhaseChanged(int,int,int)), m_ui->mccEqBox, SLOT(onMccPhaseChange(int,int,int)) );
        connect( m_ui->mccEqBox, SIGNAL(MccPhaseChanged(int,int,int)), dev->GetMccItf(), SLOT(onMccPhaseChange(int,int,int)) );
        connect( m_ui->mccEqBox, SIGNAL(MccPhaseIndexChanged(int)), dev->GetMccItf(), SLOT(onMccPhaseSelectionChange(int)) );
    }

    //////////////////////////
    // knee function
    //////////////////////////
    if (deviceFeatures.hasKneeItf)
    {
        connect( dev->GetKneeItf(), SIGNAL(KneeConfigChanged(int,int,int,int)), m_ui->kneeBox, SLOT(onKneeConfigChange(int,int,int,int)) );
        connect( m_ui->kneeBox, SIGNAL(KneeConfigChanged(int,int,int,int)), dev->GetKneeItf(), SLOT(onKneeConfigChange(int,int,int,int)) );
    }

    //////////////////////////
    // Gamma LUT
    //////////////////////////
    if (deviceFeatures.hasLutItf)
    {
        connect( dev->GetLutItf(), SIGNAL(LutEnableChanged(int,int)), m_ui->lutBox, SLOT(onLutEnableChange(int,int)) );
        connect( m_ui->lutBox, SIGNAL(LutEnableChanged(int,int)), dev->GetLutItf(), SLOT(onLutEnableChange(int,int)) );

        connect( dev->GetLutItf(), SIGNAL(LutModeChanged(int)), m_ui->lutBox, SLOT(onLutModeChange(int)) );
        connect( m_ui->lutBox, SIGNAL(LutModeChanged(int)), dev->GetLutItf(), SLOT(onLutModeChange(int)) );

        connect( dev->GetLutItf(), SIGNAL(LutFixedModeChanged(int)), m_ui->lutBox, SLOT(onLutFixedModeChange(int)) );
        connect( m_ui->lutBox, SIGNAL(LutFixedModeChanged(int)), dev->GetLutItf(), SLOT(onLutFixedModeChange(int)) );

        connect( dev->GetLutItf(), SIGNAL(LogModeChanged(int)), m_ui->inoutBox, SLOT(onLogModeChange(int)) );
        connect( m_ui->inoutBox, SIGNAL(LogModeChanged(int)), dev->GetLutItf(), SLOT(onLogModeChange(int)) );
        connect( dev->GetLutItf(), SIGNAL(LogModeChanged(int)), m_ui->lutBox, SLOT(onLogModeChange(int)) );
        connect( m_ui->inoutBox, SIGNAL(LogModeChanged(int)),  m_ui->lutBox, SLOT(onLogModeChange(int)) );

        connect( dev->GetLutItf(), SIGNAL(LutPresetChanged(int)), m_ui->lutBox, SLOT(onLutPresetChange(int)) );
        connect( m_ui->lutBox, SIGNAL(LutPresetChanged(int)), dev->GetLutItf(), SLOT(onLutPresetChange(int)) );

        connect( dev->GetLutItf(), SIGNAL(LutSampleValuesRedChanged(QVector<int>,QVector<int>)),
                 m_ui->lutBox, SLOT(onLutSampleValuesRedChange(QVector<int>,QVector<int>)) );
        connect( dev->GetLutItf(), SIGNAL(LutSampleValuesGreenChanged(QVector<int>,QVector<int>)),
                 m_ui->lutBox, SLOT(onLutSampleValuesGreenChange(QVector<int>,QVector<int>)) );
        connect( dev->GetLutItf(), SIGNAL(LutSampleValuesBlueChanged(QVector<int>,QVector<int>)),
                 m_ui->lutBox, SLOT(onLutSampleValuesBlueChange(QVector<int>,QVector<int>)) );
        connect( dev->GetLutItf(), SIGNAL(LutSampleValuesMasterChanged(QVector<int>,QVector<int>)),
                 m_ui->lutBox, SLOT(onLutSampleValuesMasterChange(QVector<int>,QVector<int>)) );

        connect( m_ui->lutBox, SIGNAL(LutSampleValuesChanged(QVector<int>, QVector<int>)),
                 dev->GetLutItf(), SLOT(onLutSampleValuesChange(QVector<int>, QVector<int>)) );
        connect( m_ui->lutBox, SIGNAL(LutSampleValuesRedChanged(QVector<int>, QVector<int>)),
                 dev->GetLutItf(), SLOT(onLutSampleValuesRedChange(QVector<int>, QVector<int>)) );
        connect( m_ui->lutBox, SIGNAL(LutSampleValuesGreenChanged(QVector<int>, QVector<int>)),
                 dev->GetLutItf(), SLOT(onLutSampleValuesGreenChange(QVector<int>, QVector<int>)) );
        connect( m_ui->lutBox, SIGNAL(LutSampleValuesBlueChanged(QVector<int>, QVector<int>)),
                 dev->GetLutItf(), SLOT(onLutSampleValuesBlueChange(QVector<int>, QVector<int>)) );
        connect( m_ui->lutBox, SIGNAL(LutSampleValuesMasterChanged(QVector<int>, QVector<int>)),
                 dev->GetLutItf(), SLOT(onLutSampleValuesMasterChange(QVector<int>, QVector<int>)) );

        connect( m_ui->lutBox, SIGNAL(LutRec709Changed(int,int,int,int,int,int)),
                 dev->GetLutItf(), SLOT(onLutRec709Change(int,int,int,int,int,int)) );

        connect( m_ui->lutBox, SIGNAL(LutResetChanged()), dev->GetLutItf(), SLOT(onLutReset()) );
        connect( m_ui->lutBox, SIGNAL(LutResetRedChanged()), dev->GetLutItf(), SLOT(onLutResetRed()) );
        connect( m_ui->lutBox, SIGNAL(LutResetGreenChanged()), dev->GetLutItf(), SLOT(onLutResetGreen()) );
        connect( m_ui->lutBox, SIGNAL(LutResetBlueChanged()), dev->GetLutItf(), SLOT(onLutResetBlue()) );
        connect( m_ui->lutBox, SIGNAL(LutResetMasterChanged()), dev->GetLutItf(), SLOT(onLutResetMaster()) );

        connect( m_ui->lutBox, SIGNAL(LutInterpolateChanged()), dev->GetLutItf(), SLOT(onLutInterpolate()) );
        connect( m_ui->lutBox, SIGNAL(LutInterpolateRedChanged()), dev->GetLutItf(), SLOT(onLutInterpolateRed()) );
        connect( m_ui->lutBox, SIGNAL(LutInterpolateGreenChanged()), dev->GetLutItf(), SLOT(onLutInterpolateGreen()) );
        connect( m_ui->lutBox, SIGNAL(LutInterpolateBlueChanged()), dev->GetLutItf(), SLOT(onLutInterpolateBlue()) );

        connect( m_ui->lutBox, SIGNAL(LutSampleValuesRedRequested()),
                 dev->GetLutItf(), SLOT(onLutSampleValuesRedRequest()) );
        connect( m_ui->lutBox, SIGNAL(LutSampleValuesGreenRequested()),
                 dev->GetLutItf(), SLOT(onLutSampleValuesGreenRequest()) );
        connect( m_ui->lutBox, SIGNAL(LutSampleValuesBlueRequested()),
                 dev->GetLutItf(), SLOT(onLutSampleValuesBlueRequest()) );

        connect( dev->GetLutItf(), SIGNAL(LutFastGammaChanged(int)), m_ui->lutBox, SLOT(onLutFastGammaChange(int)) );
        connect( m_ui->lutBox, SIGNAL(LutFastGammaChanged(int)), dev->GetLutItf(), SLOT(onLutFastGammaChange(int)) );
    }

    //////////////////////////
    // dpcc
    //////////////////////////
    if (deviceFeatures.hasDpccItf)
    {
        // enable
        connect( dev->GetDpccItf(), SIGNAL(DpccEnableChanged(int)), m_ui->dpccBox, SLOT(onDpccEnableChanged(int)) );
        connect( m_ui->dpccBox, SIGNAL(DpccEnableChanged(int)), dev->GetDpccItf(), SLOT(onDpccEnableChange(int)) );

        // test mode
        connect( dev->GetDpccItf(), SIGNAL(DpccTestModeChanged(int)), m_ui->dpccBox, SLOT(onDpccTestModeChanged(int)) );
        connect( m_ui->dpccBox, SIGNAL(DpccTestModeChanged(int)), dev->GetDpccItf(), SLOT(onDpccTestModeChange(int)) );

        // Automatically load / fill the dpcc table
        connect( m_ui->dpccBox, SIGNAL(DpccAutoLoadTable()), dev->GetDpccItf(), SLOT(onDpccAutoLoadTable()) );

        // Full dpcc feature set available
        if (deviceFeatures.hasDpccFullFeautureSet)
        {
            // correction mode
            connect( dev->GetDpccItf(), SIGNAL(DpccCorrectionModeChanged(int)), m_ui->dpccBox, SLOT(onDpccCorrectionModeChanged(int)) );
            connect( m_ui->dpccBox, SIGNAL(DpccCorrectionModeChanged(int)), dev->GetDpccItf(), SLOT(onDpccCorrectionModeChange(int)) );

            // detection level
            connect( dev->GetDpccItf(), SIGNAL(DpccDetectionLevelChanged(int)), m_ui->dpccBox, SLOT(onDpccDetectionLevelChanged(int)) );
            connect( m_ui->dpccBox, SIGNAL(DpccDetectionLevelChanged(int)), dev->GetDpccItf(), SLOT(onDpccDetectionLevelChange(int)) );

            // Store / Restore in camera Flash memory
            connect( m_ui->dpccBox, SIGNAL(DpccStoreTableInFlash()), dev->GetDpccItf(), SLOT(onDpccSaveTable()) );
            connect( m_ui->dpccBox, SIGNAL(DpccRestoreTableFromFlash()), dev->GetDpccItf(), SLOT(onDpccLoadTable()) );

            // position table
            connect( dev->GetDpccItf(), SIGNAL(DpccTableChanged(QVector<int>,QVector<int>)), m_ui->dpccBox, SLOT(onDpccTableFromCameraLoaded(QVector<int>,QVector<int>)) );
            connect( m_ui->dpccBox, SIGNAL(DpccLoadTableFromRam()), dev->GetDpccItf(), SLOT(onDpccGetTable()) );
            connect( m_ui->dpccBox, SIGNAL(DpccWriteTableToRam(QVector<int>&,QVector<int>&)), dev->GetDpccItf(), SLOT(onDpccSetTable(QVector<int>&,QVector<int>&)) );
        }

        // video mode
        connect( dev->GetChainItf(), SIGNAL(ChainVideoModeChanged(int)), m_ui->dpccBox, SLOT(onDpccVideoModeChanged(int)) );
        connect( m_ui->inoutBox, SIGNAL(ChainVideoModeChanged(int)), m_ui->dpccBox, SLOT(onDpccVideoModeChanged(int)) );
    }

    //////////////////////////
    // out
    //////////////////////////
    if (deviceFeatures.hasIspConversion)
    {
        // conversion matrix
        connect( dev->GetIspItf(), SIGNAL(ColorConversionMatrixChanged(int,int,int,int,int,int,int,int,int)),
                 m_ui->outBox, SLOT(onColorConversionMatrixChange(int,int,int,int,int,int,int,int,int)) );
        connect( m_ui->outBox, SIGNAL(ColorConversionMatrixChanged(int,int,int,int,int,int,int,int,int)),
                 dev->GetIspItf(), SLOT(onColorConversionMatrixChange(int,int,int,int,int,int,int,int,int)) );
    }

    if ( deviceFeatures.hasChainSdiSettings || deviceFeatures.hasChainHdmiSettings )
    {
        // sdi range mode
        connect( dev->GetChainItf(), SIGNAL(ChainSdiModeChanged(int)), m_ui->outBox, SLOT(onSdiModeChange(int)) );
        connect( m_ui->outBox, SIGNAL(SdiModeChanged(int)), dev->GetChainItf(), SLOT(onChainSdiModeChange(int)) );

        // sdi black
        connect( dev->GetChainItf(), SIGNAL(ChainSdiBlackLevelChanged(int)), m_ui->outBox, SLOT(onSdiBlackChange(int)) );
        connect( m_ui->outBox, SIGNAL(SdiBlackChanged(int)), dev->GetChainItf(), SLOT(onChainSdiBlackLevelChange(int)) );

        // sdi white
        connect( dev->GetChainItf(), SIGNAL(ChainSdiWhiteLevelChanged(int)), m_ui->outBox, SLOT(onSdiWhiteChange(int)) );
        connect( m_ui->outBox, SIGNAL(SdiWhiteChanged(int)), dev->GetChainItf(), SLOT(onChainSdiWhiteLevelChange(int)) );
    }

    if ( deviceFeatures.hasChainRawMode )
    {
        // raw mode
        connect( dev->GetChainItf(), SIGNAL(ChainRawModeChanged(int)), m_ui->outBox, SLOT(onRawModeChange(int)) );
        connect( m_ui->outBox, SIGNAL(RawModeChanged(int)), dev->GetChainItf(), SLOT(onChainRawModeChange(int)) );
    }

    //////////////////////////
    // info 
    //////////////////////////
    // connect system interface slots
    connect( dev->GetProVideoSystemItf(), SIGNAL(DeviceNameChanged(QString)), m_ui->infoBox, SLOT(onDeviceNameChange(QString)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(SystemPlatformChanged(QString)), m_ui->infoBox, SLOT(onSystemPlatformChange(QString)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(DeviceIdChanged(uint32_t,uint32_t,uint32_t,uint32_t)), m_ui->infoBox, SLOT(onDeviceIdChange(uint32_t,uint32_t,uint32_t,uint32_t)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(SystemValidityChanged(QString)), m_ui->infoBox, SLOT(onSystemValidityChange(QString)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(BitStreamVersionChanged(uint32_t)), m_ui->infoBox, SLOT(onBitStreamVersionChange(uint32_t)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(ApplicationVersionChanged(QString)), m_ui->infoBox, SLOT(onApplicationVersionChange(QString)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(ApplicationReleaseDateChanged(QString)), m_ui->infoBox, SLOT(onApplicationReleaseDateChange(QString)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(ApplicationBuildDateChanged(QString)), m_ui->infoBox, SLOT(onApplicationBuildDateChange(QString)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(BootloaderVersionChanged(uint32_t)), m_ui->infoBox, SLOT(onBootloaderVersionChange(uint32_t)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(FeatureMaskHwChanged(uint32_t)), m_ui->infoBox, SLOT(onFeatureMaskHwChange(uint32_t)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(FeatureMaskHwListChanged(QStringList)), m_ui->infoBox, SLOT(onFeatureMaskHwListChange(QStringList)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(FeatureMaskSwChanged(uint32_t)), m_ui->infoBox, SLOT(onFeatureMaskSwChange(uint32_t)) );

    // connect temperature info
    connect( dev->GetProVideoSystemItf(), SIGNAL(TempChanged(uint8_t,float,QString)), m_ui->infoBox, SLOT(onTempChange(uint8_t,float,QString)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(MaxTempChanged(int32_t, int32_t, int32_t)), m_ui->infoBox, SLOT(onMaxTempChange(int32_t, int32_t, int32_t)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(OverTempCountChanged(uint32_t)), m_ui->infoBox, SLOT(onOverTempCountChange(uint32_t)) );

    if ( deviceFeatures.hasSystemFan )
    {
        // fan speed and target
        connect( dev->GetProVideoSystemItf(), SIGNAL(FanSpeedChanged(uint8_t)), m_ui->infoBox, SLOT(onFanSpeedChange(uint8_t)) );
        connect( dev->GetProVideoSystemItf(), SIGNAL(FanTargetChanged(uint8_t)), m_ui->infoBox, SLOT(onFanTargetChange(uint8_t)) );
        connect( m_ui->infoBox, SIGNAL(FanTargetChanged(uint8_t)), dev->GetProVideoSystemItf(), SLOT(onFanTargetChange(uint8_t)) );
    }

    connect( m_ui->infoBox, SIGNAL(GetRunTimeRequest()), dev->GetProVideoSystemItf(), SLOT(onGetRunTimeRequest()) );
    connect( m_ui->infoBox, SIGNAL(GetTempRequest(uint8_t)), dev->GetProVideoSystemItf(), SLOT(onGetTempRequest(uint8_t)) );
    connect( m_ui->infoBox, SIGNAL(GetMaxTempRequest()), dev->GetProVideoSystemItf(), SLOT(onGetMaxTempRequest()) );
    connect( m_ui->infoBox, SIGNAL(GetFanSpeedRequest()), dev->GetProVideoSystemItf(), SLOT(onGetFanSpeedRequest()) );
    connect( m_ui->infoBox, SIGNAL(GetOverTempCountRequest()), dev->GetProVideoSystemItf(), SLOT(onGetOverTempCountRequest()) );
    connect( m_ui->infoBox, SIGNAL(MaxTempReset()), dev->GetProVideoSystemItf(), SLOT(onMaxTempReset()) );

    m_ui->infoBox->setNumTempSensors( deviceFeatures.numTempSensors );

    // connect system runtime
    if (deviceFeatures.hasSystemRuntime)
    {
        connect( dev->GetProVideoSystemItf(), SIGNAL(RunTimeChanged(uint32_t)), m_ui->infoBox, SLOT(onRunTimeChange(uint32_t)) );
    }

    //////////////////////////
    // update
    //////////////////////////
    if (deviceFeatures.hasSystemUpdate)
    {
        // connect update interface slots
        connect( dev->GetProVideoSystemItf(), SIGNAL(SystemPlatformChanged(QString)), m_ui->updBox, SLOT(onSystemPlatformChange(QString)) );
        connect( dev->GetProVideoSystemItf(), SIGNAL(PromptChanged(uint8_t)), m_ui->updBox, SLOT(onPromptChange(uint8_t)) );
        connect( dev->GetProVideoSystemItf(), SIGNAL(ApplicationVersionChanged(QString)), m_ui->updBox, SLOT(onApplicationVersionChange(QString)) );
        connect( m_ui->updBox, SIGNAL(BootIntoUpdateMode()), dev->GetProVideoSystemItf(), SLOT(onBootIntoUpdateMode()) );

        m_ui->updBox->setPortname( m_ConnectDlg->getActiveChannel()->getSystemPortName() );
        m_ui->updBox->setBaudrate( m_ConnectDlg->getActiveChannel()->getBaudRate() );
    }
    
    //////////////////////////
    // toolbar
    //////////////////////////
    if (deviceFeatures.hasSystemCopySettings)
    {
        // copy settings
        connect( this, SIGNAL(CopySettings(int,int)), dev->GetProVideoSystemItf(), SLOT(onCopySettings(int,int)) );
    }
    if (deviceFeatures.hasChainSelection)
    {
        // chain selection
        connect( dev->GetChainItf(), SIGNAL(ChainSelectedChainChanged(int)), this, SLOT(onSdiOutChange(int)) );
        connect( this, SIGNAL(SdiOutChanged(int)), dev->GetChainItf(), SLOT(onChainSelectedChainChange(int)) );

        // the lut box and out box also need to know the current chain, because each chain has its own lut / out settings
        connect( dev->GetChainItf(), SIGNAL(ChainSelectedChainChanged(int)), m_ui->lutBox, SLOT(onSdiOutChange(int)) );
        connect( dev->GetChainItf(), SIGNAL(ChainSelectedChainChanged(int)), m_ui->outBox, SLOT(onSdiOutChange(int)) );
    }
    if (deviceFeatures.hasIspSplitScreen)
    {
        // isp split screen
        connect( dev->GetIspItf(), SIGNAL(SplitScreenChanged(bool)), this, SLOT(onSplitScreenChange(bool)) );
        connect( this, SIGNAL(SplitScreenChanged(bool)), dev->GetIspItf(), SLOT(onSplitScreenChange(bool)) );
    }
    if (deviceFeatures.hasSystemSaveLoad)
    {
        connect( this, SIGNAL(SaveSettings()), dev->GetProVideoSystemItf(), SLOT(onSaveSettings()) );
        connect( this, SIGNAL(LoadSettings()), dev->GetProVideoSystemItf(), SLOT(onLoadSettings()) );
    }
    if (deviceFeatures.hasSystemBroadcast)
    {
        connect( dev->GetProVideoSystemItf(), SIGNAL(RS485BroadcastMasterChanged(uint8_t)), this, SLOT(onBroadcastChange(uint8_t)) );
    }

    //////////////////////////
    // settings dialog
    //////////////////////////
    // connect system interface slots
    connect( m_SettingsDlg, SIGNAL(DeviceNameChanged(QString)), dev->GetProVideoSystemItf(), SLOT(onDeviceNameChange(QString)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(DeviceNameChanged(QString)), m_SettingsDlg, SLOT(onDeviceNameChange(QString)) );

    // reset to factory defaults
    connect( m_SettingsDlg, SIGNAL(ResetToDefaultsClicked()), dev->GetProVideoSystemItf(), SLOT(onResetSettings()) );

    // system settings (serial connection) changed
    if (deviceFeatures.hasRS232Interface)
    {
        connect( m_ConnectDlg, SIGNAL(RS232BaudrateChanged(uint32_t)), dev->GetProVideoSystemItf(), SLOT(onRS232BaudRateChange(uint32_t)) );
        connect( dev->GetProVideoSystemItf(), SIGNAL(RS232BaudRateChanged(uint32_t)), m_SettingsDlg, SLOT(onRS232BaudrateChange(uint32_t)) );
    }

    connect( m_ConnectDlg, SIGNAL(RS485BaudrateChanged(uint32_t)), dev->GetProVideoSystemItf(), SLOT(onRS485BaudRateChange(uint32_t)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(RS485BaudRateChanged(uint32_t)), m_SettingsDlg, SLOT(onRS485BaudrateChange(uint32_t)) );

    connect( m_ConnectDlg, SIGNAL(RS485AddressChanged(uint32_t)), dev->GetProVideoSystemItf(), SLOT(onRS485AddressChange(uint32_t)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(RS485AddressChanged(uint32_t)), m_SettingsDlg, SLOT(onRS485AddressChange(uint32_t)) );

    connect( m_ConnectDlg, SIGNAL(RS485TerminationChanged(bool)), dev->GetProVideoSystemItf(), SLOT(onRS485TerminationChange(bool)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(RS485TerminationChanged(bool)), m_SettingsDlg, SLOT(onRS485TerminationChange(bool)) );

    if (deviceFeatures.hasSystemBroadcast)
    {
        connect( m_ConnectDlg, SIGNAL(RS485BroadcastAddressChanged(uint32_t)), dev->GetProVideoSystemItf(), SLOT(onRS485BroadcastAddressChange(uint32_t)) );
        connect( dev->GetProVideoSystemItf(), SIGNAL(RS485BroadcastAddressChanged(uint32_t)), m_SettingsDlg, SLOT(onRS485BroadcastAddressChange(uint32_t)) );

        connect( m_ConnectDlg, SIGNAL(RS485BroadcastMasterChanged(int32_t)), dev->GetProVideoSystemItf(), SLOT(onRS485BroadcastMasterChange(int32_t)) );
    }

    //////////////////////////
    // other signals
    //////////////////////////
    // Get supported resolutions from the device when the next resync() is called
    connect( dev->GetProVideoSystemItf(), SIGNAL(ResolutionMaskChanged(uint32_t,uint32_t,uint32_t)), this, SLOT(onResolutionMaskChange(uint32_t,uint32_t,uint32_t)) );

    //////////////////////////
    // Synchronise with the new device
    //////////////////////////
    m_dev->resync();

    // Start the check connection timer, check if device is still connected every 2 seconds
    if ( m_EnableConnectionCheck )
    {
        m_checkConnectionTimer.start( 2000 );
    }

    // Close message box
    infoDlg.close();
}

/******************************************************************************
 * MainWindow::onResolutionMaskChange
 *****************************************************************************/
void MainWindow::onResolutionMaskChange( uint32_t id0, uint32_t id1, uint32_t id2 )
{
    bool supported = false;

    m_ui->inoutBox->clearAllVideoModes();
    m_ui->inoutBox->clearAllGenlockCrosslockVideoModes();

    // fill video-mode and genlock crosslock vmode combo boxes
    for ( int i=VideoModeFirst; i<VideoModeMax; i++ )
    {
        if ( (i>=VideoModeFirstHD) && (i<=VideoModeLastHD) )
        {
            supported = ( id0 & (1u << (i-VideoModeFirstHD)) );
        }

        if ( (i>=VideoModeFirstFullHD) && (i<=VideoModeLast2K) )
        {
            supported = ( id1 & (1u << (i-VideoModeFirstFullHD)) );
        }

        if ( (i>=VideoModeFirstUHD) && (i<=VideoModeLast4K) )
        {
            supported = ( id2 & (1u << (i-VideoModeFirstUHD)) );
        }

        if ( supported )
        {
            m_ui->inoutBox->addVideoMode( GetVideoModeName( static_cast<VideoMode>(i) ), i );
            m_ui->inoutBox->addGenlockCrosslockVideoMode( GetGenlockCrosslockVmodeName( static_cast<VideoMode>(i) ), i);
        }
    }
}

/******************************************************************************
 * MainWindow::onLockCurrentTabPage
 *****************************************************************************/
void MainWindow::onLockCurrentTabPage( bool lock )
{
    for ( int i = 0; i < m_ui->tabWidget->count(); i++ )
    {
        /* If lock == true:  Disable all tabs, except the current tab.
         * If lock == false: Enable all tabs, also the current tab to prevent
         *                   deadlocks. */
        if ( i != m_ui->tabWidget->currentIndex() || !lock )
        {
            m_ui->tabWidget->setTabEnabled( i, !lock );
        }
    }

    // If lock is enabled, also lock the toolbar
    m_ui->toolBar->setEnabled( !lock );
}

/******************************************************************************
 * MainWindow::onBootIntoUpdateMode
 *****************************************************************************/
void MainWindow::onBootIntoUpdateMode()
{
    // When booting into update mode, stop the reconnect timer
    m_checkConnectionTimer.stop();
}

/******************************************************************************
 * MainWindow::onReopenSerialConnection
 *****************************************************************************/
void MainWindow::onReopenSerialConnection()
{
    /* When the serial connection is re-opened (usually this happens after
     * performing a firmware update) the check connection timer has to be
     * restarted, if it is enabled in the settings. */
    if ( m_EnableConnectionCheck )
    {
        m_checkConnectionTimer.start( 2000 );
    }
}

/******************************************************************************
 * MainWindow::onResizeMainWindow
 *****************************************************************************/
void MainWindow::onResizeMainWindow( bool force )
{
    /* Only resize if the whole GUI is visible witouth scrollbars */
    if ( !m_ScrollbarsNeeded )
    {
        /* Do not resize if the window is minimized, otherwise the layout might break.
         * Also do only resize if the the debug terminal is not visible, or if force
         * is set to true. */
        if ( (!this->isMinimized() && !m_DebugTerminal->isVisible()) || force )
        {
            // Resize to minimum size
            this->adjustSize();
            this->resize( this->minimumSizeHint() );
            QApplication::processEvents(QEventLoop::WaitForMoreEvents);
        }
    }
}

/******************************************************************************
 * MainWindow::onCheckConnection
 *****************************************************************************/
void MainWindow::onCheckConnection()
{
    if ( m_ConnectDlg && !m_ConnectDlg->isConnected() )
    {
        if ( !m_ConnectDlg->isVisible() && !m_SettingsDlg->isVisible() )
        {
            // Connection to device lost, stop check connection timer
            m_checkConnectionTimer.stop();

            // Ask user if he wants to reconnect
            QMessageBox msgBox;
            msgBox.setWindowTitle( "Connection lost" );
            msgBox.setText( "The connection to the device has been lost. Maybe the device is turned off or "
                            "its serial port settings got changed.");
            msgBox.setInformativeText( "Do you want to open the Connect Dialog now?\n\n"
                                       "If you want to disable the device connection check, click the 'Ignore' button. "
                                       "You can later enable it again in the Settings Dialog." );
            msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No | QMessageBox::Ignore );
            msgBox.setDefaultButton( QMessageBox::Yes );
            int ret = msgBox.exec();

            if ( ret == QMessageBox::Yes )
            {
                // Call on connect clicked to reconnect
                onConnectClicked();
            }
            else if ( ret == QMessageBox::Ignore )
            {
                // Disable connection check, also uncheck it in the settings dialog
                onConnectionCheckChange( false );
                m_SettingsDlg->setConnectionCheckChecked( false );
            }
        }
    }
}

/******************************************************************************
 * MainWindow::setConnectDlg
 *****************************************************************************/
void MainWindow::setConnectDlg( ConnectDialog * dlg )
{
    m_ConnectDlg = dlg;

    if ( m_ConnectDlg )
    {
        // React if a new device is connected to this window
        connect( m_ConnectDlg, SIGNAL(DeviceConnected( ProVideoDevice *)), this, SLOT(onDeviceConnected(ProVideoDevice *)) );

        // React if the connect dialog has to be re-shown
        connect( m_ConnectDlg, SIGNAL(OpenConnectDialog()), this, SLOT(onConnectClicked()) );

        // Send broadcast mode changed event to connect dialog when the button is pressed in the toolbar
        /* The broadcast mode can not be set directly on the device, because before that some changes in the serial connection
         * have to be made. Therefore we let the connecion dialog handle this */
        connect( this, SIGNAL(BroadcastChanged(bool)), m_ConnectDlg, SLOT(onBroadcastChange(bool)) );

        // The connect dialog handles serial connection close / reopen during update procedure
        connect( m_ui->updBox, SIGNAL(CloseSerialConnection()), m_ConnectDlg, SLOT(onCloseSerialConnection()) );
        connect( m_ui->updBox, SIGNAL(ReopenSerialConnection()), m_ConnectDlg, SLOT(onReopenSerialConnection()) );
    }
}

/******************************************************************************
 * MainWindow::setSettingsDlg
 *****************************************************************************/
void MainWindow::setSettingsDlg( SettingsDialog * dlg )
{
    m_SettingsDlg = dlg;

    if ( m_SettingsDlg )
    {
        // When broadcast mode is changed, some elements in the settings dialog get hiddon or shown
        connect( this, SIGNAL(BroadcastChanged(bool)), m_SettingsDlg, SLOT(onBroadcastChange(bool)) );

        connect( m_SettingsDlg, SIGNAL(UpdateDeviceName()), this, SLOT(onUpdateDeviceName()) );
        connect( m_SettingsDlg, SIGNAL(ResyncRequest()), this, SLOT(onResyncRequest()) );
        connect( m_SettingsDlg, SIGNAL(SystemSettingsChanged(int,int,int,int,bool)), this, SLOT(onSystemSettingsChange(int,int,int,int,bool)) );
        connect( m_SettingsDlg, SIGNAL(WidgetModeChanged(DctWidgetBox::Mode)), this, SLOT(onWidgetModeChange(DctWidgetBox::Mode)) );
        connect( m_SettingsDlg, SIGNAL(ConnectionCheckChanged(bool)), this, SLOT(onConnectionCheckChange(bool)) );
        connect( m_SettingsDlg, SIGNAL(SaveSettings()), this, SLOT( onSaveSettingsClicked()) );
    }
}

/******************************************************************************
 * MainWindow::setDebugTerminal
 *****************************************************************************/
void MainWindow::setDebugTerminal( DebugTerminal * dlg )
{
    m_DebugTerminal = dlg;

    if ( m_DebugTerminal )
    {
        // Connect debug terminal with RS232 channel
        connect( m_ConnectDlg->getChannelRS232(), SIGNAL(dataReceived(QString)), m_DebugTerminal, SLOT(onDataReceived(QString)) );
        connect( m_DebugTerminal, SIGNAL(sendData(QString, int)), m_ConnectDlg->getChannelRS232(), SLOT(onSendData(QString, int)) );

        // Connect debug terminal with RS485 channel
        connect( m_ConnectDlg->getChannelRS485(), SIGNAL(dataReceived(QString)), m_DebugTerminal, SLOT(onDataReceived(QString)) );
        connect( m_DebugTerminal, SIGNAL(sendData(QString, int)), m_ConnectDlg->getChannelRS485(), SLOT(onSendData(QString, int)) );

        // Setup the Debug Terminal as a dock widget
        QDockWidget *dock = new QDockWidget( tr("Debug Terminal"), this );
        dock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
        dock->setWidget( m_DebugTerminal );
        dock->hide();
        addDockWidget( Qt::RightDockWidgetArea, dock );

        // Connect settings dialog and debug terminal
        if ( m_SettingsDlg )
        {
            connect( dock->toggleViewAction(), SIGNAL(toggled(bool)), m_SettingsDlg, SLOT( onDebugTerminalVisibilityChange(bool)) );
            connect( m_SettingsDlg, SIGNAL(DebugTerminalVisibilityChanged(bool)), dock, SLOT(setVisible(bool)) );
            connect( this, SIGNAL(setDockWidgetVisible(bool)), dock, SLOT(setVisible(bool)) );
        }

        connect( dock, SIGNAL(topLevelChanged(bool)), this, SLOT(onDebugTerminalTopLevelChange(bool)) );
        connect( dock, SIGNAL(visibilityChanged(bool)), this, SLOT(onDebugTerminalVisibilityChange(bool)) );
    }
}

/******************************************************************************
 * MainWindow::onDeviceConnected
 *****************************************************************************/
void MainWindow::onDeviceConnected(ProVideoDevice * device)
{
    connectToDevice(device);
}

/******************************************************************************
 * MainWindow::onDeviceSelectionChange
 *****************************************************************************/
void MainWindow::onDeviceSelectionChange( int index )
{
    // Try to connect to the selected device
    if ( !m_ConnectDlg->connectToRS485DeviceByIndex(index) )
    {
        m_ConnectDlg->exec();

        if ( m_ConnectDlg->result() == QDialog::Rejected )
        {
            close();
        }
    }
}

/******************************************************************************
 * MainWindow::onUpdateDeviceName
 *****************************************************************************/
void MainWindow::onUpdateDeviceName()
{
    // Make the connect dialog get the new name from the device
    m_ConnectDlg->updateCurrentDeviceName();

    // Update combo box
    updateDeviceList();
}

/******************************************************************************
 * MainWindow::onSystemSettingsChange
 *****************************************************************************/
void MainWindow::onSystemSettingsChange( int rs232Baudrate, int rs485Baudrate,
                                         int rs485Address, int rs485BroadcastAddress,
                                         bool rs485Termination )
{
    // Change the comport settings on the device and in the connect dialog
    m_ConnectDlg->changeComportSettings( rs232Baudrate, rs485Baudrate,
                                         rs485Address, rs485BroadcastAddress,
                                         rs485Termination );

    // Update combo box
    updateDeviceList();
}

/******************************************************************************
 * MainWindow::onConnectClicked
 *****************************************************************************/
void MainWindow::onConnectClicked()
{
    if ( m_ConnectDlg )
    {
        m_ConnectDlg->exec();

        if ( m_ConnectDlg->result() == QDialog::Rejected )
        {
            close();
        }
    }
}

/******************************************************************************
 * MainWindow::onSettingsClicked
 *****************************************************************************/
void MainWindow::onSettingsClicked()
{
    if ( m_SettingsDlg )
    {
        m_SettingsDlg->exec();
    }
}

/******************************************************************************
 * MainWindow::onSelectSdi1Clicked
 *****************************************************************************/
void MainWindow::onSelectSdi1Clicked()
{
    if ( m_dev )
    {
        QApplication::setOverrideCursor( Qt::WaitCursor );
        m_ui->actionSelectSdi1->setChecked( true );
        m_ui->actionSelectSdi2->setChecked( false );
        emit SdiOutChanged( 1 );
        m_dev->resyncChainSpecific();
        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }
}

/******************************************************************************
 * MainWindow::onSelectSdi2Clicked
 *****************************************************************************/
void MainWindow::onSelectSdi2Clicked()
{
    if ( m_dev )
    {
        QApplication::setOverrideCursor( Qt::WaitCursor );
        m_ui->actionSelectSdi1->setChecked( false );
        m_ui->actionSelectSdi2->setChecked( true );
        emit SdiOutChanged( 2 );
        m_dev->resyncChainSpecific();
        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }
}

/******************************************************************************
 * MainWindow::onCopyChain1To2Clicked
 *****************************************************************************/
void MainWindow::onCopyChain1To2Clicked()
{
    if ( m_dev )
    {
        QApplication::setOverrideCursor( Qt::WaitCursor );

        // Copy settings from chain 1 to chain 2
        emit CopySettings( 1, 2 );

        // resync, if chain 2 is the active chain
        if ( m_ui->actionSelectSdi2->isChecked() )
        {
            m_dev->resyncChainSpecific();
        }

        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }
}

/******************************************************************************
 * MainWindow::onCopyChain2To1Clicked
 *****************************************************************************/
void MainWindow::onCopyChain2To1Clicked()
{
    if ( m_dev )
    {
        QApplication::setOverrideCursor( Qt::WaitCursor );

        // Copy settings from chain 2 to chain 1
        emit CopySettings( 2, 1 );

        // Resync, if chain 1 is the active chain
        if ( m_ui->actionSelectSdi1->isChecked() )
        {
            m_dev->resyncChainSpecific();
        }

        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }
}

/******************************************************************************
 * MainWindow::onSplitScreenClicked()
 *****************************************************************************/
void MainWindow::onSplitScreenClicked()
{
    if ( m_dev )
    {
        QApplication::setOverrideCursor( Qt::WaitCursor );

        // Call split screen command
        emit SplitScreenChanged( m_ui->actionSplitScreen->isChecked() );

        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }
}

/******************************************************************************
 * MainWindow::onLoadSettingsClicked
 *****************************************************************************/
void MainWindow::onLoadSettingsClicked()
{
    if ( m_dev )
    {
        QApplication::setOverrideCursor( Qt::WaitCursor );
        emit LoadSettings(); m_dev->resync();
        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }
}

/******************************************************************************
 * MainWindow::onSaveSettingsClicked
 *****************************************************************************/
void MainWindow::onSaveSettingsClicked()
{
    if ( m_dev )
    {
        QApplication::setOverrideCursor( Qt::WaitCursor );
        emit SaveSettings();
        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }
}

/******************************************************************************
 * MainWindow::onLoadFromFileClicked
 *****************************************************************************/
void MainWindow::onLoadFromFileClicked()
{
    QString directory = QDir::currentPath();

    // NOTE: It can fail on gtk-systems when an empty filename is given
    //       in the native dialog-box, because GTK sends a SIGSEGV-signal
    //       to process and this is not handled by Qt.
    QFileDialog dialog( this );
    dialog.setDefaultSuffix( "dct" );
    m_filename = dialog.getOpenFileName(
        this, tr("Load Device Settings"),
        directory,
        "Setting Files (*.dct);;All files (*.*)"
    );

    if ( nullptr != m_filename )
    {
        QFileInfo file( m_filename );
        if ( file.suffix().isEmpty() )
        {
            m_filename += ".dct";
        }

        if ( fileExists(m_filename) )
        {

            // Open settings
            QSettings settings( m_filename, QSettings::IniFormat );

            // Load the device name and platform from the settings file
            settings.beginGroup( MAIN_SETTINGS_SECTION_NAME );
            QString systemPlatform = settings.value( MAIN_SETTINGS_SYSTEM_PLATFORM ).toString();
            settings.endGroup();

            // Check if platform matches
            if ( systemPlatform != m_dev->getSystemPlatform() )
            {
                QApplication::setOverrideCursor( Qt::ArrowCursor );
                QMessageBox msgBox;
                msgBox.setWindowTitle( "Error Loading Settings" );
                QString msgText = QString( "Settings can not be loaded, because the devices mismatch. The file\n\n'%1'\n\ncontains settings for a '%2' device, but the GUI is currently connected to a '%3' device." ).arg( m_filename ).arg( systemPlatform ).arg( m_dev->getSystemPlatform() );
                msgBox.setText( msgText );
                msgBox.exec();
            }
            else
            {
                // Load the settings of all visible tabs
                // Get number of tabs which settinsg have to be loaded
                int progressSteps = m_activeWidgets.length();
                if ( m_dev->getSupportedFeatures().hasChainSelection )
                {
                    // Settings for the lutbox and outbox have to be loaded twice (once for each chain)
                    if ( m_activeWidgets.contains(m_ui->lutBox) )
                    {
                        progressSteps++;
                    }
                    if ( m_activeWidgets.contains(m_ui->outBox) )
                    {
                        progressSteps++;
                    }
                }

                // Create progress dialog
                QProgressDialog progressDialog( "Loading Settings...", "", 0, progressSteps, this );
                progressDialog.setCancelButton( nullptr );
                progressDialog.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);
                progressDialog.show();

                // sleep for 100ms and refresh progress bar, this ensures that the progress bar is correctly shown under linux
                QThread::msleep( 100 );
                progressDialog.setValue( 0 );
                QApplication::processEvents(QEventLoop::WaitForMoreEvents);

                // Disable updpates of the GUI
                this->setUpdatesEnabled( false );

                // Load settings for all widgets for the currently active chain
                int i;
                for ( i = 0; i < m_activeWidgets.length(); i++ )
                {
                    progressDialog.setValue( i );
                    QApplication::processEvents();
                    m_activeWidgets[i]->load( settings );
                }

                // If this device has a second chain, load settings for it too
                if ( m_dev->getSupportedFeatures().hasChainSelection )
                {
                    // Check which chain is currently used and switch to the other one
                    if ( m_ui->actionSelectSdi1->isChecked() )
                    {
                        emit SdiOutChanged( 2 );
                    }
                    else
                    {
                        emit SdiOutChanged( 1 );
                    }

                    // Load lutbox settings for other chain
                    if ( m_activeWidgets.contains(m_ui->lutBox) )
                    {
                        progressDialog.setValue( i );
                        i++;
                        QApplication::processEvents();
                        m_ui->lutBox->load( settings );
                    }

                    // Load outbox settings for other chain
                    if ( m_activeWidgets.contains(m_ui->outBox) )
                    {
                        progressDialog.setValue( i );
                        QApplication::processEvents();
                        m_ui->outBox->load( settings );
                    }

                    // Return to previous chain
                    if ( m_ui->actionSelectSdi1->isChecked() )
                    {
                        emit SdiOutChanged( 1 );
                    }
                    else
                    {
                        emit SdiOutChanged( 2 );
                    }
                }

                // Resync settings
                m_dev->resync();

                // Set dialog to 100%
                progressDialog.setValue( progressSteps );
                QApplication::processEvents();

                // Re-enable updpates of the GUI
                this->setUpdatesEnabled( true );
            }
        }
    }
}

/******************************************************************************
 * MainWindow::onSaveToFileClicked
 *****************************************************************************/
void MainWindow::onSaveToFileClicked()
{
    QString directory = QDir::currentPath();
    directory.append("/");
    directory.append(m_dev->getDeviceName());
    directory.append(".dct");

    // NOTE: It can fail on gtk-systems when an empty filename is given
    //       in the native dialog-box, because GTK sends a SIGSEGV-signal
    //       to process and this is not handled by Qt.
    QFileDialog dialog( this );
    dialog.setDefaultSuffix( "dct" );

    m_filename = dialog.getSaveFileName(
        this, tr("Save Device Settings"),
        directory,
        "Setting Files (*.dct);;All files (*.*)"
    );
    QApplication::processEvents();

    QApplication::setOverrideCursor( Qt::WaitCursor );
    if ( nullptr != m_filename )
    {
        QFileInfo file( m_filename );
        if ( file.suffix().isEmpty() )
        {
            m_filename += ".dct";
        }

        // Open settings file and make sure it is clean
        QSettings settings( m_filename, QSettings::IniFormat );
        settings.clear();

        // Get number of tabs which settinsg have to be saved
        int progressSteps = m_activeWidgets.length();
        if ( m_dev->getSupportedFeatures().hasChainSelection )
        {
            // Settings for the lutbox and outbox have to be saved twice (once for each chain)
            if ( m_activeWidgets.contains(m_ui->lutBox) )
            {
                progressSteps++;
            }
            if ( m_activeWidgets.contains(m_ui->outBox) )
            {
                progressSteps++;
            }
        }

        // Create progress dialog
        QProgressDialog progressDialog( "Saving Settings...", "", 0, progressSteps, this );
        progressDialog.setCancelButton( nullptr );
        progressDialog.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);
        progressDialog.show();

        // sleep for 100ms and refresh progress bar, this ensures that the progress bar is correctly shown under linux
        QThread::msleep( 100 );
        progressDialog.setValue( 0 );
        QApplication::processEvents(QEventLoop::WaitForMoreEvents);

        // Disable updpates of the GUI
        this->setUpdatesEnabled( false );

        // Write the device name and platform into the settings file
        settings.beginGroup( MAIN_SETTINGS_SECTION_NAME );
        settings.setValue( MAIN_SETTINGS_SYSTEM_PLATFORM, m_dev->getSystemPlatform() );
        settings.endGroup();

        // Save the settings of all active widgets
        int i;
        for ( i = 0; i < m_activeWidgets.length(); i++ )
        {
            progressDialog.setValue( i );
            QApplication::processEvents();
            m_activeWidgets[i]->save( settings );
        }

        // If this device has a second chain, save settings for it too
        if ( m_dev->getSupportedFeatures().hasChainSelection )
        {
            // Check which chain is currently used and switch to the other one
            if ( m_ui->actionSelectSdi1->isChecked() )
            {
                emit SdiOutChanged( 2 );
            }
            else
            {
                emit SdiOutChanged( 1 );
            }

            // Resync settings of this chain
            m_dev->resyncChainSpecific();

            // Save lutbox settings for other chain
            if ( m_activeWidgets.contains(m_ui->lutBox) )
            {
                progressDialog.setValue( i );
                i++;
                QApplication::processEvents();
                m_ui->lutBox->save( settings );
            }

            // Save outbox settings for other chain
            if ( m_activeWidgets.contains(m_ui->outBox) )
            {
                progressDialog.setValue( i );
                QApplication::processEvents();
                m_ui->outBox->save( settings );
            }

            // Return to previous chain and resync
            if ( m_ui->actionSelectSdi1->isChecked() )
            {
                emit SdiOutChanged( 1 );
            }
            else
            {
                emit SdiOutChanged( 2 );
            }
            m_dev->resyncChainSpecific();
        }

        // Set dialog to 100%
        progressDialog.setValue( progressSteps );
        QApplication::processEvents();

        // Re-enable updpates of the GUI
        this->setUpdatesEnabled( true );
    }
    QApplication::setOverrideCursor( Qt::ArrowCursor );
}

/******************************************************************************
 * MainWindow::onBroadcastClicked()
 *****************************************************************************/
void MainWindow::onBroadcastClicked()
{
    if ( m_dev )
    {
        QApplication::setOverrideCursor( Qt::WaitCursor );

        bool broadcastEnabled = m_ui->actionBroadcast->isChecked();

        // Toggle broadcast mode (enable / disable broadcast, show / hide settings which are not valid in bc mode)
        emit BroadcastChanged( broadcastEnabled );

        // enable the sync button, if broadcast mode is active
        m_ui->actionSync->setEnabled( broadcastEnabled );

        /* disable update tab if broadcast is enabled, before this is done,
         * switch to the first tab, if the update tab is currently selected */
        if ( broadcastEnabled )
        {
            if ( m_ui->tabWidget->currentWidget() == m_ui->tabUpdate )
            {
                m_ui->tabWidget->setCurrentIndex( 0 );
            }
        }
        int updateTabIndex = m_ui->tabWidget->indexOf( m_ui->tabUpdate );
        if ( updateTabIndex != -1 )
        {
            m_ui->tabWidget->setTabEnabled( updateTabIndex, !broadcastEnabled );
        }

        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }
}

/******************************************************************************
 * MainWindow::onSyncSettingsClicked
 *****************************************************************************/
void MainWindow::onSyncSettingsClicked()
{
    // Get number of tabs which settinsg have to be loaded
    int progressSteps = m_activeWidgets.length();
    if ( m_dev->getSupportedFeatures().hasChainSelection )
    {
        // Settings for the lutbox and outbox have to be loaded twice (once for each chain)
        if ( m_activeWidgets.contains(m_ui->lutBox) )
        {
            progressSteps++;
        }
        if ( m_activeWidgets.contains(m_ui->outBox) )
        {
            progressSteps++;
        }
    }

    // Create progress dialog
    QProgressDialog progressDialog( "Synchronising Settings...", "", 0, progressSteps, this );
    progressDialog.setCancelButton( nullptr );
    progressDialog.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);
    progressDialog.show();

    // sleep for 100ms and refresh progress bar, this ensures that the progress bar is correctly shown under linux
    QThread::msleep( 100 );
    progressDialog.setValue( 0 );
    QApplication::processEvents(QEventLoop::WaitForMoreEvents);

    // Disable updpates of the GUI
    this->setUpdatesEnabled( false );

    // Apply the settings of all visible tabs
    int i;
    for ( i = 0; i < m_activeWidgets.length(); i++ )
    {
        progressDialog.setValue( i );
        QApplication::processEvents();
        m_activeWidgets[i]->apply();
    }

    // If this device has a second chain, sync settings for it too
    if ( m_dev->getSupportedFeatures().hasChainSelection )
    {
        // Check which chain is currently used and switch to the other one
        if ( m_ui->actionSelectSdi1->isChecked() )
        {
            emit SdiOutChanged( 2 );
        }
        else
        {
            emit SdiOutChanged( 1 );
        }

        // Sync lutbox settings for other chain
        if ( m_activeWidgets.contains(m_ui->lutBox) )
        {
            progressDialog.setValue( i );
            i++;
            QApplication::processEvents();
            m_ui->lutBox->apply();
        }

        // Load outbox settings for other chain
        if ( m_activeWidgets.contains(m_ui->outBox) )
        {
            progressDialog.setValue( i );
            QApplication::processEvents();
            m_ui->outBox->apply();
        }

        // Return to previous chain
        if ( m_ui->actionSelectSdi1->isChecked() )
        {
            emit SdiOutChanged( 1 );
        }
        else
        {
            emit SdiOutChanged( 2 );
        }
    }

    // Set dialog to 100%
    progressDialog.setValue( progressSteps );
    QApplication::processEvents();

    // Re-enable updpates of the GUI
    this->setUpdatesEnabled( true );
}

/******************************************************************************
 * MainWindow::onSdiOutChange
 *****************************************************************************/
void MainWindow::onSdiOutChange( int value )
{
    if ( value == 1 )
    {
        // SDI-1 active
        m_ui->actionSelectSdi1->setChecked( true );
        m_ui->actionSelectSdi2->setChecked( false );
    }
    else
    {
        // SDI-2 active
        m_ui->actionSelectSdi1->setChecked( false );
        m_ui->actionSelectSdi2->setChecked( true );
    }
}

/******************************************************************************
 * MainWindow::onSplitScreenChange
 *****************************************************************************/
void MainWindow::onSplitScreenChange( bool flag )
{
    // check split-screen button if split-screen feature is enabled
    m_ui->actionSplitScreen->setChecked( flag );
}

/******************************************************************************
 * MainWindow::onCopyFlagChange
 *****************************************************************************/
void MainWindow::onCopyFlagChange( bool value )
{
    m_dev->SetCopyFlag( value );
}

/******************************************************************************
 * MainWindow::onWidgetModeChange
 *****************************************************************************/
void MainWindow::onWidgetModeChange( DctWidgetBox::Mode mode )
{
    // Set widget mode for all active widgets
    for ( int i = 0; i < m_activeWidgets.length(); i++ )
    {
        m_activeWidgets[i]->setMode( mode );
    }

    // Store widget mode
    m_WidgetMode = mode;
}

/******************************************************************************
 * MainWindow::onConnectionCheckChange
 *****************************************************************************/
void MainWindow::onConnectionCheckChange( bool enable )
{
    // Start or stop connection timer
    if ( enable && !m_checkConnectionTimer.isActive() )
    {
        m_checkConnectionTimer.start( 2000 );
    }
    else
    {
        m_checkConnectionTimer.stop();
    }

    // Store check connection flag
    m_EnableConnectionCheck = enable;
}

/******************************************************************************
 * MainWindow::onBroadcastChange
 *****************************************************************************/
void MainWindow::onBroadcastChange( uint8_t flag )
{
    // If the broadcast address is set to a negative value, broadcasting is disabled
    bool broadcastEnabled = (flag == 0) ? false : true;

    // set the broadcast button state
    m_ui->actionBroadcast->blockSignals( true );
    m_ui->actionBroadcast->setChecked( broadcastEnabled );
    m_ui->actionBroadcast->blockSignals( false );

    // enable the sync button, if broadcast mode is active
    m_ui->actionSync->setEnabled( broadcastEnabled );

    // emit a broadcast change event to notify other ui elements
    emit BroadcastChanged( broadcastEnabled );
}

/******************************************************************************
 * MainWindow::onDebugTerminalTopLevelChange
 *****************************************************************************/
void MainWindow::onDebugTerminalTopLevelChange( bool floating )
{
    // Resize window to minimum size if dock widget is not docked anymore
    if ( floating )
    {
         onResizeMainWindow( true );   // Force resize
    }
}

/******************************************************************************
 * MainWindow::onDebugTerminalVisibilityChange
 *****************************************************************************/
void MainWindow::onDebugTerminalVisibilityChange( bool visible )
{
    // Resize window to minimum size if dock widget is not visible anymore
    if ( !visible )
    {
        /* Note: We have to call the resize request with a short delay because the
         * visibility changed event from the dock widget is emitted, before it is
         * completely closed which causes the main window to think that it still requires
         * space and it is not correctly resized. */
        m_resizeTimer.start( 1 );
    }

    /* Store debug terminal visible state only if GUI is still visible (to avoid
     * resetting it on application close) */
    if ( this->isVisible() )
    {
        m_ShowDebugTerminal = visible;
    }
}

/******************************************************************************
 * MainWindow::onAecResyncRequest
 *****************************************************************************/
void MainWindow::onAecResyncRequest()
{
    // if aec is disabled, update exposure, gain and iris aperture
    if ( m_dev )
    {
        QApplication::setOverrideCursor( Qt::WaitCursor );
        ProVideoDevice::features features = m_dev->getSupportedFeatures();
        if (features.hasIrisItf)
        {
            m_dev->GetIrisItf()->resync();
        }
        if (features.hasCamItf)
        {
            m_dev->GetCamItf()->resync();
        }
        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }
}




/******************************************************************************
 * MainWindow::onResyncRequest
 *****************************************************************************/
void MainWindow::onResyncRequest()
{
    // if reset to factory defaults is clicked, all settings have to be resynced
    if ( m_dev )
    {
        QApplication::setOverrideCursor( Qt::WaitCursor );
        m_dev->resync();
        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }
}
