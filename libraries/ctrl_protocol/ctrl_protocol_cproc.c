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
 * @file    ctrl_protocol_cproc.c
 *
 * @brief   Implementation of generic cproc functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>
#include <ctrl_protocol/ctrl_protocol.h>

#include "ctrl_protocol_priv.h"

/******************************************************************************
 * @brief Macro for type-casting to function driver 
 *****************************************************************************/
#define CPROC_DRV( drv )        ((ctrl_protocol_cproc_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_post_bright
 *****************************************************************************/
int ctrl_protocol_get_post_bright
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t * const              brightness
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CPROC_DRV(protocol->drv), get_post_bright );
    CHECK_NOT_NULL( brightness );
    return ( CPROC_DRV(protocol->drv)->get_post_bright( protocol->ctx, channel, brightness ) );
}

/******************************************************************************
 * ctrl_protocol_set_post_bright
 *****************************************************************************/
int ctrl_protocol_set_post_bright
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t const                brightness
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CPROC_DRV(protocol->drv), set_post_bright );
    return ( CPROC_DRV(protocol->drv)->set_post_bright( protocol->ctx, channel, brightness ) );
}

/******************************************************************************
 * ctrl_protocol_get_post_cont
 *****************************************************************************/
int ctrl_protocol_get_post_cont
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             contrast
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CPROC_DRV(protocol->drv), get_post_cont );
    CHECK_NOT_NULL( contrast );
    return ( CPROC_DRV(protocol->drv)->get_post_cont( protocol->ctx, channel, contrast ) );
}

/******************************************************************************
 * ctrl_protocol_set_post_cont
 *****************************************************************************/
int ctrl_protocol_set_post_cont
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               contrast
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CPROC_DRV(protocol->drv), set_post_cont );
    return ( CPROC_DRV(protocol->drv)->set_post_cont( protocol->ctx, channel, contrast ) );
}

/******************************************************************************
 * ctrl_protocol_get_post_sat
 *****************************************************************************/
int ctrl_protocol_get_post_sat
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             saturation
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CPROC_DRV(protocol->drv), get_post_sat );
    CHECK_NOT_NULL( saturation );
    return ( CPROC_DRV(protocol->drv)->get_post_sat( protocol->ctx, channel, saturation ) );
}

/******************************************************************************
 * ctrl_protocol_set_post_sat
 *****************************************************************************/
int ctrl_protocol_set_post_sat
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               saturation
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CPROC_DRV(protocol->drv), set_post_sat );
    return ( CPROC_DRV(protocol->drv)->set_post_sat( protocol->ctx, channel, saturation ) );
}

/******************************************************************************
 * ctrl_protocol_get_post_hue
 *****************************************************************************/
int ctrl_protocol_get_post_hue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t * const              hue 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CPROC_DRV(protocol->drv), get_post_hue );
    CHECK_NOT_NULL( hue );
    return ( CPROC_DRV(protocol->drv)->get_post_hue( protocol->ctx, channel, hue ) );
}

/******************************************************************************
 * ctrl_protocol_set_post_hue
 *****************************************************************************/
int ctrl_protocol_set_post_hue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t const                hue
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CPROC_DRV(protocol->drv), set_post_hue );
    return ( CPROC_DRV(protocol->drv)->set_post_hue( protocol->ctx, channel, hue ) );
}

/******************************************************************************
 * ctrl_protocol_cproc_register
 *****************************************************************************/
int ctrl_protocol_cproc_register
(
    ctrl_protocol_handle_t const        handle,
    void * const                        ctx,
    ctrl_protocol_cproc_drv_t * const   drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = (void *)drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_cproc_unregister
 *****************************************************************************/
int ctrl_protocol_cproc_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}

