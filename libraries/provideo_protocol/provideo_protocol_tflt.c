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
 * @file    provideo_protocol_tflt.c
 *
 * @brief   Implementation of provideo protocol temporal filter functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_tflt.h>

#include <provideo_protocol/provideo_protocol.h>
#include <provideo_protocol/provideo_protocol_common.h>

/******************************************************************************
 * @brief command "tflt" 
 *****************************************************************************/
#define CMD_GET_TFLT                                ( "tflt\n" )
#define CMD_SET_TFLT                                ( "tflt %i\n" )
#define CMD_SET_TFLT_WITH_COPY_FLAG                 ( "tflt %i %i\n" )
#define CMD_SYNC_TFLT                               ( "tflt " )
#define CMD_GET_TFLT_NO_PARMS                       ( 1 )

/******************************************************************************
 * @brief command "tflt_min_max" 
 *****************************************************************************/
#define CMD_GET_TFLT_MIN_MAX                        ( "tflt_min_max\n" )
#define CMD_SET_TFLT_MIN_MAX                        ( "tflt_min_max %i %i\n" )
#define CMD_SET_TFLT_MIN_MAX_WITH_COPY_FLAG         ( "tflt_min_max %i %i %i\n" )
#define CMD_SYNC_TFLT_MIN_MAX                       ( "tflt_min_max " )
#define CMD_GET_TFLT_MIN_MAX_NO_PARMS               ( 2 )

/******************************************************************************
 * @brief command "tflt_denoise_level" 
 *****************************************************************************/
#define CMD_GET_TFLT_DENOISE_LEVEL                  ( "tflt_denoise_level\n" )
#define CMD_SET_TFLT_DENOISE_LEVEL                  ( "tflt_denoise_level %i\n" )
#define CMD_SET_TFLT_DENOISE_LEVEL_WITH_COPY_FLAG   ( "tflt_denoise_level %i %i\n" )
#define CMD_SYNC_TFLT_DENOISE_LEVEL                 ( "tflt_denoise_level " )
#define CMD_GET_TFLT_DENOISE_LEVEL_NO_PARMS         ( 1 )

/******************************************************************************
 * get_tflt_enable - Get enable status of temporal filtering
 *****************************************************************************/
static int get_tflt_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             flag
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !flag )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_TFLT, CMD_SYNC_TFLT, CMD_SET_TFLT, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_TFLT_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *flag = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_tflt_enable - Set enable status of temporal filtering 
 *****************************************************************************/
static int set_tflt_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               flag
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_TFLT_WITH_COPY_FLAG,
                                    INT( flag ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_TFLT, INT( flag ) ) );
}

/******************************************************************************
 * get_tflt_min_max - Get tflt stage min/max coefficient level
 *****************************************************************************/
int get_tflt_min_max
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    int value0, value1;
    int res;

    ctrl_protocol_tflt_index_t * indices;

    // parameter check
    if ( !values || (sizeof(*indices) != no) )
    {
        return ( -EINVAL );
    }

    // command call to get 2 parameters from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_TFLT_MIN_MAX, CMD_SYNC_TFLT_MIN_MAX, CMD_SET_TFLT_MIN_MAX, &value0, &value1 );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_TFLT_MIN_MAX_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    indices = (ctrl_protocol_tflt_index_t *)values;
    indices->min = UINT8( value0 );
    indices->max = UINT8( value1 );

    return ( 0 );
}

/******************************************************************************
 * set_tflt_min_max - Set tflt stage min/max coefficient level
 *****************************************************************************/
int set_tflt_min_max
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;
    
    ctrl_protocol_tflt_index_t * indices;

    // parameter check
    if ( !values || (sizeof(*indices) != no) )
    {
        return ( -EINVAL );
    }
    
    indices = (ctrl_protocol_tflt_index_t *)values;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_TFLT_MIN_MAX_WITH_COPY_FLAG,
                    INT( indices->min ), INT( indices->max ),
                    INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel,
                CMD_SET_TFLT_MIN_MAX, INT( indices->min ), INT( indices->max ) ) );
}

/******************************************************************************
 * get_tflt_denoise_level - Get denoise level of tflt stage
 *****************************************************************************/
int get_tflt_denoise_level
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             level
)
{
    (void) ctx;
    
    int value;
    int res;

    // parameter check
    if ( !level )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, 
            CMD_GET_TFLT_DENOISE_LEVEL, CMD_SYNC_TFLT_DENOISE_LEVEL, CMD_SET_TFLT_DENOISE_LEVEL, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_TFLT_DENOISE_LEVEL_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *level = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_tflt_denoise_level - Set denoise level of tflt stage
 *****************************************************************************/
int set_tflt_denoise_level
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               level
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_TFLT_DENOISE_LEVEL_WITH_COPY_FLAG,
                                    INT( level ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_TFLT_DENOISE_LEVEL, INT( level ) ) );
}

/******************************************************************************
 * ISP protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_tflt_drv_t provideo_tflt_drv = 
{
    .get_tflt_enable        = get_tflt_enable,
    .set_tflt_enable        = set_tflt_enable,
    .get_tflt_min_max       = get_tflt_min_max,
    .set_tflt_min_max       = set_tflt_min_max,
    .get_tflt_denoise_level = get_tflt_denoise_level,
    .set_tflt_denoise_level = set_tflt_denoise_level,
};

/******************************************************************************
 * provideo_protocol_tflt_init
 *****************************************************************************/
int provideo_protocol_tflt_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_tflt_register( handle, ctx, &provideo_tflt_drv ) );
}

