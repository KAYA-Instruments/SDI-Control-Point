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
    
    // operational mode
    void GetLutMode();
    void GetLutFixedMode();
    void GetLogMode();

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

    // fast gamma
    void GetLutFastGamma();

signals:
    // enable status
    void LutEnableChanged( int id, int value );
    
    // operational mode storage
    void LutModeChanged( int mode );
    void LutFixedModeChanged( int mode );
    void LogModeChanged( int mode );

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

    // fast gamma
    void LutFastGammaChanged( int gamma );

    // new LOG mode is set
    // This is used to synchronize other interface components.
    void NotifyLogModeChanged();
     
public slots:
    // enable status
    void onLutEnableChange( int id, int value );

    // operational mode
    void onLutModeChange( int mode );
    void onLutFixedModeChange( int mode );
    void onLogModeChange( int mode );
    
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

    // fast gamma
    void onLutFastGammaChange( int gamma );
};

#endif // _LUT_INTERFACE_H_

