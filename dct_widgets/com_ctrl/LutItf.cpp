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

#include <defines.h>

#include "common.h"
#include "LutItf.h"

#include <QtDebug>

/******************************************************************************
 * LutItf::resync()
 *****************************************************************************/
void LutItf::resync()
{
    // enable status for each chain
    /* Note: On some devices not all chains will be available, in this case GetLutEnable
     * will report an error, but that is no problem */
    for ( int i = 0; i < MAX_NUM_CHAINS; i++ )
    {
        GetLutEnable( i );
    }

    // operational mode
    GetLutMode();
    GetLutFixedMode();
    //GetLogMode();
    //GetPQMaxBrightness();

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
        ctrl_protocol_enable_t v;
        v.id = static_cast<uint8_t>(id);
        v.flag = 0u;
    
        // read enable state from device
        int res = ctrl_protocol_get_lut_enable( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), reinterpret_cast<uint8_t *>(&v) );
        HANDLE_ERROR( res );
        
        // emit a LutEnableChanged signal
        emit LutEnableChanged( id, static_cast<int>(v.flag) );
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

        // emit a LutModeChanged signal
        emit LutModeChanged( static_cast<int>(mode) );
    }
}

/******************************************************************************
 * LutItf::GetLutFixedMode
 *****************************************************************************/
void LutItf::GetLutFixedMode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LutFixedModeChanged(int))) > 0 )
    {
        uint8_t mode = 0u;

        // read mode from device
        int res = ctrl_protocol_get_lut_fixed_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &mode );
        HANDLE_ERROR( res );

        // emit a LutFixedModeChanged signal
        emit LutFixedModeChanged( static_cast<int>(mode) );
    }
}

/******************************************************************************
 * LutItf::GetLogMode
 *****************************************************************************/
void LutItf::GetLogMode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LogModeChanged(int))) > 0 )
    {
        uint8_t mode = 0u;

        // read mode from device
        int res = ctrl_protocol_get_log_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &mode );
        HANDLE_ERROR( res );

        // emit a LogModeChanged signal
        emit LogModeChanged( static_cast<int>(mode) );
    }
}

/******************************************************************************
 * LutItf::GetPQMaxBrightness
 *****************************************************************************/
void LutItf::GetPQMaxBrightness()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(PQMaxBrightnessChanged(int))) > 0 )
    {
        uint8_t percent = 0u;

        // read mode from device
        int res = ctrl_protocol_get_pq_max_brightness( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &percent );
        HANDLE_ERROR( res );

        // emit a LogModeChanged signal
        emit PQMaxBrightnessChanged( static_cast<int>(percent) );
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
        emit LutPresetChanged( static_cast<int>(value) );
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
        for( int i = 0u; i < static_cast<int>(MAX_VALUES_LUT); i++ )
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
        for( int i = 0u; i < static_cast<int>(MAX_VALUES_LUT); i++ )
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
        for( int i = 0u; i < static_cast<int>(MAX_VALUES_LUT); i++ )
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
            GET_CHANNEL_INSTANCE(this), sizeof(v), reinterpret_cast<uint8_t *>(&v) );
        HANDLE_ERROR( res );
        
        QVector<int> x( static_cast<int>(v.no) );
        QVector<int> y( static_cast<int>(v.no) );
        for( int i = 0u; i < static_cast<int>(v.no); i++ )
        {
            x[i] = static_cast<unsigned short>(v.x_i[i]);
            y[i] = static_cast<unsigned short>(v.y_i[i]);
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
            GET_CHANNEL_INSTANCE(this), sizeof(v), reinterpret_cast<uint8_t *>(&v) );
        HANDLE_ERROR( res );
        
        QVector<int> x( static_cast<int>(v.no) );
        QVector<int> y( static_cast<int>(v.no) );
        for( int i = 0u; i < static_cast<int>(v.no); i++ )
        {
            x[i] = static_cast<unsigned short>(v.x_i[i]);
            y[i] = static_cast<unsigned short>(v.y_i[i]);
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
            GET_CHANNEL_INSTANCE(this), sizeof(v), reinterpret_cast<uint8_t *>(&v) );
        HANDLE_ERROR( res );
        
        QVector<int> x( static_cast<int>(v.no) );
        QVector<int> y( static_cast<int>(v.no) );
        for( int i = 0u; i < static_cast<int>(v.no); i++ )
        {
            x[i] = static_cast<unsigned short>(v.x_i[i]);
            y[i] = static_cast<unsigned short>(v.y_i[i]);
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
            GET_CHANNEL_INSTANCE(this), sizeof(v), reinterpret_cast<uint8_t *>(&v) );
        HANDLE_ERROR( res );
        
        QVector<int> x( static_cast<int>(v.no) );
        QVector<int> y( static_cast<int>(v.no) );
        for( int i = 0u; i < static_cast<int>(v.no); i++ )
        {
            x[i] = static_cast<unsigned short>(v.x_i[i]);
            y[i] = static_cast<unsigned short>(v.y_i[i]);
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
        emit LutFastGammaChanged( static_cast<int>(gamma) );
    }
}

/******************************************************************************
 * LutItf::onLutEnableChange
 *****************************************************************************/
void LutItf::onLutEnableChange( int id, int value )
{
    ctrl_protocol_enable_t v;
    v.id = static_cast<uint8_t>(id);
    v.flag = static_cast<uint8_t>(value);

    // set LUT enable state on device
    int res = ctrl_protocol_set_lut_enable( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), sizeof(v), reinterpret_cast<uint8_t *>(&v) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onLutModeChange
 *****************************************************************************/
void LutItf::onLutModeChange( int mode )
{
    // set LUT operational mode on device
    int res = ctrl_protocol_set_lut_mode( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), static_cast<uint8_t>(mode) );
    HANDLE_ERROR( res );

    // If mode was changed to interpolation mode, sync sample data
    if ( mode == 0 )
    {
        GetLutSampleValuesMaster();
        GetLutSampleValuesRed();
        GetLutSampleValuesGreen();
        GetLutSampleValuesBlue();
    }

    // Else if mode was changed to fast gamma, get current gamma setting
    else if ( mode == 1 )
    {
        GetLutFastGamma();
    }

    // Else if mode was changed to fixed gamma curve, get fixed gamma mode
    else if ( mode == 2 )
    {
        GetLutFixedMode();
    }
}

/******************************************************************************
 * LutItf::onLutFixedModeChange
 *****************************************************************************/
void LutItf::onLutFixedModeChange( int mode )
{
    // set LUT fixed gamma curve on device
    int res = ctrl_protocol_set_lut_fixed_mode( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), static_cast<uint8_t>(mode) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onLogModeChange
 *****************************************************************************/
void LutItf::onLogModeChange( int mode )
{
    // set LOG mode on device
    int res = ctrl_protocol_set_log_mode( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), static_cast<uint8_t>(mode) );

    /* In case swtich was successful, notify LOG mode was changed to get
     * new gain range and current gain value in CamItf, then update
     * current lut mode and lut fixed mode. */
    if ( res == 0 )
    {
        // Notify other interfaces that LOG mode got changed
        emit NotifyLogModeChanged();

        // Update LUT settings which might got changed by LOG mode command
        GetLutMode();
        GetLutFixedMode();
        GetLutFastGamma();
        GetLutSampleValuesMaster();
        GetLutSampleValuesRed();
        GetLutSampleValuesGreen();
        GetLutSampleValuesBlue();
    }

    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onPQMaxBrightnessChange
 *****************************************************************************/
void LutItf::onPQMaxBrightnessChange( int percent )
{
    // set maximum PQ brigthness on device
    int res = ctrl_protocol_set_pq_max_brightness( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), static_cast<uint8_t>(percent) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onLutPresetChange
 *****************************************************************************/
void LutItf::onLutPresetChange( int value )
{
    // set LUT preset storage on device
    int res = ctrl_protocol_set_lut_preset( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), static_cast<uint8_t>(value) );
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
        GET_CHANNEL_INSTANCE(this), static_cast<uint16_t>(value) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onLutWriteIndexRedChange
 *****************************************************************************/
void LutItf::onLutWriteIndexRedChange( int value )
{
    // set LUT read/write address for red tables on device
    int res = ctrl_protocol_set_lut_write_index_red( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), static_cast<uint16_t>(value) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onLutWriteIndexGreenChange
 *****************************************************************************/
void LutItf::onLutWriteIndexGreenChange( int value )
{
    // set LUT read/write address for green tables on device
    int res = ctrl_protocol_set_lut_write_index_red( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), static_cast<uint16_t>(value) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * LutItf::onLutWriteIndexBlueChange
 *****************************************************************************/
void LutItf::onLutWriteIndexBlueChange( int value )
{
    // set LUT read/write address for blue tables on device
    int res = ctrl_protocol_set_lut_write_index_blue( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), static_cast<uint16_t>(value) );
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
    ctrl_protocol_rec709_t values;
    values.threshold   = static_cast<uint16_t>(threshold);
    values.lcontrast   = static_cast<uint16_t>(lcontrast);
    values.lbrightness = static_cast<int16_t>(lbrightness);
    values.contrast    = static_cast<uint16_t>(contrast);
    values.gamma       = static_cast<uint16_t>(gamma);
    values.brightness  = static_cast<int16_t>(brightness);

    // set mcc enable state on device
    int res = ctrl_protocol_set_lut_rec709( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), sizeof(ctrl_protocol_rec709_t), reinterpret_cast<uint8_t *>(&values) );
    HANDLE_ERROR( res );

    // sync sample data 
    //GetLutSampleValuesRed();
    //GetLutSampleValuesGreen();
    //GetLutSampleValuesBlue();
    //GetLutSampleValuesMaster();
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
        v.no = static_cast<uint32_t>(x.count());
        
        for( int i = 0u; i < static_cast<int>(v.no); i++ )
        {
            v.x_i[i] = static_cast<unsigned short>(x[i]);
            v.y_i[i] = static_cast<unsigned short>(y[i]);
        }

        // set LUT reset on device (to delete sample points)
        int res = ctrl_protocol_set_lut_reset( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this) );
        HANDLE_ERROR( res );

        // set LUT sample on device
        res = ctrl_protocol_set_lut_sample( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), reinterpret_cast<uint8_t *>(&v) );
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
        v.no = static_cast<uint32_t>(x.count());
        
        for( int i = 0u; i < static_cast<int>(v.no); i++ )
        {
            v.x_i[i] = static_cast<unsigned short>(x[i]);
            v.y_i[i] = static_cast<unsigned short>(y[i]);
        }

        // set LUT reset on device (to delete sample points)
        int res = ctrl_protocol_set_lut_reset_red( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this) );
        HANDLE_ERROR( res );

        // set LUT sample on device
        res = ctrl_protocol_set_lut_sample_red( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), reinterpret_cast<uint8_t *>(&v) );
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
        v.no = static_cast<uint32_t>(x.count());
        
        for( int i = 0u; i < static_cast<int>(v.no); i++ )
        {
            v.x_i[i] = static_cast<unsigned short>(x[i]);
            v.y_i[i] = static_cast<unsigned short>(y[i]);
        }

        // set LUT reset on device (to delete sample points)
        int res = ctrl_protocol_set_lut_reset_green( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this) );
        HANDLE_ERROR( res );

        // set LUT sample on device
        res = ctrl_protocol_set_lut_sample_green( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), reinterpret_cast<uint8_t *>(&v) );
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
        v.no = static_cast<uint32_t>(x.count());
        
        for( int i = 0u; i < static_cast<int>(v.no); i++ )
        {
            v.x_i[i] = static_cast<unsigned short>(x[i]);
            v.y_i[i] = static_cast<unsigned short>(y[i]);
        }

        // set LUT reset on device (to delete sample points)
        int res = ctrl_protocol_set_lut_reset_blue( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this) );
        HANDLE_ERROR( res );

        // set LUT sample on device
        res = ctrl_protocol_set_lut_sample_blue( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), reinterpret_cast<uint8_t *>(&v) );
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
        v.no = static_cast<uint32_t>(x.count());
        
        for( int i = 0u; i < static_cast<int>(v.no); i++ )
        {
            v.x_i[i] = static_cast<unsigned short>(x[i]);
            v.y_i[i] = static_cast<unsigned short>(y[i]);
        }

        // set LUT reset on device (to delete sample points)
        int res = ctrl_protocol_set_lut_reset_master( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this) );
        HANDLE_ERROR( res );

        // set LUT sample on device
        res = ctrl_protocol_set_lut_sample_master( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), reinterpret_cast<uint8_t *>(&v) );
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
        GET_CHANNEL_INSTANCE(this), static_cast<int16_t>(gamma) );
    HANDLE_ERROR( res );
}
