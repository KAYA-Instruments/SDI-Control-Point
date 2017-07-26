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
 * @file    fltbox.h
 *
 * @brief   Class definition of a filter configuration box 
 *
 *****************************************************************************/
#ifndef __FLT_BOX_H__
#define __FLT_BOX_H__

#include <dct_widgets_base.h>

/******************************************************************************
 * 2D Denoise Level Definitions
 *****************************************************************************/
#define FLT_2D_DENOISE_LEVEL_MIN                (        0 )
#define FLT_2D_DENOISE_LEVEL_MAX                (       63 )
#define FLT_2D_DENOISE_LEVEL_COMMA_POSITION     (        0 )
#define FLT_2D_DENOISE_LEVEL_BASE               (       10 )
#define FLT_2D_DENOISE_LEVEL_DISPLAY_MULTIPLIER (        1 )
#define FLT_2D_DENOISE_LEVEL_DISPLAY_MASK       (  "%2.0f" )

/******************************************************************************
 * 2D Detail Level Definitions
 *****************************************************************************/
#define FLT_2D_DETAIL_LEVEL_MIN                 (        0 )
#define FLT_2D_DETAIL_LEVEL_MAX                 (       63 )
#define FLT_2D_DETAIL_LEVEL_COMMA_POSITION      (        0 )
#define FLT_2D_DETAIL_LEVEL_BASE                (       10 )
#define FLT_2D_DETAIL_LEVEL_DISPLAY_MULTIPLIER  (        1 )
#define FLT_2D_DETAIL_LEVEL_DISPLAY_MASK        (  "%2.0f" )

/******************************************************************************
 * 3D Denoise Level Definitions
 *****************************************************************************/
#define FLT_3D_DENOISE_LEVEL_MIN                (        0 )
#define FLT_3D_DENOISE_LEVEL_MAX                (       19 )
#define FLT_3D_DENOISE_LEVEL_COMMA_POSITION     (        0 )
#define FLT_3D_DENOISE_LEVEL_BASE               (       10 )
#define FLT_3D_DENOISE_LEVEL_DISPLAY_MULTIPLIER (        1 )
#define FLT_3D_DENOISE_LEVEL_DISPLAY_MASK       (  "%2.0f" )

/******************************************************************************
 * 3D Detail Level Definitions
 *****************************************************************************/
#define FLT_3D_DETAIL_LEVEL_MIN                 (        0 )
#define FLT_3D_DETAIL_LEVEL_MAX                 (       15 )
#define FLT_3D_DETAIL_LEVEL_COMMA_POSITION      (        0 )
#define FLT_3D_DETAIL_LEVEL_BASE                (       10 )
#define FLT_3D_DETAIL_LEVEL_DISPLAY_MULTIPLIER  (        1 )
#define FLT_3D_DETAIL_LEVEL_DISPLAY_MASK        (  "%2.0f" )

/******************************************************************************
 * Denoising Filter Box Widget
 *****************************************************************************/
class FltBox : public DctWidgetBox
{
    Q_OBJECT

public:
    explicit FltBox( QWidget * parent = 0 );
    ~FltBox();

    bool Enable2d() const;
    void setEnable2d( const bool enable );
    int DetailLevel2d() const;
    void setDetailLevel2d( const int level );
    int DenoiseLevel2d() const;
    void setDenoiseLevel2d( const int level );

    bool Enable3d() const;
    void setEnable3d( const bool enable );
    int DetailLevel3d() const;
    void setDetailLevel3d( const int level );
    int DenoiseLevel3d() const;
    void setDenoiseLevel3d( const int level );

protected:
    void prepareMode( const Mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

signals:
    // ISP filter stage
    void FilterEnableChanged( int value );
    void FilterDetailLevelChanged( int value );
    void FilterDenoiseLevelChanged( int value );
    void FilterChanged( int enable, int detail, int denoise );

    // TFLT stage
    void TfltEnableChanged( int value );
    void TfltDenoiseLevelChanged( int value );
    void TfltMinMaxChanged( int min, int max );

public slots:
    // ISP filter stage
    void onFilterEnableChange( int value );
    void onFilterDetailLevelChange( int value );
    void onFilterDenoiseLevelChange( int value );
    void onFilterChange( int enable, int detail, int denoise );
    
    // TFLT stage
    void onTfltEnableChange( int value );
    void onTfltDenoiseLevelChange( int value );
    void onTfltMinMaxChange( int min, int max );

private slots:
    void on2dEnableChange( int value );
    void on3dEnableChange( int value );
    void onObjectSpeedChange( int value );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __FLT_BOX_H__

