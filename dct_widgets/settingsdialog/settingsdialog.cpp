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
#include <QValidator>

#include <defines.h>
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

    // Change text of the "Save" button to "Apply and Save"
    m_ui->buttonBox->button(QDialogButtonBox::Save)->setText("Apply and Save");

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
    connect (m_ui->btnApplyDeviceName, SIGNAL(clicked(bool)), this, SLOT(onBtnApplyDeviceNameClicked()) );
    connect( m_ui->btnResetToDefaults, SIGNAL(clicked(bool)), this, SLOT(onBtnResetToDefaultsClicked()) );
    connect( m_ui->btnApplySerialPortSettings, SIGNAL(clicked(bool)), this, SLOT(onBtnApplySerialPortSettingsClicked()) );

    // connect Ui settings
    connect( m_ui->cbxEngineeringMode, SIGNAL(stateChanged(int)), this, SLOT(onCbxEngineeringModeChange(int)) );
    connect( m_ui->cbxDebugTerminal, SIGNAL(stateChanged(int)), this, SLOT(onCbxShowDebugTerminalChange(int)) );
    connect( m_ui->cbxConnectionCheck, SIGNAL(stateChanged(int)), this, SLOT(onCbxConnectionCheckChange(int)) );

    // Set device ID ranges
    m_ui->sbxRS485Address->setRange( 0, MAX_DEVICE_ID );
    m_ui->sbxRS485BroadcastAddress->setRange( 0, MAX_DEVICE_ID );

    // Hiding serial port features and serial connection control
    m_ui->lblNote->hide();
    m_ui->lblSerialInterface->hide();
    m_ui->lblRS232Baudrate->hide();
    m_ui->cbxRS232Baudrate->hide();
    m_ui->lblRS485Baudrate->hide();
    m_ui->cbxRS485Baudrate->hide();
    m_ui->lblRS485Address->hide();
    m_ui->sbxRS485Address->hide();
    m_ui->lblRS485BroadcastAddress->hide();
    m_ui->sbxRS485BroadcastAddress->hide();
    m_ui->lblRS485Termination->hide();
    m_ui->cbxRS485Termination->hide();
    m_ui->label_2->hide();
    m_ui->btnApplySerialPortSettings->hide();
    m_ui->lblNote2->hide();
}

/******************************************************************************
 * SettingsDialog::~SettingsDialog
 *****************************************************************************/
SettingsDialog::~SettingsDialog()
{
    delete m_ui;
}

/******************************************************************************
 * SettingsDialog::showEvent
 *****************************************************************************/
void SettingsDialog::showEvent( QShowEvent* event )
{
    // Call inherited function
    QDialog::showEvent( event );

    // Resize window to minimum size
    QApplication::processEvents();
    this->resize( this->minimumSizeHint() );
}

/******************************************************************************
 * SettingsDialog::setEngineeringModeChecked
 *****************************************************************************/
void SettingsDialog::setEngineeringModeChecked( bool checked )
{
    /* This function is used to set the engineering mode checkbox when the
     * main window is loading its Ui settings from ini file. */
    m_ui->cbxEngineeringMode->blockSignals( true );
    m_ui->cbxEngineeringMode->setChecked( checked );
    m_ui->cbxEngineeringMode->blockSignals( false );
}

/******************************************************************************
 * SettingsDialog::setConnectionCheckChecked
 *****************************************************************************/
void SettingsDialog::setConnectionCheckChecked( bool checked )
{
    /* This function is used to set the conneciton check checkbox when the
     * main window is loading its Ui settings from ini file. */
    m_ui->cbxConnectionCheck->blockSignals( true );
    m_ui->cbxConnectionCheck->setChecked( checked );
    m_ui->cbxConnectionCheck->blockSignals( false );
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
   //m_ui->lblNote->setVisible( value );
   //m_ui->lblRS485BroadcastAddress->setVisible( value );
    //m_ui->sbxRS485BroadcastAddress->setVisible( value );
    // Need to override here, it depends on rs232 flag in the feature set
    Q_UNUSED (value);
    m_ui->lblNote->setVisible( false );
    m_ui->lblRS485BroadcastAddress->setVisible( false );
    m_ui->sbxRS485BroadcastAddress->setVisible( false );
}

/******************************************************************************
 * SettingsDialog::setRS232SettingsVisible
 *****************************************************************************/
void SettingsDialog::setRS232SettingsVisible( const bool value )
{
    //m_ui->lblRS232Baudrate->setVisible( value );
    //m_ui->cbxRS232Baudrate->setVisible( value );
    // Need to override here because in the connect dialog this features should be visible
    Q_UNUSED (value);
    m_ui->lblRS232Baudrate->setVisible( false );
    m_ui->cbxRS232Baudrate->setVisible( false );
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
    m_ui->sbxRS485Address->setValue( static_cast<int>(address) );
}

/******************************************************************************
 * SettingsDialog::onRS485BroadcastAddressChange
 *****************************************************************************/
void SettingsDialog::onRS485BroadcastAddressChange( uint32_t address )
{
    m_ui->sbxRS485BroadcastAddress->setValue( static_cast<int>(address) );
}

/******************************************************************************
 * SettingsDialog::onRS485TerminationChange
 *****************************************************************************/
void SettingsDialog::onRS485TerminationChange( bool enabled )
{
    m_ui->cbxRS485Termination->setChecked( enabled );
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

    /* Disable serial interface settings if broadcast mode is active, otherwise the
     * same address could be set to multiple devices! */
    m_ui->cbxRS232Baudrate->setEnabled( !enable );
    m_ui->cbxRS485Baudrate->setEnabled( !enable );
    m_ui->sbxRS485Address->setEnabled( !enable );
    m_ui->sbxRS485BroadcastAddress->setEnabled( !enable );
    m_ui->btnApplySerialPortSettings->setEnabled( !enable );

    // Resize window to minimum size
    QApplication::processEvents();
    this->resize( this->minimumSizeHint() );
}

/******************************************************************************
 * SettingsDialog::onRS485BroadcastAddressChange
 *****************************************************************************/
void SettingsDialog::onDebugTerminalVisibilityChange( bool visible )
{
    // Set the debug terminal visible checkbox accordingly
    m_ui->cbxDebugTerminal->blockSignals( true );
    m_ui->cbxDebugTerminal->setChecked( visible );
    m_ui->cbxDebugTerminal->blockSignals( false );
}

/******************************************************************************
 * SettingsDialog::onBtnApplyDeviceNameClicked
 *****************************************************************************/
void SettingsDialog::onBtnApplyDeviceNameClicked()
{
    // Get device name string from line edit
    QString name = m_ui->letDeviceName->text();

    // Get amount of words in string
    int numWords = name.split(QRegExp("(\\s|\\n|\\r)+"), QString::SkipEmptyParts).count();

    // If the name has more than 32 characters or 5 words, its invalid
    if ( name.length() > 32 || numWords > 5 )
    {
        // Show message box
        QMessageBox msgBox;
        msgBox.setWindowTitle( "Device Name is invalid!" );
        msgBox.setIcon( QMessageBox::Warning );
        msgBox.setText( "Please choose a different device name.\n\n"
                        "The device name can have a maximum length of 32 characters and it "
                        "can consist of up to 5 words separated by spaces.\n\n" );
        msgBox.exec();

        // Set focus to line edit so user can edit it
        m_ui->letDeviceName->setFocus();
    }
    else
    {
        QApplication::setOverrideCursor( Qt::WaitCursor );

        // Send device name changed event to change name on the device
        emit DeviceNameChanged( m_ui->letDeviceName->text() );

        // Emit update device name event
        emit UpdateDeviceName();

        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }
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
    bool rs485Termination = m_ui->cbxRS485Termination->isChecked();

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
        emit SystemSettingsChanged( rs232Baudrate, rs485Baudrate,
                                    rs485Address, rs485BroadcastAddress,
                                    rs485Termination );

        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }
}

/******************************************************************************
 * SettingsDialog::onCbxEngineeringModeChange
 *****************************************************************************/
void SettingsDialog::onCbxEngineeringModeChange( int value )
{
    // Show or hide advanced settings
    emit WidgetModeChanged( (Qt::Unchecked == value) ? DctWidgetBox::Normal : DctWidgetBox::Advanced );
}

/******************************************************************************
 * SettingsDialog::onCbxShowDebugTerminalChange
 *****************************************************************************/
void SettingsDialog::onCbxShowDebugTerminalChange( int value )
{
    // Show or hide the debug terminal
    emit DebugTerminalVisibilityChanged( (Qt::Unchecked == value) ? false : true );
}

/******************************************************************************
 * SettingsDialog::onCbxConnectionCheckChange
 *****************************************************************************/
void SettingsDialog::onCbxConnectionCheckChange( int value )
{
    // Enable or disable periodic connection check
    emit ConnectionCheckChanged( (Qt::Unchecked == value) ? false : true );
}

/******************************************************************************
 * ConnectDialog::accept
 *****************************************************************************/
void SettingsDialog::accept()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );

    // Apply device name and serial port settings
    onBtnApplyDeviceNameClicked();
    onBtnApplySerialPortSettingsClicked();

    // Send device name changed event
    emit SaveSettings();

    QApplication::setOverrideCursor( Qt::ArrowCursor );
}
