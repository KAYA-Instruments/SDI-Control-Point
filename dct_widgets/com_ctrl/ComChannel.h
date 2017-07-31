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

