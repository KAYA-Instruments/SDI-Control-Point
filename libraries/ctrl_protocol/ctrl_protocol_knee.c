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

