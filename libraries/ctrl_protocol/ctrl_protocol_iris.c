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
 * @file    ctrl_protocol_iris.c
 *
 * @brief   Implementation of generic iris functions
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
#define IRIS_DRV( drv )     ((ctrl_protocol_iris_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_iris_setup
 *****************************************************************************/
int ctrl_protocol_get_iris_setup
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no, 
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( IRIS_DRV(protocol->drv), get_iris_setup );
    CHECK_NOT_NULL( values );
    return ( IRIS_DRV(protocol->drv)->get_iris_setup( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_iris_setup
 *****************************************************************************/
int ctrl_protocol_set_iris_setup
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no, 
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( IRIS_DRV(protocol->drv), set_iris_setup );
    return ( IRIS_DRV(protocol->drv)->set_iris_setup( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_iris_apt
 *****************************************************************************/
int ctrl_protocol_get_iris_apt
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int32_t * const              pos
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( IRIS_DRV(protocol->drv), get_iris_apt );
    CHECK_NOT_NULL( pos );
    return ( IRIS_DRV(protocol->drv)->get_iris_apt( protocol->ctx, channel, pos ) );
}

/******************************************************************************
 * ctrl_protocol_set_iris_setup
 *****************************************************************************/
int ctrl_protocol_set_iris_apt
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int32_t const                pos
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( IRIS_DRV(protocol->drv), set_iris_apt );
    return ( IRIS_DRV(protocol->drv)->set_iris_apt( protocol->ctx, channel, pos ) );
}



/******************************************************************************
 * ctrl_protocol_iris_register
 *****************************************************************************/
int ctrl_protocol_iris_register
(
    ctrl_protocol_handle_t const     handle,
    void * const                     ctx,
    ctrl_protocol_iris_drv_t * const drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = (void *)drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_iris_unregister
 *****************************************************************************/
int ctrl_protocol_iris_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}

