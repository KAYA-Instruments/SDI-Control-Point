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
 * @file    KneeItf.cpp
 *
 * @brief   Implementation of Knee Interface 
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <ctrl_protocol/ctrl_protocol_knee.h>

#include "common.h"
#include "KneeItf.h"

#include <QtDebug>

/******************************************************************************
 * KneeItf::resync()
 *****************************************************************************/
void KneeItf::resync()
{
    // enable status
    GetKneeConfig();
}

/******************************************************************************
 * KneeItf::GetKneeConfig
 *****************************************************************************/
void KneeItf::GetKneeConfig()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(KneeConfigChanged(int, int, int, int))) > 0 )
    {
        ctrl_protocol_knee_t c;

        memset( &c, 0, sizeof(c) );
    
        // read knee enable from device
        int res = ctrl_protocol_get_knee_config( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(c), (uint8_t *)&c );
        HANDLE_ERROR( res );
        
        // emit a KneeEnableChanged signal
        emit KneeConfigChanged( 
            (int)c.enable, (int)c.knee_point, (int)c.knee_slope, (int)c.white_clip );
    }
}

/******************************************************************************
 * KneeItf::onKneeConfigChange
 *****************************************************************************/
void KneeItf::onKneeConfigChange( int enable, int point, int slope, int clip )
{
    ctrl_protocol_knee_t c;

    c.enable     = enable;
    c.knee_point = point;
    c.knee_slope = slope;
    c.white_clip = clip;

    // set knee function configuration on device
    int res = ctrl_protocol_set_knee_config( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), sizeof(c), (uint8_t *)&c );
    HANDLE_ERROR( res );
}

