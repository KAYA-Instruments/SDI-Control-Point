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
 * @file    LutItf.h
 *
 * @brief   LUT Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _LUT_INTERFACE_H_
#define _LUT_INTERFACE_H_

#include <QObject>
#include <QVector>

#include "ProVideoItf.h"

class LutItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit LutItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }

    // resync all settings
    void resync() override;

    // enable state 
    void GetLutEnable( int id );
    
    // preset storage
    void GetLutPreset();

    // table values
    void GetLutValuesRed();
    void GetLutValuesGreen();
    void GetLutValuesBlue();

    // sample values  (interpolation samples)
    void GetLutSampleValuesRed();
    void GetLutSampleValuesGreen();
    void GetLutSampleValuesBlue();
    void GetLutSampleValuesMaster();

signals:
    // enable status
    void LutEnableChanged( int id, int value );
    
    // preset storage
    void LutPresetChanged( int value );

    // read/write address 
    void LutWriteIndexChanged( int value );
    void LutWriteIndexRedChanged( int value );
    void LutWriteIndexGreenChanged( int value );
    void LutWriteIndexBlueChanged( int value );
    
    // table values
    void LutValuesRedChanged( QVector<int> values );
    void LutValuesGreenChanged( QVector<int> values );
    void LutValuesBlueChanged( QVector<int> values );

    // sample values (interpolation samples)
    void LutSampleValuesRedChanged( QVector<int> x, QVector<int> y );
    void LutSampleValuesGreenChanged( QVector<int> x, QVector<int> y );
    void LutSampleValuesBlueChanged( QVector<int> x, QVector<int> y );
    void LutSampleValuesMasterChanged( QVector<int> x, QVector<int> y );
     
public slots:
    // enable status
    void onLutEnableChange( int id, int value );
    
    // preset storage
    void onLutPresetChange( int value );

    // read/write address 
    void onLutWriteIndexChange( int value );
    void onLutWriteIndexRedChange( int value );
    void onLutWriteIndexGreenChange( int value );
    void onLutWriteIndexBlueChange( int value );

    // REC.709
    void onLutRec709Change( int threshold, int lcontrast,  int lbrightness, int contrast, int gamma, int brightness );
   
    // set sample values
    void onLutSampleValuesChange( QVector<int> x, QVector<int> y );
    void onLutSampleValuesRedChange( QVector<int> x, QVector<int> y );
    void onLutSampleValuesGreenChange( QVector<int> x, QVector<int> y );
    void onLutSampleValuesBlueChange( QVector<int> x, QVector<int> y );
    void onLutSampleValuesMasterChange( QVector<int> x, QVector<int> y );
    
    // request sample values
    void onLutSampleValuesRedRequest();
    void onLutSampleValuesGreenRequest();
    void onLutSampleValuesBlueRequest();
    void onLutSampleValuesMasterRequest();

    // interpolate
    void onLutInterpolate();
    void onLutInterpolateRed();
    void onLutInterpolateGreen();
    void onLutInterpolateBlue();

    // reset
    void onLutReset();
    void onLutResetRed();
    void onLutResetGreen();
    void onLutResetBlue();
    void onLutResetMaster();
};

#define CONNECT_LUT_INTERFACE(x, y)                                                         \
{                                                                                           \
    QObject::connect( x, SIGNAL(LutEnableChanged(int,int)),                                 \
                      y, SLOT(onLutEnableChange(int,int)) );                                \
    QObject::connect( x, SIGNAL(LutPresetChanged(int)),                                     \
                      y, SLOT(onLutPresetChange(int)) );                                    \
    QObject::connect( x, SIGNAL(LutSampleValuesRedChanged(QVector<int>,QVector<int>)),      \
                      y, SLOT(onLutSampleValuesRedChange(QVector<int>,QVector<int>)) );     \
    QObject::connect( x, SIGNAL(LutSampleValuesGreenChanged(QVector<int>,QVector<int>)),    \
                      y, SLOT(onLutSampleValuesGreenChange(QVector<int>,QVector<int>)) );   \
    QObject::connect( x, SIGNAL(LutSampleValuesBlueChanged(QVector<int>,QVector<int>)),     \
                      y, SLOT(onLutSampleValuesBlueChange(QVector<int>,QVector<int>)) );    \
}

#endif // _LUT_INTERFACE_H_

