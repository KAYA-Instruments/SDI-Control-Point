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
 * @file    CprocItf.cpp
 *
 * @brief   Implementation of Color Processing Interface 
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <ctrl_protocol/ctrl_protocol_cproc.h>

#include "common.h"
#include "CprocItf.h"

#include <QtDebug>

/******************************************************************************
 * CprocItf::resync()
 *****************************************************************************/
void CprocItf::resync()
{
    // sync color processing
    GetColorProcessing();
}

/******************************************************************************
 * CprocItf::GetBrightness
 *****************************************************************************/
void CprocItf::GetBrightness()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(BrightnessChanged(int))) > 0 )
    {
        int16_t value;
    
        // get color processing brightnes value from device
        int res = ctrl_protocol_get_post_bright( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a BrightnessChanged signal
        emit BrightnessChanged( (int)value );
    }
}

/******************************************************************************
 * CprocItf::GetContrast
 *****************************************************************************/
void CprocItf::GetContrast()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ContrastChanged(int))) > 0 )
    {
        uint16_t value;
    
        // get color processing contrast value from device
        int res = ctrl_protocol_get_post_cont( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a ContrastChanged signal
        emit ContrastChanged( (int)value );
    }
}

/******************************************************************************
 * CprocItf::GetSaturation
 *****************************************************************************/
void CprocItf::GetSaturation()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(SaturationChanged(int))) > 0 )
    {
        uint16_t value;
    
        // get color processing saturation value from device
        int res = ctrl_protocol_get_post_sat( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a SaturationChanged signal
        emit SaturationChanged( (int)value );
    }
}

/******************************************************************************
 * CprocItf::GetHue
 *****************************************************************************/
void CprocItf::GetHue()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(HueChanged(int))) > 0 )
    {
        int16_t value;
    
        // get color processing hue value from device
        int res = ctrl_protocol_get_post_hue( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a HueChanged signal
        emit HueChanged( (int)value );
    }
}

/******************************************************************************
 * CprocItf::GetColorProcessing
 *****************************************************************************/
void CprocItf::GetColorProcessing()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ColorProcessingChanged(int,int,int,int))) > 0 )
    {
        int16_t  brightness;
        uint16_t contrast;
        uint16_t saturation;
        int16_t  hue;
        
        // get color processing brightnes value from device
        int res = ctrl_protocol_get_post_bright( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), &brightness );
        HANDLE_ERROR( res );
        
        // get color processing contrast value from device
        res = ctrl_protocol_get_post_cont( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), &contrast );
        HANDLE_ERROR( res );
        
        // get color processing saturation value from device
        res = ctrl_protocol_get_post_sat( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), &saturation );
        HANDLE_ERROR( res );
        
        // get color processing hue value from device
        res = ctrl_protocol_get_post_hue( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), &hue );
        HANDLE_ERROR( res );
    
        // emit a ColorProcessingChanged signal
        emit ColorProcessingChanged( (int)brightness, (int)contrast, (int)saturation, (int)hue );
    }
    else
    {
        // try single stepwise
        GetBrightness();
        GetContrast();
        GetSaturation();
        GetHue();
    }
}

/******************************************************************************
 * CprocItf::onBrightnessChange
 *****************************************************************************/
void CprocItf::onBrightnessChange( int value )
{
    // set color processing brightnes value from device
    int res = ctrl_protocol_set_post_bright( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (int16_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * CprocItf::onContrastChange
 *****************************************************************************/
void CprocItf::onContrastChange( int value )
{
    // set color processing saturation value from device
    int res = ctrl_protocol_set_post_cont( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (uint16_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * CprocItf::onSaturationChange
 *****************************************************************************/
void CprocItf::onSaturationChange( int value )
{
    // set color processing saturation value from device
    int res = ctrl_protocol_set_post_sat( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (uint16_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * CprocItf::onHueChange
 *****************************************************************************/
void CprocItf::onHueChange( int value )
{
    // set color processing hue value from device
    int res = ctrl_protocol_set_post_hue( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (int16_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * CprocItf::onColorProcessingChange
 *****************************************************************************/
void CprocItf::onColorProcessingChange( int brightness, int contrast, int saturation, int hue )
{
    // set color processing brightnes value from device
    int res = ctrl_protocol_set_post_bright( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (int16_t)brightness );
    HANDLE_ERROR( res );

    // set color processing saturation value from device
    res = ctrl_protocol_set_post_cont( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (uint16_t)contrast );
    HANDLE_ERROR( res );

    // set color processing saturation value from device
    res = ctrl_protocol_set_post_sat( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (uint16_t)saturation );
    HANDLE_ERROR( res );

    // set color processing hue value from device
    res = ctrl_protocol_set_post_hue( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (int16_t)hue );
    HANDLE_ERROR( res );
}

