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
 * @file    FpncItf.cpp
 *
 * @brief   Implementation of Fix Pattern Noise Correction Interface
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <QtDebug>

#include <ctrl_protocol/ctrl_protocol_fpnc.h>

#include "common.h"
#include "FpncItf.h"

/******************************************************************************
 * FpncItf::resync()
 *****************************************************************************/
void FpncItf::resync()
{
    // enable status
    GetFpncEnable();

    // inverse gains 
    GetFpncInverseGains();
    
    // gains 
    GetFpncGains();
    
    // correction data for column 0
    GetFpncCorrectionData( 0 );
}

/******************************************************************************
 * FpncItf::GetFpncEnable
 *****************************************************************************/
void FpncItf::GetFpncEnable()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(FpncEnableChanged(int))) > 0 )
    {
        uint8_t value;
    
        // read fpnc enable status from device
        int res = ctrl_protocol_get_fpnc_enable( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a FpncEnableChanged signal
        emit FpncEnableChanged( (int)value );
    }
}

/******************************************************************************
 * FpncItf::GetFpncInverseGains
 *****************************************************************************/
void FpncItf::GetFpncInverseGains()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(FpncInverseGainsChanged(int,int,int,int))) > 0 )
    {
        ctrl_protocol_fpnc_inv_gains_t v ;
    
        // read fpnc enable status from device
        int res = ctrl_protocol_get_fpnc_inv_gains( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
        HANDLE_ERROR( res );
        
        // emit a FpncEnableChanged signal
        emit FpncInverseGainsChanged( 
            (int)v.inv_gain_0, (int)v.inv_gain_1, (int)v.inv_gain_2, (int)v.inv_gain_3 );
    }
}

/******************************************************************************
 * FpncItf::GetFpncGains
 *****************************************************************************/
void FpncItf::GetFpncGains()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(FpncGainsChanged(int,int,int,int))) > 0 )
    {
        ctrl_protocol_fpnc_gains_t v ;
    
        // read fpnc enable status from device
        int res = ctrl_protocol_get_fpnc_gains( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
        HANDLE_ERROR( res );
        
        // emit a FpncEnableChanged signal
        emit FpncGainsChanged( (int)v.gain_0, (int)v.gain_1, (int)v.gain_2, (int)v.gain_3 );
    }
}

/******************************************************************************
 * FpncItf::GetFpncCorrectionData
 *****************************************************************************/
void FpncItf::GetFpncCorrectionData( int column )
{
    onFpncColumnChanged( column );
}

/******************************************************************************
 * FpncItf::GetFpncCalibrationData
 *****************************************************************************/
void FpncItf::GetFpncCalibrationData( int column )
{
    // Is there a signal listener
    if ( receivers(SIGNAL(FpncCorectionDataChanged(int, QVector<int>, QVector<int>))) > 0 )
    {
        int res;

        QVector<int> data0( FPNC_DATA_PER_COLUMN );
        QVector<int> data1( FPNC_DATA_PER_COLUMN );

        // read fpnc calibration data 0 from device
        res = ReadCalibrationDataFromDevice( 0, column,  0, data0 );
        HANDLE_ERROR( res );
        
        res = ReadCalibrationDataFromDevice( 0, column,  4, data0 );
        HANDLE_ERROR( res );
        
        res = ReadCalibrationDataFromDevice( 0, column,  8, data0 );
        HANDLE_ERROR( res );
        
        res = ReadCalibrationDataFromDevice( 0, column, 12, data0 );
        HANDLE_ERROR( res );

        // read fpnc calibration data 1 from device
        res = ReadCalibrationDataFromDevice( 1, column,  0, data0 );
        HANDLE_ERROR( res );
        
        res = ReadCalibrationDataFromDevice( 1, column,  4, data0 );
        HANDLE_ERROR( res );
        
        res = ReadCalibrationDataFromDevice( 1, column,  8, data0 );
        HANDLE_ERROR( res );
        
        res = ReadCalibrationDataFromDevice( 1, column, 12, data0 );
        HANDLE_ERROR( res );

        // emit a FpncEnableChanged signal
        emit FpncCalibrationDataChanged( column, data0, data1 );
    }
}

/******************************************************************************
 * FpncItf::onFpncEnableChange
 *****************************************************************************/
void FpncItf::onFpncEnableChange( int value )
{
    // set fpnc enable state on device
    int res = ctrl_protocol_set_fpnc_enable( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), ( value ? 1u : 0u ) );
    HANDLE_ERROR( res );

    GetFpncInverseGains();
    GetFpncGains();
}

/******************************************************************************
 * FpncItf::onFpncInverseGainsChange
 *****************************************************************************/
void FpncItf::onFpncInverseGainsChange( int a, int b, int c, int d )
{
    ctrl_protocol_fpnc_inv_gains_t v = { (uint16_t)a, (uint16_t)b, (uint16_t)c, (uint16_t)d };
    
    // set fpnc inverse gains on device
    int res = ctrl_protocol_set_fpnc_inv_gains( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t*)&v );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * FpncItf::onFpncGainsChange
 *****************************************************************************/
void FpncItf::onFpncGainsChange( int a, int b, int c, int d )
{
    ctrl_protocol_fpnc_gains_t v = { (uint16_t)a, (uint16_t)b, (uint16_t)c, (uint16_t)d };

    // set fpnc inverse gains on device
    int res = ctrl_protocol_set_fpnc_gains( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t*)&v );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * FpncItf::onFpncColumnChanged
 *****************************************************************************/
void FpncItf::onFpncColumnChanged( int column )
{
    // Is there a signal listener
    if ( receivers(SIGNAL(FpncCorectionDataChanged(int, QVector<int>, QVector<int>))) > 0 )
    {
        QVector<int> data0( FPNC_DATA_PER_COLUMN );
        QVector<int> data1( FPNC_DATA_PER_COLUMN );

        int res;

        // read fpnc calibration data 0 from device
        res = ReadCorrectionDataFromDevice( 0, column,  0, data0 );
        HANDLE_ERROR( res );
        
        res = ReadCorrectionDataFromDevice( 0, column,  4, data0 );
        HANDLE_ERROR( res );

        res = ReadCorrectionDataFromDevice( 0, column,  8, data0 );
        HANDLE_ERROR( res );

        res = ReadCorrectionDataFromDevice( 0, column, 12, data0 );
        HANDLE_ERROR( res );

        // read fpnc calibration data 1 from device
        res = ReadCorrectionDataFromDevice( 1, column,  0, data1 );
        HANDLE_ERROR( res );

        res = ReadCorrectionDataFromDevice( 1, column,  4, data1 );
        HANDLE_ERROR( res );

        res = ReadCorrectionDataFromDevice( 1, column,  8, data1 );
        HANDLE_ERROR( res );

        res = ReadCorrectionDataFromDevice( 1, column, 12, data1 );
        HANDLE_ERROR( res );

        // uncompress data
        data0 = fpnc_uncompress( data0 );
        data1 = fpnc_uncompress( data1 );

        // emit a FpncEnableChanged signal
        emit FpncCorectionDataChanged( column, data0, data1 );
    }
}

/******************************************************************************
 * FpncItf::onFpncColumnCalibrationDataChange
 *****************************************************************************/
void FpncItf::onFpncColumnCalibrationDataChange
(
    const bool      evenLines,
    const int       column,
    QVector<int> &  data
)
{
    int res;

    int pg = (evenLines) ? 0 : 1;

    // read fpnc calibration data 0 from device
    res = WriteCalibrationDataToDevice( pg, column,  0, data );
        HANDLE_ERROR( res );

    res = WriteCalibrationDataToDevice( pg, column,  4, data );
    HANDLE_ERROR( res );

    res = WriteCalibrationDataToDevice( pg, column,  8, data );
        HANDLE_ERROR( res );

    res = WriteCalibrationDataToDevice( pg, column, 12, data );
        HANDLE_ERROR( res );
}

/******************************************************************************
 * FpncItf::onNotifyCameraGainChange
 *****************************************************************************/
void FpncItf::onNotifyCameraGainChange()
{
    GetFpncInverseGains();
    GetFpncGains();
 } 

/******************************************************************************
 * FpncItf::ReadCalibrationDataFromDevice
 *****************************************************************************/
int FpncItf::ReadCalibrationDataFromDevice
(
    const int       page,
    const int       column,
    const int       offset,
    QVector<int> &  data
)
{
    ctrl_protocol_fpnc_data_t v;
    int res;

    int retry = 10;

    v.page   = (uint32_t)(page & 0x1u);
    v.column = (uint32_t)(column>>1); 
    v.offset = (uint32_t)(offset & 0xFu);

    do
    {
        // get values from device
        res = ctrl_protocol_get_fpnc_correction_data( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );

        // evaluate values
        res = ( res ) ? res : fpnc_validate_value( v.v0 );
        res = ( res ) ? res : fpnc_validate_value( v.v1 );
        res = ( res ) ? res : fpnc_validate_value( v.v2 );
        res = ( res ) ? res : fpnc_validate_value( v.v3 );
    }
    while ( (res != 0) && (retry--) );

    if ( !res )
    {
        // successfully read
        data[offset    ] = v.v0;
        data[offset + 1] = v.v1;
        data[offset + 2] = v.v2;
        data[offset + 3] = v.v3;
    }

    return ( res );
}

/******************************************************************************
 * FpncItf::WriteCalibrationDataToDevice
 *****************************************************************************/
int FpncItf::WriteCalibrationDataToDevice
(
    const int       page,
    const int       column,
    const int       offset,
    QVector<int> &  data
)
{
    ctrl_protocol_fpnc_data_t v;
    int res;
 
    int retry = 10;

    res = fpnc_validate_value( data[offset    ] );
    if ( res )
    {
        return ( res );
    }

    res = fpnc_validate_value( data[offset + 1] );
    if ( res )
    {
        return ( res );
    }
    
    res = fpnc_validate_value( data[offset + 2] );
    if ( res )
    {
        return ( res );
    }
    
    res = fpnc_validate_value( data[offset + 3] );
    if ( res )
    {
        return ( res );
    }

    v.page   = (uint32_t)(page & 0x1u);
    v.column = (uint32_t)(column>>1); 
    v.offset = (uint32_t)(offset & 0xFu);

    v.v0 = data[offset    ];
    v.v1 = data[offset + 1];
    v.v2 = data[offset + 2];
    v.v3 = data[offset + 3];

    do
    {
        // get values from device
        res = ctrl_protocol_set_fpnc_correction_data( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
    }
    while ( (res != 0) && (retry--) );

    return ( res );
}

/******************************************************************************
 * FpncItf::ReadCorrectionDataFromDevice
 *****************************************************************************/
int FpncItf::ReadCorrectionDataFromDevice
(
    const int       page,
    const int       column,
    const int       offset,
    QVector<int> &  data
)
{
    ctrl_protocol_fpnc_data_t v;
    int res;
 
    int retry = 10;

    v.page   = (uint32_t)(page & 0x1u);
    v.column = (uint32_t)(column>>1); 
    v.offset = (uint32_t)(offset & 0xFu);

    do
    {
        // get values from device
        res = ctrl_protocol_get_fpnc_correction_data( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
        
        // evaluate values
        res = ( res ) ? res : fpnc_validate_value( v.v0 );
        res = ( res ) ? res : fpnc_validate_value( v.v1 );
        res = ( res ) ? res : fpnc_validate_value( v.v2 );
        res = ( res ) ? res : fpnc_validate_value( v.v3 );
    } 
    while ( (res != 0) && (retry--) );

    if ( !res )
    {
        uint32_t mask  = ( column & 0x1 ) ? 0x00FFF000u : 0x00000FFFu; 
        uint32_t shift = ( column & 0x1 ) ?         12u : 0u;
 
        // successfully read
        data[offset    ] = fpnc_correction_value( v.v0, mask, shift );
        data[offset + 1] = fpnc_correction_value( v.v1, mask, shift );
        data[offset + 2] = fpnc_correction_value( v.v2, mask, shift );
        data[offset + 3] = fpnc_correction_value( v.v3, mask, shift );
    }

    return ( res );
}


