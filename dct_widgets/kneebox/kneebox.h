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
 * @file    kneebox.h
 *
 * @brief   Class definition of a knee box 
 *
 *****************************************************************************/
#ifndef __KNEE_BOX_H__
#define __KNEE_BOX_H__

#include <dct_widgets_base.h>
#include <qcustomplot.h>

/******************************************************************************
 * Knee-Point Definitions
 *****************************************************************************/
#define KNEE_POINT_MIN                              (        0 )
#define KNEE_POINT_MAX                              (      100 )
#define KNEE_POINT_COMMA_POSITION                   (        0 )
#define KNEE_POINT_BASE                             (       10 )
#define KNEE_POINT_DISPLAY_MULTIPLIER               (        1 )
#define KNEE_POINT_DISPLAY_MASK                     (  "%3.0f" )

/******************************************************************************
 * Knee-Slope Definitions
 *****************************************************************************/
#define KNEE_SLOPE_MIN                              (      100 )
#define KNEE_SLOPE_MAX                              (     1600 )
#define KNEE_SLOPE_COMMA_POSITION                   (        2 )
#define KNEE_SLOPE_BASE                             (       10 )
#define KNEE_SLOPE_DISPLAY_MULTIPLIER               (        1 )
#define KNEE_SLOPE_DISPLAY_MASK                     (  "%2.2f" )

/******************************************************************************
 * White-Clip Contrast Definitions
 *****************************************************************************/
#define WHITE_CLIP_MIN                              (      100 )
#define WHITE_CLIP_MAX                              (      109 )
#define WHITE_CLIP_COMMA_POSITION                   (        0 )
#define WHITE_CLIP_BASE                             (       10 )
#define WHITE_CLIP_DISPLAY_MULTIPLIER               (        1 )
#define WHITE_CLIP_DISPLAY_MASK                     (  "%3.0f" )

/******************************************************************************
 * KneeBox Configuration Box Widget
 *****************************************************************************/
class KneeBox : public DctWidgetBox
{
    Q_OBJECT

public:
    explicit KneeBox( QWidget * parent = 0 );
    ~KneeBox();

    bool KneeEnable() const;
    void setKneeEnable( const bool );
    int  KneePoint() const;
    void setKneePoint( const int );
    int  KneeSlope() const;
    void setKneeSlope( const int );
    int  WhiteClip() const;
    void setWhiteClip( const int );

protected:
    void prepareMode( const Mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

signals:
    void KneeConfigChanged( int, int, int, int );

public slots:
    void onKneeConfigChange( int, int, int, int );
 
private:
    void SetXRangeChanged( const QCPRange &, QCustomPlot * );
    void SetYRangeChanged( const QCPRange &, QCustomPlot * );

private slots:
    void onKneeEnableChange( int );
    void onKneePointChange( int );
    void onKneeSlopeChange( int );
    void onWhiteClipChange( int );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __KNEE_BOX_H__

