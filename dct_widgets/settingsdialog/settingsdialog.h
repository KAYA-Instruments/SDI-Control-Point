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
 * @file    settingsdialog.h
 *
 * @brief   declaration of settings dialog box
 *
 *****************************************************************************/
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QValidator>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    // Show or hide UI elements
    void setBroadcastSettingsVisible( const bool value );
    void setRS232SettingsVisible( const bool value );

signals:
    void DeviceNameChanged( QString name );
    void ResetToDefaultsClicked();
    void SystemSettingsChanged( int rs232Baudrate, int rs485Baudrate, int rs485Address, int rs485BroadcastAddress );
    void EngineeringModeChanged( bool flag );

    void ResyncRequest( void );
    void SaveSettings( void );

public slots:
    // standard buttons
    void accept();

    // system interface slots
    void onDeviceNameChange( QString name );
    void onRS232BaudrateChange( uint32_t baudrate );
    void onRS485BaudrateChange( uint32_t baudrate );
    void onRS485AddressChange( uint32_t address );
    void onRS485BroadcastAddressChange( uint32_t address );
    void onBroadcastChange( bool enable );

private slots:
    void onApplyDeviceNameClicked();
    void onBtnResetToDefaultsClicked();
    void onBtnApplySerialPortSettingsClicked();
    void onCbxEngineeringModeChange( int value );

private:
    Ui::SettingsDialog *        m_ui;   // GUI instance
};

#endif // SETTINGSDIALOG_H
