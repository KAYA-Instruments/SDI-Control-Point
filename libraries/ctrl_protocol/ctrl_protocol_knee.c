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
 * @file    ctrl_protocol_knee.c
 *
 * @brief   Implementation of generic knee functions
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
#define KNEE_DRV( drv )      ((ctrl_protocol_knee_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_knee_enable
 *****************************************************************************/
int ctrl_protocol_get_knee_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( KNEE_DRV(protocol->drv), get_knee_enable );
    CHECK_NOT_NULL( enable );
    return ( KNEE_DRV(protocol->drv)->get_knee_enable( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_set_knee_enable
 *****************************************************************************/
int ctrl_protocol_set_knee_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( KNEE_DRV(protocol->drv), set_knee_enable );
    return ( KNEE_DRV(protocol->drv)->set_knee_enable( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_get_knee_config
 *****************************************************************************/
int ctrl_protocol_get_knee_config
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( KNEE_DRV(protocol->drv), get_knee_config);
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( KNEE_DRV(protocol->drv)->get_knee_config( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_knee_config
 *****************************************************************************/
int ctrl_protocol_set_knee_config
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( KNEE_DRV(protocol->drv), set_knee_config );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( KNEE_DRV(protocol->drv)->set_knee_config( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_knee_register
 *****************************************************************************/
int ctrl_protocol_knee_register
(
    ctrl_protocol_handle_t const        handle,
    void * const                        ctx,
    ctrl_protocol_knee_drv_t * const    drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = (void *)drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_knee_unregister
 *****************************************************************************/
int ctrl_protocol_knee_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}

