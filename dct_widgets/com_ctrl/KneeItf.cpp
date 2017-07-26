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

