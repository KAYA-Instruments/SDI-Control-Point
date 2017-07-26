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
 * @file    ctrl_protocol_playback.c
 *
 * @brief   Implementation of generic playback engine functions
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
#define PLAYBACK_DRV( drv )     ((ctrl_protocol_playback_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_max_frames
 *****************************************************************************/
int ctrl_protocol_get_max_frames
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             no 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), get_max_frames );
    CHECK_NOT_NULL( no );
    return ( PLAYBACK_DRV(protocol->drv)->get_max_frames( protocol->ctx, channel, no ) );
}

/******************************************************************************
 * ctrl_protocol_set_buffer_default
 *****************************************************************************/
int ctrl_protocol_set_buffer_default
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                no 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), set_buffer_default );
    return ( PLAYBACK_DRV(protocol->drv)->set_buffer_default( protocol->ctx, channel, no ) );
}

/******************************************************************************
 * ctrl_protocol_get_buffer
 *****************************************************************************/
int ctrl_protocol_get_buffer
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), get_buffer );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( PLAYBACK_DRV(protocol->drv)->get_buffer( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_buffer
 *****************************************************************************/
int ctrl_protocol_set_buffer
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), set_buffer );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( PLAYBACK_DRV(protocol->drv)->set_buffer( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_buffer_size
 *****************************************************************************/
int ctrl_protocol_get_buffer_size
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), get_buffer_size );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( PLAYBACK_DRV(protocol->drv)->get_buffer_size( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_play_mode
 *****************************************************************************/
int ctrl_protocol_get_play_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), get_play_mode );
    CHECK_NOT_NULL( mode );
    return ( PLAYBACK_DRV(protocol->drv)->get_play_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_play_mode
 *****************************************************************************/
int ctrl_protocol_set_play_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), set_play_mode );
    return ( PLAYBACK_DRV(protocol->drv)->set_play_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_record_mode
 *****************************************************************************/
int ctrl_protocol_get_record_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), get_record_mode );
    CHECK_NOT_NULL( mode );
    return ( PLAYBACK_DRV(protocol->drv)->get_record_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_record_mode
 *****************************************************************************/
int ctrl_protocol_set_record_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), set_record_mode );
    return ( PLAYBACK_DRV(protocol->drv)->set_record_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_auto_live
 *****************************************************************************/
int ctrl_protocol_get_auto_live
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), get_auto_live );
    CHECK_NOT_NULL( mode );
    return ( PLAYBACK_DRV(protocol->drv)->get_auto_live( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_auto_live
 *****************************************************************************/
int ctrl_protocol_set_auto_live
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), set_auto_live );
    return ( PLAYBACK_DRV(protocol->drv)->set_auto_live( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_marker_out
 *****************************************************************************/
int ctrl_protocol_get_marker_out
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             pos
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), get_marker_out );
    CHECK_NOT_NULL( pos );
    return ( PLAYBACK_DRV(protocol->drv)->get_marker_out( protocol->ctx, channel, pos ) );
}

/******************************************************************************
 * ctrl_protocol_set_marker_out
 *****************************************************************************/
int ctrl_protocol_set_marker_out
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               pos
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), set_marker_out );
    return ( PLAYBACK_DRV(protocol->drv)->set_marker_out( protocol->ctx, channel, pos ) );
}

/******************************************************************************
 * ctrl_protocol_get_play
 *****************************************************************************/
int ctrl_protocol_get_play
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              buf_id 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), get_play );
    CHECK_NOT_NULL( buf_id );
    return ( PLAYBACK_DRV(protocol->drv)->get_play( protocol->ctx, channel, buf_id ) );
}

/******************************************************************************
 * ctrl_protocol_set_play
 *****************************************************************************/
int ctrl_protocol_set_play
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                buf_id
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), set_play );
    return ( PLAYBACK_DRV(protocol->drv)->set_play( protocol->ctx, channel, buf_id ) );
}

/******************************************************************************
 * ctrl_protocol_get_record
 *****************************************************************************/
int ctrl_protocol_get_record
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              buf_id 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), get_record );
    CHECK_NOT_NULL( buf_id );
    return ( PLAYBACK_DRV(protocol->drv)->get_record( protocol->ctx, channel, buf_id ) );
}

/******************************************************************************
 * ctrl_protocol_set_record
 *****************************************************************************/
int ctrl_protocol_set_record
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                buf_id
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), set_record );
    return ( PLAYBACK_DRV(protocol->drv)->set_record( protocol->ctx, channel, buf_id ) );
}

/******************************************************************************
 * ctrl_protocol_stop_record
 *****************************************************************************/
int ctrl_protocol_stop_record
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), stop_record );
    return ( PLAYBACK_DRV(protocol->drv)->stop_record( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_stop_play
 *****************************************************************************/
int ctrl_protocol_stop_play
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), stop_play );
    return ( PLAYBACK_DRV(protocol->drv)->stop_play( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_pause
 *****************************************************************************/
int ctrl_protocol_pause
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), pause );
    return ( PLAYBACK_DRV(protocol->drv)->pause( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_paused
 *****************************************************************************/
int ctrl_protocol_paused
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              flag
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), paused );
    CHECK_NOT_NULL( flag );
    return ( PLAYBACK_DRV(protocol->drv)->paused( protocol->ctx, channel, flag ) );
}

/******************************************************************************
 * ctrl_protocol_live
 *****************************************************************************/
int ctrl_protocol_live
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), live );
    return ( PLAYBACK_DRV(protocol->drv)->live( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_forward
 *****************************************************************************/
int ctrl_protocol_forward
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               frames
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), forward );
    return ( PLAYBACK_DRV(protocol->drv)->forward( protocol->ctx, channel, frames ) );
}

/******************************************************************************
 * ctrl_protocol_rewind
 *****************************************************************************/
int ctrl_protocol_rewind
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               frames
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), rewind );
    return ( PLAYBACK_DRV(protocol->drv)->rewind( protocol->ctx, channel, frames ) );
}

/******************************************************************************
 * ctrl_protocol_seek
 *****************************************************************************/
int ctrl_protocol_seek
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               pos 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), seek );
    return ( PLAYBACK_DRV(protocol->drv)->seek( protocol->ctx, channel, pos ) );
}

/******************************************************************************
 * ctrl_protocol_position
 *****************************************************************************/
int ctrl_protocol_position
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             pos 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( PLAYBACK_DRV(protocol->drv), position );
    CHECK_NOT_NULL( pos );
    return ( PLAYBACK_DRV(protocol->drv)->position( protocol->ctx, channel, pos ) );
}

/******************************************************************************
 * ctrl_protocol_playback_register
 *****************************************************************************/
int ctrl_protocol_playback_register
(
    ctrl_protocol_handle_t const            handle,
    void * const                            ctx,
    ctrl_protocol_playback_drv_t * const    drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_playback_unregister
 *****************************************************************************/
int ctrl_protocol_playback_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}

