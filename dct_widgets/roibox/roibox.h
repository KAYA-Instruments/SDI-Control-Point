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
 * @file    roibox.h
 *
 * @brief   Class definition of a ROI box
 *
 *****************************************************************************/
#ifndef __ROI_BOX_H__
#define __ROI_BOX_H__

#include <dct_widgets_base.h>
#include <qcustomplot.h>

/******************************************************************************
 * ROI-Point Definitions
 *****************************************************************************/
#define ROI_POINT_MIN                              (        1 )
#define ROI_POINT_MAX                              (      100 )
#define ROI_POINT_DEFAULT                          (       85 )
#define ROI_POINT_COMMA_POSITION                   (        0 )
#define ROI_POINT_BASE                             (       10 )
#define ROI_POINT_DISPLAY_MULTIPLIER               (        1 )
#define ROI_POINT_DISPLAY_MASK                     (  "%3.0f" )

/******************************************************************************
 * ROI-Slope Definitions
 *****************************************************************************/
#define ROI_SLOPE_MIN                              (      101 )
#define ROI_SLOPE_MAX                              (     1600 )
#define ROI_SLOPE_DEFAULT                          (      140 )
#define ROI_SLOPE_COMMA_POSITION                   (        2 )
#define ROI_SLOPE_BASE                             (       10 )
#define ROI_SLOPE_DISPLAY_MULTIPLIER               (        1 )
#define ROI_SLOPE_DISPLAY_MASK                     (  "%2.2f" )

/******************************************************************************
 * White-Clip Contrast Definitions
 *****************************************************************************/
#define WHITE_CLIP_MIN                              (      100 )
#define WHITE_CLIP_MAX                              (      109 )
#define WHITE_CLIP_DEFAULT                          (      109 )
#define WHITE_CLIP_COMMA_POSITION                   (        0 )
#define WHITE_CLIP_BASE                             (       10 )
#define WHITE_CLIP_DISPLAY_MULTIPLIER               (        1 )
#define WHITE_CLIP_DISPLAY_MASK                     (  "%3.0f" )

/******************************************************************************
 * ROIBox Configuration Box Widget
 *****************************************************************************/
class ROIBox : public DctWidgetBox
{
    Q_OBJECT

public:
    explicit ROIBox( QWidget * parent = 0 );
    ~ROIBox();

protected:
    void prepareMode( const Mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

    bool eventFilter(QObject *obj, QEvent *evt) Q_DECL_OVERRIDE;

signals:
    void StatROIChanged( int width, int height, int offset_x, int offset_y );

public slots:
    void onStatROIInfoChange( int max_width, int max_height, int width_step, int height_step );
    void onStatROIChange( int width, int height, int offset_x, int offset_y );
 
private:
    void SetROIUIs();
    void StatROIChange();
    void ConfigSldChange( int & data_ref, int index, int step );
    void ConfigSbxChange( int & data_ref, int index, int step );
    void SetXRangeChanged( const QCPRange &, QCustomPlot * );
    void SetYRangeChanged( const QCPRange &, QCustomPlot * );
    void ConfigureHeightWidth();
    void ConfigureMaxOffset();

private slots:
    void onBtnResetClicked();

    void onSbxStatROIOffsetXChange( int index );
    void onSldStatROIOffsetXChange( int index );
    void onSbxStatROIOffsetYChange( int index );
    void onSldStatROIOffsetYChange( int index );

    void onSbxStatROIWidthChange( int index );
    void onSldStatROIWidthChange( int index );
    void onSbxStatROIHeightChange( int index );
    void onSldStatROIHeightChange( int index );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __ROI_BOX_H__

