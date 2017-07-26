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
 * @file    provideo_protocol_iris.c
 *
 * @brief   Implementation of provideo protocol iris functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_iris.h>

#include "provideo_protocol_common.h"

/******************************************************************************
 * @brief command "cam_iris_setup" 
 *****************************************************************************/
#define CMD_GET_CAM_IRIS_SETUP                   ( "cam_iris_setup\n" )
#define CMD_SET_CAM_IRIS_SETUP                   ( "cam_iris_setup %i %i %i %i %i %i %i %i %i %i\n" )
#define CMD_SYNC_CAM_IRIS_SETUP                  ( "cam_iris_setup " )
#define CMD_GET_CAM_IRIS_SETUP_NO_PARAMS         ( 10 )

/******************************************************************************
 * @brief command "cam_iris_apt" 
 *****************************************************************************/
#define CMD_GET_CAM_IRIS_APT                     ( "cam_iris_apt\n" )
#define CMD_SET_CAM_IRIS_APT                     ( "cam_iris_apt %i\n" )
#define CMD_SYNC_CAM_IRIS_APT                    ( "cam_iris_apt " )
#define CMD_GET_CAM_IRIS_APT_NO_PARAMS           ( 1 )

/******************************************************************************
 * get_iris_setup - Gets current iris setup values
 *****************************************************************************/
static int get_iris_setup
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

    // command call to get 3 parameters from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_CAM_IRIS_SETUP, CMD_SYNC_CAM_IRIS_SETUP, CMD_SET_CAM_IRIS_SETUP,
            &values[0], &values[1], &values[2], &values[3], &values[4],
            &values[5], &values[6], &values[7], &values[8], &values[9] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( no != CMD_GET_CAM_IRIS_SETUP_NO_PARAMS )
    {
        return ( -EFAULT );
    }

    return ( 0 );
}

/******************************************************************************
 * set_iris_setup
 *****************************************************************************/
static int set_iris_setup
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no, 
    int32_t * const             values
)
{
    (void) ctx;

    if ( no != CMD_GET_CAM_IRIS_SETUP_NO_PARAMS )
    {
        // return -EFAULT if number of parameter not matching
        return ( -EFAULT );
    }

    return ( set_param_int_X( channel, CMD_SET_CAM_IRIS_SETUP,
                INT( values[0] ), INT( values[1] ), INT( values[2] ),
                INT( values[3] ), INT( values[4] ), INT( values[5] ),
                INT( values[6] ), INT( values[7] ), INT( values[8] ),
                INT( values[9] ) ) );
}

/******************************************************************************
 * get_iris_apt
 *****************************************************************************/
static int get_iris_apt
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int32_t * const             pos 
)
{
    (void) ctx;

    int res;

    // parameter check
    if ( !pos )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_CAM_IRIS_APT, CMD_SYNC_CAM_IRIS_APT, CMD_SET_CAM_IRIS_APT, pos );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_CAM_IRIS_APT_NO_PARAMS )
    {
        return ( -EFAULT );
    }

    return ( 0 );
}

/******************************************************************************
 * set_iris_apt
 *****************************************************************************/
static int set_iris_apt
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int32_t const               pos 
)
{
    (void) ctx;
    return ( set_param_int_X( channel, CMD_SET_CAM_IRIS_APT, INT( pos ) ) );
}

/******************************************************************************
 * Auto Iris protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_iris_drv_t provideo_iris_drv = 
{
    .get_iris_setup             = get_iris_setup,
    .set_iris_setup             = set_iris_setup,
    .get_iris_apt               = get_iris_apt,
    .set_iris_apt               = set_iris_apt,
};

/******************************************************************************
 * provideo_protocol_iris_init
 *****************************************************************************/
int provideo_protocol_iris_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_iris_register( handle, ctx, &provideo_iris_drv ) );
}

