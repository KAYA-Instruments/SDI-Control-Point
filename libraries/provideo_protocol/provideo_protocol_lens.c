/******************************************************************************
 * Copyright (C) 2019 Dream Chip Technologies GmbH
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
 * @file    provideo_protocol_lens.c
 *
 * @brief   Implementation of provideo protocol lens functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_lens.h>

#include <provideo_protocol/provideo_protocol_common.h>

/******************************************************************************
 * @brief command "lens_driver_settings"
 *****************************************************************************/
#define CMD_GET_LENS_SETTINGS           ( "lens_driver_settings\n" )
#define CMD_SET_LENS_SETTINGS           ( "lens_driver_settings %i %i %i %i %i %i %i %i %i %i %i\n" )
#define CMD_SYNC_LENS_SETTINGS          ( "lens_driver_settings " )
#define CMD_GET_LENS_SETTINGS_NO_PARAMS ( 11 )

/******************************************************************************
 * @brief command "cam_iris_apt" 
 *****************************************************************************/
//#define CMD_GET_CAM_IRIS_APT                     ( "cam_iris_apt\n" )
//#define CMD_SET_CAM_IRIS_APT                     ( "cam_iris_apt %i\n" )
//#define CMD_SYNC_CAM_IRIS_APT                    ( "cam_iris_apt " )
//#define CMD_GET_CAM_IRIS_APT_NO_PARAMS           ( 1 )

/******************************************************************************
 * get_lens_settings
 *****************************************************************************/
static int get_lens_settings
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no, 
    int32_t * const             values
)
{
    (void) ctx;

    int res;

    // parameter check
    if ( !values )
    {
        return ( -EINVAL );
    }

    // return -EFAULT if number of parameter not matching
    else if ( no != CMD_GET_LENS_SETTINGS_NO_PARAMS )
    {
        return ( -EFAULT );
    }

    // command call to get 3 parameters from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_LENS_SETTINGS, CMD_SYNC_LENS_SETTINGS, CMD_SET_LENS_SETTINGS,
            &values[0], &values[1], &values[2], &values[3], &values[4],
            &values[5], &values[6], &values[7], &values[8], &values[9],
            &values[10] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    return ( 0 );
}

/******************************************************************************
 * set_lens_settings
 *****************************************************************************/
static int set_lens_settings
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no, 
    int32_t * const             values
)
{
    (void) ctx;

    if ( no != CMD_GET_LENS_SETTINGS_NO_PARAMS )
    {
        // return -EFAULT if number of parameter not matching
        return ( -EFAULT );
    }

    return ( set_param_int_X( channel, CMD_SET_LENS_SETTINGS,
                INT( values[0] ), INT( values[1] ), INT( values[2] ),
                INT( values[3] ), INT( values[4] ), INT( values[5] ),
                INT( values[6] ), INT( values[7] ), INT( values[8] ),
                INT( values[9] ), INT( values[10] ) ) );
}

///******************************************************************************
// * get_iris_apt
// *****************************************************************************/
//static int get_iris_apt
//(
//    void * const                ctx,
//    ctrl_channel_handle_t const channel,
//    int32_t * const             pos
//)
//{
//    (void) ctx;

//    int res;

//    // parameter check
//    if ( !pos )
//    {
//        return ( -EINVAL );
//    }

//    // command call to get 1 parameter from provideo system
//    res = get_param_int_X( channel, 2,
//            CMD_GET_CAM_IRIS_APT, CMD_SYNC_CAM_IRIS_APT, CMD_SET_CAM_IRIS_APT, pos );

//    // return error code
//    if ( res < 0 )
//    {
//        return ( res );
//    }

//    // return -EFAULT if number of parameter not matching
//    else if ( res != CMD_GET_CAM_IRIS_APT_NO_PARAMS )
//    {
//        return ( -EFAULT );
//    }

//    return ( 0 );
//}

///******************************************************************************
// * set_iris_apt
// *****************************************************************************/
//static int set_iris_apt
//(
//    void * const                ctx,
//    ctrl_channel_handle_t const channel,
//    int32_t const               pos
//)
//{
//    (void) ctx;
//    return ( set_param_int_X( channel, CMD_SET_CAM_IRIS_APT, INT( pos ) ) );
//}

/******************************************************************************
 * Lens protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_lens_drv_t provideo_lens_drv =
{
    .get_lens_settings            = get_lens_settings,
    .set_lens_settings            = set_lens_settings,
//    .get_iris_apt               = get_iris_apt,
//    .set_iris_apt               = set_iris_apt,
};

/******************************************************************************
 * provideo_protocol_lens_init
 *****************************************************************************/
int provideo_protocol_lens_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_lens_register( handle, ctx, &provideo_lens_drv ) );
}
