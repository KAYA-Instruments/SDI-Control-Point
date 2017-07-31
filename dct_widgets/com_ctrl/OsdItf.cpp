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

