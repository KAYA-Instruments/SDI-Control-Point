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
 * @file    TfltItf.cpp
 *
 * @brief   Temporal Filter Interface
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <ctrl_protocol/ctrl_protocol_tflt.h>

#include "common.h"
#include "TfltItf.h"

#include <QtDebug>

/******************************************************************************
 * TfltItf::resync()
 *****************************************************************************/
void TfltItf::resync()
{
    // enable status
    GetTfltEnable();

    // denoise level
    GetTfltDenoiseLevel();
    
    // denoise level
    GetTfltMinMax();
}

/******************************************************************************
 * TfltItf::GetTfltEnable
 *****************************************************************************/
void TfltItf::GetTfltEnable()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(TfltEnableChanged(int))) > 0 )
    {
        uint8_t value = 0u;
    
        // read tflt enable status from device
        int res = ctrl_protocol_get_tflt_enable( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a TfltEnableChanged signal
        emit TfltEnableChanged( (int)value );
    }
}

/******************************************************************************
 * TfltItf::GetTfltDenoiseLevel
 *****************************************************************************/
void TfltItf::GetTfltDenoiseLevel()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(TfltDenoiseLevelChanged(int))) > 0 )
    {
        uint8_t value = 0u;
    
        // read tflt denoise level from device
        int res = ctrl_protocol_get_tflt_denoise_level( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a TfltEnableChanged signal
        emit TfltDenoiseLevelChanged( (int)value );
    }
}

/******************************************************************************
 * TfltItf::GetTfltMinMax
 *****************************************************************************/
void TfltItf::GetTfltMinMax()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(TfltMinMaxChanged(int,int))) > 0 )
    {
        ctrl_protocol_tflt_index_t indices;

        memset( &indices, 0, sizeof(indices) );
    
        // read tflt denoise level from device
        int res = ctrl_protocol_get_tflt_min_max( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(indices), (uint8_t *)&indices );
        HANDLE_ERROR( res );
        
        // emit a TfltEnableChanged signal
        emit TfltMinMaxChanged( (int)indices.min, (int)indices.max );
    }
}

/******************************************************************************
 * TfltItf::onTfltEnableChange
 *****************************************************************************/
void TfltItf::onTfltEnableChange( int value )
{
    // set tflt enable state on device
    int res = ctrl_protocol_set_tflt_enable( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), ( value ? 1u : 0u ) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * TfltItf::onTfltDenoiseLevelChange
 *****************************************************************************/
void TfltItf::onTfltDenoiseLevelChange( int value )
{
    // set tflt denoise level on device
    int res = ctrl_protocol_set_tflt_denoise_level( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * TfltItf::onTfltMinMaxChange
 *****************************************************************************/
void TfltItf::onTfltMinMaxChange( int min, int max )
{
    ctrl_protocol_tflt_index_t indices = { (uint8_t)min, (uint8_t)max };

    // set tflt coefficient range on device
    int res = ctrl_protocol_set_tflt_min_max( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), sizeof(indices), (uint8_t *)&indices );
    HANDLE_ERROR( res );
}

