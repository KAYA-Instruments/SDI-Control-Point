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
 * @file    provideo_protocol_cproc.c
 *
 * @brief   Implementation of provideo protocol cproc functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_cproc.h>

#include "provideo_protocol_common.h"

/******************************************************************************
 * @brief command "post_bright" 
 *****************************************************************************/
#define CMD_GET_POST_BRIGHT                 ( "post_bright\n" )
#define CMD_SET_POST_BRIGHT                 ( "post_bright %i\n" )
#define CMD_SYNC_POST_BRIGHT                ( "post_bright " )
#define CMD_GET_POST_BRIGHT_NO_PARMS        ( 1 )

/******************************************************************************
 * @brief command "post_cont" 
 *****************************************************************************/
#define CMD_GET_POST_CONT                   ( "post_cont\n" )
#define CMD_SET_POST_CONT                   ( "post_cont %i\n" )
#define CMD_SYNC_POST_CONT                  ( "post_cont " )
#define CMD_GET_POST_CONT_NO_PARMS          ( 1 )

/******************************************************************************
 * @brief command "post_sat" 
 *****************************************************************************/
#define CMD_GET_POST_SAT                    ( "post_sat\n" )
#define CMD_SET_POST_SAT                    ( "post_sat %i\n" )
#define CMD_SYNC_POST_SAT                   ( "post_sat " )
#define CMD_GET_POST_SAT_NO_PARMS           ( 1 )

/******************************************************************************
 * @brief command "post_hue" 
 *****************************************************************************/
#define CMD_GET_POST_HUE                    ( "post_hue\n" )
#define CMD_SET_POST_HUE                    ( "post_hue %i\n" )
#define CMD_SYNC_POST_HUE                   ( "post_hue " )
#define CMD_GET_POST_HUE_NO_PARMS           ( 1 )

/******************************************************************************
 * get_post_bright - Get post processing brightness
 *****************************************************************************/
static int get_post_bright
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int16_t * const             brightness
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !brightness )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_POST_BRIGHT,
            CMD_SYNC_POST_BRIGHT, CMD_SET_POST_BRIGHT, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_POST_BRIGHT_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *brightness = INT16( value );

    return ( 0 );
}

/******************************************************************************
 * set_post_bright - Set post processing brightness
 *****************************************************************************/
static int set_post_bright
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int16_t const               brightness
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_POST_BRIGHT, INT( brightness ) ) );
}

/******************************************************************************
 * get_post_cont - Get post processing contrast
 *****************************************************************************/
static int get_post_cont
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t * const            contrast
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !contrast )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_POST_CONT,
            CMD_SYNC_POST_CONT, CMD_SET_POST_CONT, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_POST_CONT_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *contrast = UINT16( value );

    return ( 0 );
}

/******************************************************************************
 * set_post_cont - Set post processing contrast
 *****************************************************************************/
static int set_post_cont
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t const              contrast
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_POST_CONT, INT( contrast ) ) );
}

/******************************************************************************
 * get_post_sat - Get post processing color saturation
 *****************************************************************************/
static int get_post_sat
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t * const            saturation 
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !saturation )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_POST_SAT,
            CMD_SYNC_POST_SAT, CMD_SET_POST_SAT, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_POST_SAT_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *saturation = UINT16( value );

    return ( 0 );
}

/******************************************************************************
 * set_post_sat - Set post processing color saturation
 *****************************************************************************/
static int set_post_sat
( 
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t const              saturation
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_POST_SAT, INT( saturation ) ) );
}

/******************************************************************************
 * get_post_hue - Get post processing color hue offset angle
 *****************************************************************************/
static int get_post_hue
( 
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int16_t * const             hue
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !hue )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_POST_HUE,
            CMD_SYNC_POST_HUE, CMD_SET_POST_HUE, &value );
            
    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_POST_HUE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *hue = INT16( value );

    return ( 0 );
}

/******************************************************************************
 * set_post_hue - Set post processing color hue offset angle
 *****************************************************************************/
static int set_post_hue
( 
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int16_t const               hue
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_POST_HUE, INT( hue ) ) );
}

/******************************************************************************
 * CPROC protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_cproc_drv_t provideo_cproc_drv = 
{
    .get_post_bright        = get_post_bright,
    .set_post_bright        = set_post_bright,
    .get_post_cont          = get_post_cont,
    .set_post_cont          = set_post_cont,
    .get_post_sat           = get_post_sat,
    .set_post_sat           = set_post_sat,
    .get_post_hue           = get_post_hue,
    .set_post_hue           = set_post_hue,
};

/******************************************************************************
 * provideo_protocol_cproc_init
 *****************************************************************************/
int provideo_protocol_cproc_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_cproc_register( handle, ctx, &provideo_cproc_drv ) );
}

