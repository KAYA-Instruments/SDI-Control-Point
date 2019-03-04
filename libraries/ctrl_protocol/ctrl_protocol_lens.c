/******************************************************************************
 * Copyright (C) 2019 Dream Chip Technologies GmbH
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
 * @file    ctrl_protocol_lens.c
 *
 * @brief   Implementation of lens control functions
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
#define LENS_DRV( drv )     ((ctrl_protocol_lens_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_lens_settings
 *****************************************************************************/
int ctrl_protocol_get_lens_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no, 
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), get_lens_settings );
    CHECK_NOT_NULL( values );
    return ( LENS_DRV(protocol->drv)->get_lens_settings( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_lens_settings
 *****************************************************************************/
int ctrl_protocol_set_lens_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no, 
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), set_lens_settings );
    return ( LENS_DRV(protocol->drv)->set_lens_settings( protocol->ctx, channel, no, values ) );
}


/******************************************************************************
 * ctrl_protocol_get_lens_active
 *****************************************************************************/
int ctrl_protocol_get_lens_active
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int32_t * const              act
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), get_lens_active );
    CHECK_NOT_NULL( act );
    return ( LENS_DRV(protocol->drv)->get_lens_active( protocol->ctx, channel, act ) );
}

/******************************************************************************
 * ctrl_protocol_set_lens_active
 *****************************************************************************/
int ctrl_protocol_set_lens_active
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int32_t * const              act
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), set_lens_active );
    return ( LENS_DRV(protocol->drv)->set_lens_active( protocol->ctx, channel, act ) );
}


/******************************************************************************
 * ctrl_protocol_get_lens_focus_position
 *****************************************************************************/
int ctrl_protocol_get_lens_focus_position
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int32_t * const              pos
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), get_lens_focus_position );
    CHECK_NOT_NULL( pos );
    return ( LENS_DRV(protocol->drv)->get_lens_focus_position( protocol->ctx, channel, pos ) );
}

/******************************************************************************
 * ctrl_protocol_set_lens_focus_position
 *****************************************************************************/
int ctrl_protocol_set_lens_focus_position
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int32_t * const              pos
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), set_lens_focus_position );
    return ( LENS_DRV(protocol->drv)->set_lens_focus_position( protocol->ctx, channel, pos ) );
}

/******************************************************************************
 * ctrl_protocol_get_lens_focus_settings
 *****************************************************************************/
int ctrl_protocol_get_lens_focus_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), get_lens_focus_settings );
    CHECK_NOT_NULL( values );
    return ( LENS_DRV(protocol->drv)->get_lens_focus_settings( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_lens_focus_settings
 *****************************************************************************/
int ctrl_protocol_set_lens_focus_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), set_lens_focus_settings );
    return ( LENS_DRV(protocol->drv)->set_lens_focus_settings( protocol->ctx, channel, no, values ) );
}



/******************************************************************************
 * ctrl_protocol_get_lens_zoom_position
 *****************************************************************************/
int ctrl_protocol_get_lens_zoom_position
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int32_t * const              pos
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), get_lens_zoom_position );
    CHECK_NOT_NULL( pos );
    return ( LENS_DRV(protocol->drv)->get_lens_zoom_position( protocol->ctx, channel, pos ) );
}

/******************************************************************************
 * ctrl_protocol_set_lens_zoom_position
 *****************************************************************************/
int ctrl_protocol_set_lens_zoom_position
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int32_t * const              pos
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), set_lens_zoom_position );
    return ( LENS_DRV(protocol->drv)->set_lens_zoom_position( protocol->ctx, channel, pos ) );
}

/******************************************************************************
 * ctrl_protocol_get_lens_zoom_settings
 *****************************************************************************/
int ctrl_protocol_get_lens_zoom_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), get_lens_zoom_settings );
    CHECK_NOT_NULL( values );
    return ( LENS_DRV(protocol->drv)->get_lens_zoom_settings( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_lens_zoom_settings
 *****************************************************************************/
int ctrl_protocol_set_lens_zoom_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), set_lens_zoom_settings );
    return ( LENS_DRV(protocol->drv)->set_lens_zoom_settings( protocol->ctx, channel, no, values ) );
}


/******************************************************************************
 * ctrl_protocol_get_lens_iris_position
 *****************************************************************************/
int ctrl_protocol_get_lens_iris_position
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int32_t * const              pos
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), get_lens_iris_position );
    CHECK_NOT_NULL( pos );
    return ( LENS_DRV(protocol->drv)->get_lens_iris_position( protocol->ctx, channel, pos ) );
}

/******************************************************************************
 * ctrl_protocol_set_lens_iris_position
 *****************************************************************************/
int ctrl_protocol_set_lens_iris_position
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int32_t * const              pos
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), set_lens_iris_position );
    return ( LENS_DRV(protocol->drv)->set_lens_iris_position( protocol->ctx, channel, pos ) );
}

/******************************************************************************
 * ctrl_protocol_get_lens_iris_settings
 *****************************************************************************/
int ctrl_protocol_get_lens_iris_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), get_lens_iris_settings );
    CHECK_NOT_NULL( values );
    return ( LENS_DRV(protocol->drv)->get_lens_iris_settings( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_lens_iris_settings
 *****************************************************************************/
int ctrl_protocol_set_lens_iris_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), set_lens_iris_settings );
    return ( LENS_DRV(protocol->drv)->set_lens_iris_settings( protocol->ctx, channel, no, values ) );
}


/******************************************************************************
 * ctrl_protocol_get_lens_filter_position
 *****************************************************************************/
int ctrl_protocol_get_lens_filter_position
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int32_t * const              pos
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), get_lens_filter_position );
    CHECK_NOT_NULL( pos );
    return ( LENS_DRV(protocol->drv)->get_lens_filter_position( protocol->ctx, channel, pos ) );
}

/******************************************************************************
 * ctrl_protocol_set_lens_filter_position
 *****************************************************************************/
int ctrl_protocol_set_lens_filter_position
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int32_t * const              pos
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), set_lens_filter_position );
    return ( LENS_DRV(protocol->drv)->set_lens_filter_position( protocol->ctx, channel, pos ) );
}

/******************************************************************************
 * ctrl_protocol_get_lens_filter_settings
 *****************************************************************************/
int ctrl_protocol_get_lens_filter_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), get_lens_filter_settings );
    CHECK_NOT_NULL( values );
    return ( LENS_DRV(protocol->drv)->get_lens_filter_settings( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_lens_filter_settings
 *****************************************************************************/
int ctrl_protocol_set_lens_filter_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( LENS_DRV(protocol->drv), set_lens_filter_settings );
    return ( LENS_DRV(protocol->drv)->set_lens_filter_settings( protocol->ctx, channel, no, values ) );
}





/******************************************************************************
 * ctrl_protocol_lens_register
 *****************************************************************************/
int ctrl_protocol_lens_register
(
    ctrl_protocol_handle_t const     handle,
    void * const                     ctx,
    ctrl_protocol_lens_drv_t * const drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = (void *)drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_lens_unregister
 *****************************************************************************/
int ctrl_protocol_lens_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}
