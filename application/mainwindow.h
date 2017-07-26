/******************************************************************************
 *
 * Copyright 2016, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
/**
 * @file    mainwindow.h
 *
 * @brief   Application main window
 *
 *****************************************************************************/

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <QList>
#include <QComboBox>

#include <dct_widgets_base.h>
#include "ProVideoDevice.h"
#include "connectdialog.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( ConnectDialog * connectDialog = NULL, QWidget * parent = 0 );
    ~MainWindow();

    void connectToDevice(ProVideoDevice * );
    void setConnectDlg( ConnectDialog * );

signals:
    // chain selection status
    void SaveSettings();
    void LoadSettings();
    void SdiOutChanged( int value );

    // copy settings
    void CopySettings( int fromIdx, int toIdx );

    // split screen
    void SplitScreenChanged( bool enable );

    // broadcast
    void BroadcastChanged( bool enable );

private slots:
    void onDeviceConnected( ProVideoDevice * device );
    void onDeviceSelectionChange( int index );
    void onSystemSettingsChange( int rs232Baudrate, int rs485Baudrate, int rs485Address, int rs485BroadcastAddress );

    void onConnectClicked();
    void onSelectSdi1Clicked();
    void onSelectSdi2Clicked();
    void onCopyChain1To2Clicked();
    void onCopyChain2To1Clicked();
    void onSplitScreenClicked();
    void onLoadSettingsClicked();
    void onSaveSettingsClicked();
    void onLoadFromFileClicked();
    void onSaveToFileClicked();
    void onBroadcastClicked();
    void onSyncSettingsClicked();

    void onSdiOutChange( int value );
    void onSplitScreenChange( bool flag );
    void onCopyFlagChange( bool flag );
    void onEngineeringModeChange( bool flag );
    void onBroadcastChange(uint8_t flag );

    void onAecResyncRequest();
    void onResyncRequest();

    void onResolutionMaskChange( uint32_t id0, uint32_t id1, uint32_t id2 );

    void onLockCurrentTabPage( bool lock );

private:
    Ui::MainWindow *        m_ui;
    ConnectDialog *         m_dlg;
    QComboBox *             m_cbxConnectedDevices;
    ProVideoDevice *        m_dev;
    QString                 m_filename;
    QList<DctWidgetBox *>   m_activeWidgets;

    void setupUI(ProVideoDevice::features deviceFeatures);
};

#endif // __MAINWINDOW_H__
