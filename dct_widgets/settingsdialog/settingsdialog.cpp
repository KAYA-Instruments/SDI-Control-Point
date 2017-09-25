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
 * @file    settingsdialog.cpp
 *
 * @brief   Implementation of settings dialog to a proVideo device
 *
 *****************************************************************************/
#include <QMessageBox>

#include <com_ctrl/ComChannelRSxxx.h>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

/******************************************************************************
 * SettingsDialog::SettingsDialog
 *****************************************************************************/
SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::SettingsDialog)
{
    // initialize UI
    m_ui->setupUi(this);

    // add baudrates to baudrate combo boxes
    /* Note: Slow baudrates below 57600 baud are not supported by the GUI because
     * the delays / wait times get to long for a fluid user experience */
//    m_ui->cbxRS232Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_9600)  , CTRL_CHANNEL_BAUDRATE_9600 );
//    m_ui->cbxRS232Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_19200) , CTRL_CHANNEL_BAUDRATE_19200 );
//    m_ui->cbxRS232Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_38400) , CTRL_CHANNEL_BAUDRATE_38400 );
    m_ui->cbxRS232Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_57600) , CTRL_CHANNEL_BAUDRATE_57600 );
    m_ui->cbxRS232Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_115200), CTRL_CHANNEL_BAUDRATE_115200 );
    m_ui->cbxRS232Baudrate->setCurrentIndex( m_ui->cbxRS232Baudrate->findData( CTRL_CHANNEL_BAUDRATE_DEFAULT ) );

//    m_ui->cbxRS485Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_9600)  , CTRL_CHANNEL_BAUDRATE_9600 );
//    m_ui->cbxRS485Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_19200) , CTRL_CHANNEL_BAUDRATE_19200 );
//    m_ui->cbxRS485Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_38400) , CTRL_CHANNEL_BAUDRATE_38400 );
    m_ui->cbxRS485Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_57600) , CTRL_CHANNEL_BAUDRATE_57600 );
    m_ui->cbxRS485Baudrate->addItem( QString::number(CTRL_CHANNEL_BAUDRATE_115200), CTRL_CHANNEL_BAUDRATE_115200 );
    m_ui->cbxRS485Baudrate->setCurrentIndex( m_ui->cbxRS485Baudrate->findData( CTRL_CHANNEL_BAUDRATE_DEFAULT ) );

    // connect system settings
    connect( m_ui->btnResetToDefaults, SIGNAL(clicked(bool)), this, SLOT(onBtnResetToDefaultsClicked()) );
    connect( m_ui->btnApplySerialPortSettings, SIGNAL(clicked(bool)), this, SLOT(onBtnApplySerialPortSettingsClicked()) );

    // connect control settings
    connect( m_ui->cbxEngineeringMode, SIGNAL(stateChanged(int)), this, SLOT(onCbxEngineeringModeChange(int)) );
}

/******************************************************************************
 * SettingsDialog::~SettingsDialog
 *****************************************************************************/
SettingsDialog::~SettingsDialog()
{
    delete m_ui;
}

/******************************************************************************
 * SettingsDialog::onDeviceNameChange
 *****************************************************************************/
void SettingsDialog::onDeviceNameChange( QString name )
{
    m_ui->letDeviceName->setText( name );
}

/******************************************************************************
 * SettingsDialog::setBroadcastSettingsVisible
 *****************************************************************************/
void SettingsDialog::setBroadcastSettingsVisible( const bool value )
{
    m_ui->lblNote->setVisible( value );
    m_ui->lblRS485BroadcastAddress->setVisible( value );
    m_ui->sbxRS485BroadcastAddress->setVisible( value );
}

/******************************************************************************
 * SettingsDialog::setRS232SettingsVisible
 *****************************************************************************/
void SettingsDialog::setRS232SettingsVisible( const bool value )
{
    m_ui->lblRS232Baudrate->setVisible( value );
    m_ui->cbxRS232Baudrate->setVisible( value );
}

/******************************************************************************
 * SettingsDialog::onRS232BaudrateChange
 *****************************************************************************/
void SettingsDialog::onRS232BaudrateChange( uint32_t baudrate )
{
    m_ui->cbxRS232Baudrate->setCurrentIndex( m_ui->cbxRS232Baudrate->findData( baudrate ) );
}

/******************************************************************************
 * SettingsDialog::onRS485BaudrateChange
 *****************************************************************************/
void SettingsDialog::onRS485BaudrateChange( uint32_t baudrate )
{
    m_ui->cbxRS485Baudrate->setCurrentIndex( m_ui->cbxRS485Baudrate->findData( baudrate ) );
}

/******************************************************************************
 * SettingsDialog::onRS485AddressChange
 *****************************************************************************/
void SettingsDialog::onRS485AddressChange( uint32_t address )
{
    m_ui->sbxRS485Address->setValue( address );
}

/******************************************************************************
 * SettingsDialog::onRS485BroadcastAddressChange
 *****************************************************************************/
void SettingsDialog::onRS485BroadcastAddressChange( uint32_t address )
{
    m_ui->sbxRS485BroadcastAddress->setValue( address );
}

/******************************************************************************
 * SettingsDialog::onRS485BroadcastAddressChange
 *****************************************************************************/
void SettingsDialog::onBroadcastChange( bool enable )
{
    // In broadcast mode, show a note that some settings are unavailable
    m_ui->lblNote->setVisible( enable );

    /* Reset to factory defaults is disabled in broadcast mode, otherwise all
     * devices will be reset to the same devie address, making them unnaccessible */
    m_ui->btnResetToDefaults->setEnabled( !enable );

    /* All serial interface settings if broadcast mode is active, otherwise the
     * same address could be set to multiple devices! */
    m_ui->cbxRS232Baudrate->setEnabled( !enable );
    m_ui->cbxRS485Baudrate->setEnabled( !enable );
    m_ui->sbxRS485Address->setEnabled( !enable );
    m_ui->sbxRS485BroadcastAddress->setEnabled( !enable );
    m_ui->btnApplySerialPortSettings->setEnabled( !enable );
}

/******************************************************************************
 * SettingsDialog::onResetToDefaultsClicked
 *****************************************************************************/
void SettingsDialog::onBtnResetToDefaultsClicked()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );

    // Send reset settings command
    emit ResetToDefaultsClicked();

    // Reset all settings
    emit ResyncRequest();

    QApplication::setOverrideCursor( Qt::ArrowCursor );
}

/******************************************************************************
 * SettingsDialog::onBtnApplySerialPortSettingsClicked
 *****************************************************************************/
void SettingsDialog::onBtnApplySerialPortSettingsClicked()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );

    // Get data from the ui elements
    int rs232Baudrate = m_ui->cbxRS232Baudrate->itemData( m_ui->cbxRS232Baudrate->currentIndex() ).toInt();
    int rs485Baudrate = m_ui->cbxRS485Baudrate->itemData( m_ui->cbxRS485Baudrate->currentIndex() ).toInt();
    int rs485Address = m_ui->sbxRS485Address->value();
    int rs485BroadcastAddress = m_ui->sbxRS485BroadcastAddress->value();

    // check for consistency, broadcast and device address must not be identical
    if ( rs485Address == rs485BroadcastAddress )
    {
        QApplication::setOverrideCursor( Qt::ArrowCursor );

        QMessageBox msgBox;
        msgBox.setWindowTitle("RS485 Configuration Error");
        msgBox.setText("Can not set RS485 parameters, device address and broadcast address must be different.");
        msgBox.exec();
    }
    else
    {
        // Emit a system settings changed event, this will trigger reconnect to the device
        emit SystemSettingsChanged( rs232Baudrate, rs485Baudrate, rs485Address, rs485BroadcastAddress );

        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }
}

/******************************************************************************
 * SettingsDialog::onCbxEngineeringModeChange
 *****************************************************************************/
void SettingsDialog::onCbxEngineeringModeChange( int value )
{
    emit EngineeringModeChanged( (Qt::Unchecked == value) ? false : true );
}
