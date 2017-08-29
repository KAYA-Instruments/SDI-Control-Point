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
#include <QtDebug>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>

#include <ProVideoDevice.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <defines.h>

/******************************************************************************
 * Settings
 *****************************************************************************/
#define MAIN_SETTINGS_SECTION_NAME          ( "MAIN" )
#define MAIN_SETTINGS_DEVICE_NAME           ( "device" )

/******************************************************************************
 * fileExists
 *****************************************************************************/
static bool fileExists( QString & path )
{
    QFileInfo check_file( path );

    // check if file exists and if yes: Is it really a file and no directory?
    return ( check_file.exists() && check_file.isFile() );
}

/******************************************************************************
 * MainWindow::MainWindow
 *****************************************************************************/
MainWindow::MainWindow( ConnectDialog * connectDialog, QWidget * parent )
    : QMainWindow( parent )
    , m_ui( new Ui::MainWindow )
    , m_dlg( NULL )
    , m_cbxConnectedDevices( NULL )
    , m_dev ( NULL )
{
    // create ui
    m_ui->setupUi( this );

    // Connect with dialog
    setConnectDlg(connectDialog);
    
    // connect actions
    connect( m_ui->actionConnect        , SIGNAL( triggered() ), this, SLOT( onConnectClicked() ) );
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
}

/******************************************************************************
 * MainWindow::MainWindow
 *****************************************************************************/
MainWindow::~MainWindow()
{
    // When the main window closes, disable broadcast mode (this disables the broadcast master)
    emit BroadcastChanged( false );

    delete m_ui;
}

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
    QVector<ConnectDialog::detectedRS485Device> connectedRS485Devices = m_dlg->getDetectedRS485Devices();
    int currentRS485DeviceIndex = m_dlg->getCurrentRs485DeviceIndex();
    if ( !connectedRS485Devices.empty() )
    {
        // Create a new label
        QLabel * lblConnectedDevices = new QLabel( m_ui->toolBar );
        lblConnectedDevices->setText(" Select Device: ");

        // Create new combo box
        QComboBox * cbxConnectedDevices = new QComboBox( m_ui->toolBar );

        // Set size adjust policyto auto adjust to react to changing device names
        cbxConnectedDevices->setSizeAdjustPolicy( cbxConnectedDevices->AdjustToContents );

        // Add device names to the combo box
        for (int i = 0; i < connectedRS485Devices.count(); i++ )
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
        connect( cbxConnectedDevices, SIGNAL(currentIndexChanged(int)), this, SLOT(onDeviceSelectionChange(int)) );

        // Store pointer to combo box in class variable
        m_cbxConnectedDevices = cbxConnectedDevices;
    }
    else
    {
        m_cbxConnectedDevices = NULL;
    }

    // If the system supports broadcasting and there is at least one RS485 device connected show the broadcast toggle button
    if (deviceFeatures.hasSystemBroadcast && !connectedRS485Devices.empty() )
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
    m_ui->tabWidget->removeTab(m_ui->tabWidget->indexOf(m_ui->tabInOut));
    m_ui->tabWidget->removeTab(m_ui->tabWidget->indexOf(m_ui->tabBlack));
    m_ui->tabWidget->removeTab(m_ui->tabWidget->indexOf(m_ui->tabWb));
    m_ui->tabWidget->removeTab(m_ui->tabWidget->indexOf(m_ui->tabFlt));
    m_ui->tabWidget->removeTab(m_ui->tabWidget->indexOf(m_ui->tabMcc));
    m_ui->tabWidget->removeTab(m_ui->tabWidget->indexOf(m_ui->tabKnee));
    m_ui->tabWidget->removeTab(m_ui->tabWidget->indexOf(m_ui->tabGamma));
    m_ui->tabWidget->removeTab(m_ui->tabWidget->indexOf(m_ui->tabDpcc));
    m_ui->tabWidget->removeTab(m_ui->tabWidget->indexOf(m_ui->tabOut));
    m_ui->tabWidget->removeTab(m_ui->tabWidget->indexOf(m_ui->tabInfo));
    m_ui->tabWidget->removeTab(m_ui->tabWidget->indexOf(m_ui->tabUpdate));

    // Add only those tabs, which are supported by the device, add them to the active widget list
    m_activeWidgets.append(m_ui->inoutBox);
    m_ui->tabWidget->addTab(m_ui->tabInOut, QIcon(":/images/tab/inout.png"), "");
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
    if (deviceFeatures.hasSystemUpdate && m_dlg->getActiveInterface() == ConnectDialog::Rs485 )
    {
        m_activeWidgets.append(m_ui->updBox);
        m_ui->tabWidget->addTab(m_ui->tabUpdate, QIcon(":/images/tab/update.png"), "");
    }

    // Enable / disable elements inside the tabs
    // InOut Tab
    m_ui->inoutBox->setCameraSettingsVisible(deviceFeatures.hasCamItf);
    m_ui->inoutBox->setAutoExposureSettingsVisible(deviceFeatures.hasAutoItf);
    m_ui->inoutBox->setLensChadingCorrectionSettingsVisible(deviceFeatures.hasIspLsc);
    m_ui->inoutBox->setApartureVisible(deviceFeatures.hasIrisItf);
    m_ui->inoutBox->setSdi2ModeVisible(deviceFeatures.hasChainSdi2Mode);
    m_ui->inoutBox->setGenLockVisible(deviceFeatures.hasChainGenLock);
    m_ui->inoutBox->setTimeCodeVisible(deviceFeatures.hasChainTimeCode);
    m_ui->inoutBox->setFlipModeVisible(deviceFeatures.hasChainFlip);
    m_ui->inoutBox->setTestPatternVisible(deviceFeatures.hasOsdTestPattern);

    // BlackBox Tab
    m_ui->blackBox->setFlareLevelVisible(deviceFeatures.hasIspFlare);
    m_ui->blackBox->setMasterBlackLevelVisible(deviceFeatures.hasIspMasterBlackLevel);

    // WbBox Tab
    m_ui->wbBox->setWhiteBalanceSettingsVisible(deviceFeatures.hasIspAutoWhiteBalance, deviceFeatures.hasIspGain, deviceFeatures.hasIspGreenGain);

    // Dpcc Tab
    m_ui->dpccBox->setCameraFlashVisible(deviceFeatures.hasDpccFlash);

    // Out Tab
    m_ui->outBox->changeSdiStringToHdmi(deviceFeatures.hasChainHdmiSettings);
    m_ui->outBox->setRawModeVisible(deviceFeatures.hasChainRawMode);

    // Info Tab
    m_ui->infoBox->setRuntimeVisible(deviceFeatures.hasSystemRuntime);
    m_ui->infoBox->setBroadcastSettingsVisible(deviceFeatures.hasSystemBroadcast);
}

/******************************************************************************
 * MainWindow::connectToDevice
 *****************************************************************************/
void MainWindow::connectToDevice( ProVideoDevice * dev )
{
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
        connect( dev->GetCamItf(), SIGNAL(CameraInfoChanged(int,int,int,int)), m_ui->inoutBox, SLOT(onCameraInfoChange(int,int,int,int)) );

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
        if (deviceFeatures.hasChainFlip)
        {
            connect( dev->GetChainItf(), SIGNAL(ChainFlipModeChanged(int)), m_ui->inoutBox, SLOT(onChainFlipModeChange(int)) );
            connect( m_ui->inoutBox, SIGNAL(ChainFlipModeChanged(int)), dev->GetChainItf(), SLOT(onChainFlipModeChange(int)) );
        }
        if (deviceFeatures.hasChainGenLock)
        {
            // connect gen-lock mode
            connect( dev->GetChainItf(), SIGNAL(ChainGenlockModeChanged(int)), m_ui->inoutBox, SLOT(onChainGenlockModeChange(int)) );
            connect( m_ui->inoutBox, SIGNAL(ChainGenlockModeChanged(int)), dev->GetChainItf(), SLOT(onChainGenlockModeChange(int)) );

            connect( dev->GetChainItf(), SIGNAL(ChainGenlockOffsetChanged(int, int)), m_ui->inoutBox, SLOT(onChainGenlockOffsetChange(int, int)) );
            connect( m_ui->inoutBox, SIGNAL(ChainGenlockOffsetChanged(int, int)), dev->GetChainItf(), SLOT(onChainGenlockOffsetChange(int, int)) );

            connect( dev->GetChainItf(), SIGNAL(ChainGenlockTerminationChanged(int)), m_ui->inoutBox, SLOT(onChainGenlockTerminationChange(int)) );
            connect( m_ui->inoutBox, SIGNAL(ChainGenlockTerminationChanged(int)), dev->GetChainItf(), SLOT(onChainGenlockTerminationChange(int)) );
        }
        if (deviceFeatures.hasChainTimeCode)
        {
            // Timecode
            connect( dev->GetChainItf(), SIGNAL(TimecodeChanged(QVector<int>)), m_ui->inoutBox, SLOT(onTimecodeChange(QVector<int>)) );
            connect( m_ui->inoutBox, SIGNAL(TimecodeSet(QVector<int>)), dev->GetChainItf(), SLOT(onTimecodeChange(QVector<int>)) );

            connect( m_ui->inoutBox, SIGNAL(TimecodeGet()), dev->GetChainItf(), SLOT(onTimecodeGetRequest()) );

            connect( dev->GetChainItf(), SIGNAL(TimecodeHoldChanged(bool)), m_ui->inoutBox, SLOT(onTimecodeHoldChange(bool)) );
            connect( m_ui->inoutBox, SIGNAL(TimecodeHold(bool)), dev->GetChainItf(), SLOT(onTimecodeHoldChange(bool)) );
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
    }

    if (deviceFeatures.hasIrisItf)
    {
        // Auto Iris
        connect( dev->GetIrisItf(), SIGNAL(IrisSetupChanged(QVector<int>)), m_ui->inoutBox, SLOT(onIrisSetupChange(QVector<int>)) );
        connect( dev->GetIrisItf(), SIGNAL(IrisAptChanged(int)), m_ui->inoutBox, SLOT(onIrisAptChange(int)) );
        connect( dev->GetIrisItf(), SIGNAL(IrisAptError()), m_ui->inoutBox, SLOT(onIrisAptError()) );

        connect( m_ui->inoutBox, SIGNAL(IrisAptChanged(int)), dev->GetIrisItf(), SLOT(onIrisAptChange(int)) );
    }

    // when aec is disabled, exposure, gain and aperture have to be resynced for actual values
    if (deviceFeatures.hasIspItf && deviceFeatures.hasCamItf)
    {
        connect( m_ui->inoutBox, SIGNAL(ResyncRequest()), this, SLOT(onAecResyncRequest()) );
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
        // connect Hue with Dialog box
        connect( m_ui->wbBox, SIGNAL(HueChanged(int)), dev->GetCprocItf(), SLOT(onHueChange(int)) );
        connect( dev->GetCprocItf(), SIGNAL(HueChanged(int)), m_ui->wbBox, SLOT(onHueChange(int)) );

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
    }

    //////////////////////////
    // dpcc
    //////////////////////////
    if (deviceFeatures.hasDpccItf)
    {
        // enable
        connect( dev->GetDpccItf(), SIGNAL(DpccEnableChanged(int)), m_ui->dpccBox, SLOT(onDpccEnableChanged(int)) );
        connect( m_ui->dpccBox, SIGNAL(DpccEnableChanged(int)), dev->GetDpccItf(), SLOT(onDpccEnableChange(int)) );

        // correction mode
        connect( dev->GetDpccItf(), SIGNAL(DpccCorrectionModeChanged(int)), m_ui->dpccBox, SLOT(onDpccCorrectionModeChanged(int)) );
        connect( m_ui->dpccBox, SIGNAL(DpccCorrectionModeChanged(int)), dev->GetDpccItf(), SLOT(onDpccCorrectionModeChange(int)) );

        // detection level
        connect( dev->GetDpccItf(), SIGNAL(DpccDetectionLevelChanged(int)), m_ui->dpccBox, SLOT(onDpccDetectionLevelChanged(int)) );
        connect( m_ui->dpccBox, SIGNAL(DpccDetectionLevelChanged(int)), dev->GetDpccItf(), SLOT(onDpccDetectionLevelChange(int)) );

        // test mode
        connect( dev->GetDpccItf(), SIGNAL(DpccTestModeChanged(int)), m_ui->dpccBox, SLOT(onDpccTestModeChanged(int)) );
        connect( m_ui->dpccBox, SIGNAL(DpccTestModeChanged(int)), dev->GetDpccItf(), SLOT(onDpccTestModeChange(int)) );

        // position table
        connect( dev->GetDpccItf(), SIGNAL(DpccTableChanged(QVector<int>,QVector<int>)), m_ui->dpccBox, SLOT(onDpccTableFromCameraLoaded(QVector<int>,QVector<int>)) );
        connect( m_ui->dpccBox, SIGNAL(DpccLoadTableFromRam()), dev->GetDpccItf(), SLOT(onDpccGetTable()) );
        connect( m_ui->dpccBox, SIGNAL(DpccWriteTableToRam(QVector<int>&,QVector<int>&)), dev->GetDpccItf(), SLOT(onDpccSetTable(QVector<int>&,QVector<int>&)) );

        // Store / Restore in camera Flash memory
        if (deviceFeatures.hasDpccFlash)
        {
            connect( m_ui->dpccBox, SIGNAL(DpccStoreTableInFlash()), dev->GetDpccItf(), SLOT(onDpccSaveTable()) );
            connect( m_ui->dpccBox, SIGNAL(DpccRestoreTableFromFlash()), dev->GetDpccItf(), SLOT(onDpccLoadTable()) );
        }

        // Automatically load / fill the dpcc table
        connect( m_ui->dpccBox, SIGNAL(DpccAutoLoadTable()), dev->GetDpccItf(), SLOT(onDpccAutoLoadTable()) );

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
    if (deviceFeatures.hasSystemRuntime)
    {
        connect( dev->GetProVideoSystemItf(), SIGNAL(RunTimeChanged(uint32_t)), m_ui->infoBox, SLOT(onRunTimeChange(uint32_t)) );
    }

    // reset to factory defaults
    connect( m_ui->infoBox, SIGNAL(ResetToDefaultsClicked()), dev->GetProVideoSystemItf(), SLOT(onResetSettings()) );
    connect( m_ui->infoBox, SIGNAL(ResyncRequest()), this, SLOT(onResyncRequest()) );

    // system settings (serial connection) changed
    connect( m_dlg, SIGNAL(RS232BaudrateChanged(uint32_t)), dev->GetProVideoSystemItf(), SLOT(onRS232BaudRateChange(uint32_t)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(RS232BaudRateChanged(uint32_t)), m_ui->infoBox, SLOT(onRS232BaudrateChange(uint32_t)) );

    connect( m_dlg, SIGNAL(RS485BaudrateChanged(uint32_t)), dev->GetProVideoSystemItf(), SLOT(onRS485BaudRateChange(uint32_t)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(RS485BaudRateChanged(uint32_t)), m_ui->infoBox, SLOT(onRS485BaudrateChange(uint32_t)) );

    connect( m_dlg, SIGNAL(RS485AddressChanged(uint32_t)), dev->GetProVideoSystemItf(), SLOT(onRS485AddressChange(uint32_t)) );
    connect( dev->GetProVideoSystemItf(), SIGNAL(RS485AddressChanged(uint32_t)), m_ui->infoBox, SLOT(onRS485AddressChange(uint32_t)) );

    if (deviceFeatures.hasSystemBroadcast)
    {
        connect( m_dlg, SIGNAL(RS485BroadcastAddressChanged(uint32_t)), dev->GetProVideoSystemItf(), SLOT(onRS485BroadcastAddressChange(uint32_t)) );
        connect( dev->GetProVideoSystemItf(), SIGNAL(RS485BroadcastAddressChanged(uint32_t)), m_ui->infoBox, SLOT(onRS485BroadcastAddressChange(uint32_t)) );

        connect( m_dlg, SIGNAL(RS485BroadcastMasterChanged(int32_t)), dev->GetProVideoSystemItf(), SLOT(onRS485BroadcastMasterChange(int32_t)) );
    }

    connect( m_ui->infoBox, SIGNAL(SystemSettingsChanged(int,int,int,int)), this, SLOT(onSystemSettingsChange(int,int,int,int)) );

    // copy flag
    connect( m_ui->infoBox, SIGNAL(CopyFlagChanged(bool)), this, SLOT(onCopyFlagChange(bool)) );

    // engineering mode
    connect( m_ui->infoBox, SIGNAL(EngineeringModeChanged(bool)), this, SLOT(onEngineeringModeChange(bool)) );
    
    //////////////////////////
    // update
    //////////////////////////
    if (deviceFeatures.hasSystemUpdate)
    {
        // connect update interface slots
        connect( dev->GetProVideoSystemItf(), SIGNAL(SystemPlatformChanged(QString)), m_ui->updBox, SLOT(onSystemPlatformChange(QString)) );
        connect( dev->GetProVideoSystemItf(), SIGNAL(PromptChanged(uint8_t)), m_ui->updBox, SLOT(onPromptChange(uint8_t)) );
        connect( m_ui->updBox, SIGNAL(BootIntoUpdateMode()), dev->GetProVideoSystemItf(), SLOT(onBootIntoUpdateMode()) );

        connect( m_ui->updBox, SIGNAL(CloseSerialConnection()), m_dlg, SLOT(onCloseSerialConnection()) );
        connect( m_ui->updBox, SIGNAL(ReopenSerialConnection()), m_dlg, SLOT(onReopenSerialConnection()) );

        connect( m_ui->updBox, SIGNAL(LockCurrentTabPage(bool)), this, SLOT(onLockCurrentTabPage(bool)) );

        m_ui->updBox->setPortname( m_dlg->getActiveChannel()->getSystemPortName() );
        m_ui->updBox->setBaudrate( m_dlg->getActiveChannel()->getBaudRate() );
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
        /* The broadcast mode can not be set directly on the device, because before that some changes in the serial connection
         * have to be made. Therefore we let the connecion dialog handle this */
        connect( this, SIGNAL(BroadcastChanged(bool)), m_dlg, SLOT(onBroadcastChange(bool)) );
        connect( this, SIGNAL(BroadcastChanged(bool)), m_ui->infoBox, SLOT(onBroadcastChange(bool)) );
    }

    //////////////////////////
    // other signals
    //////////////////////////
    // Get supported resolutions from the device when the next resync() is called
    connect( dev->GetProVideoSystemItf(), SIGNAL(ResolutionMaskChanged(uint32_t,uint32_t,uint32_t)),
             this, SLOT(onResolutionMaskChange(uint32_t,uint32_t,uint32_t)) );

    //////////////////////////
    // Synchronise with the new device
    //////////////////////////
    m_dev->resync();
}

/******************************************************************************
 * MainWindow::onResolutionMaskChange
 *****************************************************************************/
void MainWindow::onResolutionMaskChange( uint32_t id0, uint32_t id1, uint32_t id2 )
{
    bool supported = false;

    m_ui->inoutBox->clearAllVideoModes();

    // fill video-mode combo box
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
            m_ui->inoutBox->addVideoMode( GetVideoModeName( (VideoMode)i ), i );
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
 * MainWindow::setConnectDlg
 *****************************************************************************/
void MainWindow::setConnectDlg( ConnectDialog * dlg )
{
    m_dlg = dlg;

    if ( m_dlg )
    {
        // React if a new device is connected to this window
        connect( m_dlg, SIGNAL(DeviceConnected( ProVideoDevice *)), this, SLOT(onDeviceConnected(ProVideoDevice *)) );

        // React if the connect dialog has to be re-shown
        connect( m_dlg, SIGNAL(OpenConnectDialog()), this, SLOT(onConnectClicked()) );
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
    if ( !m_dlg->connectToRS485DeviceByIndex(index) )
    {
        m_dlg->exec();

        if ( m_dlg->result() == QDialog::Rejected )
        {
            close();
        }
    }
}

/******************************************************************************
 * MainWindow::onSystemSettingsChange
 *****************************************************************************/
void MainWindow::onSystemSettingsChange( int rs232Baudrate, int rs485Baudrate, int rs485Address, int rs485BroadcastAddress )
{
    // Change the comport settings on the device and in the connect dialog
    m_dlg->changeComportSettings(rs232Baudrate, rs485Baudrate, rs485Address, rs485BroadcastAddress);

    // Adjust the devices list
    QVector<ConnectDialog::detectedRS485Device> connectedRS485Devices = m_dlg->getDetectedRS485Devices();
    int currentRS485DeviceIndex = m_dlg->getCurrentRs485DeviceIndex();
    if ( !connectedRS485Devices.empty() && m_cbxConnectedDevices != NULL)
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
 * MainWindow::onConnectClicked
 *****************************************************************************/
void MainWindow::onConnectClicked()
{
    if ( m_dlg )
    {
        m_dlg->exec();

        if ( m_dlg->result() == QDialog::Rejected )
        {
            close();
        }
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
        emit SdiOutChanged( 1 );  m_dev->resync();
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
        emit SdiOutChanged( 2 ); m_dev->resync();
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
            m_dev->resync();
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
            m_dev->resync();
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
        this, tr("Load Multi-Color Profile"),
        directory,
        "Select Profile Files (*.dct);;All files (*.*)"
    );

    if ( NULL != m_filename )
    {
        QFileInfo file( m_filename );
        if ( file.suffix().isEmpty() )
        {
            m_filename += ".dct";
        }

        if ( fileExists(m_filename) )
        {
            QSettings settings( m_filename, QSettings::IniFormat );

            // Load the device name from the settings file
            settings.beginGroup( MAIN_SETTINGS_SECTION_NAME );
            QString deviceName = settings.value( MAIN_SETTINGS_DEVICE_NAME ).toString();
            settings.endGroup();

            // Check if devices match
            if ( deviceName != m_dev->getDeviceName() )
            {
                QApplication::setOverrideCursor( Qt::ArrowCursor );
                QMessageBox msgBox;
                msgBox.setWindowTitle( "Error Loading Settings" );
                QString msgText = QString( "Settings can not be loaded, because the devices mismatch. The file\n\n'%1'\n\ncontains settings for a '%2' device, but the GUI is currently connected to a '%3' device." ).arg( m_filename ).arg( deviceName ).arg( m_dev->getDeviceName() );
                msgBox.setText( msgText );
                msgBox.exec();
            }
            else
            {
                // Load the settings of all visible tabs
                QProgressDialog progressDialog( "Loading Settings...", "", 0, m_activeWidgets.length(), this );
                progressDialog.setCancelButton( NULL );
                progressDialog.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);
                progressDialog.show();

                // sleep for 100ms and refresh progress bar, this ensures that the progress bar is correctly shown under linux
                QThread::msleep( 100 );
                progressDialog.setValue( 0 );
                QApplication::processEvents();

                for ( int i = 0; i < m_activeWidgets.length(); i++ )
                {
                    progressDialog.setValue( i );
                    QApplication::processEvents();
                    m_activeWidgets[i]->load( settings );
                }
                progressDialog.setValue( m_activeWidgets.length() );
                QApplication::processEvents();
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

    // NOTE: It can fail on gtk-systems when an empty filename is given
    //       in the native dialog-box, because GTK sends a SIGSEGV-signal
    //       to process and this is not handled by Qt.
    QFileDialog dialog( this );
    dialog.setDefaultSuffix( "dct" );

    m_filename = dialog.getSaveFileName(
        this, tr("Save Setting Profile"),
        directory,
        "Select Profile Files (*.dct);;All files (*.*)"
    );

    QApplication::setOverrideCursor( Qt::WaitCursor );
    if ( NULL != m_filename )
    {
        QFileInfo file( m_filename );
        if ( file.suffix().isEmpty() )
        {
            m_filename += ".dct";
        }

        QSettings settings( m_filename, QSettings::IniFormat );

        // Write the device name into the settings file
        settings.beginGroup( MAIN_SETTINGS_SECTION_NAME );
        settings.setValue( MAIN_SETTINGS_DEVICE_NAME, m_dev->getDeviceName() );
        settings.endGroup();

        // Save the settings of all active widgets
        for ( int i = 0; i < m_activeWidgets.length(); i++ )
        {
            m_activeWidgets[i]->save( settings );
        }
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
    // Apply the settings of all visible tabs
    QProgressDialog progressDialog( "Synchronising Settings...", "", 0, m_activeWidgets.length(), this );
    progressDialog.setCancelButton( NULL );
    progressDialog.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);
    progressDialog.show();

    // sleep for 100ms and refresh progress bar, this ensures that the progress bar is correctly shown under linux
    QThread::msleep( 100 );
    progressDialog.setValue( 0 );
    QApplication::processEvents();

    for ( int i = 0; i < m_activeWidgets.length(); i++ )
    {
        progressDialog.setValue( i );
        QApplication::processEvents();
        m_activeWidgets[i]->apply();
    }

    progressDialog.setValue( m_activeWidgets.length() );
    QApplication::processEvents();
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
 * MainWindow::onEngineeringModeChange
 *****************************************************************************/
void MainWindow::onEngineeringModeChange( bool value )
{
    DctWidgetBox::Mode m = value ? DctWidgetBox::Advanced : DctWidgetBox::Normal;
    for ( int i = 0; i < m_activeWidgets.length(); i++ )
    {
        m_activeWidgets[i]->setMode( m );
    }
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
