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
 * @file    ctrl_protocol_isp.c
 *
 * @brief   Implementation of generic isp functions
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
#define ISP_DRV( drv )      ((ctrl_protocol_isp_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_lsc
 *****************************************************************************/
int ctrl_protocol_get_lsc
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint32_t * const             values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_lsc );
    CHECK_NOT_NULL( values );
    return ( ISP_DRV(protocol->drv)->get_lsc( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_lsc
 *****************************************************************************/
int ctrl_protocol_set_lsc
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint32_t * const             values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_lsc );
    return ( ISP_DRV(protocol->drv)->set_lsc( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_bayer
 *****************************************************************************/
int ctrl_protocol_get_bayer
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              pattern
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_bayer );
    CHECK_NOT_NULL( pattern );
    return ( ISP_DRV(protocol->drv)->get_bayer( protocol->ctx, channel, pattern ) );
}

/******************************************************************************
 * ctrl_protocol_set_bayer
 *****************************************************************************/
int ctrl_protocol_set_bayer
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                pattern
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_bayer );
    return ( ISP_DRV(protocol->drv)->set_bayer( protocol->ctx, channel, pattern ) );
}

/******************************************************************************
 * ctrl_protocol_get_gain_red
 *****************************************************************************/
int ctrl_protocol_get_gain_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             gain
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_gain_red );
    CHECK_NOT_NULL( gain );
    return ( ISP_DRV(protocol->drv)->get_gain_red( protocol->ctx, channel, gain ) );
}

/******************************************************************************
 * ctrl_protocol_set_gain_red
 *****************************************************************************/
int ctrl_protocol_set_gain_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint16_t const               gain
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_gain_red );
    return ( ISP_DRV(protocol->drv)->set_gain_red( protocol->ctx, channel, gain ) );
}

/******************************************************************************
 * ctrl_protocol_get_gain_green
 *****************************************************************************/
int ctrl_protocol_get_gain_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             gain
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_gain_green );
    CHECK_NOT_NULL( gain );
    return ( ISP_DRV(protocol->drv)->get_gain_green( protocol->ctx, channel, gain ) );
}

/******************************************************************************
 * ctrl_protocol_set_gain_green
 *****************************************************************************/
int ctrl_protocol_set_gain_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               gain
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_gain_green );
    return ( ISP_DRV(protocol->drv)->set_gain_green( protocol->ctx, channel, gain ) );
}

/******************************************************************************
 * ctrl_protocol_get_gain_blue
 *****************************************************************************/
int ctrl_protocol_get_gain_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             gain
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_gain_blue );
    CHECK_NOT_NULL( gain );
    return ( ISP_DRV(protocol->drv)->get_gain_blue( protocol->ctx, channel, gain ) );
}

/******************************************************************************
 * ctrl_protocol_set_gain_blue
 *****************************************************************************/
int ctrl_protocol_set_gain_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               gain
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_gain_blue );
    return ( ISP_DRV(protocol->drv)->set_gain_blue( protocol->ctx, channel, gain ) );
}

/******************************************************************************
 * ctrl_protocol_get_black_red
 *****************************************************************************/
int ctrl_protocol_get_black_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t * const              offset
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_black_red );
    CHECK_NOT_NULL( offset );
    return ( ISP_DRV(protocol->drv)->get_black_red( protocol->ctx, channel, offset ) );
}

/******************************************************************************
 * ctrl_protocol_set_black_red
 *****************************************************************************/
int ctrl_protocol_set_black_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t const                offset
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_black_red );
    return ( ISP_DRV(protocol->drv)->set_black_red( protocol->ctx, channel, offset ) );
}

/******************************************************************************
 * ctrl_protocol_get_black_green
 *****************************************************************************/
int ctrl_protocol_get_black_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t * const              offset
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_black_green );
    CHECK_NOT_NULL( offset );
    return ( ISP_DRV(protocol->drv)->get_black_green( protocol->ctx, channel, offset ) );
}

/******************************************************************************
 * ctrl_protocol_set_black_green
 *****************************************************************************/
int ctrl_protocol_set_black_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t const                offset
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_black_green );
    return ( ISP_DRV(protocol->drv)->set_black_green( protocol->ctx, channel, offset ) );
}

/******************************************************************************
 * ctrl_protocol_get_black_blue
 *****************************************************************************/
int ctrl_protocol_get_black_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t * const              offset
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_black_blue );
    CHECK_NOT_NULL( offset );
    return ( ISP_DRV(protocol->drv)->get_black_blue( protocol->ctx, channel, offset ) );
}

/******************************************************************************
 * ctrl_protocol_set_black_blue
 *****************************************************************************/
int ctrl_protocol_set_black_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t const                offset
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_black_blue );
    return ( ISP_DRV(protocol->drv)->set_black_blue( protocol->ctx, channel, offset ) );
}

/******************************************************************************
 * ctrl_protocol_get_flare
 *****************************************************************************/
int ctrl_protocol_get_flare
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint16_t * const             values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_flare );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( ISP_DRV(protocol->drv)->get_flare( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_flare
 *****************************************************************************/
int ctrl_protocol_set_flare
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint16_t * const             values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_flare );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( ISP_DRV(protocol->drv)->set_flare( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_black_master
 *****************************************************************************/
int ctrl_protocol_get_black_master
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int16_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_black_master );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( ISP_DRV(protocol->drv)->get_black_master( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_black_master
 *****************************************************************************/
int ctrl_protocol_set_black_master
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int16_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_black_master );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( ISP_DRV(protocol->drv)->set_black_master( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_filter_enable
 *****************************************************************************/
int ctrl_protocol_get_filter_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              flag
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_filter_enable );
    CHECK_NOT_NULL( flag );
    return ( ISP_DRV(protocol->drv)->get_filter_enable( protocol->ctx, channel, flag ) );
}

/******************************************************************************
 * ctrl_protocol_set_filter_enable
 *****************************************************************************/
int ctrl_protocol_set_filter_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                flag
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_filter_enable );
    return ( ISP_DRV(protocol->drv)->set_filter_enable( protocol->ctx, channel, flag ) );
}

/******************************************************************************
 * ctrl_protocol_get_filter_detail
 *****************************************************************************/
int ctrl_protocol_get_filter_detail
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              level
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_filter_detail );
    CHECK_NOT_NULL( level );
    return ( ISP_DRV(protocol->drv)->get_filter_detail( protocol->ctx, channel, level ) );
}

/******************************************************************************
 * ctrl_protocol_set_filter_detail
 *****************************************************************************/
int ctrl_protocol_set_filter_detail
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                level
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_filter_detail );
    return ( ISP_DRV(protocol->drv)->set_filter_detail( protocol->ctx, channel, level ) );
}

/******************************************************************************
 * ctrl_protocol_get_filter_denoise
 *****************************************************************************/
int ctrl_protocol_get_filter_denoise
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              level
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_filter_denoise );
    CHECK_NOT_NULL( level );
    return ( ISP_DRV(protocol->drv)->get_filter_denoise( protocol->ctx, channel, level ) );
}

/******************************************************************************
 * ctrl_protocol_set_filter_denoise
 *****************************************************************************/
int ctrl_protocol_set_filter_denoise
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                level
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_filter_denoise );
    return ( ISP_DRV(protocol->drv)->set_filter_denoise( protocol->ctx, channel, level ) );
}

/******************************************************************************
 * ctrl_protocol_get_color_conv
 *****************************************************************************/
int ctrl_protocol_get_color_conv
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int16_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_color_conv );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( ISP_DRV(protocol->drv)->get_color_conv( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_color_conv
 *****************************************************************************/
int ctrl_protocol_set_color_conv
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int16_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_color_conv );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( ISP_DRV(protocol->drv)->set_color_conv( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_color_cross
 *****************************************************************************/
int ctrl_protocol_get_color_cross
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_color_cross );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( ISP_DRV(protocol->drv)->get_color_cross( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_color_cross
 *****************************************************************************/
int ctrl_protocol_set_color_cross
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_color_cross );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( ISP_DRV(protocol->drv)->set_color_cross( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_color_cross_offset
 *****************************************************************************/
int ctrl_protocol_get_color_cross_offset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int16_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_color_cross_offset );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( ISP_DRV(protocol->drv)->get_color_cross_offset( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_color_cross_offset
 *****************************************************************************/
int ctrl_protocol_set_color_cross_offset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int16_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_color_cross_offset );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( ISP_DRV(protocol->drv)->set_color_cross_offset( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_color_space
 *****************************************************************************/
int ctrl_protocol_get_color_space
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_color_space );
    CHECK_NOT_NULL( mode );
    return ( ISP_DRV(protocol->drv)->get_color_space( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_color_space
 *****************************************************************************/
int ctrl_protocol_set_color_space
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_color_space );
    return ( ISP_DRV(protocol->drv)->set_color_space( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_split_screen
 *****************************************************************************/
int ctrl_protocol_get_split_screen
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), get_split_screen );
    CHECK_NOT_NULL( enable );
    return ( ISP_DRV(protocol->drv)->get_split_screen( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_set_split_screen
 *****************************************************************************/
int ctrl_protocol_set_split_screen
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ISP_DRV(protocol->drv), set_split_screen );
    return ( ISP_DRV(protocol->drv)->set_split_screen( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_isp_register
 *****************************************************************************/
int ctrl_protocol_isp_register
(
    ctrl_protocol_handle_t const    handle,
    void * const                    ctx,
    ctrl_protocol_isp_drv_t * const drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = (void *)drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_isp_unregister
 *****************************************************************************/
int ctrl_protocol_isp_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}

