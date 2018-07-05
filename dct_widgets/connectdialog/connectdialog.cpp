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
 * @file    connectdialog.cpp
 *
 * @brief   Implementation of connection dialog to a proVideo device
 *
 *****************************************************************************/
#include <cerrno>
#include <QtDebug>
#include <QMessageBox>
#include <QProgressDialog>
#include <QDesktopWidget>

#include <com_ctrl/ComChannelRSxxx.h>

#include <defines.h>
#include <ProVideoProtocol.h>
#include <ProVideoDevice.h>
#include <XbowDevice.h>
#include <Condor4kDevice.h>
#include <CooperDevice.h>
#include <infodialog.h>

#include "connectdialog.h"
#include "ui_connectdialog.h"

/******************************************************************************
 * local definitions
 *****************************************************************************/
#define CON_DIALOG_INTERFACE        ( "interface" )
#define CON_DIALOG_PORT             ( "port" )
#define CON_DIALOG_SPEED            ( "baudrate" )
#define CON_DIALOG_DATABITS         ( "databits" )
#define CON_DIALOG_PARITY           ( "parity" )
#define CON_DIALOG_STOPBITS         ( "stopbits" )
#define CON_DIALOG_DEVICE_ADDRESS   ( "dev-address" )

/******************************************************************************
 * ConnectDialog::ConnectDialog
 *****************************************************************************/
ConnectDialog::ConnectDialog( QWidget * parent )
    : QDialog( parent )
    , m_ui( new Ui::dlgConnect )
    , m_rs232( new ComChannelRS232() )
    , m_rs485( new ComChannelRS4xx() )
    , m_connectedDevice ( NULL )
    , m_active( NULL )
    , m_detectedRS485Devices()
    , m_currentRS485DeviceIndex( -1 )
    , m_firstStart( true )
{
    // initialize UI
    m_ui->setupUi( this );

    // Set control channels (this fills the combo boxes with values)
    setChannelRS232(m_rs232);
    setChannelRS485(m_rs485);

    // hide combo boxes for data-bit, parity and stop-bit selection (user shall not change those values)
    // RS232
    m_ui->lblDatabitsRS232->setVisible( false );
    m_ui->cbxDatabitsRS232->setVisible( false );
    m_ui->lblParityRS232->setVisible( false );
    m_ui->cbxParityRS232->setVisible( false );
    m_ui->lblStopbitsRS232->setVisible( false );
    m_ui->cbxStopbitsRS232->setVisible( false );

    // RS485
    m_ui->lblDatabitsRS485->setVisible( false );
    m_ui->cbxDatabitsRS485->setVisible( false );
    m_ui->lblParityRS485->setVisible( false );
    m_ui->cbxParityRS485->setVisible( false );
    m_ui->lblStopbitsRS485->setVisible( false );
    m_ui->cbxStopbitsRS485->setVisible( false );

    // create an "ok" button labeled with "connect"
    QPushButton * okButton = m_ui->buttonBox->button(QDialogButtonBox::Ok);
    if ( okButton )
    {
        okButton->setText("Connect");
    }

    // add a rescan button to the button box
    m_rescan = new QPushButton( tr("&Find COM-Ports") );
    m_rescan->setToolTip( "Searches for connected serial port devices." );
    connect( m_rescan, SIGNAL(clicked()), this, SLOT(rescan()) );
    m_ui->buttonBox->addButton( m_rescan, QDialogButtonBox::ResetRole );

    // connect other internal signals
    // Detect Button on RS485 page
    connect( m_ui->btDetect, SIGNAL(clicked()), this, SLOT(onDetectButtonClick()) );
    // Scan Button on RS485 page
    connect( m_ui->btScan, SIGNAL(clicked()), this, SLOT(onScanButtonClick()) );

    setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowTitleHint );

    // Try to load connection settings from file
    QString m_SettingsFile = QDir::homePath() + "/.application.ini";

    if ( fileExists( m_SettingsFile ))
    {
        QSettings settings( m_SettingsFile, QSettings::IniFormat );
        this->loadSettings( settings );
        qDebug() << "loaded connection settings file";
    }
}

/******************************************************************************
 * ConnectDialog::~ConnectDialog
 *****************************************************************************/
ConnectDialog::~ConnectDialog()
{
    delete m_ui;

    delete m_rs232;
    delete m_rs485;

    if (m_connectedDevice != NULL)
    {
        delete m_connectedDevice;
    }
}

/******************************************************************************
 * ConnectDialog::~showEvent
 *****************************************************************************/
void ConnectDialog::showEvent( QShowEvent* event )
{
    // Call inherited function
    QDialog::showEvent( event );

    // Resize window to minimum size
    QApplication::processEvents();
    this->resize( this->minimumSizeHint() );

    // Get last used RS485 and RS232 configs
    m_lastRs232Config = getRs232Config();
    m_lastRs485Config = getRs485Config();
}

/******************************************************************************
 * ConnectDialog::getRs485Config
 *****************************************************************************/
ctrl_channel_rs4xx_open_config_t ConnectDialog::getRs485Config()
{
    ctrl_channel_rs4xx_open_config_t config;

    // create open configuration
    config.idx      = m_ui->cbxPortRS485->itemData( m_ui->cbxPortRS485->currentIndex() ).toInt();
    config.baudrate = m_ui->cbxBaudrateRS485->itemData( m_ui->cbxBaudrateRS485->currentIndex() ).toInt();
    config.data     = m_ui->cbxDatabitsRS485->itemData( m_ui->cbxDatabitsRS485->currentIndex() ).toInt();
    config.parity   = m_ui->cbxParityRS485->itemData( m_ui->cbxParityRS485->currentIndex() ).toInt();
    config.stop     = m_ui->cbxStopbitsRS485->itemData( m_ui->cbxStopbitsRS485->currentIndex() ).toInt();
    config.dev_addr = m_ui->sbxDevAddrRS485->value();

    return config;
}

/******************************************************************************
 * ConnectDialog::setRs485Config
 *****************************************************************************/
void ConnectDialog::setRs485Config( ctrl_channel_rs4xx_open_config_t const config )
{
    m_ui->cbxPortRS485->setCurrentIndex( m_ui->cbxPortRS485->findData(config.idx) );
    m_ui->cbxBaudrateRS485->setCurrentIndex( m_ui->cbxBaudrateRS485->findData(config.baudrate) );
    m_ui->cbxDatabitsRS485->setCurrentIndex( m_ui->cbxDatabitsRS485->findData(config.data) );
    m_ui->cbxParityRS485->setCurrentIndex( m_ui->cbxParityRS485->findData(config.parity) );
    m_ui->cbxStopbitsRS485->itemData( m_ui->cbxStopbitsRS485->findData(config.stop) );
    m_ui->sbxDevAddrRS485->setValue( config.dev_addr );
}

/******************************************************************************
 * ConnectDialog::getRs232Config
 *****************************************************************************/
ctrl_channel_rs232_open_config_t ConnectDialog::getRs232Config()
{
    ctrl_channel_rs232_open_config_t config;

    // create open configuration
    config.idx      = m_ui->cbxPortRS232->itemData( m_ui->cbxPortRS232->currentIndex() ).toInt();
    config.baudrate = m_ui->cbxBaudrateRS232->itemData( m_ui->cbxBaudrateRS232->currentIndex() ).toInt();
    config.data     = m_ui->cbxDatabitsRS232->itemData( m_ui->cbxDatabitsRS232->currentIndex() ).toInt();
    config.parity   = m_ui->cbxParityRS232->itemData( m_ui->cbxParityRS232->currentIndex() ).toInt();
    config.stop     = m_ui->cbxStopbitsRS232->itemData( m_ui->cbxStopbitsRS232->currentIndex() ).toInt();

    return config;
}

/******************************************************************************
 * ConnectDialog::setRs232Config
 *****************************************************************************/
void ConnectDialog::setRs232Config( ctrl_channel_rs232_open_config_t const config )
{
    m_ui->cbxPortRS232->setCurrentIndex( m_ui->cbxPortRS232->findData(config.idx) );
    m_ui->cbxBaudrateRS232->setCurrentIndex( m_ui->cbxBaudrateRS232->findData(config.baudrate) );
    m_ui->cbxDatabitsRS232->setCurrentIndex( m_ui->cbxDatabitsRS232->findData(config.data) );
    m_ui->cbxParityRS232->setCurrentIndex( m_ui->cbxParityRS232->findData(config.parity) );
    m_ui->cbxStopbitsRS232->itemData( m_ui->cbxStopbitsRS232->findData(config.stop) );
}

/******************************************************************************
 * ConnectDialog::fileExists
 *****************************************************************************/
bool ConnectDialog::fileExists( QString & path )
{
    QFileInfo check_file( path );

    // check if file exists and if yes: Is it really a file and no directory?
    return ( check_file.exists() && check_file.isFile() );
}

/******************************************************************************
 * ConnectDialog::getIsConnected
 *****************************************************************************/
bool ConnectDialog::isConnected()
{
    return ( (m_connectedDevice == NULL) ? false : true );
}

/******************************************************************************
 * ConnectDialog::connectWithDevice
 *****************************************************************************/
bool ConnectDialog::connectWithDevice()
{
    bool bOpen = false;
    bool bConnected = false;
    bool bIsKnown = false;
    bool ret = false;

    // Connecting can take a while until it times out, so set cursor to wait
    QApplication::setOverrideCursor( Qt::WaitCursor );

    // I. Try to open the interface with the current settings
    // Close the active connection first (in case we try to reconnect)
    getActiveChannel()->Close();
    bOpen = ( openInterface() == 0 ) ? true : false;
    qDebug() << "open     :" << bOpen;

    // II. Create a generic device and try to connect with it
    ProVideoDevice genericDevice ( getActiveChannel(), new ProVideoProtocol() );
    if ( bOpen )
    {
        int retryCount = 0;
        while ( !bConnected && retryCount < 5 )
        {
            qDebug() << "Trying to connect to device, retry count:" << retryCount;
            bConnected = genericDevice.isConnected();
            retryCount++;
        }
    }
    qDebug() << "connected:" << bConnected;

    // III. Check for known device
    if ( bConnected )
    {
        genericDevice.GetProVideoSystemItf()->GetSystemPlatform();
        bIsKnown = DeviceIsKnown(genericDevice.getSystemPlatform());
    }
    // if not connected, close the active channel
    else if ( bOpen && getActiveChannel() )
    {
        // Close the connection
        getActiveChannel()->Close();
    }
    qDebug() << "known    :" << bIsKnown;

    // IV. Create and initialize a specialized device which matches the HW
    QString systemPlatform = genericDevice.getSystemPlatform();
    if ( bIsKnown )
    {
        ProVideoDevice * connectedDevice = NULL;
        if ( systemPlatform == KNOWN_DEVICE_XBOW )
        {
            connectedDevice = new XbowDevice( getActiveChannel(), new ProVideoProtocol() );
        }
        else if ( systemPlatform == KNOWN_DEVICE_CONDOR4K || systemPlatform.contains(QString(KNOWN_DEVICE_CONDOR4K_MINI)) )
        {
            connectedDevice = new Condor4kDevice( getActiveChannel(), new ProVideoProtocol() );
        }
        else if ( systemPlatform == KNOWN_DEVICE_COOPER )
        {
            connectedDevice = new CooperDevice( getActiveChannel(), new ProVideoProtocol() );
        }
        qDebug() << "connected with:" << systemPlatform;

        if ( connectedDevice != NULL )  // This check should never fail
        {
            // Delete the old device
            if ( m_connectedDevice != NULL )
            {
                delete m_connectedDevice;
            }
            // Set the new device
            m_connectedDevice = connectedDevice;

            // Save settings
            QString m_SettingsFile = QDir::homePath() + "/.application.ini";
            QSettings settings( m_SettingsFile, QSettings::IniFormat );
            saveSettings( settings );

            // Call onBroadcastChange slot with "false", this will disable broadcast mode
            onBroadcastChange( false );

            // Signal that a new device is connected
            emit DeviceConnected( m_connectedDevice );

            // New device successfully connected
            ret = true;
        }
    }
    else
    {
        // Delete the connected device
        if ( m_connectedDevice != NULL )
        {
            delete m_connectedDevice;
        }
        m_connectedDevice = NULL;

        qDebug() << "Unknown device connected:" << systemPlatform;
    }

    // Set cursor to normal again
    QApplication::setOverrideCursor( Qt::ArrowCursor );

    // Display message boxes if the connection could not be established
    if ( !ret)
    {

        QMessageBox msgBox;
        msgBox.setWindowTitle("Connection Error");

        if ( !bConnected )      // Device did not respond
        {
            msgBox.setText("Could not connect to device, the device did not respond. Try another Protocol or Port.");
            msgBox.exec();
        }
        else if ( !bIsKnown )   // Device is unsupported
        {
            msgBox.setText("Could not connect to device, the device is unsupported.");
            msgBox.exec();
        }
    }

    return ret;
}

/******************************************************************************
 * ConnectDialog::setChannelRS232
 *****************************************************************************/
void ConnectDialog::setChannelRS232( ComChannelSerial * c )
{
    if ( c )
    {
        int res;

        ///////////////////////////////////////////////////////////////////////////
        // update serial stuff
        ///////////////////////////////////////////////////////////////////////////
        m_ui->cbxPortRS232->clear();
        m_ui->cbxBaudrateRS232->clear();
        m_ui->cbxDatabitsRS232->clear();
        m_ui->cbxParityRS232->clear();
        m_ui->cbxStopbitsRS232->clear();

        // add found serial ports to combobox
        for ( int i = 0; i < c->getNoPorts(); i++ )
        {
            ctrl_channel_name_t name;
            res = c->getPortName( i, name );
            if ( !res )
            {
                m_ui->cbxPortRS232->addItem( (char *)name, i );
            }
        }

        // add number of data-bit selection to combobox

        // RS232
        /* Note: Slow baudrates below 57600 baud are not supported by the GUI because
         * the delays / wait times get to long for a fluid user experience */
//        m_ui->cbxBaudrateRS232->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_9600)  , CTRL_CHANNEL_BAUDRATE_9600 );
//        m_ui->cbxBaudrateRS232->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_19200) , CTRL_CHANNEL_BAUDRATE_19200 );
//        m_ui->cbxBaudrateRS232->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_38400) , CTRL_CHANNEL_BAUDRATE_38400 );
        m_ui->cbxBaudrateRS232->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_57600) , CTRL_CHANNEL_BAUDRATE_57600 );
        m_ui->cbxBaudrateRS232->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_115200), CTRL_CHANNEL_BAUDRATE_115200 );
        m_ui->cbxBaudrateRS232->setCurrentIndex( m_ui->cbxBaudrateRS232->findData( CTRL_CHANNEL_BAUDRATE_DEFAULT ) );

        // add number of data-bit selection to combobox

        // RS232
        m_ui->cbxDatabitsRS232->addItem( QString::number(CTRL_CHANNEL_DATA_BITS_5), CTRL_CHANNEL_DATA_BITS_5 );
        m_ui->cbxDatabitsRS232->addItem( QString::number(CTRL_CHANNEL_DATA_BITS_6), CTRL_CHANNEL_DATA_BITS_6 );
        m_ui->cbxDatabitsRS232->addItem( QString::number(CTRL_CHANNEL_DATA_BITS_7), CTRL_CHANNEL_DATA_BITS_7 );
        m_ui->cbxDatabitsRS232->addItem( QString::number(CTRL_CHANNEL_DATA_BITS_8), CTRL_CHANNEL_DATA_BITS_8 );
        m_ui->cbxDatabitsRS232->setCurrentIndex( m_ui->cbxDatabitsRS232->findData( CTRL_CHANNEL_DATA_BITS_DEFAULT ) );

        // add number of parity selection to combobox

        // RS232
        m_ui->cbxParityRS232->addItem( "none", CTRL_CHANNEL_PARITY_NONE );
        m_ui->cbxParityRS232->addItem( "odd" , CTRL_CHANNEL_PARITY_ODD );
        m_ui->cbxParityRS232->addItem( "even", CTRL_CHANNEL_PARITY_EVEN );
        m_ui->cbxParityRS232->setCurrentIndex( m_ui->cbxParityRS232->findData( CTRL_CHANNEL_PARITY_DEFAULT ) );

        // add number of stop-bit selection to combobox

        // RS232
        m_ui->cbxStopbitsRS232->addItem( QString::number(CTRL_CHANNEL_STOP_BITS_1), CTRL_CHANNEL_STOP_BITS_1 );
        m_ui->cbxStopbitsRS232->addItem( QString::number(CTRL_CHANNEL_STOP_BITS_2), CTRL_CHANNEL_STOP_BITS_2 );
        m_ui->cbxStopbitsRS232->setCurrentIndex( m_ui->cbxStopbitsRS232->findData( CTRL_CHANNEL_STOP_BITS_DEFAULT ) );
    }
}

/******************************************************************************
 * ConnectDialog::setChannelRS485
 *****************************************************************************/
void ConnectDialog::setChannelRS485( ComChannelSerial * c )
{
    if ( c )
    {
        int res;

        ///////////////////////////////////////////////////////////////////////////
        // update serial stuff
        ///////////////////////////////////////////////////////////////////////////
        m_ui->cbxPortRS485->clear();
        m_ui->cbxBaudrateRS485->clear();
        m_ui->cbxDatabitsRS485->clear();
        m_ui->cbxParityRS485->clear();
        m_ui->cbxStopbitsRS485->clear();

        // add found serial ports to combobox
        for ( int i = 0; i < c->getNoPorts(); i++ )
        {
            ctrl_channel_name_t name;
            res = c->getPortName( i, name );
            if ( !res )
            {
                m_ui->cbxPortRS485->addItem( (char *)name, i );
            }
        }

        // add number of data-bit selection to combobox

        // RS485
        /* Note: Slow baudrates below 57600 baud are not supported by the GUI because
         * the delays / wait times get to long for a fluid user experience */
//        m_ui->cbxBaudrateRS485->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_9600)  , CTRL_CHANNEL_BAUDRATE_9600 );
//        m_ui->cbxBaudrateRS485->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_19200) , CTRL_CHANNEL_BAUDRATE_19200 );
//        m_ui->cbxBaudrateRS485->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_38400) , CTRL_CHANNEL_BAUDRATE_38400 );
        m_ui->cbxBaudrateRS485->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_57600) , CTRL_CHANNEL_BAUDRATE_57600 );
        m_ui->cbxBaudrateRS485->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_115200), CTRL_CHANNEL_BAUDRATE_115200 );
        m_ui->cbxBaudrateRS485->setCurrentIndex( m_ui->cbxBaudrateRS485->findData( CTRL_CHANNEL_BAUDRATE_DEFAULT ) );

        // add number of data-bit selection to combobox

        // RS485
        m_ui->cbxDatabitsRS485->addItem( QString::number(CTRL_CHANNEL_DATA_BITS_5), CTRL_CHANNEL_DATA_BITS_5 );
        m_ui->cbxDatabitsRS485->addItem( QString::number(CTRL_CHANNEL_DATA_BITS_6), CTRL_CHANNEL_DATA_BITS_6 );
        m_ui->cbxDatabitsRS485->addItem( QString::number(CTRL_CHANNEL_DATA_BITS_7), CTRL_CHANNEL_DATA_BITS_7 );
        m_ui->cbxDatabitsRS485->addItem( QString::number(CTRL_CHANNEL_DATA_BITS_8), CTRL_CHANNEL_DATA_BITS_8 );
        m_ui->cbxDatabitsRS485->setCurrentIndex( m_ui->cbxDatabitsRS485->findData( CTRL_CHANNEL_DATA_BITS_DEFAULT ) );

        // add number of parity selection to combobox

        // RS485
        m_ui->cbxParityRS485->addItem( "none", CTRL_CHANNEL_PARITY_NONE );
        m_ui->cbxParityRS485->addItem( "odd" , CTRL_CHANNEL_PARITY_ODD );
        m_ui->cbxParityRS485->addItem( "even", CTRL_CHANNEL_PARITY_EVEN );
        m_ui->cbxParityRS485->setCurrentIndex( m_ui->cbxParityRS485->findData( CTRL_CHANNEL_PARITY_DEFAULT ) );

        // add number of stop-bit selection to combobox

        // RS485
        m_ui->cbxStopbitsRS485->addItem( QString::number(CTRL_CHANNEL_STOP_BITS_1), CTRL_CHANNEL_STOP_BITS_1 );
        m_ui->cbxStopbitsRS485->addItem( QString::number(CTRL_CHANNEL_STOP_BITS_2), CTRL_CHANNEL_STOP_BITS_2 );
        m_ui->cbxStopbitsRS485->setCurrentIndex( m_ui->cbxStopbitsRS485->findData( CTRL_CHANNEL_STOP_BITS_DEFAULT ) );

        m_ui->sbxDevAddrRS485->setRange( 0, MAX_DEVICE_ID );
        m_ui->sbxDevAddrRS485->setValue( 1 );
    }
}

/******************************************************************************
 * ConnectDialog::loadSettings
 *****************************************************************************/
bool ConnectDialog::loadSettings( QSettings &s )
{
    QString string;
    int value;

    Interface iface = (Interface)s.value(CON_DIALOG_INTERFACE, "").toInt();
    if ( setActiveInterface( iface ) )
    {
        return ( false );
    }

    if ( (Rs232 == iface) || (Rs485 == iface) )
    {
        // load serial port settings

        // serial port
        string = s.value(CON_DIALOG_PORT, "").toString();
        if ( setActiveChannelName( iface, string ) )
        {
            return ( false );
        }

        // baudrate
        value = s.value(CON_DIALOG_SPEED, "").toInt();
        if ( setActiveBaudRate( iface, value ) )
        {
            return ( false );
        }

        // number of data-bits
        value = s.value(CON_DIALOG_DATABITS, "").toInt();
        if ( setActiveDataBits( iface, value ) )
        {
            return ( false );
        }

        // parity
        string = s.value(CON_DIALOG_PARITY, "").toString();
        if ( setActiveParity( iface, string ) )
        {
            return ( false );
        }

        // number of stop-bits
        value = s.value(CON_DIALOG_STOPBITS, "").toInt();
        if ( setActiveStopBits( iface, value ) )
        {
            return ( false );
        }

        if ( Rs485 == iface )
        {
            value = s.value(CON_DIALOG_DEVICE_ADDRESS, "").toInt();
            if ( setActiveDeviceAddress( iface, value ) )
            {
                return ( false );
            }
        }

        return ( true );
    }

    // default return
    return ( false );
}

/******************************************************************************
 * ConnectDialog::saveSettings
 *****************************************************************************/
void ConnectDialog::saveSettings( QSettings &s )
{
    Interface iface = getActiveInterface();
    s.setValue( CON_DIALOG_INTERFACE, iface );

    if ( (Rs232 == iface) || (Rs485 == iface) )
    {
        // save serial port settings
        s.setValue( CON_DIALOG_PORT             , getActiveChannelName() );
        s.setValue( CON_DIALOG_SPEED            , getActiveBaudRate() );
        s.setValue( CON_DIALOG_DATABITS         , getActiveDataBits() );
        s.setValue( CON_DIALOG_PARITY           , getActiveParity() );
        s.setValue( CON_DIALOG_STOPBITS         , getActiveStopBits() );

        if ( Rs485 == iface )
        {
            s.setValue( CON_DIALOG_DEVICE_ADDRESS   , getActiveDeviceAddress() );
        }
    }
}

/******************************************************************************
 * ConnectDialog::getActiveInterface
 *****************************************************************************/
ConnectDialog::Interface ConnectDialog::getActiveInterface() const
{
    Interface iface;

    switch ( m_ui->tabController->currentIndex() )
    {
        case 0:
            iface = Rs485;
            break;

        case 1:
            iface = Rs232;
            break;

        default:
            iface = Invalid;
            break;
    }

    return ( iface );
}

/******************************************************************************
 * ConnectDialog::setActiveInterface
 *****************************************************************************/
int ConnectDialog::setActiveInterface( ConnectDialog::Interface iface )
{
    int res = 0;

    if ( (iface > Invalid) && (iface < MaxInterface) )
    {
        m_ui->tabController->setCurrentIndex( (int)iface );

        switch ( iface )
        {
            case Rs232:
                m_active = m_rs232;
                break;

            case Rs485:
                m_active = m_rs485;
                break;

            default:
                m_active = NULL;
                res = -ENODEV;
                break;
        }
    }
    else
    {
        res = -ENODEV;
    }

    return ( res );
}

/******************************************************************************
 * ConnectDialog::getActiveChannelName
 *****************************************************************************/
QString ConnectDialog::getActiveChannelName() const
{
    QString s;

    switch ( getActiveInterface() )
    {
        case Rs232:
            s = m_ui->cbxPortRS232->currentText();
            break;

        case Rs485:
            s = m_ui->cbxPortRS485->currentText();
            break;

        default:
            s = QString::null;
            break;
    }

    return ( s );
}

/******************************************************************************
 * ConnectDialog::setActiveChannelName
 *****************************************************************************/
int ConnectDialog::setActiveChannelName( Interface iface, QString &name )
{
    int idx = -1;

    switch ( iface )
    {
        case Rs232:
            idx = m_ui->cbxPortRS232->findText( name );
            if ( idx != -1 )
            {
                m_ui->cbxPortRS232->setCurrentIndex( idx );
            }
            break;

        case Rs485:
            idx = m_ui->cbxPortRS485->findText( name );
            if ( idx != -1 )
            {
                m_ui->cbxPortRS485->setCurrentIndex( idx );
            }
            break;

        default:
            break;
    }

    return ( (idx < 0) ? -ENODEV : 0 );
}

/******************************************************************************
 * ConnectDialog::getActiveChannelName
 *****************************************************************************/
int ConnectDialog::getActiveBaudRate() const
{
    int v = 0;

    switch ( getActiveInterface() )
    {
        case Rs232:
            v = m_ui->cbxBaudrateRS232->itemData( m_ui->cbxBaudrateRS232->currentIndex() ).toInt();
            break;

        case Rs485:
            v = m_ui->cbxBaudrateRS485->itemData( m_ui->cbxBaudrateRS485->currentIndex() ).toInt();
            break;

        default:
            break;
    }

    return ( v );
}

/******************************************************************************
 * ConnectDialog::setActiveBaudRate
 *****************************************************************************/
int ConnectDialog::setActiveBaudRate( Interface iface, int baudrate )
{
    int idx = -1;

    switch ( iface )
    {
        case Rs232:
            idx = m_ui->cbxBaudrateRS232->findData( baudrate );
            if ( idx != -1 )
            {
                m_ui->cbxBaudrateRS232->setCurrentIndex( idx );
            }
            break;

        case Rs485:
            idx = m_ui->cbxBaudrateRS485->findData( baudrate );
            if ( idx != -1 )
            {
                m_ui->cbxBaudrateRS485->setCurrentIndex( idx );
            }
            break;

        default:
            break;
    }

    return ( (idx < 0) ? -ENODEV : 0 );
}

/******************************************************************************
 * ConnectDialog::getActiveDataBits
 *****************************************************************************/
int ConnectDialog::getActiveDataBits() const
{
    int v = 0;

    switch ( getActiveInterface() )
    {
        case Rs232:
            v = m_ui->cbxDatabitsRS232->itemData( m_ui->cbxDatabitsRS232->currentIndex() ).toInt();
            break;

        case Rs485:
            v = m_ui->cbxDatabitsRS485->itemData( m_ui->cbxDatabitsRS485->currentIndex() ).toInt();
            break;

        default:
            break;
    }

    return ( v );
}

/******************************************************************************
 * ConnectDialog::setActiveDataBits
 *****************************************************************************/
int ConnectDialog::setActiveDataBits( Interface iface, int databits )
{
    int idx = -1;

    switch ( iface )
    {
        case Rs232:
            idx = m_ui->cbxDatabitsRS232->findData( databits );
            if ( idx != -1 )
            {
                m_ui->cbxDatabitsRS232->setCurrentIndex( idx );
            }
            break;

        case Rs485:
            idx = m_ui->cbxDatabitsRS485->findData( databits );
            if ( idx != -1 )
            {
                m_ui->cbxDatabitsRS485->setCurrentIndex( idx );
            }
            break;

        default:
            break;
    }

    return ( (idx < 0) ? -ENODEV : 0 );
}

/******************************************************************************
 * ConnectDialog::getActiveParity
 *****************************************************************************/
QString ConnectDialog::getActiveParity() const
{
    QString s;

    switch ( getActiveInterface() )
    {
        case Rs232:
            s = m_ui->cbxParityRS232->currentText();
            break;

        case Rs485:
            s = m_ui->cbxStopbitsRS485->currentText();
            break;

        default:
            s = QString::null;
            break;
    }

    return ( s );
}

/******************************************************************************
 * ConnectDialog::setActiveParity
 *****************************************************************************/
int ConnectDialog::setActiveParity( Interface iface, QString &name )
{
    int idx = -1;

    switch ( iface )
    {
        case Rs232:
            idx = m_ui->cbxParityRS232->findText( name );
            if ( idx != -1 )
            {
                m_ui->cbxParityRS232->setCurrentIndex( idx );
            }
            break;

        case Rs485:
            idx = m_ui->cbxStopbitsRS485->findText( name );
            if ( idx != -1 )
            {
                m_ui->cbxStopbitsRS485->setCurrentIndex( idx );
            }
            break;

        default:
            break;
    }

    return ( (idx < 0) ? -ENODEV : 0 );
}

/******************************************************************************
 * ConnectDialog::getActiveStopBits
 *****************************************************************************/
int ConnectDialog::getActiveStopBits() const
{
    int v = 0;

    QString s;

    switch ( getActiveInterface() )
    {
        case Rs232:
            v = m_ui->cbxStopbitsRS232->itemData( m_ui->cbxStopbitsRS232->currentIndex() ).toInt();
            break;

        case Rs485:
            v = m_ui->cbxStopbitsRS485->itemData( m_ui->cbxStopbitsRS485->currentIndex() ).toInt();
            break;

        default:
            break;
    }

    return ( v );
}

/******************************************************************************
 * ConnectDialog::setActiveStopBits
 *****************************************************************************/
int ConnectDialog::setActiveStopBits( Interface iface, int stopbits )
{
    int idx = -1;

    switch ( iface )
    {
        case Rs232:
            idx = m_ui->cbxStopbitsRS232->findData( stopbits );
            if ( idx != -1 )
            {
                m_ui->cbxStopbitsRS232->setCurrentIndex( idx );
            }
            break;

        case Rs485:
            idx = m_ui->cbxStopbitsRS485->findData( stopbits );
            if ( idx != -1 )
            {
                m_ui->cbxStopbitsRS485->setCurrentIndex( idx );
            }
            break;

        default:
            break;
    }

    return ( (idx < 0) ? -ENODEV : 0 );
}

/******************************************************************************
 * ConnectDialog::getActiveDeviceAddress
 *****************************************************************************/
int ConnectDialog::getActiveDeviceAddress() const
{
    int v = 0;

    if ( Rs485 == getActiveInterface() )
    {
        v = m_ui->sbxDevAddrRS485->value();
    }

    return ( v );
}

/******************************************************************************
 * ConnectDialog::setActiveDeviceAddress
 *****************************************************************************/
int ConnectDialog::setActiveDeviceAddress( Interface iface, int address )
{
    if ( Rs485 == iface )
    {
        m_ui->sbxDevAddrRS485->setValue( address );
        return ( 0 );
    }

    return ( -ENODEV );
}

/******************************************************************************
 * ConnectDialog::getCurrentRs485DeviceIndex
 *****************************************************************************/
int ConnectDialog::getCurrentRs485DeviceIndex() const
{
    return m_currentRS485DeviceIndex;
}

/******************************************************************************
 * ConnectDialog::getDetectedRS485Devices
 *****************************************************************************/
QVector<ConnectDialog::detectedRS485Device> ConnectDialog::getDetectedRS485Devices() const
{
    return m_detectedRS485Devices;
}

/******************************************************************************
 * ConnectDialog::setCurrentRs485DeviceIndex
 *****************************************************************************/
void ConnectDialog::setCurrentRs485DeviceIndex(int index)
{
    m_currentRS485DeviceIndex = index;
}

/******************************************************************************
 * ConnectDialog::connectToRS485DeviceByIndex
 *****************************************************************************/
bool ConnectDialog::connectToRS485DeviceByIndex( int index )
{
    // Range check of index
    if ( index < 0 || index >= m_detectedRS485Devices.count() )
    {
        return false;
    }

    // Get configuration and set UI elements accordingly
    ctrl_channel_rs4xx_open_config_t openCfg = m_detectedRS485Devices[index].config;
    setRs485Config( openCfg );

    /* set index of the currently connected device to the given index, this is needed
     * for the main window to be able to get the correct entry of the device selection
     * combo box after it reconfigured itself for the new device. */
    setCurrentRs485DeviceIndex( index );

    // Close active channel, open new interface
    getActiveChannel()->Close();
    setActiveInterface( Rs485 );

    // Connect to device
    return connectWithDevice();
}

/******************************************************************************
 * ConnectDialog::updateCurrentDeviceName
 *****************************************************************************/
void ConnectDialog::updateCurrentDeviceName()
{
    m_connectedDevice->GetProVideoSystemItf()->GetDeviceName();
    QString deviceName = m_connectedDevice->getDeviceName();

    if ( !m_detectedRS485Devices.empty() && m_currentRS485DeviceIndex < m_detectedRS485Devices.count() )
    {
        m_detectedRS485Devices[m_currentRS485DeviceIndex].name = deviceName;
    }
}

/******************************************************************************
 * ConnectDialog::openInterface
 *****************************************************************************/
int ConnectDialog::openInterface()
{
    Interface iface = getActiveInterface();
    int res = 0;

    // RS232
    if ( iface == Rs232 )
    {
        // create open configuration
        ctrl_channel_rs232_open_config_t open_cfg = getRs232Config();

        // open channel
        res = m_rs232->Open( (void *)&open_cfg, sizeof(open_cfg) );

        if ( res )
        {
            return ( res );
        }

        // set active interface
        m_active = m_rs232;
    }

    // RS4xx
    else if ( iface == Rs485 )
    {
        // create open configuration
        ctrl_channel_rs4xx_open_config_t open_cfg = getRs485Config();

        // open channel
        res = m_rs485->Open( (void *)&open_cfg, sizeof(open_cfg) );
        if ( res )
        {
            return ( res );
        }

        // set active interface
        m_active = m_rs485;
    }

    return ( res );
}

/******************************************************************************
 * ConnectDialog::detectAndConnect
 *****************************************************************************/
bool ConnectDialog::detectAndConnect()
{
    // Check if there is at least one com port available
    if ( QSerialPortInfo::availablePorts().count() == 0 )
    {
        // Show error message
        QMessageBox msgBox;
        msgBox.setWindowTitle("No Com-Port detected");
        msgBox.setText("There is no Com-Port available in your System. Please connect a USB-to-Serial adapter or equivalent "
                       "device and rescan for new Com-Ports.");
        msgBox.exec();

        // Return false to indicate connection could not be established
        return false;
    }

    // select RS485 as the active interface
    setActiveInterface( Rs485 );

    // clear list of found devices (needed if we try to reconnect, otherwise old items stay in list)
    m_detectedRS485Devices.clear();

    // create initial open configuration with values from the GUI
    ctrl_channel_rs4xx_open_config_t openCfg = getRs485Config();

    // open the com port with initial settings
    if (  getActiveChannel()->Open((void *)&openCfg, sizeof(openCfg)) )
    {
        qDebug() << "Can not open RS485 channel for port " << openCfg.idx << ", address " << openCfg.dev_addr << " and baudrate " << openCfg.baudrate;

        // Show a message box
        QMessageBox msgBox;
        msgBox.setWindowTitle("Can not open Com-Port");
        msgBox.setText("The given Com-Port could not be opened. Please rescan the ports or try using a different port or protocol.");
        msgBox.exec();

        return false;
    }

    // Show a message box to indicate device scan is ongoing
    InfoDialog infoDlg( QString(":/icons/connect_64x64.png"), QString("Detecting Devices..."), this->parentWidget() );
    infoDlg.show();

    // sleep for 100ms and process events, this ensures that the message box is correctly shown under linux
    QThread::msleep( 100 );
    QApplication::processEvents(QEventLoop::WaitForMoreEvents);

    // create a generic device and pass the com port to it
    ProVideoDevice genericDevice ( getActiveChannel(), new ProVideoProtocol() );

    // Find connected devices
    // Set address to the fail safe address, all devices will answer on that address
    openCfg.dev_addr = 100;
    ((ComChannelRS4xx*)getActiveChannel())->setDeviceAddress( openCfg.dev_addr );


    // Get the device list for this baudrate
    // The timeout depends on the baudrate
    uint32_t timeout = 1000;                                        // Default timeout for 115200 baud is 1000ms
    timeout *= CTRL_CHANNEL_BAUDRATE_115200 / openCfg.baudrate;     // Increase timeout for slower baudrates
    timeout += 200;                                                 // Add safety margin

    genericDevice.GetProVideoSystemItf()->GetDeviceList( timeout );
    QList<rs485Device> deviceList = genericDevice.getDeviceList();

    // Loop over the device list and add the devices to the list of connected devices
    for ( int i = 0; i < deviceList.count(); i++ )
    {
        if ( DeviceIsKnown(deviceList.at(i).device_platform) )
        {
            // If the device is known, add it to the list of detected RS485 devices
            detectedRS485Device detectedDevice;

            // Store device parameters in struct
            detectedDevice.name = deviceList.at(i).device_name;
            detectedDevice.platform = deviceList.at(i).device_platform;
            detectedDevice.config = openCfg;
            detectedDevice.config.dev_addr = deviceList.at(i).rs485_address;

            // Get broadcast address and broadcast master flag
            detectedDevice.broadcastAddress = deviceList.at(i).rs485_bc_address;
            detectedDevice.isBroadcastMaster = deviceList.at(i).rs485_bc_master;

            // If the broadcast address equals the device address, this is the broadcast channel, do not add the device
            if ( detectedDevice.config.dev_addr == detectedDevice.broadcastAddress )
            {
                getActiveChannel()->Close();
                qDebug() << "Address" << openCfg.dev_addr << "with baudrate" << openCfg.baudrate << "is a broadcast address, the device is skipped";
                continue;
            }

            // Add device to list of detected devices
            m_detectedRS485Devices.append( detectedDevice );
            qDebug() << "Found a" << detectedDevice.platform << "device with the name" << detectedDevice.name << "connected at address" << detectedDevice.config.dev_addr << "with baudrate " << detectedDevice.config.baudrate;
        }
        else
        {
            qDebug() << "Device" << deviceList.at(i).device_platform << "which is connected at address" << deviceList.at(i).rs485_address << "with baudrate" << openCfg.baudrate << "is unknown";
            continue;
        }
    }

    // Close message box and re-enable ui elements
    infoDlg.close();

    // Open first device in list and connect to it (if list not empty)
    if ( !m_detectedRS485Devices.empty() )
    {
        // Get config and adjust UI elements with new config
        openCfg = m_detectedRS485Devices.first().config;
        setRs485Config( openCfg );

        // Set current device index to 0 (we connected with first device in list)
        setCurrentRs485DeviceIndex( 0 );

        // connect with the device
        return connectWithDevice();
    }
    else
    {

        QMessageBox msgBox;
        msgBox.setWindowTitle( "Auto-Detection failed" );
        msgBox.setWindowIcon( QIcon(":/icons/disconnect_64x64.png") );
        msgBox.setText( "The Auto-Detection could not find any devices which use the RS485 protocol.\n\n"
                        "Try using a different port, baudrate or protocol and make sure that each device has a unique address.\n\n"
                        "Note: After a baudrate change the automatic detection might fail due to garbage in the device buffers. "
                        "In such cases please try starting the automtaic detection again.");
        msgBox.exec();
    }

    // Delete the connected device, no device was found
    if ( m_connectedDevice != NULL )
    {
        delete m_connectedDevice;
    }
    m_connectedDevice = NULL;

    return false;
}

/******************************************************************************
 * ConnectDialog::scanAndConnect
 *****************************************************************************/
bool ConnectDialog::scanAndConnect()
{
    // Check if there is at least one com port available
    if ( QSerialPortInfo::availablePorts().count() == 0 )
    {
        // Show error message
        QMessageBox msgBox;
        msgBox.setWindowTitle("No Com-Port detected");
        msgBox.setText("There is no Com-Port available in your System. Please connect a USB-to-Serial adapter or equivalent "
                       "device and rescan for new Com-Ports.");
        msgBox.exec();

        // Return false to indicate connection could not be established
        return false;
    }

    // Constants
    const int numAddresses = MAX_DEVICE_ID + 1; // 0 is a valid address to, thus: +1
    const int numBaudrates = 2;

    // Array that holds all baudrates that will be scanned
    /* Note: Slow baudrates below 57600 baud are not supported by the GUI because
     * the delays / wait times get to long for a fluid user experience */
    const int baudrates[numBaudrates] = { /* CTRL_CHANNEL_BAUDRATE_9600,
                                           CTRL_CHANNEL_BAUDRATE_19200,
                                           CTRL_CHANNEL_BAUDRATE_38400, */
                                           CTRL_CHANNEL_BAUDRATE_57600,
                                           CTRL_CHANNEL_BAUDRATE_115200 };

    // select RS485 as the active interface
    setActiveInterface( Rs485 );

    // clear list of found devices (needed if we try to reconnect, otherwise old items stay in list)
    m_detectedRS485Devices.clear();

    // create initial open configuration with values from the GUI
    ctrl_channel_rs4xx_open_config_t openCfg = getRs485Config();

    // open the com port with initial settings
    if (  getActiveChannel()->Open((void *)&openCfg, sizeof(openCfg)) )
    {
        qDebug() << "Can not open RS485 channel for port" << openCfg.idx << ", address" << openCfg.dev_addr << "and baudrate" << openCfg.baudrate;

        // Show a message box
        QMessageBox msgBox;
        msgBox.setWindowTitle("Can not open Com-Port");
        msgBox.setText("The given Com-Port could not be opened. Please rescan the ports or try using a different port or protocol.");
        msgBox.exec();

        return false;
    }

    // create a generic device and pass the com port to it
    ProVideoDevice genericDevice ( getActiveChannel(), new ProVideoProtocol() );

    // Show a progress bar
    this->setEnabled(false);
    QProgressDialog progressDialog( "Scanning...\nDevices found: 0", "Stop Scan", 0, numAddresses * numBaudrates, this );
    progressDialog.setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint );

    // sleep for 100ms and refresh progress bar, this ensures that the progress bar is correctly shown under linux
    QThread::msleep( 100 );
    progressDialog.setValue( 0 );
    QApplication::processEvents();

    /* If the connect dialog is not visible (this is the case when the programm is started)
     * show the progress bar in the middle of the screen, instead of in the middle of the connect dialog.
     * This fixes a problem where the progress bar is schon in the top left corner of the screen. */
    if ( !this->isVisible() )
    {
        const QRect screen = QApplication::desktop()->screenGeometry();
        progressDialog.move( screen.center() - progressDialog.rect().center() );
    }

    progressDialog.open();

    // Scan address 0 to numAddresses for connected devices
    for ( int address = 0; address < numAddresses; address++ )
    {
        // Scan all available baudrates
        for ( int baudrateIndex = 0; baudrateIndex < numBaudrates; baudrateIndex++ )
        {
            // I. Adjust progress bar, check if scan was aborted
            progressDialog.setValue( (address - 1) * numBaudrates + baudrateIndex );
            QApplication::processEvents();

            // check if scan was aborted
            if ( progressDialog.wasCanceled() )
            {
                // break to outer loop, check cancel there as well
                break;
            }

            // II. Set channel and baudrate of the comport
            openCfg.dev_addr = address;
            openCfg.baudrate = baudrates[baudrateIndex];

            ((ComChannelRS4xx*)getActiveChannel())->setDeviceAddress( openCfg.dev_addr );
            getActiveChannel()->setBaudRate( openCfg.baudrate);
            getActiveChannel()->ReOpen();   // After baudrate change, com port has to be reopened

            // III. Try to connect with the device
            int retryCount = 0;
            const int numRetrys = 3;
            while ( !genericDevice.isConnected() && retryCount < numRetrys )
            {
                retryCount++;
            }
            if ( retryCount >= numRetrys )
            {
                qDebug() << "No device connected at address" << openCfg.dev_addr << "with baudrate" << openCfg.baudrate;
                continue;
            }

            // IV. Get the device platform and find out if it is a known device
            genericDevice.GetProVideoSystemItf()->GetSystemPlatform();
            QString systemPlatform = genericDevice.getSystemPlatform();
            if ( !DeviceIsKnown(systemPlatform) )
            {
                qDebug() << "Device" << systemPlatform << "which is connected at address" << openCfg.dev_addr << "with baudrate" << openCfg.baudrate << "is unknown";
                continue;
            }

            // V. If the device is known, add it to the list of detected RS485 devices
            detectedRS485Device detectedDevice;

            // Get the (user defined) device name
            genericDevice.GetProVideoSystemItf()->GetDeviceName();
            QString deviceName = genericDevice.getDeviceName();

            // Store device parameters in struct
            detectedDevice.name = deviceName;
            detectedDevice.platform = systemPlatform;
            detectedDevice.config = openCfg;

            // Get broadcast address and broadcast master flag from device
            genericDevice.GetProVideoSystemItf()->GetRS485BroadcastAddress();
            detectedDevice.broadcastAddress = genericDevice.getBroadcastAddress();
            genericDevice.GetProVideoSystemItf()->GetRS485BroadcastMaster();
            detectedDevice.isBroadcastMaster = genericDevice.getBroadcastMasterMode();

            // If the broadcast address equals the device address, this is the broadcast channel, do not add the device
            if ( detectedDevice.config.dev_addr == detectedDevice.broadcastAddress )
            {
                getActiveChannel()->Close();
                qDebug() << "Address" << openCfg.dev_addr << "with baudrate" << openCfg.baudrate << "is a broadcast address, the device is skipped";
                continue;
            }

            // Add device to list of detected devices
            m_detectedRS485Devices.append( detectedDevice );
            qDebug() << "Found a" << systemPlatform << "device with the name" << deviceName << "connected at address" << openCfg.dev_addr << "with baudrate" << openCfg.baudrate;

            // Update progress dialog text
            progressDialog.setLabelText( QString("Scanning...\nDevices found: %1").arg(m_detectedRS485Devices.count()) );
            QApplication::processEvents();

            // If a device was found for this address, do not scan the other baudrates (there can not be two device with the same address)
            break;
        }

        // check if scan was aborted
        if ( progressDialog.wasCanceled() )
        {
            break;
        }
    }

    // Set progress bar to 100%
    if ( !progressDialog.wasCanceled() )
    {
        progressDialog.setValue( numAddresses * numBaudrates );
        QApplication::processEvents();
    }

    // Renable ui elements
    this->setEnabled( true );

    // Open first device in list and connect to it (if list not empty)
    if ( !m_detectedRS485Devices.empty() )
    {
        // Get config and adjust UI elements with new config
        openCfg = m_detectedRS485Devices.first().config;
        setRs485Config( openCfg );

        // Set current device index to 0 (we connected with first device in list)
        setCurrentRs485DeviceIndex( 0 );

        // connect with the device
        return connectWithDevice();
    }
    // If the scan was not aborted and no device was found, show message box
    else if ( !progressDialog.wasCanceled() )
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle( "Auto-Detection failed" );
        msgBox.setWindowIcon( QIcon(":/icons/connect_128x128.png") );
        msgBox.setText( "The Auto-Detection could not find any devices which use the RS485 protocol. Try using a different port or protocol and "
                        "make sure that each device has a unique address.");
        msgBox.exec();
    }

    // Delete connected device (no device was found)
    if ( m_connectedDevice != NULL )
    {
        delete m_connectedDevice;
    }
    m_connectedDevice = NULL;

    return false;
}

/******************************************************************************
 * ConnectDialog::changeComportSettings
 *****************************************************************************/
void ConnectDialog::changeComportSettings( int rs232Baudrate, int rs485Baudrate, int rs485Address, int rs485BroadcastAddress )
{
    // Change RS232 Settings
    // Emit a change event to change the baudrate on the device
    emit RS232BaudrateChanged( rs232Baudrate );

    // If we are currently connected to the device over RS232, we have to change the com ports baudrate
    if ( getActiveInterface() == Rs232 )
    {
        getActiveChannel()->setBaudRate( rs232Baudrate);
        getActiveChannel()->ReOpen();   // After baudrate change, com port has to be reopened
    }

    // Change RS485 Settings
    // Emit a change event to change the baudrate on the device
    emit RS485BaudrateChanged( rs485Baudrate );

    // If we are currently connected to the device over RS485, we have to change the com ports baudrate
    if ( getActiveInterface() == Rs485 )
    {
        getActiveChannel()->setBaudRate( rs485Baudrate);
        getActiveChannel()->ReOpen();   // After baudrate change, com port has to be reopened
    }

    // Emit a change event to change the address on the device
    emit RS485AddressChanged( rs485Address );

    // If we are currently connected to the device over RS485, we have to change the com ports address
    if ( getActiveInterface() == Rs485 )
    {
        ((ComChannelRS4xx*)getActiveChannel())->setDeviceAddress( rs485Address );
    }

    // Emit a change event to change the broadcast address on the device
    emit RS485BroadcastAddressChanged( rs485BroadcastAddress );

    // Setup connect dialog with new values
    // RS232
    ctrl_channel_rs232_open_config_t rs232Config = getRs232Config();
    rs232Config.baudrate = rs232Baudrate;
    setRs232Config( rs232Config );

    // RS485
    ctrl_channel_rs4xx_open_config_t rs485Config = getRs485Config();
    rs485Config.baudrate = rs485Baudrate;
    rs485Config.dev_addr = rs485Address;
    setRs485Config( rs485Config );

    // If we are connected after a scan operation, update the list of detected devices
    if ( !m_detectedRS485Devices.empty() )
    {
        m_detectedRS485Devices[m_currentRS485DeviceIndex].config.baudrate = rs485Baudrate;
        m_detectedRS485Devices[m_currentRS485DeviceIndex].config.dev_addr = rs485Address;
        m_detectedRS485Devices[m_currentRS485DeviceIndex].broadcastAddress = rs485BroadcastAddress;

        // Rearrange the list, so that it is sorted by device address in ascending order
        std::sort( m_detectedRS485Devices.begin(), m_detectedRS485Devices.end(),
                   [](const detectedRS485Device & a, const detectedRS485Device & b)
                   { return a.config.dev_addr < b.config.dev_addr; } );

        // Find index of the device in the list
        m_currentRS485DeviceIndex = std::find_if( m_detectedRS485Devices.begin(), m_detectedRS485Devices.end(),
                                                  [rs485Address](const detectedRS485Device & a)
                                                  { return a.config.dev_addr == rs485Address; } ) - m_detectedRS485Devices.begin();
    }
}

/******************************************************************************
 * ConnectDialog::onDetectButtonClick
 *****************************************************************************/
void ConnectDialog::onDetectButtonClick()
{
    if ( detectAndConnect() )
    {
        // call derived function to close
        QDialog::accept();
    }
}

/******************************************************************************
 * ConnectDialog::onScanButtonClick
 *****************************************************************************/
void ConnectDialog::onScanButtonClick()
{
    if ( scanAndConnect() )
    {
        // call derived function to close
        QDialog::accept();
    }
}

/******************************************************************************
 * ConnectDialog::onBroadcastChange
 *****************************************************************************/
void ConnectDialog::onBroadcastChange( bool enabled )
{
    // Check if detected device list is empty. This prevents segmentation faults when the programm is closing
    if ( m_detectedRS485Devices.empty() )
    {
        return;
    }

    // Check if we are connected over RS485 (broadcast not possible over RS232)
    if ( getActiveInterface() == Rs485 )
    {
        if ( enabled )
        {
            // Check whether all devices in this broadcast group are identical, otherwise show a info message
            QString currentDevicePlatform = m_detectedRS485Devices[m_currentRS485DeviceIndex].platform;
            unsigned int currentDeviceBroadcastAddress = m_detectedRS485Devices[m_currentRS485DeviceIndex].broadcastAddress;
            for ( int i = 0; i < m_detectedRS485Devices.count(); i++ )
            {
                // Skip current device
                if ( i == m_currentRS485DeviceIndex )
                {
                    continue;
                }

                // Check for same broadcast group but different device platform
                if ( m_detectedRS485Devices[i].broadcastAddress == currentDeviceBroadcastAddress &&
                     m_detectedRS485Devices[i].platform != currentDevicePlatform )
                {
                    // Before showing message box, revert to normal cursor
                    Qt::CursorShape currentCursorShape = QApplication::overrideCursor()->shape();
                    QApplication::setOverrideCursor( Qt::ArrowCursor );

                    // Show message box
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("Different Device Models in Broadcast Group");
                    msgBox.setText("Devices of different models are assigend to the current broadcast group.\n\n"
                                   "Please keep in mind that devices that do not match the currently selected device "
                                   "may not support all of the features displayed in the user interface.");
                    msgBox.exec();

                    // Revert to former cursor style
                    QApplication::setOverrideCursor( currentCursorShape );

                    break;
                }
            }

            // Change the address of the Com-Port to the broadcast address
            ((ComChannelRS4xx*)getActiveChannel())->setDeviceAddress( m_detectedRS485Devices[m_currentRS485DeviceIndex].broadcastAddress );

            /* Switch broadcast master and try to connect with it, this might take several trys because
             * there can be garbage left in the tx/rx buffers of the device */
            const int numRetrys = 5;
            bool connected = false;
            for ( int i = 0; i < numRetrys; i++ )
            {
                // Make the current device the broadcast master for alle devices that use this broadcast address
                emit RS485BroadcastMasterChanged( m_detectedRS485Devices[m_currentRS485DeviceIndex].config.dev_addr );

                if ( m_connectedDevice->isConnected() )
                {
                    connected = true;
                    break;
                }
            }

            if ( !connected )
            {
                qDebug() << "Can not enable broadcast mode";
            }
        }
        else
        {
            // Change the address of the Com-Port to the broadcast address to make sure all devices receive the command
            ((ComChannelRS4xx*)getActiveChannel())->setDeviceAddress( m_detectedRS485Devices[m_currentRS485DeviceIndex].broadcastAddress );

            // Disable broadcast master on all devices
            emit RS485BroadcastMasterChanged( -1 );

            // Change the address of the Com-Port to the device address
            ((ComChannelRS4xx*)getActiveChannel())->setDeviceAddress( m_detectedRS485Devices[m_currentRS485DeviceIndex].config.dev_addr );
        }
    }
}

/******************************************************************************
 * ConnectDialog::onCloseSerialConnection
 *****************************************************************************/
void ConnectDialog::onCloseSerialConnection( void )
{
    getActiveChannel()->Close();
}

/******************************************************************************
 * ConnectDialog::onReopenSerialConnection
 *****************************************************************************/
void ConnectDialog::onReopenSerialConnection( void )
{
    // Show message box while trying to reconnect
    QMessageBox msgBox;
    msgBox.setWindowTitle( "Restarting Device" );
    msgBox.setText( "The update was successful and the device is now restarting.\n\n"
                    "It can take up to two minutes until the device is accessible again, please be patient." );
    msgBox.setWindowModality( Qt::ApplicationModal );
    msgBox.setStandardButtons( 0 );
    msgBox.show();
    QApplication::processEvents();

    // Reopen the serial port
    getActiveChannel()->ReOpen();

    // Try to connect with the device, maximum wait time 120 * 1s = 2min
    int retryCount = 0;
    const int numRetrys = 120;
    while ( !m_connectedDevice->isConnected() && retryCount < numRetrys )
    {
        // wait 1s before retry
        QApplication::processEvents();
        QThread::sleep(1);
        retryCount++;
    }

    // Reconnect failed
    if ( retryCount >= numRetrys )
    {
        // Close message box
        msgBox.close();

        // Set normal cursor
        QApplication::setOverrideCursor( Qt::ArrowCursor );
        QApplication::processEvents();

        // Show message box stating that reconnect was not possible
        QMessageBox msgBoxError;
        msgBoxError.setWindowTitle("Device not found");
        msgBoxError.setText( "Could not reconnect to the device after the update. Please power-cycle the device and "
                             "try to reconnect.");
        msgBoxError.exec();

        // Tell main window to open the connect dialog
        emit OpenConnectDialog();
    }

    // Reconnect was successful
    else
    {
        // Resync connected device
        m_connectedDevice->resync();

        // Close message box
        msgBox.close();

        // set normal cursor
        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }
}

/******************************************************************************
 * ConnectDialog::accept
 *****************************************************************************/
void ConnectDialog::accept()
{
    // Check if there is at least one com port available
    if ( QSerialPortInfo::availablePorts().count() == 0 )
    {
        // Show error message
        QMessageBox msgBox;
        msgBox.setWindowTitle("No Com-Port detected");
        msgBox.setText("There is no Com-Port available in your System. Please connect a USB-to-Serial adapter or equivalent "
                       "device and rescan for new Com-Ports.");
        msgBox.exec();
    }
    else
    {
        // Clear the device list, we only connect with one device now
        m_detectedRS485Devices.clear();

        // Set current device index to -1 to indicate that no device is selected
        setCurrentRs485DeviceIndex( -1 );

        // Try to connect with the device
        if ( connectWithDevice() )
        {
            // GUI was successfully connected, next time the connect dialog is opend will not be the first start
            m_firstStart = false;

            // call derived function to close
            QDialog::accept();
        }
    }
}

/******************************************************************************
 * ConnectDialog::reject
 *****************************************************************************/
void ConnectDialog::reject()
{
    // Check if a connection is established
    if ( this->isConnected() )
    {
        // Check if connection settings got changed by the user
        ctrl_channel_rs232_open_config_t currentRs232Config = getRs232Config();
        ctrl_channel_rs4xx_open_config_t currentRs485Config = getRs485Config();

        if ( m_lastRs232Config.baudrate != currentRs232Config.baudrate ||
             m_lastRs232Config.data != currentRs232Config.data         ||
             m_lastRs232Config.idx != currentRs232Config.idx           ||
             m_lastRs232Config.parity != currentRs232Config.parity     ||
             m_lastRs232Config.stop != currentRs232Config.stop         ||
             m_lastRs485Config.baudrate != currentRs485Config.baudrate ||
             m_lastRs485Config.dev_addr != currentRs485Config.dev_addr ||
             m_lastRs485Config.data != currentRs485Config.data         ||
             m_lastRs485Config.idx != currentRs485Config.idx           ||
             m_lastRs485Config.parity != currentRs485Config.parity     ||
             m_lastRs485Config.stop != currentRs485Config.stop         )
        {
            /* If the settings got changed tell the user that he can either go back and
             * connect to a device or quit the GUI */
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question( this, "Restore previous Com-Port Settings?",
                                           "The Com-Port settings where changed. To return to the main window the previous settings "
                                           "have to be restored.\n\n"
                                           "Click 'Yes' to restore the previous settings and return to the main window, click 'No' "
                                           "to stay in the connect dialog and connect with a new device.",
                                           QMessageBox::Yes | QMessageBox::No );

            if ( reply == QMessageBox::No )
            {
                // Do nothing to stay in the connect dialog
            }
            else
            {
                // Restore settings, return to main window
                setRs232Config( m_lastRs232Config );
                setRs485Config( m_lastRs485Config );
                QDialog:: accept();
            }
        }
        else
        {
            // Settins were not changed, close the connect dialog and return to the main window
            QDialog::accept();
        }
    }
    // If no connection is established (e.g. connection failed) and this is not the first start of the GUI
    else if ( !m_firstStart )
    {
        // Ask user if he wants to leave the GUI
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question( this, "Close GUI?",
                                       "The GUI is not connected to any device. Closing the connect dialog will also also close the GUI.\n\n"
                                       "Click 'Yes' to close the GUI, click 'No' to stay in the connect dialog.",
                                       QMessageBox::Yes | QMessageBox::No );

        if ( reply == QMessageBox::No )
        {
            // Do nothing to stay in the connect dialog
        }
        else
        {
            // Call reject to close the main window
            QDialog::reject();
        }
    }
    // No connection is established, but this is the first start (main window was never shown)
    else
    {
        // Call reject to close the main window
        QDialog::reject();
    }
}

/******************************************************************************
 * ConnectDialog::rescan
 *****************************************************************************/
void ConnectDialog::rescan()
{
    setChannelRS232( m_rs232 );
    setChannelRS485( m_rs485 );
}
