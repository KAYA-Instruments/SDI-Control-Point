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
 * @file    lutbox.h
 *
 * @brief   Class definition of a lut box 
 *
 *****************************************************************************/
#ifndef __LUT_BOX_H__
#define __LUT_BOX_H__

#include <dct_widgets_base.h>
#include <qcustomplot.h>

#include <QAbstractButton>
#include <QItemSelection>

/******************************************************************************
 * Linear Contrast Definitions
 *****************************************************************************/
#define LUT_LINEAR_CONTRAST_MIN                   (        0 )
#define LUT_LINEAR_CONTRAST_MAX                   (    10000 )
#define LUT_LINEAR_CONTRAST_DECIMALS              (        3 )
#define LUT_LINEAR_CONTRAST_STEP                  (        1 )

/******************************************************************************
 * Linear Brightness Definitions
 *****************************************************************************/
#define LUT_LINEAR_BRIGHTNESS_MIN                 (    -1000 )
#define LUT_LINEAR_BRIGHTNESS_MAX                 (     1000 )
#define LUT_LINEAR_BRIGHTNESS_DECIMALS            (        3 )
#define LUT_LINEAR_BRIGHTNESS_STEP                (        1 )

/******************************************************************************
 * Threshold Definitions
 *****************************************************************************/
#define LUT_THRESHOLD_MIN                         (        0 )
#define LUT_THRESHOLD_MAX                         (     1000 )
#define LUT_THRESHOLD_DECIMALS                    (        3 )
#define LUT_THRESHOLD_STEP                        (        1 )

/******************************************************************************
 * Contrast Definitions
 *****************************************************************************/
#define LUT_CONTRAST_MIN                          (     1000 )
#define LUT_CONTRAST_MAX                          (    10000 )
#define LUT_CONTRAST_DECIMALS                     (        3 )
#define LUT_CONTRAST_STEP                         (        1 )

/******************************************************************************
 * Gamma Definitions
 *****************************************************************************/
#define LUT_GAMMA_MIN                             (     1100 )
#define LUT_GAMMA_MAX                             (     3000 )
#define LUT_GAMMA_DECIMALS                        (        3 )
#define LUT_GAMMA_STEP                            (        1 )

/******************************************************************************
 * Brightness Definitions
 *****************************************************************************/
#define LUT_BRIGHTNESS_MIN                        (    -1000 )
#define LUT_BRIGHTNESS_MAX                        (     1000 )
#define LUT_BRIGHTNESS_DECIMALS                   (        3 )
#define LUT_BRIGHTNESS_STEP                       (        1 )

/******************************************************************************
 * Fast Gamma Knob Box Definitions
 *****************************************************************************/
#define FAST_GAMMA_MIN                          ( LUT_GAMMA_MIN )
#define FAST_GAMMA_MAX                          ( LUT_GAMMA_MAX )
#define FAST_GAMMA_COMMA_POSITION               (             3 )
#define FAST_GAMMA_BASE                         (            10 )
#define FAST_GAMMA_DISPLAY_MULTIPLIER           (             1 )
#define FAST_GAMMA_DISPLAY_MASK                 (       "%1.3f" )

/******************************************************************************
 * LutBox Configuration Box Widget
 *****************************************************************************/
class LutBox : public DctWidgetBox
{
    Q_OBJECT

public:
    enum LutChannel
    {
        Master = 0,
        Red = 1,
        Green = 2,
        Blue = 3,
        LutChannelMax
    };

    explicit LutBox( QWidget * parent = 0 );
    ~LutBox();

    bool LutEnable() const;
    void setLutEnable( const bool );

    int LutMode() const;
    void setLutMode( const int );

    int LutFixedMode() const;
    void setLutFixedMode( const int );
    
    int LutPresetStorage() const;
    void setLutPresetStorage( const int );

    int LutFastGamma() const;
    void setLutFastGamma( const int );

protected:
    void prepareMode( const Mode mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;
    
    void loadProfile( QSettings & s );
    void saveProfile( QSettings & s );

signals:
    void LutEnableChanged( int, int );
    void LutModeChanged( int );
    void LutFixedModeChanged( int );
    void LutPresetChanged( int );

    void LutRec709Changed( int, int, int, int, int, int );

    void LutSampleValuesChanged( QVector<int>, QVector<int> );
    void LutSampleValuesRedChanged( QVector<int>, QVector<int> );
    void LutSampleValuesGreenChanged( QVector<int>, QVector<int> );
    void LutSampleValuesBlueChanged( QVector<int>, QVector<int> );
    void LutSampleValuesMasterChanged( QVector<int>, QVector<int> );

    void LutSampleValuesRedRequested();
    void LutSampleValuesGreenRequested();
    void LutSampleValuesBlueRequested();

    void LutInterpolateChanged();
    void LutInterpolateRedChanged();
    void LutInterpolateGreenChanged();
    void LutInterpolateBlueChanged();

    void LutResetChanged();
    void LutResetMasterChanged();
    void LutResetRedChanged();
    void LutResetGreenChanged();
    void LutResetBlueChanged();

    void LutFastGammaChanged( int );

public slots:
    void onLutEnableChange( int, int );
    void onLutModeChange( int );
    void onLutFixedModeChange( int );
    void onLutPresetChange( int );

    void onLutSampleValuesMasterChange( QVector<int> x, QVector<int> y );
    void onLutSampleValuesRedChange( QVector<int> x, QVector<int> y );
    void onLutSampleValuesGreenChange( QVector<int> x, QVector<int> y );
    void onLutSampleValuesBlueChange( QVector<int> x, QVector<int> y );

    void onLutFastGammaChange( int );

private:
    void SetXRangeChanged( const QCPRange &, QCustomPlot * );
    void SetYRangeChanged( const QCPRange &, QCustomPlot * );

    void SampleChanged( LutChannel ch );
    void PlotClicked( QMouseEvent * evt, LutChannel ch );

private slots:
    // lut plot tab-widget
    void onColorSelectChange( int );
    void onMasterXRangeChanged( const QCPRange & );
    void onMasterYRangeChanged( const QCPRange & );
    void onRedXRangeChanged( const QCPRange & );
    void onRedYRangeChanged( const QCPRange & );
    void onGreenXRangeChanged( const QCPRange & );
    void onGreenYRangeChanged( const QCPRange & );
    void onBlueXRangeChanged( const QCPRange & );
    void onBlueYRangeChanged( const QCPRange & );

    // lut operational mode
    void onLutModeInterpolateClicked( bool );
    void onLutModeFastGammaClicked( bool );
    void onLutFixedModeRec709Clicked( bool );
    void onLutFixedModePQClicked( bool );
    void onLutFixedModeHLGClicked( bool );

    // lut storage select
    void onLutPresetClicked( QAbstractButton * );
    
    // sample 
    void onRemoveSampleClicked();
    void onResetClicked();
    void onInterpolateClicked();
    void onImportClicked();
    void onExportClicked();

    void onMasterSampleChanged( const QModelIndex &, const QModelIndex & );
    void onRedSampleChanged( const QModelIndex &, const QModelIndex & );
    void onGreenSampleChanged( const QModelIndex &, const QModelIndex & );
    void onBlueSampleChanged( const QModelIndex &, const QModelIndex & );
    
    void onMasterSampleRemoved( const QModelIndex &, int, int );
    void onRedSampleRemoved( const QModelIndex &, int, int );
    void onGreenSampleRemoved( const QModelIndex &, int, int );
    void onBlueSampleRemoved( const QModelIndex &, int, int );
    
    void onMasterPlotClicked( QMouseEvent * );
    void onRedPlotClicked( QMouseEvent * );
    void onGreenPlotClicked( QMouseEvent * );
    void onBluePlotClicked( QMouseEvent * );

    void onSelectionChange( const QItemSelection &, const QItemSelection & );

    // REC.709
    void onDefaultRec709Clicked();
    void onSetRec709Clicked();

    // fast gamma
    void onFastGammaChanged( int );
    void onDefaultFastGammaClicked();

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __LUT_BOX_H__

