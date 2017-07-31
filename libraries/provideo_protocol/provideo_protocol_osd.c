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
 * @file    provideo_protocol_osd.c
 *
 * @brief   Implementation of provideo protocol OSD functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_osd.h>

#include <provideo_protocol/provideo_protocol_common.h>

/******************************************************************************
 * @brief command "test_pattern" 
 *****************************************************************************/
#define CMD_GET_TEST_PATTERN                ( "test_pattern\n" )
#define CMD_SET_TEST_PATTERN                ( "test_pattern %i\n" )
#define CMD_SYNC_TEST_PATTERN               ( "test_pattern " )
#define CMD_TEST_PATTERN_NO_PARMS           ( 1 )

/******************************************************************************
 * @brief command "center_marker" 
 *****************************************************************************/
#define CMD_GET_CENTER_MARKER               ( "center_marker\n" )
#define CMD_SET_CENTER_MARKER               ( "center_marker %i\n" )
#define CMD_SYNC_CENTER_MARKER              ( "center_marker " )
#define CMD_CENTER_MARKER_NO_PARMS          ( 1 )

/******************************************************************************
 * @brief command "zebra" 
 *****************************************************************************/
#define CMD_GET_ZEBRA                       ( "zebra\n" )
#define CMD_SET_ZEBRA                       ( "zebra %i %i %i\n" )
#define CMD_SYNC_ZEBRA                      ( "zebra " )
#define CMD_ZEBRA_NO_PARMS                  ( 3 )

/******************************************************************************
 * @brief command "logo" 
 *****************************************************************************/
#define CMD_GET_LOGO                        ( "logo\n" )
#define CMD_SET_LOGO                        ( "logo %i\n" )
#define CMD_SYNC_LOGO                       ( "logo " )
#define CMD_LOGO_NO_PARMS                   ( 1 )

/******************************************************************************
 * get_test_pattern - Gets test pattern enable mode.
 *****************************************************************************/
static int get_test_pattern
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
            CMD_GET_TEST_PATTERN, CMD_SYNC_TEST_PATTERN, CMD_SET_TEST_PATTERN, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_TEST_PATTERN_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *enable = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_test_pattern - Set enable status of test pattern.
 *****************************************************************************/
static int set_test_pattern
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               enable
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_TEST_PATTERN, INT( enable ) ) );
}

/******************************************************************************
 * get_center_marker - Gets the center marker mode.
 *****************************************************************************/
static int get_center_marker
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
            CMD_GET_CENTER_MARKER, CMD_SYNC_CENTER_MARKER, CMD_SET_CENTER_MARKER, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_CENTER_MARKER_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_center_marker - Set enable status of center marker.
 *****************************************************************************/
static int set_center_marker
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_CENTER_MARKER, INT( mode ) ) );
}

/******************************************************************************
 * get_logo - Gets the logo mode (id: 0=disable, 1=DCT-logo, ...).
 *****************************************************************************/
static int get_logo
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             id 
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !id )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_LOGO, CMD_SYNC_LOGO, CMD_SET_LOGO, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_LOGO_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *id = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_logo - Set logo mode.
 *****************************************************************************/
static int set_logo
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               id 
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_LOGO, INT( id ) ) );
}

/******************************************************************************
 * get_zebra - Gets zebra pattern configuration
 *****************************************************************************/
static int get_zebra
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    ctrl_protocol_osd_zebra_t * zebra;

    int res;
    int on;
    int min;
    int max;

    // parameter check
    if ( !no || !values || (sizeof(*zebra) != no) )
    {
        return ( -EINVAL );
    }

    // command call to get 2 parameters from provideo system
    res = get_param_int_X( channel, 2, 
            CMD_GET_ZEBRA, CMD_SYNC_ZEBRA, CMD_SET_ZEBRA, &on, &min, &max );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_ZEBRA_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    zebra       = (ctrl_protocol_osd_zebra_t *)values;
    zebra->mode =  UINT8( on );
    zebra->min  = UINT16( min );
    zebra->max  = UINT16( max );

    return ( res );    
}

/******************************************************************************
 * set_zebra - Set zebra pattern configuration
 *****************************************************************************/
static int set_zebra
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;
    
    ctrl_protocol_osd_zebra_t * zebra;

    // parameter check
    if ( !no || !values || (sizeof(*zebra) != no) )
    {
        return ( -EINVAL );
    }
    
    zebra = (ctrl_protocol_osd_zebra_t *)values;

    return ( set_param_int_X( channel,
                CMD_SET_ZEBRA, INT( zebra->mode ), INT( zebra->min ), INT( zebra->max ) ) );

}

/******************************************************************************
 * OSD protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_osd_drv_t provideo_osd_drv = 
{
    .get_test_pattern   = get_test_pattern,
    .set_test_pattern   = set_test_pattern,
    .get_center_marker  = get_center_marker,
    .set_center_marker  = set_center_marker,
    .get_logo           = get_logo,
    .set_logo           = set_logo,
    .get_zebra          = get_zebra,
    .set_zebra          = set_zebra,
};

/******************************************************************************
 * provideo_protocol_osd_init
 *****************************************************************************/
int provideo_protocol_osd_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_osd_register( handle, ctx, &provideo_osd_drv ) );
}

