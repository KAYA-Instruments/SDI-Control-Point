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
#define AUTO_DRV( drv )     ((ctrl_protocol_auto_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_aec_enable
 *****************************************************************************/
int ctrl_protocol_get_aec_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              enable 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), get_aec_enable );
    CHECK_NOT_NULL( enable );
    return ( AUTO_DRV(protocol->drv)->get_aec_enable( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_set_aec_enable
 *****************************************************************************/
int ctrl_protocol_set_aec_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), set_aec_enable );
    return ( AUTO_DRV(protocol->drv)->set_aec_enable( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_get_aec_setup
 *****************************************************************************/
int ctrl_protocol_get_aec_setup
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no, 
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), get_aec_setup );
    CHECK_NOT_NULL( values );
    return ( AUTO_DRV(protocol->drv)->get_aec_setup( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_aec_setup
 *****************************************************************************/
int ctrl_protocol_set_aec_setup
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no, 
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), set_aec_setup );
    return ( AUTO_DRV(protocol->drv)->set_aec_setup( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_awb_enable
 *****************************************************************************/
int ctrl_protocol_get_awb_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              enable 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), get_awb_enable );
    CHECK_NOT_NULL( enable );
    return ( AUTO_DRV(protocol->drv)->get_awb_enable( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_set_awb_enable
 *****************************************************************************/
int ctrl_protocol_set_awb_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), set_awb_enable );
    return ( AUTO_DRV(protocol->drv)->set_awb_enable( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_get_awb_speed
 *****************************************************************************/
int ctrl_protocol_get_awb_speed
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              speed 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), get_awb_speed );
    CHECK_NOT_NULL( speed );
    return ( AUTO_DRV(protocol->drv)->get_awb_speed( protocol->ctx, channel, speed ) );
}

/******************************************************************************
 * ctrl_protocol_set_awb_speed
 *****************************************************************************/
int ctrl_protocol_set_awb_speed
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                speed
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), set_awb_speed );
    return ( AUTO_DRV(protocol->drv)->set_awb_speed( protocol->ctx, channel, speed ) );
}

/******************************************************************************
 * ctrl_protocol_run_wb
 *****************************************************************************/
int ctrl_protocol_run_wb
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), run_wb );
    return ( AUTO_DRV(protocol->drv)->run_wb( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_get_no_wbpresets
 *****************************************************************************/
int ctrl_protocol_get_no_wbpresets
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              no 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), get_no_wbpresets );
    CHECK_NOT_NULL( no );
    return ( AUTO_DRV(protocol->drv)->get_no_wbpresets( protocol->ctx, channel, no ) );
}

/******************************************************************************
 * ctrl_protocol_get_wbpresets
 *****************************************************************************/
int ctrl_protocol_get_wbpresets
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no, 
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), get_wbpresets );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( AUTO_DRV(protocol->drv)->get_wbpresets( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_wbpreset
 *****************************************************************************/
int ctrl_protocol_set_wbpreset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                id
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), set_wbpreset );
    return ( AUTO_DRV(protocol->drv)->set_wbpreset( protocol->ctx, channel, id ) );
}

/******************************************************************************
 * ctrl_protocol_get_stat_rgb
 *****************************************************************************/
int ctrl_protocol_get_stat_rgb
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint16_t * const             values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), get_stat_rgb );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( AUTO_DRV(protocol->drv)->get_stat_rgb( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_stat_histogram_enable
 *****************************************************************************/
int ctrl_protocol_get_stat_histogram_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              flag
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), get_stat_histogram_enable );
    CHECK_NOT_NULL( flag );
    return ( AUTO_DRV(protocol->drv)->get_stat_histogram_enable( protocol->ctx, channel, flag ) );
}

/******************************************************************************
 * ctrl_protocol_set_stat_histogram_enable
 *****************************************************************************/
int ctrl_protocol_set_stat_histogram_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                flag
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), set_stat_histogram_enable );
    return ( AUTO_DRV(protocol->drv)->set_stat_histogram_enable( protocol->ctx, channel, flag ) );
}

/******************************************************************************
 * ctrl_protocol_get_stat_histogram
 *****************************************************************************/
int ctrl_protocol_get_stat_histogram
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint32_t * const             values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), get_stat_histogram );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( AUTO_DRV(protocol->drv)->get_stat_histogram( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_stat_exposure_enable
 *****************************************************************************/
int ctrl_protocol_get_stat_exposure_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              flag
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), get_stat_exposure_enable );
    CHECK_NOT_NULL( flag );
    return ( AUTO_DRV(protocol->drv)->get_stat_exposure_enable( protocol->ctx, channel, flag ) );
}

/******************************************************************************
 * ctrl_protocol_set_stat_exposure_enable
 *****************************************************************************/
int ctrl_protocol_set_stat_exposure_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                flag
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), set_stat_exposure_enable );
    return ( AUTO_DRV(protocol->drv)->set_stat_exposure_enable( protocol->ctx, channel, flag ) );
}

/******************************************************************************
 * ctrl_protocol_get_stat_exposure
 *****************************************************************************/
int ctrl_protocol_get_stat_exposure
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint16_t * const             values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), get_stat_exposure );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( AUTO_DRV(protocol->drv)->get_stat_exposure( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_stat_xyz
 *****************************************************************************/
int ctrl_protocol_get_stat_xyz
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), get_stat_xyz );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( AUTO_DRV(protocol->drv)->get_stat_xyz( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_color_xyz
 *****************************************************************************/
int ctrl_protocol_get_color_xyz
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( AUTO_DRV(protocol->drv), get_color_xyz );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( AUTO_DRV(protocol->drv)->get_color_xyz( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_auto_register
 *****************************************************************************/
int ctrl_protocol_auto_register
(
    ctrl_protocol_handle_t const     handle,
    void * const                     ctx,
    ctrl_protocol_auto_drv_t * const drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = (void *)drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_auto_unregister
 *****************************************************************************/
int ctrl_protocol_auto_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}

