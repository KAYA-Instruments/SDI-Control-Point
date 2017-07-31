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
 * @file    connectdialog_bt.h
 *
 * @brief   declaration of connect dialog box
 *
 *****************************************************************************/
#ifndef __CONNECT_DIALOG_BT_H__
#define __CONNECT_DIALOG_BT_H__

#include <QDialog>
#include <QPushButton>
#include <QtBluetooth>

#include <com_ctrl/ComChannelBluetooth.h>

namespace Ui {
    class dlgConnectBt;
}

class ConnectDialogBt : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialogBt(QWidget *parent = 0);
    ~ConnectDialogBt();

    void setChannel( ComChannelBluetooth * c );

    ComChannelBluetooth * getActiveChannel() const
    {
        return ( m_bt );
    }

    // open the com interface to device
    int openInterface();

    // settings
    bool loadSettings( QSettings & );
    void saveSettings( QSettings & );

public slots:

private slots:
    void scanClicked();
    void connectClicked();
    void connected();

    void serviceDiscovered( const QBluetoothServiceInfo &serviceInfo );
    void discoveryFinished();

private:
    QBluetoothServiceDiscoveryAgent * m_discovery;   // discovery agent
    QBluetoothLocalDevice           * m_local;       // local device
    Ui::dlgConnectBt                * m_ui;          // GUI instance

    QPushButton                     * m_rescan;      // rescan button
    QPushButton                     * m_connect;     // connect button

    QMap<int, QBluetoothServiceInfo>  m_services;
    QBluetoothServiceInfo             m_service;

    ComChannelBluetooth *             m_bt;
};

#endif // __CONNECT_DIALOG_BT_H__
