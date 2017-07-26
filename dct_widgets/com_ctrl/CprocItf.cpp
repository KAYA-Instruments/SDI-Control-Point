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

