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
 * @file    AutoItf.h
 *
 * @brief   Auto Processing Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _AUTO_INTERFACE_H_
#define _AUTO_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

class AutoItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit AutoItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }
 
    // resync all settings
    void resync() override;

    // auto processing
    void GetAecEnable();
    void GetAecSetup();
    void GetAecWeights();
    void GetAwbEnable();
    void GetAwbSpeed();
    void GetNoWbPresets();
    void GetWbPresets( int const no = 10 );
    void GetStatRGB();
    void GetStatXYZ();
    void GetStatHistogramEnable();
    void GetStatHistogram();
    void GetStatExposureEnable();
    void GetStatExposure();
    void GetColorXYZ();

signals:
    // auto processing
    void AecEnableChanged( int value );
    void AecSetupChanged( QVector<int> value );
    void AecWeightsChanged( QVector<int> weights );
    void AwbEnableChanged( int value );
    void AwbSpeedChanged( int speed );
    void NoWbPresetsChanged( int value );
    void WbPresetsChanged( int id, QString name, int ct );
    void StatRGBChanged( int red, int green, int blue );
    void StatXYZChanged( int x, int y, int z );
    void StatHistogramEnableChanged( int value );
    void StatHistogramChanged( QVector<int> );
    void StatExposureEnableChanged( int value );
    void StatExposureChanged( QVector<int> );
    void ColorXYZChanged( int c0, int c1, int c2,
                          int c3, int c4, int c5,
                          int c6, int c7, int c8 );

    // white balance changed
    // This is used to synchronize other interface components
    // which are effected by white-balancing
    void NotifyWhiteBalanceChanged();

public slots:
    // auto white balance processing
    void onWbUpdate();
    void onAwbEnableChange( int enable );
    void onAwbSpeedChange( int enable );
    void onWbChange();
    void onWbPresetChange( int id );
    void onStatHistogramEnableChange( int flag );
    void onStatExposureEnableChange( int flag );
    
    // auto exposure processing
    void onAecEnableChange( int enable );
    void onAecSetupChange( QVector<int> value );
    void onAecWeightChange( int index, int weight );

    // notifier slot for white-balance update 
    void onNotifyWhiteBalanceUpdate();
};

#endif // _AUTO_INTERFACE_H_

