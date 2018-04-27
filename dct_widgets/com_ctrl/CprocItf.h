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
 * @file    CprocItf.h
 *
 * @brief   Color Processing Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _CPROC_INTERFACE_H_
#define _CPROC_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

class CprocItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit CprocItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }
 
    // resync all settings
    void resync() override;

    // color processing
    void GetBrightness();
    void GetContrast();
    void GetSaturation();
    void GetHue();
    void GetColorProcessing();

signals:
    // color processing
    void BrightnessChanged( int value );
    void ContrastChanged( int value );
    void SaturationChanged( int value );
    void HueChanged( int value );
    void ColorProcessingChanged( int brightness, int contrast, int saturation, int hue );

public slots:
    // color processing
    void onBrightnessChange( int value );
    void onContrastChange( int value );
    void onSaturationChange( int value );
    void onHueChange( int value );
    void onColorProcessingChange( int brightness, int contrast, int saturation, int hue );
};

#endif // _CPROC_INTERFACE_H_

