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
 * @file    OsdItf.cpp
 *
 * @brief   Implementation of Osd Interface
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <ctrl_protocol/ctrl_protocol_osd.h>

#include "common.h"
#include "OsdItf.h"

#include <QtDebug>

/******************************************************************************
 * OsdItf::resync()
 *****************************************************************************/
void OsdItf::resync()
{
    // enable status
    GetTestPattern();
}

/******************************************************************************
 * OsdItf::GetTestPattern
 *****************************************************************************/
void OsdItf::GetTestPattern()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(TestPatternChanged(int))) > 0 )
    {
        uint8_t value;

        // read test_pattern mode from device
        int res = ctrl_protocol_get_test_pattern( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a test pattern changed signal
        emit TestPatternChanged( value );
    }
}

/******************************************************************************
 * OsdItf::onTestPatternChange
 *****************************************************************************/
void OsdItf::onTestPatternChange( int pattern )
{
    // set test pattern on device
    int res = ctrl_protocol_set_test_pattern( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint8_t)pattern );
    HANDLE_ERROR( res );
}

