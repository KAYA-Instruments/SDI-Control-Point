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
 * @file    provideo_protocol_mcc.c
 *
 * @brief   Implementation of provideo protocol mcc functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_mcc.h>

#include <provideo_protocol/provideo_protocol.h>
#include <provideo_protocol/provideo_protocol_common.h>

/******************************************************************************
 * @brief command "mcc" 
 *****************************************************************************/
#define CMD_GET_MCC                         ( "mcc\n" )
#define CMD_SET_MCC                         ( "mcc %i\n" )
#define CMD_SET_MCC_WITH_COPY_FLAG          ( "mcc %i %i\n" )
#define CMD_SYNC_MCC                        ( "mcc " )
#define CMD_GET_MCC_NO_PARMS                ( 1 )

/******************************************************************************
 * @brief command "mcc_opmode" 
 *****************************************************************************/
#define CMD_GET_MCC_OPMODE                  ( "mcc_opmode\n" )
#define CMD_SET_MCC_OPMODE                  ( "mcc_opmode %i\n" )
#define CMD_SET_MCC_OPMODE_WITH_COPY_FLAG   ( "mcc_opmode %i\n" )
#define CMD_SYNC_MCC_OPMODE                 ( "mcc_opmode " )
#define CMD_GET_MCC_OPMODE_NO_PARMS         ( 1 )

/******************************************************************************
 * @brief command "mcc_set" 
 *****************************************************************************/
#define CMD_GET_MCC_SET                     ( "mcc_set %i\n" )
#define CMD_SET_MCC_SET                     ( "mcc_set %i %i %i\n" )
#define CMD_SET_MCC_SET_WITH_COPY_FLAG      ( "mcc_set %i %i %i %i\n" )
#define CMD_SYNC_MCC_SET                    ( "mcc_set" )
#define CMD_GET_MCC_SET_NO_PARMS            ( 3 )

/******************************************************************************
 * @brief command "mcc_blink" 
 *****************************************************************************/
#define CMD_GET_MCC_BLINK                   ( "mcc_blink\n" )
#define CMD_SET_MCC_BLINK                   ( "mcc_blink %i %i\n" )
#define CMD_SET_MCC_BLINK_WITH_COPY_FLAG    ( "mcc_blink %i %i %i\n" )
#define CMD_SYNC_MCC_BLINK                  ( "mcc_blink " )
#define CMD_GET_MCC_BLINK_NO_PARMS          ( 2 )

/******************************************************************************
 * get_mcc_enable - Get enable status of multi color controller
 *****************************************************************************/
static int get_mcc_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             enable
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !enable )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_MCC, CMD_SYNC_MCC, CMD_SET_MCC, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_MCC_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *enable = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_mcc_enable - Set enable status of multi color controller
 *****************************************************************************/
static int set_mcc_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               flag 
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_MCC_WITH_COPY_FLAG,
                                    INT( flag ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_MCC, INT( flag ) ) );
}

/******************************************************************************
 * get_mcc_opmode - Get operation mode of multi color controller
 *****************************************************************************/
static int get_mcc_opmode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             mode
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !mode )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_MCC_OPMODE, CMD_SYNC_MCC_OPMODE, CMD_SET_MCC_OPMODE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_MCC_OPMODE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_mcc_opmode - Set operation mode of multi color controller
 *****************************************************************************/
static int set_mcc_opmode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_MCC_OPMODE_WITH_COPY_FLAG,
                                    INT( mode ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_MCC_OPMODE, INT( mode ) ) );
}

/******************************************************************************
 * get_mcc_phase - Get configuration of a single color phase 
 *****************************************************************************/
static int get_mcc_phase
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    char command[CMD_SINGLE_LINE_COMMAND_SIZE];
    char data[2*CMD_SINGLE_LINE_RESPONSE_SIZE];

    ctrl_protocol_mcc_phase_t * phase;
    
    int res;

    // parameter check
    if ( !no || !values || (sizeof(*phase) != no) )
    {
        return ( -EINVAL );
    }
    
    phase = (ctrl_protocol_mcc_phase_t *)values;

    // clear command buffer
    memset( command, 0, sizeof(command) );

    // create command to send
    sprintf( command, CMD_GET_MCC_SET, phase->id );

    // send get-command to control channel
    ctrl_channel_send_request( channel, (uint8_t *)command, strlen(command) );

    // read response from provideo device
    res = evaluate_get_response( channel, data, sizeof(data) );
    if ( !res )
    {
        char * s = strstr( data, CMD_SYNC_MCC_SET );
        if ( s )
        {
            int v0, v1, v2;

            res = sscanf( s, CMD_SET_MCC_SET, &v0, &v1, &v2 );
            if ( (res == CMD_GET_MCC_SET_NO_PARMS) && (UINT8(v0) == phase->id) )
            {
                phase->saturation = UINT32( v1 );
                phase->hue        = INT32( v2 );
                return ( 0 );
            }
            else
            {
                return ( -EINVAL );
            }
        }
        else
        {
            return ( -EFAULT );
        }
    }
    else
    {
        res = evaluate_error_response( data, res );
    }

    return ( res );    
}

/******************************************************************************
 * set_mcc_phase - Set configuration of a single color phase 
 *****************************************************************************/
static int set_mcc_phase
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    ctrl_protocol_mcc_phase_t * phase;

    // parameter check
    if ( !no || !values || (sizeof(*phase) != no) )
    {
        return ( -EINVAL );
    }
    
    phase = (ctrl_protocol_mcc_phase_t *)values;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_MCC_SET_WITH_COPY_FLAG,
                    INT( phase->id ), INT( phase->saturation ), INT( phase->hue ),
                    INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel,
                CMD_SET_MCC_SET, INT( phase->id ), INT( phase->saturation ), INT( phase->hue ) ) );
}

/******************************************************************************
 * get_mcc_blink - Get saturation blink mode of multi color controller
 *****************************************************************************/
static int get_mcc_blink
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint32_t * const            values
)
{
    (void) ctx;

    int mask, period;
    int res;

    // parameter check
    if ( !values )
    {
        return ( -EINVAL );
    }

    if ( no != CMD_GET_MCC_BLINK_NO_PARMS )
    {
        // return -EFAULT if number of parameter not matching
        return ( -EFAULT );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_MCC_BLINK, CMD_SYNC_MCC_BLINK, CMD_SET_MCC_BLINK, &mask, &period );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_MCC_BLINK_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    values[0] = UINT32( mask );
    values[1] = UINT32( period );

    return ( 0 );
}

/******************************************************************************
 * set_mcc_blink - Set operation mode of multi color controller
 *****************************************************************************/
static int set_mcc_blink
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint32_t * const            values
)
{
    (void) ctx;

    if ( no != CMD_GET_MCC_BLINK_NO_PARMS )
    {
        // return -EFAULT if number of parameter not matching
        return ( -EFAULT );
    }

    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_MCC_BLINK_WITH_COPY_FLAG,
                                    INT( values[0] ), INT( values[1] ), INT( user->use_copy_flag ) ) );
    }

    // type-cast to range
    uint32_t temp1 = values[0];
    uint32_t temp2 = values[1];

    return ( set_param_int_X( channel, CMD_SET_MCC_BLINK,  values[0] , values[1]  ) );
}

/******************************************************************************
 * MCC protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_mcc_drv_t provideo_mcc_drv = 
{
    .get_mcc_enable = get_mcc_enable,
    .set_mcc_enable = set_mcc_enable,
    .get_mcc_opmode = get_mcc_opmode,
    .set_mcc_opmode = set_mcc_opmode,
    .get_mcc_phase  = get_mcc_phase,
    .set_mcc_phase  = set_mcc_phase,
    .get_mcc_blink  = get_mcc_blink,
    .set_mcc_blink  = set_mcc_blink,
};

/******************************************************************************
 * provideo_protocol_mcc_init
 *****************************************************************************/
int provideo_protocol_mcc_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_mcc_register( handle, ctx, &provideo_mcc_drv ) );
}

