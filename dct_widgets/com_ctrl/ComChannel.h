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
 * @file    ComChannel.h
 *
 * @brief   
 *
 *****************************************************************************/
#ifndef _COM_CHANNEL_H_
#define _COM_CHANNEL_H_

// generic control channel layer
#include <ctrl_channel/ctrl_channel.h>

class ComChannel
{
public:
    explicit ComChannel();
    virtual ~ComChannel();

    ctrl_channel_handle_t GetInstance() const
    {
        return ( m_channel );
    }

    int Open( void * param, int size );

    virtual void ReOpen() = 0;

    bool isOpen( ) const;

    void Close();

private:
    ctrl_channel_handle_t   m_channel;      // control channel instance
};

#endif // _COM_CHANNEL_H_

