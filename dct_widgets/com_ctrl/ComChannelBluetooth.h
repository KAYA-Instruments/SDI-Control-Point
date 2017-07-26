/******************************************************************************
 *
 * Copyright 2015, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
/**
 * @file    ComChannelBluetooth.h
 *
 * @brief   
 *
 *****************************************************************************/
#ifndef _COM_CHANNEL_BLUETOOTH_H_
#define _COM_CHANNEL_BLUETOOTH_H_

#include <QtBluetooth>

// generic control channel layer
#include <ctrl_channel/ctrl_channel_generic.h>

#include "ComChannel.h"

class ComChannelBluetooth : public ComChannel
{
public:
    explicit ComChannelBluetooth();

    QBluetoothSocket * getSocket()
    {
        return ( m_socket );
    }

    void setSocket( QBluetoothSocket * s )
    {
        m_socket = s;
    }

private:
    QBluetoothSocket * m_socket;
};

#endif // _COM_CHANNEL_BLUETOOTH_H_

