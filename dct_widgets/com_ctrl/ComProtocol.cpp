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
 * @file    ComProtocol.cpp
 *
 * @brief   Implementation of a generic control protocol
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include "common.h"
#include "ComProtocol.h"

/******************************************************************************
 * ComProtocol::ComProtocol
 *****************************************************************************/
ComProtocol::ComProtocol( instanceType i, void * )
{
    // get protocol instance size
    int size = ctrl_protocol_get_instance_size();

    // create protocol instance
    m_protocol = (ctrl_protocol_handle_t)(new char[size]);
    if ( !m_protocol )
    {
        showError( -ENOMEM, __FILE__, __FUNCTION__, __LINE__ ); 
        return;
    }

    // clear protocol instance memory
    memset( m_protocol, 0, size );

    m_instanceType = i;
}

/******************************************************************************
 * ComProtocol::~ComProtocol
 *****************************************************************************/
ComProtocol::~ComProtocol()
{
    delete[] ((char *)m_protocol);
}

