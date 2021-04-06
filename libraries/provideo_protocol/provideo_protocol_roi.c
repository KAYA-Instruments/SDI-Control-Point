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
 * @file    provideo_protocol_roi.c
 *
 * @brief   Implementation of provideo protocol ROI functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_roi.h>

#include <provideo_protocol/provideo_protocol_common.h>

/******************************************************************************
 * @brief command definition to read out the stat roi info
 *****************************************************************************/
#define CMD_GET_STAT_ROI_INFO           ( "stat_roi_info\n" )
#define CMD_SET_STAT_ROI_INFO           ( "stat_roi_info %i %i %i %i\n" )
#define CMD_SYNC_STAT_ROI_INFO          ( "stat_roi_info " )
#define CMD_GET_STAT_ROI_INFO_NO_PARMS  ( 4 )

/******************************************************************************
 * @brief command "stat_roi "
 *****************************************************************************/
#define CMD_GET_STAT_ROI                ( "stat_roi\n" )
#define CMD_SET_STAT_ROI                ( "stat_roi %i %i %i %i\n" )
#define CMD_SYNC_STAT_ROI               ( "stat_roi " )
#define CMD_GET_STAT_ROI_NO_PARMS       ( 4 )

/**************************************************************************//**
 * get_stat_roi_info - get stat ROI information
 *****************************************************************************/
static int get_stat_roi_info
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    ctrl_protocol_stat_roi_info_t * v;

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
            CMD_GET_STAT_ROI_INFO, CMD_SYNC_STAT_ROI_INFO, CMD_SET_STAT_ROI_INFO, &v0, &v1, &v2, &v3);

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_STAT_ROI_INFO_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    v              = (ctrl_protocol_stat_roi_info_t *)values;
    v->max_width   = UINT32( v0 );
    v->max_height  = UINT32( v1 );
    v->width_step  = UINT32( v2 );
    v->height_step = UINT32( v3 );

    return ( 0 );
}

/**************************************************************************//**
 * get_stat_roi - get stat ROI of camera-device
 *****************************************************************************/
static int get_stat_roi
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    ctrl_protocol_stat_roi_t * v;

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
            CMD_GET_STAT_ROI, CMD_SYNC_STAT_ROI, CMD_SET_STAT_ROI, &v0, &v1, &v2, &v3);

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_STAT_ROI_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    v           = (ctrl_protocol_stat_roi_t *)values;
    v->width    = UINT32( v0 );
    v->height   = UINT32( v1 );
    v->offset_x = UINT32( v2 );
    v->offset_y = UINT32( v3 );

    return ( 0 );
}

/******************************************************************************
 * set_stat_roi - set stat ROI of camera-device
 *****************************************************************************/
static int set_stat_roi
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint32_t * const            values
)
{
    (void) ctx;

    if ( no != CMD_GET_STAT_ROI_NO_PARMS )
    {
        // return -EFAULT if number of parameter not matching
        return ( -EFAULT );
    }

    return ( set_param_int_X( channel, CMD_SET_STAT_ROI, INT(values[0]), INT(values[1]), INT(values[2]), INT(values[3])) );
}

/******************************************************************************
 * ROU protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_roi_drv_t provideo_roi_drv =
{
    .get_stat_roi_info  = get_stat_roi_info,
    .get_stat_roi       = get_stat_roi,
    .set_stat_roi       = set_stat_roi,
};

/******************************************************************************
 * provideo_protocol_roi_init
 *****************************************************************************/
int provideo_protocol_roi_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_roi_register( handle, ctx, &provideo_roi_drv ) );
}

