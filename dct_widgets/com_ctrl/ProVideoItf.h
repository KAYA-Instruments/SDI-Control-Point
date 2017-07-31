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
 * @file    ProVideoItf.h
 *
 * @brief   ProVideo Control Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _PROVIDEO_INTERFACE_H_
#define _PROVIDEO_INTERFACE_H_

#include <QObject>

#include "ComChannel.h"
#include "ComProtocol.h"

class ProVideoItf : public QObject
{
    Q_OBJECT

public:
    explicit ProVideoItf( ComChannel * c, ComProtocol * p )
        : m_ComChannel( c ), m_ComProtocol( p )
    { }
   
    // get Communication Control Channel
    ComChannel * GetComChannel() const
    {
        return ( m_ComChannel );
    }
    
    // set Communication Control Channel
    void SetComChannel( ComChannel * channel )
    {
        m_ComChannel = channel;
    }

    // get Communication Control Protocol
    ComProtocol * GetComProtocol() const
    {
        return ( m_ComProtocol );
    }

protected:   
    // abstract resync all settings
    // (needs to be implementaed in derived interface class)
    virtual void resync() = 0;

private:
    ComChannel *    m_ComChannel;
    ComProtocol *   m_ComProtocol;
};

#define GET_CHANNEL_INSTANCE(x)     ( x->GetComChannel()->GetInstance() )
#define GET_PROTOCOL_INSTANCE(x)    ( x->GetComProtocol()->GetInstance() )

#endif // _PROVIDEO_INTERFACE_H_

