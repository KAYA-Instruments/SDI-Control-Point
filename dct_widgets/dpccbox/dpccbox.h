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
 * @file    dpccbox.h
 *
 * @brief   Class definition of a dpcc box
 *
 *****************************************************************************/
#ifndef __DPCC_BOX_H__
#define __DPCC_BOX_H__

#include <QItemSelection>
#include <QVector>

#include <dct_widgets_base.h>

/******************************************************************************
 * DpccBox Configuration Box Widget
 *****************************************************************************/
class DpccBox : public DctWidgetBox
{
    Q_OBJECT

public:
    explicit DpccBox( QWidget * parent = 0 );
    ~DpccBox();

    // Show or hide UI elements
    void setCameraFlashVisible(bool visible);

    // Clear table (usefull when connecting to new device)
    void clearDpccTable();

protected:
    void prepareMode( const Mode mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

signals:
    void DpccEnableChanged( int );
    void DpccCorrectionModeChanged( int );
    void DpccDetectionLevelChanged( int );
    void DpccTestModeChanged( int );
    void DpccWriteTableToRam( QVector<int> & xPos, QVector<int> & yPos );
    void DpccLoadTableFromRam();
    void DpccStoreTableInFlash();
    void DpccRestoreTableFromFlash();
    void DpccAutoLoadTable();

public slots:
    // Enable / Disable
    void onDpccEnableChanged( int enable );

    // Settings
    void onDpccCorrectionModeChanged( int mode );
    void onDpccDetectionLevelChanged( int level );
    void onDpccTestModeChanged( int mode );

    // Pixel Position Table
    void onDpccTableFromCameraLoaded(QVector<int> xPos , QVector<int> yPos);

    // Video Mode (Ranges of the Table have to be changed when the resolution changes)
    void onDpccVideoModeChanged( int mode);
 
private slots:
    // Camera Interactino
    void onTransmittTableClicked();
    void onLoadTableClicked();
    void onAutoLoadTableClicked();

    // Settings
    void onCbxEnableChanged( int value );
    void onCbxCorrectionModeChanged (int mode );
    void onSbxDetectionLevelChanged( int level );
    void onCbxTestModeChanged (int mode );

    // Pixel Position Table
    void onAddClicked();
    void onRemoveClicked();
    void onSortClicked();
    void onClearClicked();
    void onImportClicked();
    void onExportClicked();

private:
    class PrivateData;
    PrivateData * d_data;

    bool getDpccEnabled() const;
    void setDpccEnabled( const bool enable );
    int getDpccMode() const;
    void setDpccMode( const int mode );
    int getDpccLevel() const;
    void setDpccLevel( const int level );
    int getDpccTestMode() const;
    void setDpccTestMode( const int mode );
};

#endif // __DPCC_BOX_H__

