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
 * @file    ProVideoProtocol.cpp
 *
 * @brief   Implementation of a proVideo control protocol
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <QtDebug>

#include "common.h"
#include "ComProtocol.h"
#include "ProVideoProtocol.h"

// specific control protocol implementation
#include <provideo_protocol/provideo_protocol.h>

/******************************************************************************
 * ProvideoProtocol::ProvideoProtocol
 *****************************************************************************/
ProVideoProtocol::ProVideoProtocol( void * UsrCtx )
    : ComProtocol( SYS_INSTANCE ) 
{
    int res = provideo_protocol_sys_init( GetInstance(), UsrCtx );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProvideoProtocol::ProvideoProtocol
 *****************************************************************************/
ProVideoProtocol::ProVideoProtocol( instanceType i, void * UsrCtx )
    : ComProtocol( i )
{
    int res = -EINVAL;

    switch ( i )
    {
        case ISP_INSTANCE:
            res = provideo_protocol_isp_init( GetInstance(), UsrCtx );
            break;
        
        case CPROC_INSTANCE:
            res = provideo_protocol_cproc_init( GetInstance(), UsrCtx );
            break;
        
        case AUTO_INSTANCE:
            res = provideo_protocol_auto_init( GetInstance(), UsrCtx );
            break;
        
        case CAM_INSTANCE:
            res = provideo_protocol_cam_init( GetInstance(), UsrCtx );
            break;
        
        case MCC_INSTANCE:
            res = provideo_protocol_mcc_init( GetInstance(), UsrCtx );
            break;
        
        case LUT_INSTANCE:
            res = provideo_protocol_lut_init( GetInstance(), UsrCtx );
            break;
        
        case CHAIN_INSTANCE:
            res = provideo_protocol_chain_init( GetInstance(), UsrCtx );
            break;
        
        case OSD_INSTANCE:
            res = provideo_protocol_osd_init( GetInstance(), UsrCtx );
            break;
        
        case TFLT_INSTANCE:
            res = provideo_protocol_tflt_init( GetInstance(), UsrCtx );
            break;
        
        case FPNC_INSTANCE:
            res = provideo_protocol_fpnc_init( GetInstance(), UsrCtx );
            break;
        
        case PLAYBACK_INSTANCE:
            res = provideo_protocol_playback_init( GetInstance(), UsrCtx );
            break;

        case IRIS_INSTANCE:
            res = provideo_protocol_iris_init( GetInstance(), UsrCtx );
            break;

        case KNEE_INSTANCE:
            res = provideo_protocol_knee_init( GetInstance(), UsrCtx );
            break;

        case DPCC_INSTANCE:
            res = provideo_protocol_dpcc_init( GetInstance(), UsrCtx );
            break;

        default:
            break;
    }

    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProvideoProtocol::clone
 *****************************************************************************/
ProVideoProtocol * ProVideoProtocol::clone( instanceType i, void * UsrCtx ) const
{
    return ( new ProVideoProtocol( i, UsrCtx ) );
}

/******************************************************************************
 * ProvideoProtocol::createUserContext
 *****************************************************************************/
void * ProVideoProtocol::createUserContext() const
{
    int size = sizeof(provideo_protocol_user_ctx_t);
    void * buf = ( (void *)(new char[size]) );
    memset( buf, 0, size );
    return ( buf );
}

/******************************************************************************
 * ProvideoProtocol::deleteUserContext
 *****************************************************************************/
void ProVideoProtocol::deleteUserContext( void * UsrCtx )
{
    delete[] ( (char *)UsrCtx );
}

/******************************************************************************
 * ProvideoProtocol::CopyFlag
 *****************************************************************************/
bool ProVideoProtocol::CopyFlag( void * UsrCtx ) const
{
    if ( UsrCtx )
    {
        provideo_protocol_user_ctx_t * ctx = (provideo_protocol_user_ctx_t *)UsrCtx;
        return ( ctx->use_copy_flag ? true : false );
    }

    return ( false );
}

/******************************************************************************
 * ProvideoProtocol::setCopyFlag
 *****************************************************************************/
void ProVideoProtocol::setCopyFlag( void * UsrCtx, const bool flag )
{
    if ( UsrCtx )
    {
        provideo_protocol_user_ctx_t * ctx = (provideo_protocol_user_ctx_t *)UsrCtx;
        ctx->use_copy_flag = flag ? 1 : 0;
    }
}

