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

