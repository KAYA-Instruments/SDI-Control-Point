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

    // color space
    void GetColorSpace();

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

    // color space
    void ColorSpaceChanged( int value );

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

    void onColorConversionMatrixRequested();

    // color space
    void onColorSpaceChange( int value );

    // split-screen
    void onSplitScreenChange( bool value );

    // slot to update the white blance settings
    void onNotifyWhiteBalanceUpdate();
    
    // slot to update black level on camera gain changes
    void onNotifyCameraGainChange();
};

#endif // _ISP_INTERFACE_H_

