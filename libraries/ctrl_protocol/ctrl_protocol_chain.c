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
 * @file    ctrl_protocol_chain.c
 *
 * @brief   Implementation of generic chain functions
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
#define CHAIN_DRV( drv )        ((ctrl_protocol_chain_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_output_chain
 *****************************************************************************/
int ctrl_protocol_get_output_chain
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              chidx
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_output_chain );
    CHECK_NOT_NULL( chidx );
    return ( CHAIN_DRV(protocol->drv)->get_output_chain( protocol->ctx, channel, chidx ) );
}

/******************************************************************************
 * ctrl_protocol_set_output_chain
 *****************************************************************************/
int ctrl_protocol_set_output_chain
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                chidx
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_output_chain );
    return ( CHAIN_DRV(protocol->drv)->set_output_chain( protocol->ctx, channel, chidx ) );
}

/******************************************************************************
 * ctrl_protocol_get_video_mode
 *****************************************************************************/
int ctrl_protocol_get_video_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_video_mode );
    CHECK_NOT_NULL( mode );
    return ( CHAIN_DRV(protocol->drv)->get_video_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_video_mode
 *****************************************************************************/
int ctrl_protocol_set_video_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_video_mode );
    return ( CHAIN_DRV(protocol->drv)->set_video_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_raw_mode
 *****************************************************************************/
int ctrl_protocol_get_raw_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_raw_mode );
    CHECK_NOT_NULL( mode );
    return ( CHAIN_DRV(protocol->drv)->get_raw_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_raw_mode
 *****************************************************************************/
int ctrl_protocol_set_raw_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_raw_mode );
    return ( CHAIN_DRV(protocol->drv)->set_raw_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_sdi2_mode
 *****************************************************************************/
int ctrl_protocol_get_sdi2_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_sdi2_mode );
    CHECK_NOT_NULL( mode );
    return ( CHAIN_DRV(protocol->drv)->get_sdi2_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_sdi2_mode
 *****************************************************************************/
int ctrl_protocol_set_sdi2_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_sdi2_mode );
    return ( CHAIN_DRV(protocol->drv)->set_sdi2_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_flip_mode
 *****************************************************************************/
int ctrl_protocol_get_flip_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_flip_mode );
    CHECK_NOT_NULL( mode );
    return ( CHAIN_DRV(protocol->drv)->get_flip_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_flip_mode
 *****************************************************************************/
int ctrl_protocol_set_flip_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_flip_mode );
    return ( CHAIN_DRV(protocol->drv)->set_flip_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_sdi_range
 *****************************************************************************/
int ctrl_protocol_get_sdi_range
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_sdi_range );
    CHECK_NOT_NULL( mode );
    return ( CHAIN_DRV(protocol->drv)->get_sdi_range( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_sdi_range
 *****************************************************************************/
int ctrl_protocol_set_sdi_range
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_sdi_range );
    return ( CHAIN_DRV(protocol->drv)->set_sdi_range( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_sdi_black
 *****************************************************************************/
int ctrl_protocol_get_sdi_black
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int8_t * const               offset
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_sdi_black );
    CHECK_NOT_NULL( offset );
    return ( CHAIN_DRV(protocol->drv)->get_sdi_black( protocol->ctx, channel, offset ) );
}

/******************************************************************************
 * ctrl_protocol_set_sdi_black
 *****************************************************************************/
int ctrl_protocol_set_sdi_black
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int8_t const                 offset
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_sdi_black );
    return ( CHAIN_DRV(protocol->drv)->set_sdi_black( protocol->ctx, channel, offset ) );
}

/******************************************************************************
 * ctrl_protocol_get_sdi_white
 *****************************************************************************/
int ctrl_protocol_get_sdi_white
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int8_t * const               offset
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_sdi_white );
    CHECK_NOT_NULL( offset );
    return ( CHAIN_DRV(protocol->drv)->get_sdi_white( protocol->ctx, channel, offset ) );
}

/******************************************************************************
 * ctrl_protocol_set_sdi_white
 *****************************************************************************/
int ctrl_protocol_set_sdi_white
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int8_t const                 offset
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_sdi_white );
    return ( CHAIN_DRV(protocol->drv)->set_sdi_white( protocol->ctx, channel, offset ) );
}

/******************************************************************************
 * ctrl_protocol_get_genlock_mode
 *****************************************************************************/
int ctrl_protocol_get_genlock_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_genlock_mode );
    CHECK_NOT_NULL( mode );
    return ( CHAIN_DRV(protocol->drv)->get_genlock_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_genlock_mode
 *****************************************************************************/
int ctrl_protocol_set_genlock_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_genlock_mode );
    return ( CHAIN_DRV(protocol->drv)->set_genlock_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_genlock_offset
 *****************************************************************************/
int ctrl_protocol_get_genlock_offset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int16_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_genlock_offset );
    CHECK_NOT_NULL( values );
    return ( CHAIN_DRV(protocol->drv)->get_genlock_offset( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_genlock_offset
 *****************************************************************************/
int ctrl_protocol_set_genlock_offset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int16_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_genlock_offset );
    CHECK_NOT_NULL( values );
    return ( CHAIN_DRV(protocol->drv)->set_genlock_offset( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_genlock_termination
 *****************************************************************************/
int ctrl_protocol_get_genlock_termination
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              flag 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_genlock_termination );
    CHECK_NOT_NULL( flag );
    return ( CHAIN_DRV(protocol->drv)->get_genlock_termination( protocol->ctx, channel, flag ) );
}

/******************************************************************************
 * ctrl_protocol_set_genlock_termination
 *****************************************************************************/
int ctrl_protocol_set_genlock_termination
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                flag
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_genlock_termination );
    return ( CHAIN_DRV(protocol->drv)->set_genlock_termination( protocol->ctx, channel, flag ) );
}

/******************************************************************************
 * ctrl_protocol_chain_register
 *****************************************************************************/
int ctrl_protocol_chain_register
(
    ctrl_protocol_handle_t const        handle,
    void * const                        ctx,
    ctrl_protocol_chain_drv_t * const   drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = (void *)drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_get_timecode
 *****************************************************************************/
int ctrl_protocol_get_timecode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no, 
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_timecode );
    CHECK_NOT_NULL( values );
    return ( CHAIN_DRV(protocol->drv)->get_timecode( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_timecode
 *****************************************************************************/
int ctrl_protocol_set_timecode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no, 
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_timecode );
    return ( CHAIN_DRV(protocol->drv)->set_timecode( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_chain_unregister
 *****************************************************************************/
int ctrl_protocol_chain_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}

