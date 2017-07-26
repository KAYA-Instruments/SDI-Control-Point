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
 * @file    ctrl_protocol_fpnc.c
 *
 * @brief   Implementation of generic fpnc functions
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
#define FPNC_DRV( drv )     ((ctrl_protocol_fpnc_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_fpnc_enable
 *****************************************************************************/
int ctrl_protocol_get_fpnc_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( FPNC_DRV(protocol->drv), get_fpnc_enable );
    CHECK_NOT_NULL( mode );
    return ( FPNC_DRV(protocol->drv)->get_fpnc_enable( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_fpnc_enable
 *****************************************************************************/
int ctrl_protocol_set_fpnc_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( FPNC_DRV(protocol->drv), set_fpnc_enable );
    return ( FPNC_DRV(protocol->drv)->set_fpnc_enable( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_fpnc_inv_gains
 *****************************************************************************/
int ctrl_protocol_get_fpnc_inv_gains
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( FPNC_DRV(protocol->drv), get_fpnc_inv_gains );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( FPNC_DRV(protocol->drv)->get_fpnc_inv_gains( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_fpnc_inv_gains
 *****************************************************************************/
int ctrl_protocol_set_fpnc_inv_gains
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( FPNC_DRV(protocol->drv), set_fpnc_inv_gains );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( FPNC_DRV(protocol->drv)->set_fpnc_inv_gains( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_fpnc_gains
 *****************************************************************************/
int ctrl_protocol_get_fpnc_gains
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( FPNC_DRV(protocol->drv), get_fpnc_gains );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( FPNC_DRV(protocol->drv)->get_fpnc_gains( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_fpnc_gains
 *****************************************************************************/
int ctrl_protocol_set_fpnc_gains
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( FPNC_DRV(protocol->drv), set_fpnc_gains );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( FPNC_DRV(protocol->drv)->set_fpnc_gains( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_fpnc_correction_data
 *****************************************************************************/
int ctrl_protocol_get_fpnc_correction_data
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( FPNC_DRV(protocol->drv), get_fpnc_correction_data );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( FPNC_DRV(protocol->drv)->get_fpnc_correction_data( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_fpnc_correction_data
 *****************************************************************************/
int ctrl_protocol_set_fpnc_correction_data
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( FPNC_DRV(protocol->drv), set_fpnc_correction_data );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( FPNC_DRV(protocol->drv)->set_fpnc_correction_data( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_fpnc_register
 *****************************************************************************/
int ctrl_protocol_fpnc_register
(
    ctrl_protocol_handle_t const        handle,
    void * const                        ctx,
    ctrl_protocol_fpnc_drv_t * const    drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_fpnc_unregister
 *****************************************************************************/
int ctrl_protocol_fpnc_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}

