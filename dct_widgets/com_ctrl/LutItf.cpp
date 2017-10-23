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
 * @file    LutItf.cpp
 *
 * @brief   Implementation of LUT Interface 
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <ctrl_protocol/ctrl_protocol_lut.h>

#include "common.h"
#include "LutItf.h"

#include <QtDebug>

/******************************************************************************
 * LutItf::resync()
 *****************************************************************************/
void LutItf::resync()
{
    // enable status
    GetLutEnable( 0 );
    GetLutEnable( 1 );

    // operational mode
    GetLutMode();

    // preset
    // Note: Do this before GetLutFastGamma() otherwise the Lutbox might display the wrong plot
    GetLutPreset();

    // sync sample data 
    GetLutSampleValuesRed();
    GetLutSampleValuesGreen();
    GetLutSampleValuesBlue();
    GetLutSampleValuesMaster();

    // fast gamma setting
    GetLutFastGamma();
}

/******************************************************************************
 * LutItf::GetLutEnable
 *****************************************************************************/
void LutItf::GetLutEnable( int id )
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LutEnableChanged(int,int))) > 0 )
    {
        ctrl_protocol_enable_t v = { .id = (uint8_t)id, .flag = 0u };
    
        // read enable state from device
        int res = ctrl_protocol_get_lut_enable( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
        HANDLE_ERROR( res );
        
        // emit a LutEnableChanged signal
        emit LutEnableChanged( id, (int)v.flag );
    }
}

/******************************************************************************
 * LutItf::GetLutMode
 *****************************************************************************/
void LutItf::GetLutMode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LutModeChanged(int))) > 0 )
    {
        uint8_t mode = 0u;

        // read mode from device
        int res = ctrl_protocol_get_lut_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &mode );
        HANDLE_ERROR( res );

        // emit a LutPresetChanged signal
        emit LutModeChanged( (int)mode );
    }
}

/******************************************************************************
 * LutItf::GetLutPreset
 *****************************************************************************/
void LutItf::GetLutPreset()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LutPresetChanged(int))) > 0 )
    {
        uint8_t value = 0u;
    
        // read preset number from device
        int res = ctrl_protocol_get_lut_preset( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a LutPresetChanged signal
        emit LutPresetChanged( (int)value );
    }
}

/******************************************************************************
 * LutItf::GetLutValuesRed
 *****************************************************************************/
void LutItf::GetLutValuesRed()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LutValuesRedChanged(QVector<int> values))) > 0 )
    {
        uint16_t v[MAX_VALUES_LUT];

        // read red values from device
        int res = ctrl_protocol_get_lut_read_red( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v)/sizeof(v[0]), v );
        HANDLE_ERROR( res );
        
        QVector<int> d( MAX_VALUES_LUT );
        for( unsigned i=0u; i<MAX_VALUES_LUT; i++ )
        { 
            d[i] = v[i];
        }
        
        // emit a LutValuesRedChanged signal
        emit LutValuesRedChanged( d );
    }
}

/******************************************************************************
 * LutItf::GetLutValuesGreen
 *****************************************************************************/
void LutItf::GetLutValuesGreen()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LutValuesGreenChanged(QVector<int> values))) > 0 )
    {
        uint16_t v[MAX_VALUES_LUT];

        // read green values from device
        int res = ctrl_protocol_get_lut_read_green( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v)/sizeof(v[0]), v );
        HANDLE_ERROR( res );
        
        QVector<int> d( MAX_VALUES_LUT );
        for( unsigned i=0u; i<MAX_VALUES_LUT; i++ )
        { 
            d[i] = v[i];
        }
        
        // emit a LutValuesGreenChanged signal
        emit LutValuesGreenChanged( d );
    }
}

/******************************************************************************
 * LutItf::GetLutValuesBlue
 *****************************************************************************/
void LutItf::GetLutValuesBlue()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LutValuesBlueChanged(QVector<int> values))) > 0 )
    {
        uint16_t v[MAX_VALUES_LUT];

        // read blue values from device
        int res = ctrl_protocol_get_lut_read_blue( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v)/sizeof(v[0]), v );
        HANDLE_ERROR( res );
        
        QVector<int> d( MAX_VALUES_LUT );
        for( unsigned i=0u; i<MAX_VALUES_LUT; i++ )
        { 
            d[i] = v[i];
        }
        
        // emit a LutValuesBlueChanged signal
        emit LutValuesBlueChanged( d );
    }
}

/******************************************************************************
 * LutItf::GetLutSampleValuesRed
 *****************************************************************************/
void LutItf::GetLutSampleValuesRed()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LutSampleValuesRedChanged(QVector<int>, QVector<int>))) > 0 )
    {
        ctrl_protocol_samples_t v;

        memset( &v, 0, sizeof(v) );
        v.no = MAX_NO_SAMPLE_POINTS;

        // read red samples from device
        int res = ctrl_protocol_get_lut_sample_red( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
        HANDLE_ERROR( res );
        
        QVector<int> x( v.no );
        QVector<int> y( v.no );
        for( unsigned i=0u; i<v.no; i++ )
        { 
            x[i] = v.x_i[i];
            y[i] = v.y_i[i];
        }
        
        // emit a LutSampleValuesRedChanged signal
        emit LutSampleValuesRedChanged( x, y );
    }
}

/******************************************************************************
 * LutItf::GetLutSampleValuesGreen
 *****************************************************************************/
void LutItf::GetLutSampleValuesGreen()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LutSampleValuesGreenChanged(QVector<int>, QVector<int>))) > 0 )
    {
        ctrl_protocol_samples_t v;

        memset( &v, 0, sizeof(v) );
        v.no = MAX_NO_SAMPLE_POINTS;

        // read green samples from device
        int res = ctrl_protocol_get_lut_sample_green( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
        HANDLE_ERROR( res );
        
        QVector<int> x( v.no );
        QVector<int> y( v.no );
        for( unsigned i=0u; i<v.no; i++ )
        { 
            x[i] = v.x_i[i];
            y[i] = v.y_i[i];
        }
        
        // emit a LutSampleValuesGreenChanged signal
        emit LutSampleValuesGreenChanged( x, y );
    }
}

/******************************************************************************
 * LutItf::GetLutSampleValuesBlue
 *****************************************************************************/
void LutItf::GetLutSampleValuesBlue()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LutSampleValuesBlueChanged(QVector<int>, QVector<int>))) > 0 )
    {
        ctrl_protocol_samples_t v;

        memset( &v, 0, sizeof(v) );
        v.no = MAX_NO_SAMPLE_POINTS;

        // read blue samples from device
        int res = ctrl_protocol_get_lut_sample_blue( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
        HANDLE_ERROR( res );
        
        QVector<int> x( v.no );
        QVector<int> y( v.no );
        for( unsigned i=0u; i<v.no; i++ )
        { 
            x[i] = v.x_i[i];
            y[i] = v.y_i[i];
        }
        
        // emit a LutSampleValuesBlueChanged signal
        emit LutSampleValuesBlueChanged( x, y );
    }
}

/******************************************************************************
 * LutItf::GetLutSampleValuesMaster
 *****************************************************************************/
void LutItf::GetLutSampleValuesMaster()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LutSampleValuesMasterChanged(QVector<int>, QVector<int>))) > 0 )
    {
        ctrl_protocol_samples_t v;

        memset( &v, 0, sizeof(v) );
        v.no = MAX_NO_SAMPLE_POINTS;

        // read blue samples from device
        int res = ctrl_protocol_get_lut_sample_master( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
        HANDLE_ERROR( res );
        
        QVector<int> x( v.no );
        QVector<int> y( v.no );
        for( unsigned i=0u; i<v.no; i++ )
        { 
            x[i] = v.x_i[i];
            y[i] = v.y_i[i];
        }

        // emit a LutSampleValuesBlueChanged signal
        emit LutSampleValuesMasterChanged( x, y );
    }
}

/******************************************************************************
 * LutItf::GetLutFastGamma
 *****************************************************************************/
void LutItf::GetLutFastGamma()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LutFastGammaChanged(int))) > 0 )
    {
        int16_t gamma = 0u;

        // read fast gamma setting from device
        int res = ctrl_protocol_get_lut_fast_gamma( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &gamma );
        HANDLE_ERROR( res );

        // emit a LutPresetChanged signal
        emit LutFastGammaChanged( (int)gamma );
    }
}

/******************************************************************************
 * LutItf::onLutEnableChange
 *****************************************************************************/
void LutItf::onLutEnableChange( int id, int value )
{
    ctrl_protocol_enable_t v = { .id = (uint8_t)id, .flag = (uint8_t)value };

    // set LUT enable state on device
    int res = ctrl_protocol_set_lut_enable( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onLutModeChange
 *****************************************************************************/
void LutItf::onLutModeChange( int mode )
{
    // set LUT operational mode on device
    int res = ctrl_protocol_set_lut_mode( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint8_t)mode );
    HANDLE_ERROR( res );

    // If mode was changed to interpolation mode, sync sample data
    if ( mode == 0 )
    {
        GetLutSampleValuesMaster();
        GetLutSampleValuesRed();
        GetLutSampleValuesGreen();
        GetLutSampleValuesBlue();
    }

    // Else if mode was changed t ofast gamma, get current gamma setting
    else if ( mode == 1 )
    {
        GetLutFastGamma();
    }
}

/******************************************************************************
 * LutItf::onLutPresetChange
 *****************************************************************************/
void LutItf::onLutPresetChange( int value )
{
    // set LUT preset storage on device
    int res = ctrl_protocol_set_lut_preset( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
    
    // sync sample data 
    GetLutSampleValuesMaster();
    GetLutSampleValuesRed();
    GetLutSampleValuesGreen();
    GetLutSampleValuesBlue();
}

/******************************************************************************
 * LutItf::onLutWriteIndexChange
 *****************************************************************************/
void LutItf::onLutWriteIndexChange( int value )
{
    // set LUT read/write address for all tables on device
    int res = ctrl_protocol_set_lut_write_index( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint16_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onLutWriteIndexRedChange
 *****************************************************************************/
void LutItf::onLutWriteIndexRedChange( int value )
{
    // set LUT read/write address for red tables on device
    int res = ctrl_protocol_set_lut_write_index_red( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint16_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onLutWriteIndexGreenChange
 *****************************************************************************/
void LutItf::onLutWriteIndexGreenChange( int value )
{
    // set LUT read/write address for green tables on device
    int res = ctrl_protocol_set_lut_write_index_red( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint16_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onLutWriteIndexBlueChange
 *****************************************************************************/
void LutItf::onLutWriteIndexBlueChange( int value )
{
    // set LUT read/write address for blue tables on device
    int res = ctrl_protocol_set_lut_write_index_blue( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint16_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onLutRec709Change
 *****************************************************************************/
void LutItf::onLutRec709Change
(
    int threshold,
    int lcontrast,
    int lbrightness,
    int contrast,
    int gamma,
    int brightness
)
{
    ctrl_protocol_rec709_t values =
    {
        .threshold   = (uint16_t)threshold,
        .lcontrast   = (uint16_t)lcontrast,
        .lbrightness = (int16_t )lbrightness,
        .contrast    = (uint16_t)contrast,
        .gamma       = (uint16_t)gamma,
        .brightness  = (int16_t )brightness
    };

    // set mcc enable state on device
    int res = ctrl_protocol_set_lut_rec709( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), sizeof(ctrl_protocol_rec709_t), (uint8_t *)&values );
    HANDLE_ERROR( res );

    // sync sample data 
    GetLutSampleValuesRed();
    GetLutSampleValuesGreen();
    GetLutSampleValuesBlue();
    GetLutSampleValuesMaster();
}

/******************************************************************************
 * LutItf::onLutSampleValuesChange
 *****************************************************************************/
void LutItf::onLutSampleValuesChange( QVector<int> x, QVector<int> y )
{
    if ( (x.count() > 0) && (x.count() == y.count()) )
    {
        ctrl_protocol_samples_t v;
        
        memset( &v, 0, sizeof(v) );
        v.no = x.count();
        
        for( unsigned i=0u; i<v.no; i++ )
        { 
            v.x_i[i] = x[i];
            v.y_i[i] = y[i];
        }

        // set LUT reset on device (to delete sample points)
        int res = ctrl_protocol_set_lut_reset( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this) );
        HANDLE_ERROR( res );

        // set LUT sample on device
        res = ctrl_protocol_set_lut_sample( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
        HANDLE_ERROR( res );
    }
}

/******************************************************************************
 * LutItf::onLutSampleValuesRedChange
 *****************************************************************************/
void LutItf::onLutSampleValuesRedChange( QVector<int> x, QVector<int> y )
{
    if ( (x.count() > 0) && (x.count() == y.count()) )
    {
        ctrl_protocol_samples_t v;
        
        memset( &v, 0, sizeof(v) );
        v.no = x.count();
        
        for( unsigned i=0u; i<v.no; i++ )
        { 
            v.x_i[i] = x[i];
            v.y_i[i] = y[i];
        }

        // set LUT reset on device (to delete sample points)
        int res = ctrl_protocol_set_lut_reset_red( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this) );
        HANDLE_ERROR( res );

        // set LUT sample on device
        res = ctrl_protocol_set_lut_sample_red( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
        HANDLE_ERROR( res );
    }
}

/******************************************************************************
 * LutItf::onLutSampleValuesGreenChange
 *****************************************************************************/
void LutItf::onLutSampleValuesGreenChange( QVector<int> x, QVector<int> y )
{
    if ( (x.count() > 0) && (x.count() == y.count()) )
    {
        ctrl_protocol_samples_t v;
        
        memset( &v, 0, sizeof(v) );
        v.no = x.count();
        
        for( unsigned i=0u; i<v.no; i++ )
        { 
            v.x_i[i] = x[i];
            v.y_i[i] = y[i];
        }

        // set LUT reset on device (to delete sample points)
        int res = ctrl_protocol_set_lut_reset_green( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this) );
        HANDLE_ERROR( res );

        // set LUT sample on device
        res = ctrl_protocol_set_lut_sample_green( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
        HANDLE_ERROR( res );
    }
}

/******************************************************************************
 * LutItf::onLutSampleValuesBlueChange
 *****************************************************************************/
void LutItf::onLutSampleValuesBlueChange( QVector<int> x, QVector<int> y )
{
    if ( (x.count() > 0) && (x.count() == y.count()) )
    {
        ctrl_protocol_samples_t v;
        
        memset( &v, 0, sizeof(v) );
        v.no = x.count();
        
        for( unsigned i=0u; i<v.no; i++ )
        {
            v.x_i[i] = x[i];
            v.y_i[i] = y[i];
        }

        // set LUT reset on device (to delete sample points)
        int res = ctrl_protocol_set_lut_reset_blue( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this) );
        HANDLE_ERROR( res );

        // set LUT sample on device
        res = ctrl_protocol_set_lut_sample_blue( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
        HANDLE_ERROR( res );
    }
}

/******************************************************************************
 * LutItf::onLutSampleValuesMasterChange
 *****************************************************************************/
void LutItf::onLutSampleValuesMasterChange( QVector<int> x, QVector<int> y )
{
    if ( (x.count() > 0) && (x.count() == y.count()) )
    {
        ctrl_protocol_samples_t v;
        
        memset( &v, 0, sizeof(v) );
        v.no = x.count();
        
        for( unsigned i=0u; i<v.no; i++ )
        {
            v.x_i[i] = x[i];
            v.y_i[i] = y[i];
        }

        // set LUT reset on device (to delete sample points)
        int res = ctrl_protocol_set_lut_reset_master( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this) );
        HANDLE_ERROR( res );

        // set LUT sample on device
        res = ctrl_protocol_set_lut_sample_master( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
        HANDLE_ERROR( res );
    }
}

/******************************************************************************
 * LutItf::onLutSampleValuesRedRequest
 *****************************************************************************/
void LutItf::onLutSampleValuesRedRequest()
{
    GetLutSampleValuesRed();
}

/******************************************************************************
 * LutItf::onLutSampleValuesGreenRequest
 *****************************************************************************/
void LutItf::onLutSampleValuesGreenRequest()
{
    GetLutSampleValuesGreen();
}

/******************************************************************************
 * LutItf::onLutSampleValuesBlueRequest
 *****************************************************************************/
void LutItf::onLutSampleValuesBlueRequest()
{
    GetLutSampleValuesBlue();
}

/******************************************************************************
 * LutItf::onLutSampleValuesMasterRequest
 *****************************************************************************/
void LutItf::onLutSampleValuesMasterRequest()
{
    GetLutSampleValuesMaster();
}

/******************************************************************************
 * LutItf::onLutInterpolate
 *****************************************************************************/
void LutItf::onLutInterpolate( void )
{
    // set LUT interpolate on device
    int res = ctrl_protocol_set_lut_interpolate( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onLutInterpolateRed
 *****************************************************************************/
void LutItf::onLutInterpolateRed( void )
{
    // set Red-LUT interpolate on device
    int res = ctrl_protocol_set_lut_interpolate_red( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onLutInterpolateGreen
 *****************************************************************************/
void LutItf::onLutInterpolateGreen( void )
{
    // set Green-LUT interpolate on device
    int res = ctrl_protocol_set_lut_interpolate_green( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onLutInterpolateBlue
 *****************************************************************************/
void LutItf::onLutInterpolateBlue( void )
{
    // set Blue-LUT interpolate on device
    int res = ctrl_protocol_set_lut_interpolate_blue( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onLutReset
 *****************************************************************************/
void LutItf::onLutReset( void )
{
    // set LUT reset on device
    int res = ctrl_protocol_set_lut_reset( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );

    // sync sample data 
    GetLutSampleValuesRed();
    GetLutSampleValuesGreen();
    GetLutSampleValuesBlue();
}

/******************************************************************************
 * LutItf::onLutResetRed
 *****************************************************************************/
void LutItf::onLutResetRed( void )
{
    // set Red-LUT reset on device
    int res = ctrl_protocol_set_lut_reset_red( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );

    // sync sample data 
    GetLutSampleValuesRed();
}

/******************************************************************************
 * LutItf::onLutResetGreen
 *****************************************************************************/
void LutItf::onLutResetGreen( void )
{
    // set Green-LUT reset on device
    int res = ctrl_protocol_set_lut_reset_green( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );

    // sync sample data 
    GetLutSampleValuesGreen();
}

/******************************************************************************
 * LutItf::onLutResetBlue
 *****************************************************************************/
void LutItf::onLutResetBlue( void )
{
    // set Blue-LUT reset on device
    int res = ctrl_protocol_set_lut_reset_blue( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );

    // sync sample data 
    GetLutSampleValuesBlue();
}

/******************************************************************************
 * LutItf::onLutResetMaster
 *****************************************************************************/
void LutItf::onLutResetMaster( void )
{
    // set Blue-LUT reset on device
    int res = ctrl_protocol_set_lut_reset_master( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );

    // sync sample data 
    GetLutSampleValuesMaster();
}

/******************************************************************************
 * LutItf::onLutFastGammaChange
 *****************************************************************************/
void LutItf::onLutFastGammaChange( int gamma )
{
    // set LUT fast gamma on device
    int res = ctrl_protocol_set_lut_fast_gamma( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (int16_t)gamma );
    HANDLE_ERROR( res );
}
