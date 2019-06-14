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
 * @file    ctrl_protocol_gamma.c
 *
 * @brief   Implementation of generic gamma functions
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
#define LUT_DRV( drv )      ((ctrl_protocol_lut_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_lut_enable
 *****************************************************************************/
int ctrl_protocol_get_lut_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_lut_enable );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( LUT_DRV(protocol->drv)->get_lut_enable( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_enable
 *****************************************************************************/
int ctrl_protocol_set_lut_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_enable );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( LUT_DRV(protocol->drv)->set_lut_enable( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_lut_mode
 *****************************************************************************/
int ctrl_protocol_get_lut_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_lut_mode );
    CHECK_NOT_NULL( mode );
    return ( LUT_DRV(protocol->drv)->get_lut_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_mode
 *****************************************************************************/
int ctrl_protocol_set_lut_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_mode );
    return ( LUT_DRV(protocol->drv)->set_lut_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_lut_mode
 *****************************************************************************/
int ctrl_protocol_get_lut_fixed_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_lut_fixed_mode );
    CHECK_NOT_NULL( mode );
    return ( LUT_DRV(protocol->drv)->get_lut_fixed_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_mode
 *****************************************************************************/
int ctrl_protocol_set_lut_fixed_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_fixed_mode );
    return ( LUT_DRV(protocol->drv)->set_lut_fixed_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_lut_preset
 *****************************************************************************/
int ctrl_protocol_get_lut_preset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              value
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_lut_preset );
    CHECK_NOT_NULL( value );
    return ( LUT_DRV(protocol->drv)->get_lut_preset( protocol->ctx, channel, value ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_preset
 *****************************************************************************/
int ctrl_protocol_set_lut_preset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                value
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_preset );
    return ( LUT_DRV(protocol->drv)->set_lut_preset( protocol->ctx, channel, value ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_write_index
 *****************************************************************************/
int ctrl_protocol_set_lut_write_index
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               index 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_write_index );
    return ( LUT_DRV(protocol->drv)->set_lut_write_index( protocol->ctx, channel, index ) );
}

/******************************************************************************
 * ctrl_protocol_get_lut_write_index_red
 *****************************************************************************/
int ctrl_protocol_get_lut_write_index_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             index
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_lut_write_index_red );
    CHECK_NOT_NULL( index );
    return ( LUT_DRV(protocol->drv)->get_lut_write_index_red( protocol->ctx, channel, index ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_write_index_red
 *****************************************************************************/
int ctrl_protocol_set_lut_write_index_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               index 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_write_index_red );
    return ( LUT_DRV(protocol->drv)->set_lut_write_index_red( protocol->ctx, channel, index ) );
}

/******************************************************************************
 * ctrl_protocol_get_lut_write_index_green
 *****************************************************************************/
int ctrl_protocol_get_lut_write_index_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             index
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_lut_write_index_green );
    CHECK_NOT_NULL( index );
    return ( LUT_DRV(protocol->drv)->get_lut_write_index_green( protocol->ctx, channel, index ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_write_index_green
 *****************************************************************************/
int ctrl_protocol_set_lut_write_index_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               index 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_write_index_green );
    return ( LUT_DRV(protocol->drv)->set_lut_write_index_green( protocol->ctx, channel, index ) );
}

/******************************************************************************
 * ctrl_protocol_get_lut_write_index_blue
 *****************************************************************************/
int ctrl_protocol_get_lut_write_index_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             index
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_lut_write_index_blue );
    CHECK_NOT_NULL( index );
    return ( LUT_DRV(protocol->drv)->get_lut_write_index_blue( protocol->ctx, channel, index ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_write_index_blue
 *****************************************************************************/
int ctrl_protocol_set_lut_write_index_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               index 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_write_index_blue );
    return ( LUT_DRV(protocol->drv)->set_lut_write_index_blue( protocol->ctx, channel, index ) );
}

/******************************************************************************
 * ctrl_protocol_get_lut_read_red
 *****************************************************************************/
int ctrl_protocol_get_lut_read_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint16_t * const             values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_lut_read_red );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( LUT_DRV(protocol->drv)->get_lut_read_red( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_lut_read_green
 *****************************************************************************/
int ctrl_protocol_get_lut_read_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint16_t * const             values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_lut_read_green );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( LUT_DRV(protocol->drv)->get_lut_read_green( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_lut_read_blue
 *****************************************************************************/
int ctrl_protocol_get_lut_read_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint16_t * const             values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_lut_read_blue );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( LUT_DRV(protocol->drv)->get_lut_read_blue( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_reset
 *****************************************************************************/
int ctrl_protocol_set_lut_reset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_reset );
    return ( LUT_DRV(protocol->drv)->set_lut_reset( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_reset_red
 *****************************************************************************/
int ctrl_protocol_set_lut_reset_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_reset_red );
    return ( LUT_DRV(protocol->drv)->set_lut_reset_red( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_reset_green
 *****************************************************************************/
int ctrl_protocol_set_lut_reset_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_reset_green );
    return ( LUT_DRV(protocol->drv)->set_lut_reset_green( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_reset_blue
 *****************************************************************************/
int ctrl_protocol_set_lut_reset_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_reset_blue );
    return ( LUT_DRV(protocol->drv)->set_lut_reset_blue( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_reset_master
 *****************************************************************************/
int ctrl_protocol_set_lut_reset_master
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_reset_master );
    return ( LUT_DRV(protocol->drv)->set_lut_reset_master( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_sample
 *****************************************************************************/
int ctrl_protocol_set_lut_sample
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_sample );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( LUT_DRV(protocol->drv)->set_lut_sample( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_lut_sample_red
 *****************************************************************************/
int ctrl_protocol_get_lut_sample_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_lut_sample_red );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( LUT_DRV(protocol->drv)->get_lut_sample_red( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_sample_red
 *****************************************************************************/
int ctrl_protocol_set_lut_sample_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_sample_red );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( LUT_DRV(protocol->drv)->set_lut_sample_red( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_lut_sample_green
 *****************************************************************************/
int ctrl_protocol_get_lut_sample_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_lut_sample_green );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( LUT_DRV(protocol->drv)->get_lut_sample_green( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_sample_green
 *****************************************************************************/
int ctrl_protocol_set_lut_sample_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_sample_green );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( LUT_DRV(protocol->drv)->set_lut_sample_green( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_lut_sample_blue
 *****************************************************************************/
int ctrl_protocol_get_lut_sample_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_lut_sample_blue );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( LUT_DRV(protocol->drv)->get_lut_sample_blue( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_sample_blue
 *****************************************************************************/
int ctrl_protocol_set_lut_sample_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_sample_blue );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( LUT_DRV(protocol->drv)->set_lut_sample_blue( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_lut_sample_master
 *****************************************************************************/
int ctrl_protocol_get_lut_sample_master
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_lut_sample_master );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( LUT_DRV(protocol->drv)->get_lut_sample_master( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_sample_master
 *****************************************************************************/
int ctrl_protocol_set_lut_sample_master
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_sample_master );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( LUT_DRV(protocol->drv)->set_lut_sample_master( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_rec709
 *****************************************************************************/
int ctrl_protocol_set_lut_rec709
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_rec709 );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( LUT_DRV(protocol->drv)->set_lut_rec709( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_interpolate
 *****************************************************************************/
int ctrl_protocol_set_lut_interpolate
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_interpolate );
    return ( LUT_DRV(protocol->drv)->set_lut_interpolate( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_interpolate_red
 *****************************************************************************/
int ctrl_protocol_set_lut_interpolate_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_interpolate_red );
    return ( LUT_DRV(protocol->drv)->set_lut_interpolate_red( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_interpolate_green
 *****************************************************************************/
int ctrl_protocol_set_lut_interpolate_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_interpolate_green );
    return ( LUT_DRV(protocol->drv)->set_lut_interpolate_green( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_interpolate_blue
 *****************************************************************************/
int ctrl_protocol_set_lut_interpolate_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_interpolate_blue );
    return ( LUT_DRV(protocol->drv)->set_lut_interpolate_blue( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_get_lut_fast_gamma
 *****************************************************************************/
int ctrl_protocol_get_lut_fast_gamma
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int16_t * const              gamma
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_lut_fast_gamma );
    CHECK_NOT_NULL( gamma );
    return ( LUT_DRV(protocol->drv)->get_lut_fast_gamma( protocol->ctx, channel, gamma ) );
}

/******************************************************************************
 * ctrl_protocol_set_lut_fast_gamma
 *****************************************************************************/
int ctrl_protocol_set_lut_fast_gamma
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int16_t const                gamma
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_lut_fast_gamma );
    return ( LUT_DRV(protocol->drv)->set_lut_fast_gamma( protocol->ctx, channel, gamma ) );
}

/******************************************************************************
 * ctrl_protocol_get_log_mode
 *****************************************************************************/
int ctrl_protocol_get_log_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_log_mode );
    CHECK_NOT_NULL( mode );
    return ( LUT_DRV(protocol->drv)->get_log_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_log_mode
 *****************************************************************************/
int ctrl_protocol_set_log_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_log_mode );
    return ( LUT_DRV(protocol->drv)->set_log_mode( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_pq_max_brightness
 *****************************************************************************/
int ctrl_protocol_get_pq_max_brightness
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              percent
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), get_pq_max_brightness );
    CHECK_NOT_NULL( percent );
    return ( LUT_DRV(protocol->drv)->get_pq_max_brightness( protocol->ctx, channel, percent ) );
}

/******************************************************************************
 * ctrl_protocol_set_pq_max_brightness
 *****************************************************************************/
int ctrl_protocol_set_pq_max_brightness
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                percent
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LUT_DRV(protocol->drv), set_pq_max_brightness );
    return ( LUT_DRV(protocol->drv)->set_pq_max_brightness( protocol->ctx, channel, percent ) );
}

/******************************************************************************
 * ctrl_protocol_lut_register
 *****************************************************************************/
int ctrl_protocol_lut_register
(
    ctrl_protocol_handle_t const    handle,
    void * const                    ctx,
    ctrl_protocol_lut_drv_t * const drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = (void *)drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_lut_unregister
 *****************************************************************************/
int ctrl_protocol_lut_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}

