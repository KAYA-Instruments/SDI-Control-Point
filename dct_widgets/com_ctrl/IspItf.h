/******************************************************************************
 *
 * Copyright 2015, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
/**
 * @file    IspItf.h
 *
 * @brief   ISP Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _ISP_INTERFACE_H_
#define _ISP_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

class IspItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit IspItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }
 
    // resync all settings
    void resync() override;

    // lens shading correction
    void GetLsc();
    
    // bayer pattern
    void GetBayerPattern();

    // white balance gains
    void GetRedGain();
    void GetGreenGain();
    void GetBlueGain();
    void GetGain();

    // sensor black level
    void GetRedBlackLevel();
    void GetGreenBlackLevel();
    void GetBlueBlackLevel();
    void GetBlackLevel();
    
    // flare level
    void GetFlareLevel();
    
    // master black level
    void GetMasterBlackLevel();

    // detail and de-noise filter stage
    void GetFilterEnable();
    void GetFilterDetailLevel();
    void GetFilterDenoiseLevel();
    void GetFilter();

    // color correction
    void GetColorCorrectionMatrix();
    void GetColorCorrectionOffset();
    void GetColorCorrection();
    
    // yuv conversion
    void GetColorConversionMatrix();

    // split-screen
    void GetSplitScreen();

signals:
    // lens shading correction
    void LscChanged( QVector<uint> values );

    // bayer pattern
    void BayerPatternChanged( int value );

    // white balance gains changed
    void RedGainChanged( int value );
    void GreenGainChanged( int value );
    void BlueGainChanged( int value );
    void GainChanged( int red, int green, int blue );

    // sensor black level changed
    void RedBlackLevelChanged( int value );
    void GreenBlackLevelChanged( int value );
    void BlueBlackLevelChanged( int value );
    void BlackLevelChanged( int red, int green, int blue );
    
    // flare level
    void FlareLevelChanged( int red, int green, int blue );

    // master black level
    void MasterBlackLevelChanged( int red, int green, int blue );
    
    // detail and de-noise filter stage
    void FilterEnableChanged( int value );
    void FilterDetailLevelChanged( int value );
    void FilterDenoiseLevelChanged( int value );
    void FilterChanged( int enable, int detail, int denoise );

    // color correction
    void ColorCorrectionMatrixChanged( int c0, int c1, int c2,
                                       int c3, int c4, int c5,
                                       int c6, int c7, int c8 );
    void ColorCorrectionOffsetChanged( int red, int green, int blue );
    void ColorCorrectionChanged( int c0, int c1, int c2,
                                 int c3, int c4, int c5,
                                 int c6, int c7, int c8,
                                 int red, int green, int blue );

    // yuv conversion
    void ColorConversionMatrixChanged( int c0, int c1, int c2,
                                       int c3, int c4, int c5,
                                       int c6, int c7, int c8 );

    // split-screen
    void SplitScreenChanged( bool value );

public slots:
    // lens shading correction
    void onLscChange( QVector<uint> value );

    // bayer pattern
    void onBayerPatternChange( int value );

    // white balance gains
    void onRedGainChange( int value );
    void onGreenGainChange( int value );
    void onBlueGainChange( int value );
    void onGainChange( int red, int green, int blue );

    // sensor black level
    void onRedBlackLevelChange( int value );
    void onGreenBlackLevelChange( int value );
    void onBlueBlackLevelChange( int value );
    void onBlackLevelChange( int red, int green, int blue );

    // flare level
    void onFlareLevelChange( int red, int green, int blue );

    // master black level
    void onMasterBlackLevelChange( int red, int green, int blue );

    // detail and de-noise filter stage
    void onFilterEnableChange( int value );
    void onFilterDetailLevelChange( int value );
    void onFilterDenoiseLevelChange( int value );
    void onFilterChange( int enable, int detail, int denoise );

    // color correction
    void onColorCorrectionMatrixChange( int c0, int c1, int c2,
                                        int c3, int c4, int c5,
                                        int c6, int c7, int c8 );
    void onColorCorrectionOffsetChange( int red, int green, int blue );
    void onColorCorrectionChange( int c0, int c1, int c2,          
                                  int c3, int c4, int c5,
                                  int c6, int c7, int c8,
                                  int red, int green, int blue );

    // yuv conversion
    void onColorConversionMatrixChange( int c0, int c1, int c2,
                                        int c3, int c4, int c5,
                                        int c6, int c7, int c8 );

    // split-screen
    void onSplitScreenChange( bool value );

    // slot to update the white blance settings
    void onNotifyWhiteBalanceUpdate();
    
    // slot to update black level on camera gain changes
    void onNotifyCameraGainChange();
};

#define CONNECT_ISP_INTERFACE(x, y)                                                                         \
{                                                                                                           \
    QObject::connect( x, SIGNAL(LscChanged(int)),                                                           \
                      y, SLOT(onLscChange( QVector<uint> value);                                            \
    QObject::connect( x, SIGNAL(BayerPatternChanged(int)),                                                  \
                      y, SLOT(onBayerPatternChange(int)) );                                                 \
    QObject::connect( x, SIGNAL(RedGainChanged(int)),                                                       \
                      y, SLOT(onRedGainChange(int)) );                                                      \
    QObject::connect( x, SIGNAL(GreenGainChanged(int)),                                                     \
                      y, SLOT(onGreenGainChange(int)) );                                                    \
    QObject::connect( x, SIGNAL(BlueGainChanged(int)),                                                      \
                      y, SLOT(onBlueGainChange(int)) );                                                     \
    QObject::connect( x, SIGNAL(GainChanged(int,int,int)),                                                  \
                      y, SLOT(onGainChange(int,int,int)) );                                                 \
    QObject::connect( x, SIGNAL(RedBlackLevelChanged(int)),                                                 \
                      y, SLOT(onRedBlackLevelChange(int)) );                                                \
    QObject::connect( x, SIGNAL(GreenBlackLevelChanged(int)),                                               \
                      y, SLOT(onGreenBlackLevelChange(int)) );                                              \
    QObject::connect( x, SIGNAL(BlueBlackLevelChanged(int)),                                                \
                      y, SLOT(onBlueBlackLevelChange(int)) );                                               \
    QObject::connect( x, SIGNAL(BlackLevelChanged(int,int,int)),                                            \
                      y, SLOT(onBlackLevelChange(int,int,int)) );                                           \
    QObject::connect( x, SIGNAL(FlareLevelChanged(int,int,int)),                                            \
                      y, SLOT(onFlareLevelChange(int,int,int)) );                                           \
    QObject::connect( x, SIGNAL(MasterBlackLevelChanged(int,int,int)),                                      \
                      y, SLOT(onMasterBlackLevelChange(int,int,int)) );                                     \
    QObject::connect( x, SIGNAL(FilterEnableChanged(int)),                                                  \
                      y, SLOT(onFilterEnableChange(int)) );                                                 \
    QObject::connect( x, SIGNAL(FilterDetailLevelChanged(int)),                                             \
                      y, SLOT(onFilterDetailLevelChange(int)) );                                            \
    QObject::connect( x, SIGNAL(FilterDenoiseLevelChanged(int)),                                            \
                      y, SLOT(onFilterDenoiseLevelChange(int)) );                                           \
    QObject::connect( x, SIGNAL(FilterChanged(int,int,int)),                                                \
                      y, SLOT(onFilterChange(int,int,int)) );                                               \
    QObject::connect( x, SIGNAL(ColorCorrectionMatrixChanged(int,int,int,int,int,int,int,int,int)),         \
                      y, SLOT(onColorCorrectionMatrixChange(int,int,int,int,int,int,int,int,int)) );        \
    QObject::connect( x, SIGNAL(ColorCorrectionOffsetChanged(int,int,int)),                                 \
                      y, SLOT(onColorCorrectionOffsetChange(int,int,int)) );                                \
    QObject::connect( x, SIGNAL(ColorCorrectionChanged(int,int,int,int,int,int,int,int,int,int,int,int)),   \
                      y, SLOT(onColorCorrectionChange(int,int,int,int,int,int,int,int,int,int,int,int)) );  \
    QObject::connect( x, SIGNAL(ColorConversionMatrixChanged(int,int,int,int,int,int,int,int,int)),         \
                      y, SLOT(onColorConversionMatrixChange(int,int,int,int,int,int,int,int,int)) );        \
}

#endif // _ISP_INTERFACE_H_

