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

