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

