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
 * @file    ctrl_protocol_dpcc.c
 *
 * @brief   Implementation of generic dpcc functions
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
#define DPCC_DRV( drv )     ((ctrl_protocol_dpcc_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_dpcc_enable
 *****************************************************************************/
int ctrl_protocol_get_dpcc_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( DPCC_DRV(protocol->drv), get_dpcc_enable );
    CHECK_NOT_NULL( enable );
    return ( DPCC_DRV(protocol->drv)->get_dpcc_enable( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_set_dpcc_enable
 *****************************************************************************/
int ctrl_protocol_set_dpcc_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( DPCC_DRV(protocol->drv), set_dpcc_enable );
    return ( DPCC_DRV(protocol->drv)->set_dpcc_enable( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_get_dpcc_mode
 *****************************************************************************/
int ctrl_protocol_get_dpcc_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( DPCC_DRV(protocol->drv), get_dpcc_mode );
    CHECK_NOT_NULL( mode );
    return ( DPCC_DRV(protocol->drv)->get_dpcc_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_dpcc_mode
 *****************************************************************************/
int ctrl_protocol_set_dpcc_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( DPCC_DRV(protocol->drv), set_dpcc_mode );
    return ( DPCC_DRV(protocol->drv)->set_dpcc_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_dpcc_level
 *****************************************************************************/
int ctrl_protocol_get_dpcc_level
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( DPCC_DRV(protocol->drv), get_dpcc_level );
    CHECK_NOT_NULL( mode );
    return ( DPCC_DRV(protocol->drv)->get_dpcc_level( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_dpcc_level
 *****************************************************************************/
int ctrl_protocol_set_dpcc_level
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( DPCC_DRV(protocol->drv), set_dpcc_level );
    return ( DPCC_DRV(protocol->drv)->set_dpcc_level( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_dpcc_table
 *****************************************************************************/
int ctrl_protocol_get_dpcc_table
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint32_t * const             buf 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( DPCC_DRV(protocol->drv), get_dpcc_table );
    return ( DPCC_DRV(protocol->drv)->get_dpcc_table( protocol->ctx, channel, no, buf ) );
}

/******************************************************************************
 * ctrl_protocol_add_pixel
 *****************************************************************************/
int ctrl_protocol_add_pixel
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint16_t * const             buf 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( DPCC_DRV(protocol->drv), add_dpcc_pixel );
    return ( DPCC_DRV(protocol->drv)->add_dpcc_pixel( protocol->ctx, channel, no, buf ) );
}

/******************************************************************************
 * ctrl_protocol_clear_dpcc_table
 *****************************************************************************/
int ctrl_protocol_clear_dpcc_table
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( DPCC_DRV(protocol->drv), clear_dpcc_table );
    return ( DPCC_DRV(protocol->drv)->clear_dpcc_table( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_save_dpcc_table
 *****************************************************************************/
int ctrl_protocol_save_dpcc_table
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( DPCC_DRV(protocol->drv), save_dpcc_table );
    return ( DPCC_DRV(protocol->drv)->save_dpcc_table( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_load_dpcc_table
 *****************************************************************************/
int ctrl_protocol_load_dpcc_table
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( DPCC_DRV(protocol->drv), load_dpcc_table );
    return ( DPCC_DRV(protocol->drv)->load_dpcc_table( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_load_dpcc_table
 *****************************************************************************/
int ctrl_protocol_auto_load_dpcc_table
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( DPCC_DRV(protocol->drv), auto_load_dpcc_table );
    return ( DPCC_DRV(protocol->drv)->auto_load_dpcc_table( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_get_dpcc_test_mode
 *****************************************************************************/
int ctrl_protocol_get_dpcc_test_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( DPCC_DRV(protocol->drv), get_dpcc_test_mode );
    CHECK_NOT_NULL( mode );
    return ( DPCC_DRV(protocol->drv)->get_dpcc_test_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_dpcc_mode
 *****************************************************************************/
int ctrl_protocol_set_dpcc_test_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( DPCC_DRV(protocol->drv), set_dpcc_test_mode );
    return ( DPCC_DRV(protocol->drv)->set_dpcc_test_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_dpcc_register
 *****************************************************************************/
int ctrl_protocol_dpcc_register
(
    ctrl_protocol_handle_t const        handle,
    void * const                        ctx,
    ctrl_protocol_dpcc_drv_t * const    drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = (void *)drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_dpcc_unregister
 *****************************************************************************/
int ctrl_protocol_dpcc_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}
