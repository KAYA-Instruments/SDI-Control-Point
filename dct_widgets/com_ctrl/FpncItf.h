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
 * @file    FpncItf.h
 *
 * @brief   Fix Pattern Noise Correction Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _FPNC_INTERFACE_H_
#define _FPNC_INTERFACE_H_

#include <QObject>
#include <QVector>

#include "ProVideoItf.h"
#include "FpncData.h"

class FpncItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit FpncItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }

    // resync all settings
    void resync() override;

    // enable state 
    void GetFpncEnable();
    
    // inverse gains (pre correction)
    void GetFpncInverseGains();
    
    // gains (post correction)
    void GetFpncGains();

    // fpnc correction data
    void GetFpncCorrectionData( int column );
    
    // fpnc calibration data
    void GetFpncCalibrationData( int column );

signals:
    // enable status
    void FpncEnableChanged( int flag );
    
    // inverse gains
    void FpncInverseGainsChanged( int a, int b, int c, int d );
    
    // gains
    void FpncGainsChanged( int a, int b, int c, int d );
    
    // correction data
    void FpncCorectionDataChanged( int column, QVector<int> data0, QVector<int> data1 );
    
    // calibration data 
    void FpncCalibrationDataChanged( int column, QVector<int> data0, QVector<int> data1 );
    
public slots:
    // enable status
    void onFpncEnableChange( int flag );

    // inverse gains (pre correction)
    void onFpncInverseGainsChange( int a, int b, int c, int d );

    // gains (post correction)
    void onFpncGainsChange( int a, int b, int c, int d );

    // column change
    void onFpncColumnChanged( int value );

    // calibration data
    void onFpncColumnCalibrationDataChange( const bool evenLines, const int column, QVector<int> & data );

    // slot to update gains on camera gain changes
    void onNotifyCameraGainChange();

private:
    // read calibration data (value = 24 bit)
    int ReadCalibrationDataFromDevice( const int, const int, const int, QVector<int> & );
   
    // write calibration data (value = 24 bit)
    int WriteCalibrationDataToDevice( const int, const int, const int, QVector<int> & );
    
    // read correction data (value = 12 bit)
    int ReadCorrectionDataFromDevice( const int, const int, const int, QVector<int> & );
};

#define CONNECT_FPNC_INTERFACE(x, y)                            \
{                                                               \
    QObject::connect( x, SIGNAL(FpncEnableChanged(int)),        \
                      y, SLOT(onFpncEnableChange(int)) );       \
}


#endif // _FPNC_INTERFACE_H_

