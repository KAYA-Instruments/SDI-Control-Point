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
 * @file    connectdialog.h
 *
 * @brief   declaration of connect dialog box
 *
 *****************************************************************************/
#ifndef __CONNECT_DIALOG_H__
#define __CONNECT_DIALOG_H__

#include <QDialog>
#include <QPushButton>
#include <QSettings>

#include <com_ctrl/ComChannelRSxxx.h>

#include <ProVideoDevice.h>

namespace Ui {
    class dlgConnect;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    enum Interface
    {
        Invalid = -1,
        Rs485   =  0,
        Rs232   =  1,
        Network =  2,
        MaxInterface
    };

    struct detectedRS485Device
    {
        QString                             name;
        QString                             platform;
        ctrl_channel_rs4xx_open_config_t    config;
        uint32_t                            broadcastAddress;
        bool                                isBroadcastMaster;
    };

    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

    void setChannelRS232( ComChannelSerial * com );
    void setChannelRS485( ComChannelSerial * com );

    ComChannelSerial * getActiveChannel() const
    {
        return ( m_active );
    }

    // open the com interface to device
    int openInterface();

    // settings
    bool loadSettings( QSettings & );
    void saveSettings( QSettings & );
    
    Interface getActiveInterface() const;
    int setActiveInterface( ConnectDialog::Interface iface );

    QString getActiveChannelName() const;
    int setActiveChannelName( Interface, QString & );

    int getActiveBaudRate() const;
    int setActiveBaudRate( Interface, int );
    
    int getActiveDataBits() const;
    int setActiveDataBits( Interface, int );
    
    QString getActiveParity() const;
    int setActiveParity( Interface, QString & );

    int getActiveStopBits() const;
    int setActiveStopBits( Interface, int );
    
    int getActiveDeviceAddress() const;
    int setActiveDeviceAddress( Interface, int );

    int getCurrentRs485DeviceIndex() const;
    QVector<ConnectDialog::detectedRS485Device> getDetectedRS485Devices() const;

    // Returns whether a connection to a device exists
    bool isConnected();
    // Scans for all available RS485 devices, stores them in m_detectedRS485Devices and connects to the first device found
    bool scanAndConnect();
     // Changes comport settings on the device and in the connect dialog
    void changeComportSettings( int rs232Baudrate, int rs485Baudrate, int rs485Address , int rs485BroadcastAddress );
    // Connect to a device from the m_detectedRS485Devices list
    bool connectToRS485DeviceByIndex( int index );

signals:
    // connected to a new device
    void DeviceConnected( ProVideoDevice * );
    void OpenConnectDialog();

    // change device serial connection parameters
    void RS232BaudrateChanged( uint32_t baudrate );
    void RS485BaudrateChanged( uint32_t baudrate );
    void RS485AddressChanged( uint32_t address );
    void RS485BroadcastAddressChanged( uint32_t address );
    void RS485BroadcastMasterChanged( int32_t address );

    // Reopen serial port
    void ReopenFailed();
    void ReopenSuccessful();

public slots:
    void accept();
    void reject();

    void onBroadcastChange( bool enabled );
    void onCloseSerialConnection( void );
    void onReopenSerialConnection( void );

private slots:
    void rescan();
    void onScanButtonClick();

private:
    Ui::dlgConnect *             m_ui;                      // GUI instance
    ComChannelRS232 *            m_rs232;                   // control channel instance
    ComChannelRS4xx *            m_rs485;                   // control channel instance
    ProVideoDevice *             m_connectedDevice;         // Holds the connected device.
    ComChannelSerial *           m_active;                  // activated/opened com channel
    QVector<detectedRS485Device> m_detectedRS485Devices;    // list of the RS485 devices which were detected during scan
    int                          m_currentRS485DeviceIndex; // Index of the connected device from the m_detectedRS485Devices list that is currently connected

    QPushButton *       m_rescan;           // rescan button

    ctrl_channel_rs4xx_open_config_t getRs485Config();
    void setRs485Config( ctrl_channel_rs4xx_open_config_t const config );

    ctrl_channel_rs232_open_config_t getRs232Config();
    void setRs232Config( ctrl_channel_rs232_open_config_t const config );

    void setCurrentRs485DeviceIndex(int index);

    bool fileExists( QString & path );
    void setIsConnected( bool value );
    bool connectWithDevice();
};

#endif // __CONNECT_DIALOG_H__
