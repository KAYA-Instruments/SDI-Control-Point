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
 * @file    ctrl_protocol_auto.c
 *
 * @brief   Implementation of generic auto functions
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
#define CAM_DRV( drv )      ((ctrl_protocol_cam_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_cam_info
 *****************************************************************************/
int ctrl_protocol_get_cam_info
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CAM_DRV(protocol->drv), get_cam_info );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( CAM_DRV(protocol->drv)->get_cam_info( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_cam_gain
 *****************************************************************************/
int ctrl_protocol_get_cam_gain
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             gain
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CAM_DRV(protocol->drv), get_cam_gain );
    CHECK_NOT_NULL( gain );
    return ( CAM_DRV(protocol->drv)->get_cam_gain( protocol->ctx, channel, gain ) );
}

/******************************************************************************
 * ctrl_protocol_set_cam_gain
 *****************************************************************************/
int ctrl_protocol_set_cam_gain
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               gain
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CAM_DRV(protocol->drv), set_cam_gain );
    return ( CAM_DRV(protocol->drv)->set_cam_gain( protocol->ctx, channel, gain ) );
}

/******************************************************************************
 * ctrl_protocol_get_cam_exposure
 *****************************************************************************/
int ctrl_protocol_get_cam_exposure
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             exposure
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CAM_DRV(protocol->drv), get_cam_exposure );
    CHECK_NOT_NULL( exposure );
    return ( CAM_DRV(protocol->drv)->get_cam_exposure( protocol->ctx, channel, exposure ) );
}

/******************************************************************************
 * ctrl_protocol_set_cam_exposure
 *****************************************************************************/
int ctrl_protocol_set_cam_exposure
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               exposure
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( CAM_DRV(protocol->drv), set_cam_exposure );
    return ( CAM_DRV(protocol->drv)->set_cam_exposure( protocol->ctx, channel, exposure ) );
}

/******************************************************************************
 * ctrl_protocol_cam_register
 *****************************************************************************/
int ctrl_protocol_cam_register
(
    ctrl_protocol_handle_t const    handle,
    void * const                    ctx,
    ctrl_protocol_cam_drv_t * const drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = (void *)drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_cam_unregister
 *****************************************************************************/
int ctrl_protocol_cam_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}

