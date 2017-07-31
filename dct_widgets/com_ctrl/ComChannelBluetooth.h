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

