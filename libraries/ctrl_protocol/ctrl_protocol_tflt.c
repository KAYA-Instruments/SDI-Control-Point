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
 * @file    ctrl_protocol_tflt.c
 *
 * @brief   Implementation of generic tflt functions
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
#define TFLT_DRV( drv )     ((ctrl_protocol_tflt_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_tflt_enable
 *****************************************************************************/
int ctrl_protocol_get_tflt_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( TFLT_DRV(protocol->drv), get_tflt_enable );
    CHECK_NOT_NULL( mode );
    return ( TFLT_DRV(protocol->drv)->get_tflt_enable( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_tflt_enable
 *****************************************************************************/
int ctrl_protocol_set_tflt_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( TFLT_DRV(protocol->drv), set_tflt_enable );
    return ( TFLT_DRV(protocol->drv)->set_tflt_enable( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_tflt_min_max
 *****************************************************************************/
int ctrl_protocol_get_tflt_min_max
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    int const                       no,
    uint8_t * const                 buf 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( TFLT_DRV(protocol->drv), get_tflt_min_max );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( buf );
    return ( TFLT_DRV(protocol->drv)->get_tflt_min_max( protocol->ctx, channel, no, buf ) );
}

/******************************************************************************
 * ctrl_protocol_set_tflt_min_max
 *****************************************************************************/
int ctrl_protocol_set_tflt_min_max
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    int const                       no,
    uint8_t * const                 buf 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( TFLT_DRV(protocol->drv), set_tflt_min_max );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( buf );
    return ( TFLT_DRV(protocol->drv)->set_tflt_min_max( protocol->ctx, channel, no, buf ) );
}

/******************************************************************************
 * ctrl_protocol_get_tflt_denoise_level
 *****************************************************************************/
int ctrl_protocol_get_tflt_denoise_level
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              level 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( TFLT_DRV(protocol->drv), get_tflt_denoise_level );
    CHECK_NOT_NULL( level );
    return ( TFLT_DRV(protocol->drv)->get_tflt_denoise_level( protocol->ctx, channel, level ) );
}

/******************************************************************************
 * ctrl_protocol_set_tflt_denoise_level
 *****************************************************************************/
int ctrl_protocol_set_tflt_denoise_level
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                level
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( TFLT_DRV(protocol->drv), set_tflt_denoise_level );
    return ( TFLT_DRV(protocol->drv)->set_tflt_denoise_level( protocol->ctx, channel, level ) );
}

/******************************************************************************
 * ctrl_protocol_tflt_register
 *****************************************************************************/
int ctrl_protocol_tflt_register
(
    ctrl_protocol_handle_t const        handle,
    void * const                        ctx,
    ctrl_protocol_tflt_drv_t * const    drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = (void *)drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_tflt_unregister
 *****************************************************************************/
int ctrl_protocol_tflt_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}

