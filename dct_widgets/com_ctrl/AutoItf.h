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

    // notifier slot for white-balance update 
    void onNotifyWhiteBalanceUpdate();
};

#define CONNECT_AUTO_INTERFACE(x, y)                                    \
{                                                                       \
    QObject::connect( x, SIGNAL(NoWbPresetsChanged(int)),               \
                      y, SLOT(onNoWbPresetsChange(int)) );              \
    QObject::connect( x, SIGNAL(WbPresetsChanged(int,QString,int)),     \
                      y, SLOT(onWbPresetsChange(int,QString,int)) );    \
}
 
#endif // _AUTO_INTERFACE_H_

