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

