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
 * @file    provideo_protocol_isp.c
 *
 * @brief   Implementation of provideo protocol isp functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_isp.h>

#include <provideo_protocol/provideo_protocol.h>
#include <provideo_protocol/provideo_protocol_common.h>

/******************************************************************************
 * @brief command "lsc"
 *****************************************************************************/
#define CMD_GET_LSC                         ( "lsc\n" )
#define CMD_SET_LSC                         ( "lsc %u %u %u %u\n" )
#define CMD_SET_LSC_WITH_COPY_FLAG          ( "lsc %u %u %u %u %i\n" )
#define CMD_SYNC_LSC                        ( "lsc " )
#define CMD_LSC_NO_PARMS                    ( 4 )

/******************************************************************************
 * @brief command "bayer"
 *****************************************************************************/
#define CMD_GET_BAYER                       ( "bayer\n" )
#define CMD_SET_BAYER                       ( "bayer %i\n" )
#define CMD_SET_BAYER_WITH_COPY_FLAG        ( "bayer %i %i\n" )
#define CMD_SYNC_BAYER                      ( "bayer " )
#define CMD_BAYER_NO_PARMS                  ( 1 )

/******************************************************************************
 * @brief command "gain_red" 
 *****************************************************************************/
#define CMD_GET_GAIN_RED                    ( "gain_red\n" )
#define CMD_SET_GAIN_RED                    ( "gain_red %i\n" )
#define CMD_SET_GAIN_RED_WITH_COPY_FLAG     ( "gain_red %i %i\n" )
#define CMD_SYNC_GAIN_RED                   ( "gain_red " )
#define CMD_GAIN_RED_NO_PARMS               ( 1 )

/******************************************************************************
 * @brief command "gain_green" 
 *****************************************************************************/
#define CMD_GET_GAIN_GREEN                  ( "gain_green\n" )
#define CMD_SET_GAIN_GREEN                  ( "gain_green %i\n" )
#define CMD_SET_GAIN_GREEN_WITH_COPY_FLAG   ( "gain_green %i %i\n" )
#define CMD_SYNC_GAIN_GREEN                 ( "gain_green " )
#define CMD_GAIN_GREEN_NO_PARMS             ( 1 )

/******************************************************************************
 * @brief command "gain_blue" 
 *****************************************************************************/
#define CMD_GET_GAIN_BLUE                   ( "gain_blue\n" )
#define CMD_SET_GAIN_BLUE                   ( "gain_blue %i\n" )
#define CMD_SET_GAIN_BLUE_WITH_COPY_FLAG    ( "gain_blue %i %i\n" )
#define CMD_SYNC_GAIN_BLUE                  ( "gain_blue " )
#define CMD_GAIN_BLUE_NO_PARMS              ( 1 )

/******************************************************************************
 * @brief command "black_red" 
 *****************************************************************************/
#define CMD_GET_BLACK_RED                   ( "black_red\n" )
#define CMD_SET_BLACK_RED                   ( "black_red %i\n" )
#define CMD_SET_BLACK_RED_WITH_COPY_FLAG    ( "black_red %i %i\n" )
#define CMD_SYNC_BLACK_RED                  ( "black_red " )
#define CMD_BLACK_RED_NO_PARMS              ( 1 )

/******************************************************************************
 * @brief command "black_green" 
 *****************************************************************************/
#define CMD_GET_BLACK_GREEN                 ( "black_green\n" )
#define CMD_SET_BLACK_GREEN                 ( "black_green %i\n" )
#define CMD_SET_BLACK_GREEN_WITH_COPY_FLAG  ( "black_green %i %i\n" )
#define CMD_SYNC_BLACK_GREEN                ( "black_green " )
#define CMD_BLACK_GREEN_NO_PARMS            ( 1 )

/******************************************************************************
 * @brief command "black_blue" 
 *****************************************************************************/
#define CMD_GET_BLACK_BLUE                  ( "black_blue\n" )
#define CMD_SET_BLACK_BLUE                  ( "black_blue %i\n" )
#define CMD_SET_BLACK_BLUE_WITH_COPY_FLAG   ( "black_blue %i %i\n" )
#define CMD_SYNC_BLACK_BLUE                 ( "black_blue " )
#define CMD_BLACK_BLUE_NO_PARMS             ( 1 )

/******************************************************************************
 * @brief command "flare" 
 *****************************************************************************/
#define CMD_GET_FLARE                       ( "flare\n" )
#define CMD_SET_FLARE                       ( "flare %i %i %i\n" )
#define CMD_SET_FLARE_WITH_COPY_FLAG        ( "flare %i %i %i %i\n" )
#define CMD_SYNC_FLARE                      ( "flare " )
#define CMD_FLARE_NO_PARMS                  ( 3 )

/******************************************************************************
 * @brief command "black_master" 
 *****************************************************************************/
#define CMD_GET_BLACK_MASTER                ( "black_master\n" )
#define CMD_SET_BLACK_MASTER                ( "black_master %i %i %i\n" )
#define CMD_SET_BLACK_MASTER_WITH_COPY_FLAG ( "black_master %i %i %i %i\n" )
#define CMD_SYNC_BLACK_MASTER               ( "black_master " )
#define CMD_BLACK_MASTER_NO_PARMS           ( 3 )

/******************************************************************************
 * @brief command "filter_enable" 
 *****************************************************************************/
#define CMD_GET_FILTER_ENABLE                   ( "filter_enable\n" )
#define CMD_SET_FILTER_ENABLE                   ( "filter_enable %i\n" )
#define CMD_SET_FILTER_ENABLE_WITH_COPY_FLAG    ( "filter_enable %i %i\n" )
#define CMD_SYNC_FILTER_ENABLE                  ( "filter_enable " )
#define CMD_FILTER_ENABLE_NO_PARMS              ( 1 )
#define CMD_GET_FILTER_TMO                      ( 120 ) // Default 100ms timeout is to short when using slower 57600 baudrate

/******************************************************************************
 * @brief command "filter_detail" 
 *****************************************************************************/
#define CMD_GET_FILTER_DETAIL                   ( "filter_detail\n" )
#define CMD_SET_FILTER_DETAIL                   ( "filter_detail %i\n" )
#define CMD_SET_FILTER_DETAIL_WITH_COPY_FLAG    ( "filter_detail %i %i\n" )
#define CMD_SYNC_FILTER_DETAIL                  ( "filter_detail " )
#define CMD_FILTER_DETAIL_NO_PARMS              ( 1 )

/******************************************************************************
 * @brief command "filter_denoise" 
 *****************************************************************************/
#define CMD_GET_FILTER_DENOISE                  ( "filter_denoise\n" )
#define CMD_SET_FILTER_DENOISE                  ( "filter_denoise %i\n" )
#define CMD_SET_FILTER_DENOISE_WITH_COPY_FLAG   ( "filter_denoise %i %i\n" )
#define CMD_SYNC_FILTER_DENOISE                 ( "filter_denoise " )
#define CMD_FILTER_DENOISE_NO_PARMS             ( 1 )

/******************************************************************************
 * @brief command "color_conv" 
 *****************************************************************************/
#define CMD_GET_COLOR_CONV                  ( "color_conv\n" )
#define CMD_SET_COLOR_CONV                  ( "color_conv %i %i %i %i %i %i %i %i %i\n" )
#define CMD_SET_COLOR_CONV_WITH_COPY_FLAG   ( "color_conv %i %i %i %i %i %i %i %i %i %i\n" )
#define CMD_SYNC_COLOR_CONV                 ( "color_conv " )
#define CMD_COLOR_CONV_NO_PARMS             ( 9 )

/******************************************************************************
 * @brief command "color_cross" 
 *****************************************************************************/
#define CMD_GET_COLOR_CROSS                 ( "color_cross\n" )
#define CMD_SET_COLOR_CROSS                 ( "color_cross %i %i %i %i %i %i %i %i %i\n" )
#define CMD_SET_COLOR_CROSS_WITH_COPY_FLAG  ( "color_cross %i %i %i %i %i %i %i %i %i %i\n" )
#define CMD_SYNC_COLOR_CROSS                ( "color_cross " )
#define CMD_COLOR_CROSS_NO_PARMS            ( 9 )

/******************************************************************************
 * @brief command "color_cross_offset" 
 *****************************************************************************/
#define CMD_GET_COLOR_CROSS_OFFSET                  ( "color_cross_offset\n" )
#define CMD_SET_COLOR_CROSS_OFFSET                  ( "color_cross_offset %i %i %i\n" )
#define CMD_SET_COLOR_CROSS_OFFSET_WITH_COPY_FLAG   ( "color_cross_offset %i %i %i %i\n" )
#define CMD_SYNC_COLOR_CROSS_OFFSET                 ( "color_cross_offset " )
#define CMD_COLOR_CROSS_OFFSET_NO_PARMS             ( 3 )

/******************************************************************************
 * @brief command "black_sun_correction" 
 *****************************************************************************/
#define CMD_GET_BLACK_SUN_CORRECTION        ( "black_sun_correction\n" )
#define CMD_SET_BLACK_SUN_CORRECTION        ( "black_sun_correction %i %i %i\n" )
#define CMD_SYNC_BLACK_SUN_CORRECTION       ( "black_sun_correction " )
#define CMD_BLACK_SUN_CORRECTION_NO_PARMS   ( 3 )

/******************************************************************************
 * @brief command "split_screen"
 *****************************************************************************/
#define CMD_GET_SPLIT_SCREEN                ( "split_screen\n" )
#define CMD_SET_SPLIT_SCREEN                ( "split_screen %i\n" )
#define CMD_SET_SPLIT_SCREEN_WITH_COPY_FLAG ( "split_screen %i %i\n" )
#define CMD_SYNC_SPLIT_SCREEN               ( "split_screen " )
#define CMD_SPLIT_SCREEN_NO_PARMS           ( 1 )

/******************************************************************************
 * get_lsc - Gets setup of the lens shade correction module
 *****************************************************************************/
static int get_lsc
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint32_t * const            values
)
{
    (void) ctx;
    (void) no;

    int res;

    // parameter check
    if ( !values )
    {
        return ( -EINVAL );
    }

    // command call to get 5 parameters from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_LSC,
            CMD_SYNC_LSC, CMD_SET_LSC,
            &values[0], &values[1], &values[2], &values[3], &values[4]);

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_LSC_NO_PARMS )
    {
        return ( -EFAULT );
    }

    return ( 0 );
}

/******************************************************************************
 * set_lsc - Configures the lens shading correction module
 *****************************************************************************/
static int set_lsc
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint32_t * const            values
)
{
    (void) ctx;

    if ( no != CMD_LSC_NO_PARMS )
    {
        return ( -EFAULT );
    }

    return ( set_param_int_X( channel, CMD_SET_LSC,
                              UINT32( values[0] ), UINT32( values[1] ), UINT32( values[2] ),
                              UINT32( values[3] ), UINT32( values[4] ) ) );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of getting currently configured 
 *        bayer-pattern from a provideo device.
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] pattern  pattern to get value
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_bayer
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             pattern
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !pattern )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_BAYER, CMD_SYNC_BAYER, CMD_SET_BAYER, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_BAYER_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *pattern = UINT8( value );

    return ( 0 );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of setting bayer-pattern to a 
 *        provideo device.
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[in]  pattern  bayer pattern to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_bayer
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               pattern
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_BAYER_WITH_COPY_FLAG,
                                    INT( pattern ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_BAYER, INT( pattern ) ) );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of getting currently configured 
 *        red gain from a provideo device.
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] gain     current red gain
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_gain_red
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
            CMD_GET_GAIN_RED, CMD_SYNC_GAIN_RED, CMD_SET_GAIN_RED, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GAIN_RED_NO_PARMS )
    {
        return ( -EFAULT );
    }
    
    // type-cast to range
    *gain = UINT16( value );
    
    return ( 0 );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of setting red gain to a 
 *        provideo device.
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[in]  gain     red gain to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_gain_red
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t const              gain
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_GAIN_RED_WITH_COPY_FLAG,
                                    INT( gain ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_GAIN_RED, INT( gain ) ) );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of getting currently configured 
 *        green gain from a provideo device.
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] gain     current green gain
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_gain_green
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
            CMD_GET_GAIN_GREEN, CMD_SYNC_GAIN_GREEN, CMD_SET_GAIN_GREEN, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GAIN_RED_NO_PARMS )
    {
        return ( -EFAULT );
    }
    
    // type-cast to range
    *gain = UINT16( value );

    return ( 0 );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of setting green gain to a 
 *        provideo device.
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[in]  gain     green gain to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_gain_green
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t const              gain
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_GAIN_GREEN_WITH_COPY_FLAG,
                                    INT( gain ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_GAIN_GREEN, INT( gain ) ) );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of getting currently configured 
 *        blue gain from a provideo device.
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] gain     current blue gain
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_gain_blue
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
            CMD_GET_GAIN_BLUE, CMD_SYNC_GAIN_BLUE, CMD_SET_GAIN_BLUE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GAIN_RED_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *gain = UINT16( value );

    return ( 0 );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of setting blue gain to a 
 *        provideo device.
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[in]  gain     blue gain to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_gain_blue
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t const              gain
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_GAIN_BLUE_WITH_COPY_FLAG,
                                    INT( gain ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_GAIN_BLUE, INT( gain ) ) );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of getting currently configured 
 *        black level offset of red component from a provideo device.
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] offset   current red offset
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_black_red
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int16_t * const             offset
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !offset )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_BLACK_RED, CMD_SYNC_BLACK_RED, CMD_SET_BLACK_RED, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_BLACK_RED_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *offset = INT16( value );

    return ( 0 );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of setting black level offset of
 *        red component to a provideo device.
 *
 * @param[in]  com      com-port number to use
 * @param[in]  channel  control channel to send the request
 * @param[in]  offset   red black level to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_black_red
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int16_t const               offset
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_BLACK_RED_WITH_COPY_FLAG,
                                    INT( offset ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_BLACK_RED, INT( offset ) ) );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of getting currently configured 
 *        black level offset of green component from a provideo device.
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] offset   current green offset
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_black_green
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int16_t * const             offset
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !offset )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, 
            CMD_GET_BLACK_GREEN, CMD_SYNC_BLACK_GREEN, CMD_SET_BLACK_GREEN, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_BLACK_GREEN_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *offset = INT16( value );

    return ( 0 );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of setting black level offset of
 *        green component to a provideo device.
 *
 * @param[in]  com      com-port number to use
 * @param[in]  channel  control channel to send the request
 * @param[in]  offset   green black level to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_black_green
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int16_t const               offset
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_BLACK_GREEN_WITH_COPY_FLAG,
                                    INT( offset ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_BLACK_GREEN, INT( offset ) ) );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of getting currently configured 
 *        black level offset of blue component from a provideo device.
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] offset   current blue offset
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_black_blue
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int16_t * const             offset
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !offset )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_BLACK_BLUE, CMD_SYNC_BLACK_BLUE, CMD_SET_BLACK_BLUE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_BLACK_BLUE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *offset = INT16( value );

    return ( 0 );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of setting black level offset of
 *        blue component to a provideo device.
 *
 * @param[in]  com      com-port number to use
 * @param[in]  channel  control channel to send the request
 * @param[in]  offset   blue black level to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_black_blue
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int16_t const               offset
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_BLACK_BLUE_WITH_COPY_FLAG,
                                    INT( offset ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_BLACK_BLUE, INT( offset ) ) );
}

/******************************************************************************
 * get_flare - Get the flare factors for all colors
 *****************************************************************************/
static int get_flare
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint16_t * const            values
)
{
    (void) ctx;

    int v[CMD_FLARE_NO_PARMS];
    int res;

    // parameter check
    if ( !no || !values || (no != CMD_FLARE_NO_PARMS) )
    {
        return ( -EINVAL );
    }

    // command call to get 3 parameter from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_FLARE,
            CMD_SYNC_FLARE, CMD_SET_FLARE, &v[0u], &v[1u], &v[2u] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_FLARE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    values[0] = UINT16( v[0u] );
    values[1] = UINT16( v[1u] );
    values[2] = UINT16( v[2u] );

    return ( 0 );
}

/******************************************************************************
 * set_black_master - Set the black-level offset for all colors, pre-gamma
 *****************************************************************************/
static int set_flare
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint16_t * const            values
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    // parameter check
    if ( !no || !values || (no != CMD_FLARE_NO_PARMS) )
    {
        return ( -EINVAL );
    }

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_FLARE_WITH_COPY_FLAG,
                    INT( values[0] ), INT( values[1] ), INT( values[2] ),
                    INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_FLARE,
                INT( values[0] ), INT( values[1] ), INT( values[2] ) ) );
}

/******************************************************************************
 * get_black_master - Get the black-level offset for all colors, pre-gamma
 *****************************************************************************/
static int get_black_master
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    int16_t * const             values
)
{
    (void) ctx;

    int v[CMD_BLACK_MASTER_NO_PARMS];
    int res;

    // parameter check
    if ( !no || !values || (no != CMD_BLACK_MASTER_NO_PARMS) )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_BLACK_MASTER,
            CMD_SYNC_BLACK_MASTER, CMD_SET_BLACK_MASTER, &v[0u], &v[1u], &v[2u] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_BLACK_MASTER_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    values[0] = INT16( v[0u] );
    values[1] = INT16( v[1u] );
    values[2] = INT16( v[2u] );

    return ( 0 );
}

/******************************************************************************
 * set_black_master - Set the black-level offset for all colors, pre-gamma
 *****************************************************************************/
static int set_black_master
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    int16_t * const             values
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    // parameter check
    if ( !no || !values || (no != CMD_BLACK_MASTER_NO_PARMS) )
    {
        return ( -EINVAL );
    }

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_BLACK_MASTER_WITH_COPY_FLAG,
                    INT( values[0] ), INT( values[1] ), INT( values[2] ),
                    INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_BLACK_MASTER,
                INT( values[0] ), INT( values[1] ), INT( values[2] ) ) );
}

/******************************************************************************
 * get_filter_enable - Gets the filter functions flag
 *****************************************************************************/
static int get_filter_enable
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
    res = get_param_int_X_with_tmo( channel, 2, CMD_GET_FILTER_ENABLE,
            CMD_SYNC_FILTER_ENABLE, CMD_SET_FILTER_ENABLE, CMD_GET_FILTER_TMO, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_FILTER_ENABLE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *flag = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_filter_enable - Enable function for the filter functions
 *****************************************************************************/
static int set_filter_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               flag
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_FILTER_ENABLE_WITH_COPY_FLAG,
                                    INT( flag ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_FILTER_ENABLE, INT( flag ) ) );
}

/******************************************************************************
 * get_filter_detail - Get the detail enhance level
 *****************************************************************************/
static int get_filter_detail
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
    res = get_param_int_X( channel, 2, CMD_GET_FILTER_DETAIL,
            CMD_SYNC_FILTER_DETAIL, CMD_SET_FILTER_DETAIL, &value );
    
    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_FILTER_DETAIL_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *level = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_filter_detail - Set the detail enhance level
 *****************************************************************************/
static int set_filter_detail
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               level
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_FILTER_DETAIL_WITH_COPY_FLAG,
                                    INT( level ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_FILTER_DETAIL, INT( level ) ) );
}

/******************************************************************************
 * get_filter_denoise - Get the denoise level.
 *****************************************************************************/
static int get_filter_denoise
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
    res = get_param_int_X( channel, 2, CMD_GET_FILTER_DENOISE,
            CMD_SYNC_FILTER_DENOISE, CMD_SET_FILTER_DENOISE, &value );
    
    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_FILTER_DENOISE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *level = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_filter_denoise - Set the denoise level.
 *****************************************************************************/
static int set_filter_denoise
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               level
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_FILTER_DENOISE_WITH_COPY_FLAG,
                                    INT( level ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_FILTER_DENOISE, INT( level ) ) );
}

/******************************************************************************
 * get_color_conv - Gets the color conversion matrix
 *****************************************************************************/
static int get_color_conv
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                    no,
    int16_t * const              values
)
{
    (void) ctx;

    int v[CMD_COLOR_CONV_NO_PARMS];

    int res;
    
    // parameter check
    if ( !no || !values || (no != CMD_COLOR_CONV_NO_PARMS) )
    {
        return ( -EINVAL );
    }
    
    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_COLOR_CONV, CMD_SYNC_COLOR_CONV, CMD_SET_COLOR_CONV,
            &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6], &v[7], &v[8] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_COLOR_CONV_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    values[0] = INT16( v[0] );
    values[1] = INT16( v[1] );
    values[2] = INT16( v[2] );
    values[3] = INT16( v[3] );
    values[4] = INT16( v[4] );
    values[5] = INT16( v[5] );
    values[6] = INT16( v[6] );
    values[7] = INT16( v[7] );
    values[8] = INT16( v[8] );

    return ( 0 );
}

/******************************************************************************
 * set_color_conv - Sets the color conversion matrix.
 *****************************************************************************/
static int set_color_conv
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    int16_t * const             values
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    // parameter check
    if ( !no || !values || (no != CMD_COLOR_CONV_NO_PARMS) )
    {
        return ( -EINVAL );
    }

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_COLOR_CONV_WITH_COPY_FLAG,
                    INT( values[0] ), INT( values[1] ), INT( values[2] ),
                    INT( values[3] ), INT( values[4] ), INT( values[5] ),
                    INT( values[6] ), INT( values[7] ), INT( values[8] ),
                    INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_COLOR_CONV, 
                INT( values[0] ), INT( values[1] ), INT( values[2] ),
                INT( values[3] ), INT( values[4] ), INT( values[5] ),
                INT( values[6] ), INT( values[7] ), INT( values[8] ) ) );
}

/******************************************************************************
 * get_color_cross - Gets the color cross talk matrix
 *****************************************************************************/
static int get_color_cross
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                    no,
    int32_t * const              values
)
{
    (void) ctx;

    int v[CMD_COLOR_CROSS_NO_PARMS];

    int res;
    
    // parameter check
    if ( !no || !values || (no != CMD_COLOR_CROSS_NO_PARMS) )
    {
        return ( -EINVAL );
    }
    
    // clear local value buffer
    memset( v, 0, sizeof(v) );
    
    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_COLOR_CROSS, CMD_SYNC_COLOR_CROSS, CMD_SET_COLOR_CROSS,
            &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6], &v[7], &v[8] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_COLOR_CROSS_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    values[0] = INT32( v[0] );
    values[1] = INT32( v[1] );
    values[2] = INT32( v[2] );
    values[3] = INT32( v[3] );
    values[4] = INT32( v[4] );
    values[5] = INT32( v[5] );
    values[6] = INT32( v[6] );
    values[7] = INT32( v[7] );
    values[8] = INT32( v[8] );

    return ( 0 );
}

/******************************************************************************
 * set_color_cross - Sets the color cross talk matrix
 *****************************************************************************/
static int set_color_cross
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                    no,
    int32_t * const              values
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    // parameter check
    if ( !no || !values || (no != CMD_COLOR_CROSS_NO_PARMS) )
    {
        return ( -EINVAL );
    }

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_COLOR_CROSS_WITH_COPY_FLAG,
                    INT( values[0] ), INT( values[1] ), INT( values[2] ),
                    INT( values[3] ), INT( values[4] ), INT( values[5] ),
                    INT( values[6] ), INT( values[7] ), INT( values[8] ),
                    INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_COLOR_CROSS,
                INT( values[0] ), INT( values[1] ), INT( values[2] ),
                INT( values[3] ), INT( values[4] ), INT( values[5] ),
                INT( values[6] ), INT( values[7] ), INT( values[8] ) ) );
}

/******************************************************************************
 * get_color_cross_offset - Gets the color cross talk offset in the FPGA
 *****************************************************************************/
static int get_color_cross_offset
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    int16_t * const             values
)
{
    (void) ctx;

    int v[CMD_COLOR_CROSS_OFFSET_NO_PARMS];
    int res;

    // parameter check
    if ( !no || !values || (no != CMD_COLOR_CROSS_OFFSET_NO_PARMS) )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_COLOR_CROSS_OFFSET,
            CMD_SYNC_COLOR_CROSS_OFFSET, CMD_SET_COLOR_CROSS_OFFSET, &v[0u], &v[1u], &v[2u] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_COLOR_CROSS_OFFSET_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    values[0] = INT16( v[0u] );
    values[1] = INT16( v[1u] );
    values[2] = INT16( v[2u] );

    return ( 0 );
}

/******************************************************************************
 * set_color_cross_offset - Sets the color cross talk offset in the FPGA.
 *****************************************************************************/
static int set_color_cross_offset
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    int16_t * const             values
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    // parameter check
    if ( !no || !values || (no != CMD_COLOR_CROSS_OFFSET_NO_PARMS) )
    {
        return ( -EINVAL );
    }

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_COLOR_CROSS_OFFSET_WITH_COPY_FLAG,
                    INT( values[0] ), INT( values[1] ), INT( values[2] ),
                    INT( user->use_copy_flag ) ) );
    }
 
    return ( set_param_int_X( channel, CMD_SET_COLOR_CROSS_OFFSET,
                INT( values[0] ), INT( values[1] ), INT( values[2] ) ) );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of getting currently configured
 *        split-screen mode from provideo device.
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] enable   enable to get value
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_split_screen
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
            CMD_GET_SPLIT_SCREEN, CMD_SYNC_SPLIT_SCREEN, CMD_SET_SPLIT_SCREEN, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_SPLIT_SCREEN_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *enable = UINT8( value );

    return ( 0 );
}

/**************************************************************************//**
 * @brief Pro-Video protocol implementation of enabeling split-screen mode on a
 *        provideo device.
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[in]  enable   enable to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_split_screen
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               enable
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_SPLIT_SCREEN_WITH_COPY_FLAG,
                                    INT( enable ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_SPLIT_SCREEN, INT( enable ) ) );
}


/******************************************************************************
 * ISP protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_isp_drv_t provideo_isp_drv = 
{
    .get_lsc                = get_lsc,
    .set_lsc                = set_lsc,
    .get_bayer              = get_bayer,
    .set_bayer              = set_bayer,
    .get_gain_red           = get_gain_red,
    .set_gain_red           = set_gain_red,
    .get_gain_green         = get_gain_green,
    .set_gain_green         = set_gain_green,
    .get_gain_blue          = get_gain_blue,
    .set_gain_blue          = set_gain_blue,
    .get_black_red          = get_black_red,
    .set_black_red          = set_black_red,
    .get_black_green        = get_black_green,
    .set_black_green        = set_black_green,
    .get_black_blue         = get_black_blue,
    .set_black_blue         = set_black_blue,
    .get_flare              = get_flare,
    .set_flare              = set_flare,
    .get_black_master       = get_black_master,
    .set_black_master       = set_black_master,
    .get_filter_enable      = get_filter_enable,
    .set_filter_enable      = set_filter_enable,
    .get_filter_detail      = get_filter_detail,
    .set_filter_detail      = set_filter_detail,
    .get_filter_denoise     = get_filter_denoise,
    .set_filter_denoise     = set_filter_denoise,
    .get_color_conv         = get_color_conv,
    .set_color_conv         = set_color_conv,
    .get_color_cross        = get_color_cross,
    .set_color_cross        = set_color_cross,
    .get_color_cross_offset = get_color_cross_offset,
    .set_color_cross_offset = set_color_cross_offset,
    .get_split_screen       = get_split_screen,
    .set_split_screen       = set_split_screen,
};

/******************************************************************************
 * provideo_protocol_isp_init
 *****************************************************************************/
int provideo_protocol_isp_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_isp_register( handle, ctx, &provideo_isp_drv ) );
}

