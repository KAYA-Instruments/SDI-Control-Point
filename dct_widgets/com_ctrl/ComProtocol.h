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
 * @file    ComProtocol.h
 *
 * @brief   Definition of a generic control protocol 
 *
 *****************************************************************************/
#ifndef _COM_PROTOCOL_H_
#define _COM_PROTOCOL_H_

// generic control protocol layer
#include <ctrl_protocol/ctrl_protocol.h>

// generic communication protocol class 
class ComProtocol
{
public:
    enum instanceType
    {
        SYS_INSTANCE = 0,
        ISP_INSTANCE,
        CPROC_INSTANCE,
        AUTO_INSTANCE,
        MCC_INSTANCE,
        CHAIN_INSTANCE,
        LUT_INSTANCE,
        CAM_INSTANCE,
        TFLT_INSTANCE,
        FPNC_INSTANCE,
        PLAYBACK_INSTANCE,
        OSD_INSTANCE,
        TIMECODE_INSTANCE,
        IRIS_INSTANCE,
        KNEE_INSTANCE,
        DPCC_INSTANCE
    };

    explicit ComProtocol( instanceType i, void * UsrCtx = NULL );
    virtual ~ComProtocol();

    instanceType GetInstanceType() const
    {
        return ( m_instanceType );
    }

    ctrl_protocol_handle_t GetInstance() const
    {
        return ( m_protocol );
    }

    virtual ComProtocol * clone( instanceType i, void * UsrCtx = NULL ) const = 0;
    virtual void * createUserContext() const = 0;
    virtual void deleteUserContext( void * UsrCtx ) = 0;
    virtual bool CopyFlag( void * UsrCtx ) const = 0;
    virtual void setCopyFlag( void * UsrCtx, const bool flag ) = 0;

private:
    ctrl_protocol_handle_t  m_protocol;         // control protocol handle
    instanceType            m_instanceType;
};

#endif // _COM_PROTOCOL_H_

