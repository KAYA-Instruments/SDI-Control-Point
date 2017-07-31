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
 * @file    ctrl_protocol_osd.c
 *
 * @brief   Implementation of generic osd functions
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
#define OSD_DRV( drv )      ((ctrl_protocol_osd_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_test_pattern
 *****************************************************************************/
int ctrl_protocol_get_test_pattern
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( OSD_DRV(protocol->drv), get_test_pattern );
    CHECK_NOT_NULL( enable );
    return ( OSD_DRV(protocol->drv)->get_test_pattern( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_set_test_pattern
 *****************************************************************************/
int ctrl_protocol_set_test_pattern
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( OSD_DRV(protocol->drv), set_test_pattern );
    return ( OSD_DRV(protocol->drv)->set_test_pattern( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_get_center_marker
 *****************************************************************************/
int ctrl_protocol_get_center_marker
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( OSD_DRV(protocol->drv), get_center_marker );
    CHECK_NOT_NULL( mode );
    return ( OSD_DRV(protocol->drv)->get_center_marker( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_set_center_marker
 *****************************************************************************/
int ctrl_protocol_set_center_marker
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( OSD_DRV(protocol->drv), set_center_marker );
    return ( OSD_DRV(protocol->drv)->set_center_marker( protocol->ctx, channel, mode ) );
}

/******************************************************************************
 * ctrl_protocol_get_logo
 *****************************************************************************/
int ctrl_protocol_get_logo
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              id 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( OSD_DRV(protocol->drv), get_logo );
    CHECK_NOT_NULL( id );
    return ( OSD_DRV(protocol->drv)->get_logo( protocol->ctx, channel, id ) );
}

/******************************************************************************
 * ctrl_protocol_set_logo
 *****************************************************************************/
int ctrl_protocol_set_logo
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                id 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( OSD_DRV(protocol->drv), set_logo );
    return ( OSD_DRV(protocol->drv)->set_logo( protocol->ctx, channel, id ) );
}

/******************************************************************************
 * ctrl_protocol_get_zebra
 *****************************************************************************/
int ctrl_protocol_get_zebra
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              buf 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( OSD_DRV(protocol->drv), get_zebra );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( buf );
    return ( OSD_DRV(protocol->drv)->get_zebra( protocol->ctx, channel, no, buf ) );
}

/******************************************************************************
 * ctrl_protocol_set_zebra
 *****************************************************************************/
int ctrl_protocol_set_zebra
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              buf 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( OSD_DRV(protocol->drv), set_zebra );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( buf );
    return ( OSD_DRV(protocol->drv)->set_zebra( protocol->ctx, channel, no, buf ) );
}

/******************************************************************************
 * ctrl_protocol_osd_register
 *****************************************************************************/
int ctrl_protocol_osd_register
(
    ctrl_protocol_handle_t const    handle,
    void * const                    ctx,
    ctrl_protocol_osd_drv_t * const drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = (void *)drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_osd_unregister
 *****************************************************************************/
int ctrl_protocol_osd_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}

