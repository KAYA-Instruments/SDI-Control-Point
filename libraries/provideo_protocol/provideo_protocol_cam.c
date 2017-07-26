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
 * @file    provideo_protocol_cam.c
 *
 * @brief   Implementation of provideo protocol cam functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_cam.h>

#include "provideo_protocol_common.h"

/******************************************************************************
 * @brief command "cam_gain" 
 *****************************************************************************/
#define CMD_GET_CAM_GAIN                    ( "cam_gain\n" )
#define CMD_SET_CAM_GAIN                    ( "cam_gain %i\n" )
#define CMD_SYNC_CAM_GAIN                   ( "cam_gain " )
#define CMD_GET_CAM_GAIN_NO_PARMS           ( 1 )

/******************************************************************************
 * @brief command "cam_exposure" 
 *****************************************************************************/
#define CMD_GET_CAM_EXPOSURE                ( "cam_exposure\n" )
#define CMD_SET_CAM_EXPOSURE                ( "cam_exposure %i\n" )
#define CMD_SYNC_CAM_EXPOSURE               ( "cam_exposure " )
#define CMD_GET_CAM_EXPOSURE_NO_PARMS       ( 1 )

/******************************************************************************
 * @brief command definition to read out the camera info
 *****************************************************************************/
#define CMD_GET_CAM_INFO                    ( "cam_info\n" )
#define CMD_SET_CAM_INFO                    ( "cam_info %i %i %i %i\n" )
#define CMD_SYNC_CAM_INFO                   ( "cam_info " )
#define CMD_GET_CAM_INFO_NO_PARMS           ( 4 )

/**************************************************************************//**
 * get_cam_info - get camera information
 *****************************************************************************/
static int get_cam_info
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    ctrl_protocol_cam_info_t * v;

    int v0;
    int v1;
    int v2;
    int v3;

    int res;

    // parameter check
    if ( !no || !values )
    {
        return ( -EINVAL );
    }

    // command call to get 4 parameters from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_CAM_INFO, CMD_SYNC_CAM_INFO, CMD_SET_CAM_INFO, &v0, &v1, &v2, &v3 );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_CAM_INFO_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    v                    = (ctrl_protocol_cam_info_t *)values;
    v->min_gain          = UINT32( v0 );
    v->max_gain          = UINT32( v1 );
    v->min_exposure_time = UINT32( v2 );
    v->max_exposure_time = UINT32( v3 );

    return ( 0 );
}

/**************************************************************************//**
 * get_cam_gain - get camera analogue gain
 *****************************************************************************/
static int get_cam_gain
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t * const            gain
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !gain )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_CAM_GAIN, CMD_SYNC_CAM_GAIN, CMD_SET_CAM_GAIN, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_CAM_GAIN_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *gain = UINT16( value );

    return ( 0 );
}

/**************************************************************************//**
 * set_cam_gain - set camera analogue gain
 *****************************************************************************/
static int set_cam_gain
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t const              gain
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_CAM_GAIN, INT( gain ) ) );
}

/******************************************************************************
 * get_cam_exposure - get exposure time in microseconds of camera-device
 *****************************************************************************/
static int get_cam_exposure
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            time
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !time )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_CAM_EXPOSURE, CMD_SYNC_CAM_EXPOSURE, CMD_SET_CAM_EXPOSURE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_CAM_EXPOSURE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *time = UINT32( value );

    return ( 0 );
}

/******************************************************************************
 * set_cam_exposure - set exposure time in microseconds of camera-device
 *****************************************************************************/
static int set_cam_exposure
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t const              time
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_CAM_EXPOSURE, INT( time ) ) );
}

/******************************************************************************
 * CAM protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_cam_drv_t provideo_cam_drv = 
{
    .get_cam_info       = get_cam_info,
    .get_cam_gain       = get_cam_gain,
    .set_cam_gain       = set_cam_gain,
    .get_cam_exposure   = get_cam_exposure,
    .set_cam_exposure   = set_cam_exposure,
};

/******************************************************************************
 * provideo_protocol_cam_init
 *****************************************************************************/
int provideo_protocol_cam_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_cam_register( handle, ctx, &provideo_cam_drv ) );
}

