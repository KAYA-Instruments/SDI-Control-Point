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

#include <provideo_protocol/provideo_protocol_common.h>

/******************************************************************************
 * @brief command "cam_gain" 
 *****************************************************************************/
#define CMD_GET_CAM_GAIN                      ( "cam_gain\n" )
#define CMD_SET_CAM_GAIN                      ( "cam_gain %i\n" )
#define CMD_SYNC_CAM_GAIN                     ( "cam_gain " )
#define CMD_GET_CAM_GAIN_NO_PARMS             ( 1 )

/******************************************************************************
 * @brief command "cam_exposure" 
 *****************************************************************************/
#define CMD_GET_CAM_EXPOSURE                  ( "cam_exposure\n" )
#define CMD_SET_CAM_EXPOSURE                  ( "cam_exposure %i\n" )
#define CMD_SYNC_CAM_EXPOSURE                 ( "cam_exposure " )
#define CMD_GET_CAM_EXPOSURE_NO_PARMS         ( 1 )

/************************************************* *****************************
 * @brief command definition to read out the camera info
 *****************************************************************************/
#define CMD_GET_CAM_INFO                      ( "cam_info\n" )
#define CMD_SET_CAM_INFO                      ( "cam_info %i %i %i %i %i\n" )
#define CMD_SYNC_CAM_INFO                     ( "cam_info " )
#define CMD_GET_CAM_INFO_NO_PARMS             ( 5 )

/******************************************************************************
 * @brief command definition to read out the camera roi offset info
 *****************************************************************************/
#define CMD_GET_CAM_ROI_OFFSET_INFO           ( "cam_roi_offset_info\n" )
#define CMD_SET_CAM_ROI_OFFSET_INFO           ( "cam_roi_offset_info %i %i %i %i\n" )
#define CMD_SYNC_CAM_ROI_OFFSET_INFO          ( "cam_roi_offset_info " )
#define CMD_GET_CAM_ROI_OFFSET_INFO_NO_PARMS  ( 4 )

/******************************************************************************
 * @brief command "cam_roi_offset "
 *****************************************************************************/
#define CMD_GET_CAM_ROI_OFFSET                ( "cam_roi_offset\n" )
#define CMD_SET_CAM_ROI_OFFSET                ( "cam_roi_offset %i %i\n" )
#define CMD_SYNC_CAM_ROI_OFFSET               ( "cam_roi_offset " )
#define CMD_GET_CAM_ROI_OFFSET_NO_PARMS       ( 2 )

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
    int v4;

    int res;

    // parameter check
    if ( !no || !values )
    {
        return ( -EINVAL );
    }

    // command call to get 4 parameters from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_CAM_INFO, CMD_SYNC_CAM_INFO, CMD_SET_CAM_INFO, &v0, &v1, &v2, &v3, &v4 );

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
    v->min_iso           = UINT32( v4 );

    return ( 0 );
}

/**************************************************************************//**
 * get_cam_gain - get camera analogue gain
 *****************************************************************************/
static int get_cam_gain
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            gain
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
    *gain = UINT32( value );

    return ( 0 );
}

/**************************************************************************//**
 * set_cam_gain - set camera analogue gain
 *****************************************************************************/
static int set_cam_gain
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t const              gain
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

/**************************************************************************//**
 * get_cam_roi_offset_info - get camera ROI offset information
 *****************************************************************************/
static int get_cam_roi_offset_info
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint16_t * const             values
)
{
    (void) ctx;

    ctrl_protocol_cam_roi_offset_info_t * v;

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
            CMD_GET_CAM_ROI_OFFSET_INFO, CMD_SYNC_CAM_ROI_OFFSET_INFO, CMD_SET_CAM_ROI_OFFSET_INFO, &v0, &v1, &v2, &v3);

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_CAM_ROI_OFFSET_INFO_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    v                = (ctrl_protocol_cam_roi_offset_info_t *)values;
    v->offset_x_max  = UINT16( v0 );
    v->offset_y_max  = UINT16( v1 );
    v->offset_x_step = UINT16( v2 );
    v->offset_y_step = UINT16( v3 );

    return ( 0 );
}

/**************************************************************************//**
 * get_cam_roi_offset - get ROI offset of camera-device
 *****************************************************************************/
static int get_cam_roi_offset
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint16_t * const             values
)
{
    (void) ctx;

    ctrl_protocol_cam_roi_offset_t * v;

    int v0;
    int v1;

    int res;

    // parameter check
    if ( !no || !values )
    {
        return ( -EINVAL );
    }

    // command call to get 4 parameters from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_CAM_ROI_OFFSET, CMD_SYNC_CAM_ROI_OFFSET, CMD_SET_CAM_ROI_OFFSET, &v0, &v1);

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_CAM_ROI_OFFSET_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    v                = (ctrl_protocol_cam_roi_offset_t *)values;
    v->offset_x  = UINT16( v0 );
    v->offset_y  = UINT16( v1 );

    return ( 0 );
}

/******************************************************************************
 * set_cam_roi_offset - set ROI offset of camera-device
 *****************************************************************************/
static int set_cam_roi_offset
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint16_t * const             values
)
{
    (void) ctx;

    if ( no != CMD_GET_CAM_ROI_OFFSET_NO_PARMS )
    {
        // return -EFAULT if number of parameter not matching
        return ( -EFAULT );
    }

    return ( set_param_int_X( channel, CMD_SET_CAM_ROI_OFFSET, INT(values[0]), INT(values[1])) );
}

/******************************************************************************
 * CAM protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_cam_drv_t provideo_cam_drv = 
{
    .get_cam_info             = get_cam_info,
    .get_cam_gain             = get_cam_gain,
    .set_cam_gain             = set_cam_gain,
    .get_cam_exposure         = get_cam_exposure,
    .set_cam_exposure         = set_cam_exposure,
    .get_cam_roi_offset_info  = get_cam_roi_offset_info,
    .get_cam_roi_offset       = get_cam_roi_offset,
    .set_cam_roi_offset       = set_cam_roi_offset,
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

