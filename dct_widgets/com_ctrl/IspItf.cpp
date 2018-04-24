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
 * @file    IspItf.cpp
 *
 * @brief   Implementation of ISP Interface 
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <ctrl_protocol/ctrl_protocol_isp.h>

#include "common.h"
#include "IspItf.h"

#include <QtDebug>

/******************************************************************************
 * IspItf::resync()
 *****************************************************************************/
void IspItf::resync()
{
    // lens shading correction
    GetLsc();

    // bayer pattern
    GetBayerPattern();

    // sync gains
    GetGain();

    // sync sensor black level
    GetBlackLevel();

    // sync flare level
    GetFlareLevel();

    // sync master black level
    GetMasterBlackLevel();
    
    // sync filter stage
    GetFilter();
    
    // sync color correction
    GetColorCorrection();

    // sync yuv conversion
    GetColorConversionMatrix();

    // split-screen
    GetSplitScreen();
}

/******************************************************************************
 * IspItf::GetLsc
 *****************************************************************************/
void IspItf::GetLsc()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LscChanged(QVector<uint>))) > 0 )
    {
        uint32_t values[NO_VALUES_LSC];

        // read red gain from device
        int res = ctrl_protocol_get_lsc( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_LSC, values );
        HANDLE_ERROR( res );

        QVector<uint> v_values( NO_VALUES_LSC );
        for ( int i = 0; i < NO_VALUES_LSC; i++ )
        {
            v_values[i] = values[i];
        }

        // emit a LensShadingCorrectionChanged signal
        emit LscChanged( v_values );
    }
}

/******************************************************************************
 * IspItf::GetBayerPattern
 *****************************************************************************/
void IspItf::GetBayerPattern()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(BayerPatternChanged(int))) > 0 )
    {
        uint8_t value;

        // read red gain from device
        int res = ctrl_protocol_get_bayer( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a BayerPatternChanged signal
        emit BayerPatternChanged( (int)value );
    }
}

/******************************************************************************
 * IspItf::GetRedGain
 *****************************************************************************/
void IspItf::GetRedGain()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(RedGainChanged(int))) > 0 )
    {
        uint16_t value;

        // read red gain from device
        int res = ctrl_protocol_get_gain_red( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a RedGainChanged signal
        emit RedGainChanged( (int)value );
    }
}

/******************************************************************************
 * IspItf::GetGreenGain
 *****************************************************************************/
void IspItf::GetGreenGain()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(GreenGainChanged(int))) > 0 )
    {
        uint16_t value;

        // read green gain from device
        int res = ctrl_protocol_get_gain_green( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a GreenGainChanged signal
        emit GreenGainChanged( (int)value );
    }
}

/******************************************************************************
 * IspItf::GetBlueGain
 *****************************************************************************/
void IspItf::GetBlueGain()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(BlueGainChanged(int))) > 0 )
    {
        uint16_t value;

        // read blue gain from device
        int res = ctrl_protocol_get_gain_blue( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a BlueGainChanged signal
        emit BlueGainChanged( (int)value );
    }
}

/******************************************************************************
 * IspItf::GetGain
 *****************************************************************************/
void IspItf::GetGain()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(GainChanged(int,int,int))) > 0 )
    {
        uint16_t red, green, blue;
        
        // read red gain from device
        int res = ctrl_protocol_get_gain_red( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &red );
        HANDLE_ERROR( res );
    
        // read green gain from device
        res = ctrl_protocol_get_gain_green( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &green );
        HANDLE_ERROR( res );

        // read blue gain from device
        res = ctrl_protocol_get_gain_blue( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &blue );
        HANDLE_ERROR( res );
        
        // emit a BlueGainChanged signal
        emit GainChanged( (int)red, (int)green, (int)blue );
    }
    else
    {
        GetRedGain();
        GetGreenGain();
        GetBlueGain();
    }
}

/******************************************************************************
 * IspItf::GetRedBlackLevel
 *****************************************************************************/
void IspItf::GetRedBlackLevel()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(RedBlackLevelChanged(int))) > 0 )
    {
        int16_t value;

        // read red sensor black level from device
        int res = ctrl_protocol_get_black_red( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a RedBlackLevelChanged signal
        emit RedBlackLevelChanged( (int)value );
    }
}

/******************************************************************************
 * IspItf::GetGreenBlackLevel
 *****************************************************************************/
void IspItf::GetGreenBlackLevel()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(GreenBlackLevelChanged(int))) > 0 )
    {
        int16_t value;

        // read green sensor black level from device
        int res = ctrl_protocol_get_black_green( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a GreenBlackLevelChanged signal
        emit GreenBlackLevelChanged( (int)value );
    }
}

/******************************************************************************
 * IspItf::GetBlueBlackLevel
 *****************************************************************************/
void IspItf::GetBlueBlackLevel()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(BlueBlackLevelChanged(int))) > 0 )
    {
        int16_t value;

        // read blue sensor black level from device
        int res = ctrl_protocol_get_black_blue( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a BlueBlackLevelChanged signal
        emit BlueBlackLevelChanged( (int)value );
    }
}

/******************************************************************************
 * IspItf::GetBlackLevel
 *****************************************************************************/
void IspItf::GetBlackLevel()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(BlackLevelChanged(int,int,int))) > 0 )
    {
        int16_t red, green, blue;

        // read red sensor black level from device
        int res = ctrl_protocol_get_black_red( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &red );
        HANDLE_ERROR( res );

        // read green sensor black level from device
        res = ctrl_protocol_get_black_green( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &green );
        HANDLE_ERROR( res );
    
        // read blue sensor black level from device
        res = ctrl_protocol_get_black_blue( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &blue );
        HANDLE_ERROR( res );

        // emit a BlueGainChanged signal
        emit BlackLevelChanged( (int)red, (int)green, (int)blue );
    }
    else
    {
        // try single stepwise
        GetRedBlackLevel();
        GetGreenBlackLevel();
        GetBlueBlackLevel();
    }
}

/******************************************************************************
 * IspItf::GetFlareLevel
 *****************************************************************************/
void IspItf::GetFlareLevel()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(FlareLevelChanged(int,int,int))) > 0 )
    {
        uint16_t v[NO_VALUES_ISP_FLARE];
    
        // get flare level from device
        int res = ctrl_protocol_get_flare( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_ISP_FLARE, v );
        HANDLE_ERROR( res );

        // emit a BlueGainChanged signal
        emit FlareLevelChanged( (int)v[0], (int)v[1], (int)v[2] );
    }
}

/******************************************************************************
 * IspItf::GetMasterBlackLevel
 *****************************************************************************/
void IspItf::GetMasterBlackLevel()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(MasterBlackLevelChanged(int,int,int))) > 0 )
    {
        int16_t v[NO_VALUES_BLACK_MASTER];
    
        // get master black level from device
        int res = ctrl_protocol_get_black_master( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_BLACK_MASTER, v );
        HANDLE_ERROR( res );

        // emit a BlueGainChanged signal
        emit MasterBlackLevelChanged( (int)v[0], (int)v[1], (int)v[2] );
    }
}

/******************************************************************************
 * IspItf::GetFilterEnable
 *****************************************************************************/
void IspItf::GetFilterEnable()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(FilterEnableChanged(int))) > 0 )
    {
        uint8_t enable;
    
        // get filter enable status
        int res = ctrl_protocol_get_filter_enable( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &enable );
        HANDLE_ERROR( res );
        
        // emit a BlueGainChanged signal
        emit FilterEnableChanged( (int)enable );
    }
}

/******************************************************************************
 * IspItf::GetFilterDetailLevel
 *****************************************************************************/
void IspItf::GetFilterDetailLevel()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(FilterDetailLevelChanged(int))) > 0 )
    {
        uint8_t detail;
    
        // get filter enable status
        int res = ctrl_protocol_get_filter_detail( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &detail );
        HANDLE_ERROR( res );
        
        // emit a BlueGainChanged signal
        emit FilterDetailLevelChanged( (int)detail );
    }
}

/******************************************************************************
 * IspItf::GetFilterDenoiseLevel
 *****************************************************************************/
void IspItf::GetFilterDenoiseLevel()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(FilterDenoiseLevelChanged(int))) > 0 )
    {
        uint8_t denoise;
    
        // get filter enable status
        int res = ctrl_protocol_get_filter_denoise( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &denoise );
        HANDLE_ERROR( res );
        
        // emit a BlueGainChanged signal
        emit FilterDenoiseLevelChanged( (int)denoise );
    }
}

/******************************************************************************
 * IspItf::GetFilter
 *****************************************************************************/
void IspItf::GetFilter()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(FilterChanged(int,int,int))) > 0 )
    {
        uint8_t enable, detail, denoise;
    
        // get filter enable status
        int res = ctrl_protocol_get_filter_enable( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &enable );
        HANDLE_ERROR( res );
        
        // get filter detail level
        res = ctrl_protocol_get_filter_detail( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &detail );
        HANDLE_ERROR( res );
        
        // get filter denoise level
        res = ctrl_protocol_get_filter_denoise( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &denoise );
        HANDLE_ERROR( res );

        // emit a BlueGainChanged signal
        emit FilterChanged( (int)enable, (int)detail, (int)denoise );
    }
    else
    {
        // try single stepwise
        GetFilterEnable();
        GetFilterDetailLevel();
        GetFilterDenoiseLevel();
    }
}

/******************************************************************************
 * IspItf::GetColorCorrectionMatrix
 *****************************************************************************/
void IspItf::GetColorCorrectionMatrix()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ColorCorrectionMatrixChanged(int,int,int,int,int,int,int,int,int))) > 0 )
    {
        // matrix coefficients
        int32_t c[NO_VALUES_CROSS];
    
        // get color correction matrix
        int res = ctrl_protocol_get_color_cross( 
                        GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this),
                        NO_VALUES_CROSS, c );
        HANDLE_ERROR( res );
        
        // emit a ColorCorrectionMatrixChanged signal
        emit ColorCorrectionMatrixChanged( (int)c[0], (int)c[1], (int)c[2],
                                           (int)c[3], (int)c[4], (int)c[5],
                                           (int)c[6], (int)c[7], (int)c[8] );
    }
}

/******************************************************************************
 * IspItf::GetColorCorrectionOffset
 *****************************************************************************/
void IspItf::GetColorCorrectionOffset()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ColorCorrectionOffsetChanged(int,int,int))) > 0 )
    {
        // offsets
        int16_t o[NO_VALUES_CROSS_OFFSET];
    
        // get color correction offsets
        int res = ctrl_protocol_get_color_cross_offset(
                        GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this),
                        NO_VALUES_CROSS_OFFSET, o );
        HANDLE_ERROR( res );
        
        // emit a ColorCorrectionOffsetChanged signal
        emit ColorCorrectionOffsetChanged( (int)o[0], (int)o[1], (int)o[2] );
    }
}

/******************************************************************************
 * IspItf::GetColorCorrection
 *****************************************************************************/
void IspItf::GetColorCorrection()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ColorCorrectionChanged(int,int,int,int,int,int,int,int,int,int,int,int))) > 0 )
    {
        // matrix coefficients
        int32_t c[NO_VALUES_CROSS];

        // offsets
        int16_t o[NO_VALUES_CROSS_OFFSET];
    
        // get color correction matrix
        int res = ctrl_protocol_get_color_cross( 
                        GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this),
                        NO_VALUES_CROSS, c );
        HANDLE_ERROR( res );
        
        // get color correction offsets
        res = ctrl_protocol_get_color_cross_offset(
                        GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this),
                        NO_VALUES_CROSS_OFFSET, o );
        HANDLE_ERROR( res );

        // emit a ColorCorrectionChanged signal
        emit ColorCorrectionChanged( (int)c[0], (int)c[1], (int)c[2],
                                     (int)c[3], (int)c[4], (int)c[5],
                                     (int)c[6], (int)c[7], (int)c[8],
                                     (int)o[0], (int)o[1], (int)o[2] );
    }
    else
    {
        // try single stepwise
        GetColorCorrectionMatrix();
        GetColorCorrectionOffset();
    }
}

/******************************************************************************
 * IspItf::GetColorConversionMatrix
 *****************************************************************************/
void IspItf::GetColorConversionMatrix()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ColorConversionMatrixChanged(int,int,int,int,int,int,int,int,int))) > 0 )
    {
        // matrix coefficients
        int16_t c[NO_VALUES_COLOR_CONVERSION]; 
    
        // get color correction matrix
        int res = ctrl_protocol_get_color_conv( 
                        GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this),
                        NO_VALUES_COLOR_CONVERSION, c );
        HANDLE_ERROR( res );
        
        // emit a ColorConversionMatrixChanged signal
        emit ColorConversionMatrixChanged( (int)c[0], (int)c[1], (int)c[2],
                                           (int)c[3], (int)c[4], (int)c[5],
                                           (int)c[6], (int)c[7], (int)c[8] );
    }
}

/******************************************************************************
 * IspItf::GetSplitScreen
 *****************************************************************************/
void IspItf::GetSplitScreen()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(SplitScreenChanged(bool))) > 0 )
    {
        uint8_t value;

        // read split-screen mode from device
        int res = ctrl_protocol_get_split_screen( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a BayerPatternChanged signal
        emit SplitScreenChanged( (bool)value );
    }
}

/******************************************************************************
 * IspItf::onLscChange
 *****************************************************************************/
void IspItf::onLscChange( QVector<uint> value )
{
    uint value_array[NO_VALUES_LSC];
    for ( int i = 0; i < NO_VALUES_LSC; i++ )
    {
        value_array[i] = value[i];
    }

    int res = ctrl_protocol_set_lsc( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_LSC, value_array );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onBayerPatternChange
 *****************************************************************************/
void IspItf::onBayerPatternChange( int value )
{
    // set bayer-pattern on device
    int res = ctrl_protocol_set_bayer( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onRedGainChange
 *****************************************************************************/
void IspItf::onRedGainChange( int value )
{
    // set red gain on device
    int res = ctrl_protocol_set_gain_red( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint16_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onGreenGainChange
 *****************************************************************************/
void IspItf::onGreenGainChange( int value )
{
    // set green gain on device
    int res = ctrl_protocol_set_gain_green( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint16_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onBlueGainChange
 *****************************************************************************/
void IspItf::onBlueGainChange( int value )
{
    // set blue gain on device
    int res = ctrl_protocol_set_gain_blue( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint16_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onGainChange
 *****************************************************************************/
void IspItf::onGainChange( int red, int green, int blue )
{
    // set red gain on device
    int res = ctrl_protocol_set_gain_red( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint16_t)red );
    HANDLE_ERROR( res );
    
    // set green gain on device
    res = ctrl_protocol_set_gain_green( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint16_t)green );
    HANDLE_ERROR( res );

    // set blue gain on device
    res = ctrl_protocol_set_gain_blue( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint16_t)blue );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onRedBlackLevelChange
 *****************************************************************************/
void IspItf::onRedBlackLevelChange( int value )
{
    // set red sensor black level on device
    int res = ctrl_protocol_set_black_red( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (int16_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onGreenBlackLevelChange
 *****************************************************************************/
void IspItf::onGreenBlackLevelChange( int value )
{
    // set green sensor black level on device
    int res = ctrl_protocol_set_black_green( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (int16_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onBlueBlackLevelChange
 *****************************************************************************/
void IspItf::onBlueBlackLevelChange( int value )
{
    // set blue sensor black level on device
    int res = ctrl_protocol_set_black_blue( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (int16_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onBlackLevelChange
 *****************************************************************************/
void IspItf::onBlackLevelChange( int red, int green, int blue )
{
    // set red sensor black level on device
    int res = ctrl_protocol_set_black_red( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (int16_t)red );
    HANDLE_ERROR( res );

    // set green sensor black level on device
    res = ctrl_protocol_set_black_green( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (int16_t)green );
    HANDLE_ERROR( res );

    // set blue sensor black level on device
    res = ctrl_protocol_set_black_blue( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (int16_t)blue );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onFlareLevelChange
 *****************************************************************************/
void IspItf::onFlareLevelChange( int red, int green, int blue )
{
    uint16_t v[NO_VALUES_ISP_FLARE] = { (uint16_t)red, (uint16_t)green, (uint16_t)blue };

    // set flare level on device
    int res = ctrl_protocol_set_flare( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), NO_VALUES_ISP_FLARE, v );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onMasterBlackLevelChange
 *****************************************************************************/
void IspItf::onMasterBlackLevelChange( int red, int green, int blue )
{
    int16_t v[NO_VALUES_BLACK_MASTER] = { (int16_t)red, (int16_t)green, (int16_t)blue };

    // set master black level on device
    int res = ctrl_protocol_set_black_master( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), NO_VALUES_BLACK_MASTER, v );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onFilterEnableChange
 *****************************************************************************/
void IspItf::onFilterEnableChange( int value )
{
    // set filter enable status on device
    int res = ctrl_protocol_set_filter_enable( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onFilterDetailLevelChange
 *****************************************************************************/
void IspItf::onFilterDetailLevelChange( int value )
{
    // set filter detail level on device
    int res = ctrl_protocol_set_filter_detail( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onFilterDenoiseLevelChange
 *****************************************************************************/
void IspItf::onFilterDenoiseLevelChange( int value )
{
    // set filter de-noise level on device
    int res = ctrl_protocol_set_filter_denoise( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onFilterChange
 *****************************************************************************/
void IspItf::onFilterChange( int enable, int detail, int denoise )
{
    // set filter enable status on device
    int res = ctrl_protocol_set_filter_enable( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint8_t)enable );
    HANDLE_ERROR( res );

    // set filter detail level on device
    res = ctrl_protocol_set_filter_detail( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint8_t)detail );
    HANDLE_ERROR( res );

    // set filter de-noise level on device
    res = ctrl_protocol_set_filter_denoise( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint8_t)denoise );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onColorCorrectionMatrixChange
 *****************************************************************************/
void IspItf::onColorCorrectionMatrixChange( int c0, int c1, int c2,          
                                            int c3, int c4, int c5,
                                            int c6, int c7, int c8 )
{
    // matrix coefficients
    int32_t c[NO_VALUES_CROSS] =
    {
        (int32_t)c0, (int32_t)c1, (int32_t)c2,
        (int32_t)c3, (int32_t)c4, (int32_t)c5,
        (int32_t)c6, (int32_t)c7, (int32_t)c8
    };

    // set color correction matrix
    int res = ctrl_protocol_set_color_cross( 
                    GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this),
                    NO_VALUES_CROSS_OFFSET, c );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onColorCorrectionOffsetChange
 *****************************************************************************/
void IspItf::onColorCorrectionOffsetChange( int red, int green, int blue )
{
    int16_t o[NO_VALUES_CROSS_OFFSET] = { (int16_t)red, (int16_t)green, (int16_t)blue };

    // set color correction offsets
    int res = ctrl_protocol_set_color_cross_offset(
                    GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this),
                    NO_VALUES_CROSS_OFFSET, o );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onColorCorrectionChange
 *****************************************************************************/
void IspItf::onColorCorrectionChange( int c0, int c1, int c2,          
                                      int c3, int c4, int c5,
                                      int c6, int c7, int c8,
                                      int red, int green, int blue )
{
    // matrix coefficients
    int32_t c[NO_VALUES_CROSS] =
    {
        (int32_t)c0, (int32_t)c1, (int32_t)c2,
        (int32_t)c3, (int32_t)c4, (int32_t)c5,
        (int32_t)c6, (int32_t)c7, (int32_t)c8
    };

    // offsets
    int16_t o[NO_VALUES_CROSS_OFFSET] = { (int16_t)red, (int16_t)green, (int16_t)blue };

    // set color correction matrix
    int res = ctrl_protocol_set_color_cross( 
                    GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this),
                    NO_VALUES_CROSS, c );
    HANDLE_ERROR( res );

    // set color correction offsets
    res = ctrl_protocol_set_color_cross_offset(
                    GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this),
                    NO_VALUES_CROSS_OFFSET, o );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onColorConversionMatrixChange
 *****************************************************************************/
void IspItf::onColorConversionMatrixChange( int c0, int c1, int c2,          
                                            int c3, int c4, int c5,
                                            int c6, int c7, int c8 )
{
    int16_t c[NO_VALUES_COLOR_CONVERSION] =
    {
        (int16_t)c0, (int16_t)c1, (int16_t)c2,
        (int16_t)c3, (int16_t)c4, (int16_t)c5,
        (int16_t)c6, (int16_t)c7, (int16_t)c8
    };

    // set color correction matrix
    int res = ctrl_protocol_set_color_conv( 
                    GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this),
                    NO_VALUES_COLOR_CONVERSION, c );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onSplitScreenChange
 *****************************************************************************/
void IspItf::onSplitScreenChange(bool value )
{
    // set split-screen mode
    int res = ctrl_protocol_set_split_screen( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * IspItf::onNotifyWhiteBalanceUpdate
 *****************************************************************************/
void IspItf::onNotifyWhiteBalanceUpdate()
{
    GetGain();
    GetRedGain();
    GetGreenGain();
    GetBlueGain();

    GetColorCorrection();
    GetColorCorrectionMatrix();
    GetColorCorrectionOffset();
}

/******************************************************************************
 * IspItf::onNotifyCameraGainChange
 *****************************************************************************/
void IspItf::onNotifyCameraGainChange()
{
    // Nothing to do here
//    GetBlackLevel();  // Why was that here? The black levels do not change on gain change, so no need to fetch them
}

