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
 * @file    gammabox.h
 *
 * @brief   Class definition of a gamma box 
 *
 *****************************************************************************/
#ifndef __GAMMA_BOX_H__
#define __GAMMA_BOX_H__

#include <dct_widgets_base.h>
#include <qcustomplot.h>

/******************************************************************************
 * Threshold Definitions
 *****************************************************************************/
#define GAMMA_THRESHOLD_MIN                         (        0 )
#define GAMMA_THRESHOLD_MAX                         (     1000 )
#define GAMMA_THRESHOLD_COMMA_POSITION              (        3 )
#define GAMMA_THRESHOLD_BASE                        (       10 )
#define GAMMA_THRESHOLD_DISPLAY_MULTIPLIER          (        1 )
#define GAMMA_THRESHOLD_DISPLAY_MASK                (  "%1.3f" )

/******************************************************************************
 * Gamma Definitions
 *****************************************************************************/
#define GAMMA_GAMMA_MIN                             (        0 )
#define GAMMA_GAMMA_MAX                             (     1000 )
#define GAMMA_GAMMA_COMMA_POSITION                  (        3 )
#define GAMMA_GAMMA_BASE                            (       10 )
#define GAMMA_GAMMA_DISPLAY_MULTIPLIER              (        1 )
#define GAMMA_GAMMA_DISPLAY_MASK                    (  "%1.3f" )

/******************************************************************************
 * Linear Contrast Definitions
 *****************************************************************************/
#define GAMMA_LINEAR_CONTRAST_MIN                   (        0 )
#define GAMMA_LINEAR_CONTRAST_MAX                   (    10000 )
#define GAMMA_LINEAR_CONTRAST_COMMA_POSITION        (        3 )
#define GAMMA_LINEAR_CONTRAST_BASE                  (       10 )
#define GAMMA_LINEAR_CONTRAST_DISPLAY_MULTIPLIER    (        1 )
#define GAMMA_LINEAR_CONTRAST_DISPLAY_MASK          (  "%2.3f" )

/******************************************************************************
 * Linear Brightness Definitions
 *****************************************************************************/
#define GAMMA_LINEAR_BRIGHTNESS_MIN                 (    -1000 )
#define GAMMA_LINEAR_BRIGHTNESS_MAX                 (     1000 )
#define GAMMA_LINEAR_BRIGHTNESS_COMMA_POSITION      (        3 )
#define GAMMA_LINEAR_BRIGHTNESS_BASE                (       10 )
#define GAMMA_LINEAR_BRIGHTNESS_DISPLAY_MULTIPLIER  (        1 )
#define GAMMA_LINEAR_BRIGHTNESS_DISPLAY_MASK        ( "%-1.3f" )

/******************************************************************************
 * Contrast Definitions
 *****************************************************************************/
#define GAMMA_CONTRAST_MIN                          (        0 )
#define GAMMA_CONTRAST_MAX                          (    10000 )
#define GAMMA_CONTRAST_COMMA_POSITION               (        3 )
#define GAMMA_CONTRAST_BASE                         (       10 )
#define GAMMA_CONTRAST_DISPLAY_MULTIPLIER           (        1 )
#define GAMMA_CONTRAST_DISPLAY_MASK                 (  "%2.3f" )

/******************************************************************************
 * Brightness Definitions
 *****************************************************************************/
#define GAMMA_BRIGHTNESS_MIN                        (    -1000 )
#define GAMMA_BRIGHTNESS_MAX                        (     1000 )
#define GAMMA_BRIGHTNESS_COMMA_POSITION             (        3 )
#define GAMMA_BRIGHTNESS_BASE                       (       10 )
#define GAMMA_BRIGHTNESS_DISPLAY_MULTIPLIER         (        1 )
#define GAMMA_BRIGHTNESS_DISPLAY_MASK               ( "%-1.3f" )

/******************************************************************************
 * GammaBox Configuration Box Widget
 *****************************************************************************/
class GammaBox : public DctWidgetBox
{
    Q_OBJECT

public:
    enum GammaChannel { Master = 0, Red = 1, Green = 2, Blue = 3, GammaChannelMax };

    explicit GammaBox( QWidget * parent = 0 );
    ~GammaBox();

    bool GammaEnable() const;
    void setGammaEnable( const bool );

    int  Threshold( const GammaBox::GammaChannel ) const;
    int  Gamma( const GammaBox::GammaChannel ) const;
    int  LinearContrast( const GammaBox::GammaChannel ) const;
    int  LinearBrightness( const GammaBox::GammaChannel ) const;
    int  Contrast( const GammaBox::GammaChannel ) const;
    int  Brightness( const GammaBox::GammaChannel ) const;

    void setGammaCurve( const GammaBox::GammaChannel,
            const int, const int, const int, const int, const int, const int );

protected:
    void prepareMode( const Mode mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & s ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

signals:
    void GammaEnableChanged( int value );
    void GammaChanged( int, int, int, int, int, int );
    void GammaRedChanged( int, int, int, int, int, int );
    void GammaGreenChanged( int, int, int, int, int, int );
    void GammaBlueChanged( int, int, int, int, int, int );

public slots:
    void onGammaEnableChange( int );
    void onGammaRedChange( int, int, int, int, int, int );
    void onGammaGreenChange( int, int, int, int, int, int );
    void onGammaBlueChange( int, int, int, int, int, int );
    
private:
    void SetXRangeChanged( const QCPRange &, QCustomPlot * );
    void SetYRangeChanged( const QCPRange &, QCustomPlot * );

private slots:
    void onThresholdChange( int );
    void onLinearContrastChange( int );
    void onLinearBrightnessChange( int );
    void onContrastChange( int );
    void onGammaChange( int );
    void onBrightnessChange( int );
    
    void onGammaSelectChange( int );

    void onMasterXRangeChanged( const QCPRange & );
    void onMasterYRangeChanged( const QCPRange & );
    void onRedXRangeChanged( const QCPRange & );
    void onRedYRangeChanged( const QCPRange & );
    void onGreenXRangeChanged( const QCPRange & );
    void onGreenYRangeChanged( const QCPRange & );
    void onBlueXRangeChanged( const QCPRange & );
    void onBlueYRangeChanged( const QCPRange & );
    
    void onEnableClicked();
    void onStraightClicked();
    void onRec709Clicked();
    void onBt601Clicked();
    void onApplyClicked();

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __GAMMA_BOX_H__

