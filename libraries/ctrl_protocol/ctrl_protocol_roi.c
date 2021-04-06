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
 * @file    ctrl_protocol_roi.c
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
#define ROI_DRV( drv )      ((ctrl_protocol_roi_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_stat_roi_info
 *****************************************************************************/

int ctrl_protocol_get_stat_roi_info
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ROI_DRV(protocol->drv), get_stat_roi_info );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( ROI_DRV(protocol->drv)->get_stat_roi_info( protocol->ctx, channel, no, values ) );

}

/******************************************************************************
 * ctrl_protocol_get_stat_roi
 *****************************************************************************/

int ctrl_protocol_get_stat_roi
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( ROI_DRV(protocol->drv), get_stat_roi );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( ROI_DRV(protocol->drv)->get_stat_roi( protocol->ctx, channel, no, values ) );

}

/******************************************************************************
 * ctrl_protocol_set_stat_roi
 *****************************************************************************/

int ctrl_protocol_set_stat_roi
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint32_t * const             values
)
{
    CHECK_DRV_FUNC( ROI_DRV(protocol->drv), set_stat_roi );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( ROI_DRV(protocol->drv)->set_stat_roi( protocol->ctx, channel, no, values ) );

}


/******************************************************************************
 * ctrl_protocol_roi_register
 *****************************************************************************/
int ctrl_protocol_roi_register
(
    ctrl_protocol_handle_t const        handle,
    void * const                        ctx,
    ctrl_protocol_roi_drv_t * const     drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = (void *)drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_roi_unregister
 *****************************************************************************/
int ctrl_protocol_roi_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}

