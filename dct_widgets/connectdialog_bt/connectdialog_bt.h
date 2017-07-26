/******************************************************************************
 *
 * Copyright 2015, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
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
