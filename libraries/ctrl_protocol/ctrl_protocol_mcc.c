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
 * @file    ctrl_protocol_mcc.c
 *
 * @brief   Implementation of generic mcc functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>
#include <ctrl_protocol/ctrl_protocol.h>

#include "ctrl_protocol_priv.h"

/**************************************************************************//**
 * @brief 12 Segment operation mode
 *****************************************************************************/
#define MCC_OPERATION_MODE_12_SEGMENTS  ( 0u )

/**************************************************************************//**
 * @brief 16 Segment operation mode
 *****************************************************************************/
#define MCC_OPERATION_MODE_16_SEGMENTS  ( 1u )

/**************************************************************************//**
 * @brief 24 Segment operation mode
 *****************************************************************************/
#define MCC_OPERATION_MODE_24_SEGMENTS  ( 2u )

/**************************************************************************//**
 * @brief 32 Segment operation mode
 *****************************************************************************/
#define MCC_OPERATION_MODE_32_SEGMENTS  ( 3u )

/******************************************************************************
 * @brief Macro for type-casting to function driver 
 *****************************************************************************/
#define MCC_DRV( drv )      ((ctrl_protocol_mcc_drv_t *)drv)

/**************************************************************************//**
 * ctrl_protocol_get_mcc_no_segments - converts operation mode into number of 
 *                                     color segments
 *****************************************************************************/
int ctrl_protocol_get_mcc_no_phases( uint8_t const opmode, uint8_t * const no )
{
    if ( no && (opmode < MAX_MCC_OPERATION_MODES) )
    {
        uint8_t segements[MAX_MCC_OPERATION_MODES] = { 12u, 16u, 24u, 32u };
        *no = segements[opmode];
        return ( 0 );
    }

    return ( -EINVAL );
}

/******************************************************************************
 * ctrl_protocol_get_mcc_enable
 *****************************************************************************/
int ctrl_protocol_get_mcc_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( MCC_DRV(protocol->drv), get_mcc_enable );
    CHECK_NOT_NULL( enable );
    return ( MCC_DRV(protocol->drv)->get_mcc_enable( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_set_mcc_enable
 *****************************************************************************/
int ctrl_protocol_set_mcc_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( MCC_DRV(protocol->drv), set_mcc_enable );
    return ( MCC_DRV(protocol->drv)->set_mcc_enable( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_get_mcc_opmode
 *****************************************************************************/
int ctrl_protocol_get_mcc_opmode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( MCC_DRV(protocol->drv), get_mcc_opmode );
    CHECK_NOT_NULL( mode );
    return ( MCC_DRV(protocol->drv)->get_mcc_opmode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_mcc_opmode
 *****************************************************************************/
int ctrl_protocol_set_mcc_opmode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( MCC_DRV(protocol->drv), set_mcc_opmode );
    return ( MCC_DRV(protocol->drv)->set_mcc_opmode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_mcc_blink
 *****************************************************************************/
int ctrl_protocol_get_mcc_blink
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( MCC_DRV(protocol->drv), get_mcc_blink );
    CHECK_NOT_NULL( mode );
    return ( MCC_DRV(protocol->drv)->get_mcc_blink( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_mcc_blink
 *****************************************************************************/
int ctrl_protocol_set_mcc_blink
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( MCC_DRV(protocol->drv), set_mcc_blink );
    return ( MCC_DRV(protocol->drv)->set_mcc_blink( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_mcc_phase
 *****************************************************************************/
int ctrl_protocol_get_mcc_phase
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              buf 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( MCC_DRV(protocol->drv), get_mcc_phase );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( buf );
    return ( MCC_DRV(protocol->drv)->get_mcc_phase( protocol->ctx, channel, no, buf ) );
}

/******************************************************************************
 * ctrl_protocol_set_mcc_phase
 *****************************************************************************/
int ctrl_protocol_set_mcc_phase
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              buf 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( MCC_DRV(protocol->drv), set_mcc_phase );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( buf );
    return ( MCC_DRV(protocol->drv)->set_mcc_phase( protocol->ctx, channel, no, buf ) );
}

/******************************************************************************
 * ctrl_protocol_mcc_register
 *****************************************************************************/
int ctrl_protocol_mcc_register
(
    ctrl_protocol_handle_t const    handle,
    void * const                    ctx,
    ctrl_protocol_mcc_drv_t * const drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = (void *)drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_mcc_unregister
 *****************************************************************************/
int ctrl_protocol_mcc_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}

