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

