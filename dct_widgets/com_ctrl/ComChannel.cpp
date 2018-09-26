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
 * @file    ComChannel.cpp
 *
 * @brief   
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include "common.h"
#include "ComChannel.h"

/******************************************************************************
 * ComChannel::ComChannel
 *****************************************************************************/
ComChannel::ComChannel()
{
    size_t size = static_cast<size_t>(ctrl_channel_get_instance_size());

    // create control channel instance
    m_channel = static_cast<ctrl_channel_handle_t>(malloc(size));
    if ( !m_channel )
    {
        showError( -ENOMEM, __FILE__, __FUNCTION__, __LINE__ );
        return;
    }

    // clear memory
    memset( m_channel, 0, size );
}

/******************************************************************************
 * ComChannel::~ComChannel
 *****************************************************************************/
ComChannel::~ComChannel()
{
    (void)ctrl_channel_close( m_channel );
    free( m_channel );
}

/******************************************************************************
 * ComChannel::Open
 *****************************************************************************/
int ComChannel::Open( void * param, int size ) 
{
    return ( ctrl_channel_open( m_channel, param, size ) );
}

/******************************************************************************
 * ComChannel::isOpen
 *****************************************************************************/
bool ComChannel::isOpen() const
{
    return ( CTRL_CHANNEL_STATE_CONNECTED == ctrl_channel_get_state( m_channel ) );
}

/******************************************************************************
 * ComChannel::Close
 *****************************************************************************/
void ComChannel::Close()
{
    (void)ctrl_channel_close( m_channel );
}

