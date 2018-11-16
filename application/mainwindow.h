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
#include <QTimer>

#include <dct_widgets_base.h>
#include "ProVideoDevice.h"
#include "connectdialog.h"
#include "settingsdialog.h"
#include "debugterminal.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( ConnectDialog * connectDialog = nullptr, QWidget * parent = nullptr );
    ~MainWindow() Q_DECL_OVERRIDE;

    void connectToDevice(ProVideoDevice * );

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

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

    // dock widget
    void setDockWidgetVisible( bool visible );

private slots:
    void onDeviceConnected( ProVideoDevice * device );
    void onDeviceSelectionChange( int index );
    void onUpdateDeviceName();
    void onSystemSettingsChange( int rs232Baudrate, int rs485Baudrate, int rs485Address, int rs485BroadcastAddress );

    void onConnectClicked();
    void onSettingsClicked();
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
    void onWidgetModeChange( DctWidgetBox::Mode mode );
    void onConnectionCheckChange( bool enable );
    void onBroadcastChange(uint8_t flag );
    void onDebugTerminalTopLevelChange( bool floating );
    void onDebugTerminalVisibilityChange( bool visible );

    void onAecResyncRequest();
    void onResyncRequest();

    void onResolutionMaskChange( uint32_t id0, uint32_t id1, uint32_t id2 );

    void onLockCurrentTabPage( bool lock );
    void onResizeMainWindow( bool force = false );
    void onCheckConnection();

private:
    Ui::MainWindow *        m_ui;
    ConnectDialog *         m_ConnectDlg;
    SettingsDialog *        m_SettingsDlg;
    DebugTerminal *         m_DebugTerminal;
    QComboBox *             m_cbxConnectedDevices;
    ProVideoDevice *        m_dev;
    QString                 m_filename;
    QList<DctWidgetBox *>   m_activeWidgets;
    QTimer                  m_resizeTimer;
    QTimer                  m_checkConnectionTimer;
    bool                    m_ScrollbarsNeeded;
    DctWidgetBox::Mode      m_WidgetMode;
    bool                    m_ShowDebugTerminal;
    bool                    m_EnableConnectionCheck;

    void setConnectDlg( ConnectDialog * );
    void setSettingsDlg( SettingsDialog * );
    void setDebugTerminal( DebugTerminal * );
    void setupUI(ProVideoDevice::features deviceFeatures);
    bool fileExists( QString & path );
    void loadUiSettings( QSettings &s );
    void saveUiSettings( QSettings &s );

    void updateDeviceList();
};

#endif // __MAINWINDOW_H__
