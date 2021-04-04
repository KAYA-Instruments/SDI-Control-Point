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
 * @file    provideo_protocol_knee.c
 *
 * @brief   ProVideo Knee protocol implementations
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_knee.h>

#include <provideo_protocol/provideo_protocol.h>
#include <provideo_protocol/provideo_protocol_common.h>

/******************************************************************************
 * @brief command "knee"
 *****************************************************************************/
#define CMD_SET_KNEE_ENABLE                     ( "knee %i\n" )

#define CMD_GET_KNEE                            ( "knee\n" )
#define CMD_SET_KNEE                            ( "knee %i %i %i %i\n" )
#define CMD_SYNC_KNEE                           ( "knee " )
#define CMD_GET_KNEE_NO_PARMS                   ( 4 )
#define CMD_SET_KNEE_TMO                        ( 1000 )

/******************************************************************************
 * get_knee_enable - Get enable status of knee function 
 *****************************************************************************/
static int get_knee_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             enable
)
{
    (void) ctx;

    int v0;
    int v1;
    int v2;
    int v3;
    int res;

    // parameter check
    if ( !enable )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_KNEE, CMD_SYNC_KNEE, CMD_SET_KNEE, &v0, &v1, &v2, &v3 );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_KNEE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *enable = UINT8( v0 );

    return ( 0 );
}

/******************************************************************************
 * set_knee_enable - Set enable status of knee function 
 *****************************************************************************/
static int set_knee_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               flag 
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_KNEE_ENABLE, INT( flag ) ) );
}

/******************************************************************************
 * get_knee_config - Get current configuration of knee function 
 *****************************************************************************/
static int get_knee_config
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    int v0;
    int v1;
    int v2;
    int v3;
    int res;

    ctrl_protocol_knee_t * cfg;

    // parameter check
    if ( !no || !values || (no != sizeof(ctrl_protocol_knee_t)) )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_KNEE, CMD_SYNC_KNEE, CMD_SET_KNEE, &v0, &v1, &v2, &v3 );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_KNEE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    cfg             = (ctrl_protocol_knee_t *)values;
    cfg->enable     = UINT8( v0 );
    cfg->knee_point = UINT8( v1 );
    cfg->knee_slope = UINT16( v2 );
    cfg->white_clip = UINT16( v3 );

    return ( 0 );
}

/******************************************************************************
 * set_knee_config - Set current configuration of knee function 
 *****************************************************************************/
static int set_knee_config
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    ctrl_protocol_knee_t * cfg;

    // parameter check
    if ( !no || !values || (no != sizeof(ctrl_protocol_knee_t)) )
    {
        return ( -EINVAL );
    }

    cfg = (ctrl_protocol_knee_t *)values;
    return ( set_param_int_X_with_tmo( channel, CMD_SET_KNEE, CMD_SET_KNEE_TMO,
             INT( cfg->enable ), INT( cfg->knee_point ),
             INT( cfg->knee_slope ), INT( cfg->white_clip ) ) );
}

/******************************************************************************
 * Knee protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_knee_drv_t provideo_knee_drv = 
{
    .get_knee_enable = get_knee_enable,
    .set_knee_enable = set_knee_enable,
    .get_knee_config = get_knee_config,
    .set_knee_config = set_knee_config,
};

/******************************************************************************
 * provideo_protocol_knee_init
 *****************************************************************************/
int provideo_protocol_knee_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_knee_register( handle, ctx, &provideo_knee_drv ) );
}

