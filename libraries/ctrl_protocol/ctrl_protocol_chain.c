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
 * ctrl_protocol_get_downscale_mode
 *****************************************************************************/
int ctrl_protocol_get_downscale_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_downscale_mode );
    CHECK_NOT_NULL( values );
    return ( CHAIN_DRV(protocol->drv)->get_downscale_mode( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_downscale_mode
 *****************************************************************************/
int ctrl_protocol_set_downscale_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_downscale_mode );
    CHECK_NOT_NULL( values );
    return ( CHAIN_DRV(protocol->drv)->set_downscale_mode( protocol->ctx, channel, no, values ) );
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
 * ctrl_protocol_get_genlock_crosslock
 *****************************************************************************/
int ctrl_protocol_get_genlock_crosslock
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const               values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_genlock_crosslock );
    CHECK_NOT_NULL( values );
    return ( CHAIN_DRV(protocol->drv)->get_genlock_crosslock( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_genlock_crosslock
 *****************************************************************************/
int ctrl_protocol_set_genlock_crosslock
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const               values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_genlock_crosslock );
    CHECK_NOT_NULL( values );
    return ( CHAIN_DRV(protocol->drv)->set_genlock_crosslock( protocol->ctx, channel, no, values ) );
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
 * ctrl_protocol_get_genlock_loss_of_signal
 *****************************************************************************/
int ctrl_protocol_get_genlock_loss_of_signal
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
int ctrl_protocol_set_genlock_loss_of_signal
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                flag
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_genlock_loss_of_signal );
    return ( CHAIN_DRV(protocol->drv)->set_genlock_loss_of_signal( protocol->ctx, channel, flag ) );
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
 * ctrl_protocol_get_timecode_hold
 *****************************************************************************/
int ctrl_protocol_get_timecode_hold
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_timecode_hold );
    CHECK_NOT_NULL( enable );
    return ( CHAIN_DRV(protocol->drv)->get_timecode_hold( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_set_timecode_hold
 *****************************************************************************/
int ctrl_protocol_set_timecode_hold
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_timecode_hold );
    return ( CHAIN_DRV(protocol->drv)->set_timecode_hold( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_get_audio_enable
 *****************************************************************************/
int ctrl_protocol_get_audio_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_audio_enable );
    CHECK_NOT_NULL( enable );
    return ( CHAIN_DRV(protocol->drv)->get_audio_enable( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_set_audio_enable
 *****************************************************************************/
int ctrl_protocol_set_audio_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_audio_enable );
    return ( CHAIN_DRV(protocol->drv)->set_audio_enable( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_get_audio_gain
 *****************************************************************************/
int ctrl_protocol_get_audio_gain
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint16_t * const             gain
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), get_audio_gain );
    CHECK_NOT_NULL( gain );
    return ( CHAIN_DRV(protocol->drv)->get_audio_gain( protocol->ctx, channel, gain ) );
}

/******************************************************************************
 * ctrl_protocol_set_audio_gain
 *****************************************************************************/
int ctrl_protocol_set_audio_gain
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint16_t const               gain
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CHAIN_DRV(protocol->drv), set_audio_gain );
    return ( CHAIN_DRV(protocol->drv)->set_audio_gain( protocol->ctx, channel, gain ) );
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

