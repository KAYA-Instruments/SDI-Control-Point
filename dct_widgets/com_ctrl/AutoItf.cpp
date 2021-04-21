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
 * @file    AutoItf.cpp
 *
 * @brief   Implementation of Auto Processing Interface 
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <ctrl_protocol/ctrl_protocol_auto.h>

#include "common.h"
#include "AutoItf.h"

#include <QtDebug>

/******************************************************************************
 * AutoItf::resync()
 *****************************************************************************/
void AutoItf::resync()
{
    // sync auto processing configuration
    GetNoWbPresets();
    GetWbPresets();
    GetStatHistogramEnable();
    GetStatExposureEnable();
    GetAecEnable();
    GetAecSetup();
    //GetAecWeights();
    GetAwbEnable();
    //GetAwbSpeed();
    GetAwbThreshold();

    GetStatRGB();
}

/******************************************************************************
 * AutoItf::GetNoWbPresets
 *****************************************************************************/
void AutoItf::GetNoWbPresets()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(NoWbPresetsChanged(int))) > 0 )
    {
        uint8_t value;
    
        // get auto processing number of white balance presets from device
        int res = ctrl_protocol_get_no_wbpresets( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a NoWbPresetsChanged signal
        emit NoWbPresetsChanged( (int)value );
    }
}

/******************************************************************************
 * AutoItf::GetAecEnable
 *****************************************************************************/
void AutoItf::GetAecEnable()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(AecEnableChanged(int))) > 0 )
    {
        uint8_t value;
    
        // get auto processing number of white balance presets from device
        int res = ctrl_protocol_get_aec_enable( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a AecEnableChanged signal
        emit AecEnableChanged( (int)value );
    }
}

/******************************************************************************
 * AutoItf::GetAecSetup
 *****************************************************************************/
void AutoItf::GetAecSetup()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(AecSetupChanged(QVector<int>))) > 0 )
    {
        int32_t value[NO_VALUES_AEC_SETUP];
    
        // get auto processing number of white balance presets from device
        int res = ctrl_protocol_get_aec_setup( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_AEC_SETUP, &value[0] );
        HANDLE_ERROR( res );
        
        QVector<int> v_values( NO_VALUES_AEC_SETUP );
        for ( int i = 0; i < NO_VALUES_AEC_SETUP; i++ )
        {
            v_values[i] = value[i];
        }
        
        // emit a AecSetupChanged signal
        emit AecSetupChanged( v_values );
    }
}

/******************************************************************************
 * AutoItf::GetAecWeights
 *****************************************************************************/
void AutoItf::GetAecWeights()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(AecWeightsChanged(QVector<int>))) > 0 )
    {
        uint8_t weights[NO_AEC_WEIGHTS];

        // get auto processing number of white balance presets from device
        int res = ctrl_protocol_get_aec_weights( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_AEC_WEIGHTS, &weights[0] );
        HANDLE_ERROR( res );

        QVector<int> weightsVector;
        for ( int i = 0; i < NO_AEC_WEIGHTS; i++ )
        {
            weightsVector.append( (int)weights[i] );
        }

        emit AecWeightsChanged( weightsVector );
    }
}

/******************************************************************************
 * AutoItf::GetAwbEnable
 *****************************************************************************/
void AutoItf::GetAwbEnable()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(AwbEnableChanged(int))) > 0 )
    {
        uint8_t value;
    
        // get enable state of continuous auto-white balance
        int res = ctrl_protocol_get_awb_enable( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a AwbEnableChanged signal
        emit AwbEnableChanged( (int)value );
    }
}

/******************************************************************************
 * AutoItf::GetAwbThreshold
 *****************************************************************************/
void AutoItf::GetAwbThreshold()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(AwbThresholdChanged(int))) > 0 )
    {
        uint16_t value;

        // get threshold value
        int res = ctrl_protocol_get_awb_threshold( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a AwbThresholdChanged signal
        emit AwbThresholdChanged( (int)value );
    }
}

/******************************************************************************
 * AutoItf::GetAwbSpeed
 *****************************************************************************/
void AutoItf::GetAwbSpeed()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(AwbSpeedChanged(int))) > 0 )
    {
        uint8_t value;
    
        // get speed (damping value) of continuous auto-white balance
        int res = ctrl_protocol_get_awb_speed( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a AwbSpeedChanged signal
        emit AwbSpeedChanged( (int)value );
    }
}

/******************************************************************************
 * AutoItf::GetNoWbPresets
 *****************************************************************************/
void AutoItf::GetWbPresets( int const no )
{
    // Is there a signal listener
    if ( receivers(SIGNAL(WbPresetsChanged(int,QString,int))) > 0 )
    {
        // g++ supports of Variable Length Arrays (VLA) as extension, standard C++ does not support it, so to compile with MSVC we use vector:
        //ctrl_protocol_wb_preset_t presets[no]; VLA Fix:
        std::vector<ctrl_protocol_wb_preset_t> presets(no);
        
        int i;

        // clear memory
        //memset( presets, 0, sizeof( presets ) ); VLA Fix:
        memset( presets.data(), 0, sizeof( ctrl_protocol_wb_preset_t ) * no );

        // get auto processing white-blance presets from device
        int res = ctrl_protocol_get_wbpresets( GET_PROTOCOL_INSTANCE(this),
                    //GET_CHANNEL_INSTANCE(this), sizeof(presets), (uint8_t *)presets ); VLA Fix:
                    GET_CHANNEL_INSTANCE(this), sizeof(ctrl_protocol_wb_preset_t) * no, (uint8_t *)presets.data() );
        HANDLE_ERROR( res );

        // emit WbPresetsChanged signals
        for ( i=0; (i<no) && (presets[i].id == i); i++ )
        {
            emit WbPresetsChanged( presets[i].id, QString((char *)presets[i].name), presets[i].ct );
        }
    }
}

/******************************************************************************
 * AutoItf::GetStatRGB
 *****************************************************************************/
void AutoItf::GetStatRGB()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(StatRGBChanged(int,int,int))) > 0 )
    {
        uint16_t rgb[NO_VALUES_RGB_STATISTIC];
    
        // get color processing brightnes value from device
        int res = ctrl_protocol_get_stat_rgb( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_RGB_STATISTIC, rgb );
        HANDLE_ERROR( res );

        // emit a StatRGBChanged signal
        emit StatRGBChanged( (int)rgb[0], (int)rgb[1], (int)rgb[2] );
    }
}

/******************************************************************************
 * AutoItf::GetStatXYZ
 *****************************************************************************/
void AutoItf::GetStatXYZ()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(StatXYZChanged(int,int,int))) > 0 )
    {
        int32_t c[NO_VALUES_XYZ_STATISTIC];

        // get color processing brightnes value from device
        int res = ctrl_protocol_get_stat_xyz( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_XYZ_STATISTIC, c );
        HANDLE_ERROR( res );

        // emit a StatXYZChanged signal
        emit StatXYZChanged( (int)c[0], (int)c[1], (int)c[2] );
    }
}

/******************************************************************************
 * AutoItf::GetStatHistogramEnable
 *****************************************************************************/
void AutoItf::GetStatHistogramEnable()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(StatHistogramEnableChanged(int))) > 0 )
    {
        uint8_t value;
    
        // get color processing brightnes value from device
        int res = ctrl_protocol_get_stat_histogram_enable( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a NoWbPresetsChanged signal
        emit StatHistogramEnableChanged( (int)value );
    }
}

/******************************************************************************
 * AutoItf::GetStatHistogram
 *****************************************************************************/
void AutoItf::GetStatHistogram()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(StatHistogramChanged(QVector<int>))) > 0 )
    {
        uint32_t v[NO_VALUES_HISTOGRAM_STATISTIC];
    
        // get color processing brightnes value from device
        int res = ctrl_protocol_get_stat_histogram( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_HISTOGRAM_STATISTIC, v );
        HANDLE_ERROR( res );

        // TODO: check to remove iteration (c++11 ?)
        QVector<int> d( NO_VALUES_HISTOGRAM_STATISTIC );
        for ( int i = 0; i< NO_VALUES_HISTOGRAM_STATISTIC; i++ )
        {
            d[i] = (int)v[i];
        }

        // emit a NoWbPresetsChanged signal
        emit StatHistogramChanged( d );
    }
}

/******************************************************************************
 * AutoItf::GetStatHistogramEnable
 *****************************************************************************/
void AutoItf::GetStatExposureEnable()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(StatExposureEnableChanged(int))) > 0 )
    {
        uint8_t value;
    
        // get color processing brightnes value from device
        int res = ctrl_protocol_get_stat_exposure_enable( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a NoWbPresetsChanged signal
        emit StatExposureEnableChanged( (int)value );
    }
}

/******************************************************************************
 * AutoItf::GetStatExposure
 *****************************************************************************/
void AutoItf::GetStatExposure()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(StatExposureChanged(QVector<int>))) > 0 )
    {
        uint16_t v[NO_VALUES_EXPOSURE_STATISTIC];
    
        // get color processing brightnes value from device
        int res = ctrl_protocol_get_stat_exposure( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_EXPOSURE_STATISTIC, v );
        HANDLE_ERROR( res );

        // TODO: check to optimize iteration (c++11 ?)
        QVector<int> d( NO_VALUES_EXPOSURE_STATISTIC );
        for ( int i = 0; i < NO_VALUES_EXPOSURE_STATISTIC; i++ )
        {
            d[i] = (int)v[i];
        }

        // emit a NoWbPresetsChanged signal
        emit StatExposureChanged( d );
    }
}

/******************************************************************************
 * AutoItf::GetColorXYZ
 *****************************************************************************/
void AutoItf::GetColorXYZ()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ColorXYZChanged(int,int,int,int,int,int,int,int,int))) > 0 )
    {
        int32_t c[NO_VALUES_COLOR_XYZ];
    
        // get color processing brightnes value from device
        int res = ctrl_protocol_get_color_xyz( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_COLOR_XYZ, c );
        HANDLE_ERROR( res );

        // emit a ColorXYZChanged signal
        emit ColorXYZChanged( (int)c[0], (int)c[1], (int)c[2],
                              (int)c[3], (int)c[4], (int)c[5],
                              (int)c[6], (int)c[7], (int)c[8] );
    }
}

/******************************************************************************
 * AutoItf::onWbUpdate
 *****************************************************************************/
void AutoItf::onWbUpdate()
{
    // emit a NotifyWhiteBalanceChanged signal
    emit NotifyWhiteBalanceChanged();
}

/******************************************************************************
 * AutoItf::onAwbEnableChange
 *****************************************************************************/
void AutoItf::onAwbEnableChange( int enable )
{
    int res = ctrl_protocol_set_awb_enable( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), enable ? 1u : 0u );
    HANDLE_ERROR( res );

    GetAwbEnable();
    
    // emit a NotifyWhiteBalanceChanged signal
    emit NotifyWhiteBalanceChanged();
}

/******************************************************************************
 * AutoItf::onAwbThresholdChange
 *****************************************************************************/
void AutoItf::onAwbThresholdChange( int threshold )
{
    int res = ctrl_protocol_set_awb_threshold( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), (uint16_t)threshold );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * AutoItf::onAwbSpeedChange
 *****************************************************************************/
void AutoItf::onAwbSpeedChange( int speed )
{
    int res = ctrl_protocol_set_awb_speed( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), (uint8_t)speed );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * AutoItf::onWbChange
 *****************************************************************************/
void AutoItf::onWbChange()
{
    // run auto white balance
    int res = ctrl_protocol_run_wb( GET_PROTOCOL_INSTANCE(this),
                GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );
    
    // emit a NotifyWhiteBalanceChanged signal
    emit NotifyWhiteBalanceChanged();
}

/******************************************************************************
 * AutoItf::onWbPresetChange
 *****************************************************************************/
void AutoItf::onWbPresetChange( int id )
{
    // run auto white balance
    int res = ctrl_protocol_set_wbpreset( GET_PROTOCOL_INSTANCE(this),
                GET_CHANNEL_INSTANCE(this), (uint8_t)id );
    HANDLE_ERROR( res );

    // emit a NotifyWhiteBalanceChanged signal
    emit NotifyWhiteBalanceChanged();
}

/******************************************************************************
 * AutoItf::onStatHistogramEnableChange
 *****************************************************************************/
void AutoItf::onStatHistogramEnableChange( int flag )
{
    // run auto white balance
    int res = ctrl_protocol_set_stat_histogram_enable( GET_PROTOCOL_INSTANCE(this),
                GET_CHANNEL_INSTANCE(this), (uint8_t)flag );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * AutoItf::onStatExposureEnableChange
 *****************************************************************************/
void AutoItf::onStatExposureEnableChange( int flag )
{
    // run auto white balance
    int res = ctrl_protocol_set_stat_exposure_enable( GET_PROTOCOL_INSTANCE(this),
                GET_CHANNEL_INSTANCE(this), (uint8_t)flag );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * AutoItf::onWhiteBalanceUpdate
 *****************************************************************************/
void AutoItf::onNotifyWhiteBalanceUpdate()
{
    GetStatRGB();
}

/******************************************************************************
 * AutoItf::onAecEnableChange
 *****************************************************************************/
void AutoItf::onAecEnableChange( int enable )
{
    int res = ctrl_protocol_set_aec_enable( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), (uint8_t) enable );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * AutoItf::onAecSetupChange
 *****************************************************************************/
void AutoItf::onAecSetupChange( QVector<int> value )
{
    int value_array[NO_VALUES_AEC_SETUP];
    for ( int i = 0; i < NO_VALUES_AEC_SETUP; i++ )
    {
        value_array[i] = value[i];
    }

    int res = ctrl_protocol_set_aec_setup( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_AEC_SETUP, &value_array[0] );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * AutoItf::onAecWeightChange
 *****************************************************************************/
void AutoItf::onAecWeightChange( int index, int weight )
{
    uint8_t value_array[2];

    value_array[0] = (uint8_t)index;
    value_array[1] = (uint8_t)weight;

    int res = ctrl_protocol_set_aec_weight( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), 2, &value_array[0] );
    HANDLE_ERROR( res );
}
