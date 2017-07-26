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
    int size = ctrl_channel_get_instance_size();

    // create control channel instance
    m_channel = (ctrl_channel_handle_t)(new char[size]);
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
    delete[] ((char *)m_channel);
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
    return ( (bool)( CTRL_CHANNEL_STATE_CONNECTED == ctrl_channel_get_state( m_channel ) ) );
}

/******************************************************************************
 * ComChannel::Close
 *****************************************************************************/
void ComChannel::Close()
{
    (void)ctrl_channel_close( m_channel );
}

