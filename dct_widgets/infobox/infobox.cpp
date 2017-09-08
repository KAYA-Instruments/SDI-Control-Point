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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/
/**
 * @file    infobox.cpp
 *
 * @brief   Implementation of system information box 
 *
 *****************************************************************************/
#include <QtDebug>
#include <QMessageBox>

#include <textviewer.h>

#include <com_ctrl/ComChannelRSxxx.h>

#include "infobox.h"
#include "ui_infobox.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_InfoBox;
}

/******************************************************************************
 * InfoBox::PrivateData
 *****************************************************************************/
class InfoBox::PrivateData
{
public:
    PrivateData()
        : m_ui( new Ui::UI_InfoBox )
    {
        // do nothing
    };

    ~PrivateData()
    {
        delete m_ui;
    };

    Ui::UI_InfoBox *    m_ui;                   /**< ui handle */
};

/******************************************************************************
 * InfoBox::InfoBox
 *****************************************************************************/
InfoBox::InfoBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData;

    // initialize UI
    d_data->m_ui->setupUi( this );

    // fill GUI version line edit
    d_data->m_ui->letGuiVersion->setText(VERSION_STRING);

    // hide UI elements for sw and hw mask, they are of no interest for the user
    d_data->m_ui->lblFeatureMaskSw->setVisible( false );
    d_data->m_ui->letFeatureMaskSw->setVisible( false );
    d_data->m_ui->lblFeatureMaskHw->setVisible( false );
    d_data->m_ui->letFeatureMaskHw->setVisible( false );

    // add baudrates to baudrate combo boxes
    /* Note: Slow baudrates below 57600 baud are not supported by the GUI because
     * the delays / wait times get to long for a fluid user experience */
//    d_data->m_ui->cbxRS232Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_9600)  , CTRL_CHANNEL_BAUDRATE_9600 );
//    d_data->m_ui->cbxRS232Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_19200) , CTRL_CHANNEL_BAUDRATE_19200 );
//    d_data->m_ui->cbxRS232Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_38400) , CTRL_CHANNEL_BAUDRATE_38400 );
    d_data->m_ui->cbxRS232Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_57600) , CTRL_CHANNEL_BAUDRATE_57600 );
    d_data->m_ui->cbxRS232Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_115200), CTRL_CHANNEL_BAUDRATE_115200 );
    d_data->m_ui->cbxRS232Baudrate->setCurrentIndex( d_data->m_ui->cbxRS232Baudrate->findData( CTRL_CHANNEL_BAUDRATE_DEFAULT ) );

//    d_data->m_ui->cbxRS485Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_9600)  , CTRL_CHANNEL_BAUDRATE_9600 );
//    d_data->m_ui->cbxRS485Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_19200) , CTRL_CHANNEL_BAUDRATE_19200 );
//    d_data->m_ui->cbxRS485Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_38400) , CTRL_CHANNEL_BAUDRATE_38400 );
    d_data->m_ui->cbxRS485Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_57600) , CTRL_CHANNEL_BAUDRATE_57600 );
    d_data->m_ui->cbxRS485Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_115200), CTRL_CHANNEL_BAUDRATE_115200 );
    d_data->m_ui->cbxRS485Baudrate->setCurrentIndex( d_data->m_ui->cbxRS485Baudrate->findData( CTRL_CHANNEL_BAUDRATE_DEFAULT ) );

    // connect system settings
    connect( d_data->m_ui->btnResetToDefaults, SIGNAL(clicked(bool)), this, SLOT(onBtnResetToDefaultsClicked()) );
    connect( d_data->m_ui->btnApplySerialPortSettings, SIGNAL(clicked(bool)), this, SLOT(onBtnApplySerialPortSettingsClicked()) );

    // connect control settings
    connect( d_data->m_ui->cbxEngineeringMode, SIGNAL(stateChanged(int)), this, SLOT(onCbxEngineeringModeChange(int)) );

    // connect software license dialogs
    connect ( d_data->m_ui->btnShowLicense, SIGNAL(clicked(bool)), this, SLOT(onShowLicenseClicked()) );
    connect ( d_data->m_ui->btnShowThirdPartyLicenses, SIGNAL(clicked(bool)), this, SLOT(onShowThirdPartyLicensesClicked()) );

    // FIXME: THIS IS TEMPORARY UNTIL THE GUI IS RELEASED OPEN SOURCE
    // Do not show license group box
//    d_data->m_ui->gpxLicenses->setVisible( false );
}

/******************************************************************************
 * InfoBox::~InfoBox
 *****************************************************************************/
InfoBox::~InfoBox()
{
    delete d_data;
}

/******************************************************************************
 * InfoBox::prepareMode
 *****************************************************************************/
void InfoBox::prepareMode( const Mode )
{
    // do nothing here
}

/******************************************************************************
 * InfoBox::loadSettings
 *****************************************************************************/
void InfoBox::loadSettings( QSettings & )
{
    // do nothing here
}

/******************************************************************************
 * InfoBox::saveSettings
 *****************************************************************************/
void InfoBox::saveSettings( QSettings & )
{
    // do nothing here
}

/******************************************************************************
 * InfoBox::applySettings
 *****************************************************************************/
void InfoBox::applySettings( void )
{
    // do nothing here
}

/******************************************************************************
 * InfoBox::setRuntimeVisibility
 *****************************************************************************/
void InfoBox::setRuntimeVisible( const bool value )
{
    d_data->m_ui->lblRuntime->setVisible(value);
    d_data->m_ui->tetRuntime->setVisible(value);
}

/******************************************************************************
 * InfoBox::onDeviceNameChange
 *****************************************************************************/
void InfoBox::onDeviceNameChange( QString name )
{
    d_data->m_ui->letDeviceName->setText( name );
}

/******************************************************************************
 * InfoBox::setBroadcastSettingsVisible
 *****************************************************************************/
void InfoBox::setBroadcastSettingsVisible( const bool value )
{
    d_data->m_ui->lblNote->setVisible( value );
    d_data->m_ui->lblRS485BroadcastAddress->setVisible( value );
    d_data->m_ui->sbxRS485BroadcastAddress->setVisible( value );
}

/******************************************************************************
 * InfoBox::setRS232SettingsVisible
 *****************************************************************************/
void InfoBox::setRS232SettingsVisible( const bool value )
{
    d_data->m_ui->lblRS232Baudrate->setVisible( value );
    d_data->m_ui->cbxRS232Baudrate->setVisible( value );
}

/******************************************************************************
 * InfoBox::onDeviceIdChange
 *****************************************************************************/
void InfoBox::onDeviceIdChange( uint32_t id0, uint32_t id1, uint32_t id2, uint32_t id3 )
{
    QString s;
    s.sprintf( "0x%08x-0x%08x-0x%08x-0x%08x", id0, id1, id2, id3 );
    d_data->m_ui->letSystemId->setText( s );
}

/******************************************************************************
 * InfoBox::onSystemValidityChange
 *****************************************************************************/
void InfoBox::onSystemValidityChange( QString validity )
{
    d_data->m_ui->letSystemValidity->setText( validity );
}

/******************************************************************************
 * InfoBox::onBitStreamVersionChange
 *****************************************************************************/
void InfoBox::onBitStreamVersionChange( uint32_t version )
{
    QString s;
    s.sprintf( "0x%x", version );
    d_data->m_ui->letBitstreamId->setText( s );
}

/******************************************************************************
 * InfoBox::onApplicationVersionChange
 *****************************************************************************/
void InfoBox::onApplicationVersionChange( QString version )
{
    d_data->m_ui->letSoftwareId->setText( version );
}

/******************************************************************************
 * InfoBox::onApplicationReleaseDateChange
 *****************************************************************************/
void InfoBox::onApplicationReleaseDateChange( QString date )
{
    d_data->m_ui->letSoftwareReleaseDate->setText( date );
}

/******************************************************************************
 * InfoBox::onApplicationBuildDateChange
 *****************************************************************************/
void InfoBox::onApplicationBuildDateChange( QString date )
{
    d_data->m_ui->letSoftwareBuildDate->setText( date );
}

/******************************************************************************
 * InfoBox::onBootloaderVersionChange
 *****************************************************************************/
void InfoBox::onBootloaderVersionChange( uint32_t version )
{
    QString s;
    s.sprintf( "%d", version );
    d_data->m_ui->letLoaderId->setText( s );
}

/******************************************************************************
 * InfoBox::onFeatureMaskHwChange
 *****************************************************************************/
void InfoBox::onFeatureMaskHwChange( uint32_t mask )
{
    QString s;
    s.sprintf( "0x%08x", mask );
    d_data->m_ui->letFeatureMaskHw->setText( s );
}

/******************************************************************************
 * InfoBox::onFeatureMaskHwListChange
 *****************************************************************************/
void InfoBox::onFeatureMaskHwListChange( QStringList features )
{
    d_data->m_ui->lstFeatureMaskHw->clear();
    d_data->m_ui->lstFeatureMaskHw->addItems( features );
}

/******************************************************************************
 * InfoBox::onFeatureMaskSwChange
 *****************************************************************************/
void InfoBox::onFeatureMaskSwChange( uint32_t mask )
{
    QString s;
    s.sprintf( "0x%08x", mask );
    d_data->m_ui->letFeatureMaskSw->setText( s );
}

/******************************************************************************
 * InfoBox::onRunTimeChange
 *****************************************************************************/
void InfoBox::onRunTimeChange( uint32_t seconds )
{
    QTime t( 0, 0 , 0 );
    d_data->m_ui->tetRuntime->setTime( t.addSecs( seconds ) );
}

/******************************************************************************
 * InfoBox::onRS232BaudrateChange
 *****************************************************************************/
void InfoBox::onRS232BaudrateChange( uint32_t baudrate )
{
    d_data->m_ui->cbxRS232Baudrate->setCurrentIndex( d_data->m_ui->cbxRS232Baudrate->findData( baudrate ) );
}

/******************************************************************************
 * InfoBox::onRS485BaudrateChange
 *****************************************************************************/
void InfoBox::onRS485BaudrateChange( uint32_t baudrate )
{
    d_data->m_ui->cbxRS485Baudrate->setCurrentIndex( d_data->m_ui->cbxRS485Baudrate->findData( baudrate ) );
}

/******************************************************************************
 * InfoBox::onRS485AddressChange
 *****************************************************************************/
void InfoBox::onRS485AddressChange( uint32_t address )
{
    d_data->m_ui->sbxRS485Address->setValue( address );
}

/******************************************************************************
 * InfoBox::onRS485BroadcastAddressChange
 *****************************************************************************/
void InfoBox::onRS485BroadcastAddressChange( uint32_t address )
{
    d_data->m_ui->sbxRS485BroadcastAddress->setValue( address );
}

/******************************************************************************
 * InfoBox::onRS485BroadcastAddressChange
 *****************************************************************************/
void InfoBox::onBroadcastChange( bool enable )
{
    // In broadcast mode, show a note that some settings are unavailable
    d_data->m_ui->lblNote->setVisible( enable );

    /* Reset to factory defaults is disabled in broadcast mode, otherwise all
     * devices will be reset to the same devie address, making them unnaccessible */
    d_data->m_ui->btnResetToDefaults->setEnabled( !enable );

    /* All serial interface settings if broadcast mode is active, otherwise the
     * same address could be set to multiple devices! */
    d_data->m_ui->cbxRS232Baudrate->setEnabled( !enable );
    d_data->m_ui->cbxRS485Baudrate->setEnabled( !enable );
    d_data->m_ui->sbxRS485Address->setEnabled( !enable );
    d_data->m_ui->sbxRS485BroadcastAddress->setEnabled( !enable );
    d_data->m_ui->btnApplySerialPortSettings->setEnabled( !enable );
}

/******************************************************************************
 * InfoBox::onResetToDefaultsClicked
 *****************************************************************************/
void InfoBox::onBtnResetToDefaultsClicked()
{
    setWaitCursor();

    // Send reset settings command
    emit ResetToDefaultsClicked();

    // Reset all settings
    emit ResyncRequest();

    setNormalCursor();
}

/******************************************************************************
 * InfoBox::onBtnApplySerialPortSettingsClicked
 *****************************************************************************/
void InfoBox::onBtnApplySerialPortSettingsClicked()
{
    setWaitCursor();

    // Get data from the ui elements
    int rs232Baudrate = d_data->m_ui->cbxRS232Baudrate->itemData( d_data->m_ui->cbxRS232Baudrate->currentIndex() ).toInt();
    int rs485Baudrate = d_data->m_ui->cbxRS485Baudrate->itemData( d_data->m_ui->cbxRS485Baudrate->currentIndex() ).toInt();
    int rs485Address = d_data->m_ui->sbxRS485Address->value();
    int rs485BroadcastAddress = d_data->m_ui->sbxRS485BroadcastAddress->value();

    // check for consistency, broadcast and device address must not be identical
    if ( rs485Address == rs485BroadcastAddress )
    {
        setNormalCursor();

        QMessageBox msgBox;
        msgBox.setWindowTitle("RS485 Configuration Error");
        msgBox.setText("Can not set RS485 parameters, device address and broadcast address must be different.");
        msgBox.exec();
    }
    else
    {
        // Emit a system settings changed event, this will trigger reconnect to the device
        emit SystemSettingsChanged( rs232Baudrate, rs485Baudrate, rs485Address, rs485BroadcastAddress );

        setNormalCursor();
    }
}

/******************************************************************************
 * InfoBox::onCbxEngineeringModeChange
 *****************************************************************************/
void InfoBox::onCbxEngineeringModeChange( int value )
{
    emit EngineeringModeChanged( (Qt::Unchecked == value) ? false : true );
}

/******************************************************************************
 * InfoBox::onShowLicenseClicked
 *****************************************************************************/
void InfoBox::onShowLicenseClicked()
{
    TextViewer * licenseView = new TextViewer( ":/doc/license.txt", "License" );
    licenseView->show();
}

/******************************************************************************
 * InfoBox::onShowThirdPartyLicensesClicked
 *****************************************************************************/
void InfoBox::onShowThirdPartyLicensesClicked()
{
    TextViewer * licenseView = new TextViewer( ":/doc/third-party-licenses.txt", "Third-Party Licenses" );
    licenseView->show();
}

