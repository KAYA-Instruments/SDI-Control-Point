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
 * @file    provideo_protocol_chain.c
 *
 * @brief   Implementation of provideo protocol chain functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_chain.h>

#include <provideo_protocol/provideo_protocol_common.h>

/******************************************************************************
 * @brief command "out" 
 *****************************************************************************/
#define CMD_GET_OUT                         ( "out\n" )
#define CMD_SET_OUT                         ( "out %i\n" )
#define CMD_SYNC_OUT                        ( "out " )
#define CMD_GET_OUT_NO_PARMS                ( 1 )

/******************************************************************************
 * @brief command "video_mode" 
 *****************************************************************************/
#define CMD_GET_VIDEO_MODE                  ( "video_mode\n" )
#define CMD_SET_VIDEO_MODE                  ( "video_mode %i\n" )
#define CMD_SYNC_VIDEO_MODE                 ( "video_mode " )
#define CMD_GET_VIDEO_MODE_NO_PARMS         ( 1 )
#define CMD_SET_VIDEO_MODE_TMO              ( 15000 )

/******************************************************************************
 * @brief command "raw" 
 *****************************************************************************/
#define CMD_GET_RAW                         ( "raw\n" )
#define CMD_SET_RAW                         ( "raw %i\n" )
#define CMD_SYNC_RAW                        ( "raw " )
#define CMD_GET_RAW_NO_PARMS                ( 1 )

/******************************************************************************
 * @brief command "sdi2" 
 *****************************************************************************/
#define CMD_GET_SDI2                        ( "sdi2\n" )
#define CMD_SET_SDI2                        ( "sdi2 %i\n" )
#define CMD_SYNC_SDI2                       ( "sdi2 " )
#define CMD_GET_SDI2_NO_PARMS               ( 1 )
#define CMD_SET_SDI2_TMO                    ( CMD_SET_VIDEO_MODE_TMO )  // SDI2 mode change internally requires vmode change on the device

/******************************************************************************
 * @brief command "downscale"
 *****************************************************************************/
#define CMD_GET_DOWNSCALE                   ( "downscale %i\n" )
#define CMD_SET_DOWNSCALE                   ( "downscale %i %i %i\n" )
#define CMD_SYNC_DOWNSCALE                  ( "downscale " )
#define CMD_GET_DOWNSCALE_NO_PARMS          ( 3 )
#define CMD_SET_DOWNSCALE_TMO               ( CMD_SET_VIDEO_MODE_TMO )  // Downscale mode change internally requires vmode change on the device

/******************************************************************************
 * @brief command "flip" 
 *****************************************************************************/
#define CMD_GET_FLIP                        ( "flip\n" )
#define CMD_SET_FLIP                        ( "flip %i\n" )
#define CMD_SYNC_FLIP                       ( "flip " )
#define CMD_GET_FLIP_NO_PARMS               ( 1 )

/******************************************************************************
 * @brief command "sdi_range" 
 *****************************************************************************/
#define CMD_GET_SDI_RANGE                   ( "sdi_range\n" )
#define CMD_SET_SDI_RANGE                   ( "sdi_range %i\n" )
#define CMD_SYNC_SDI_RANGE                  ( "sdi_range " )
#define CMD_GET_SDI_RANGE_NO_PARMS          ( 1 )

/******************************************************************************
 * @brief command "sdi_black" 
 *****************************************************************************/
#define CMD_GET_SDI_BLACK                   ( "sdi_black\n" )
#define CMD_SET_SDI_BLACK                   ( "sdi_black %i\n" )
#define CMD_SYNC_SDI_BLACK                  ( "sdi_black " )
#define CMD_GET_SDI_BLACK_NO_PARMS          ( 1 )

/******************************************************************************
 * @brief command "sdi_white" 
 *****************************************************************************/
#define CMD_GET_SDI_WHITE                   ( "sdi_white\n" )
#define CMD_SET_SDI_WHITE                   ( "sdi_white %i\n" )
#define CMD_SYNC_SDI_WHITE                  ( "sdi_white " )
#define CMD_GET_SDI_WHITE_NO_PARMS          ( 1 )

/******************************************************************************
 * @brief command "genlock" 
 *****************************************************************************/
#define CMD_GET_GENLOCK                     ( "genlock\n" )
#define CMD_SET_GENLOCK                     ( "genlock %i\n" )
#define CMD_SYNC_GENLOCK                    ( "genlock " )
#define CMD_GET_GENLOCK_NO_PARMS            ( 1 )
#define CMD_SET_GENLOCK_TMO                 ( 15000 )

/******************************************************************************
 * @brief command "genlock_offset" 
 *****************************************************************************/
#define CMD_GET_GENLOCK_OFFSET              ( "genlock_offset\n" )
#define CMD_SET_GENLOCK_OFFSET              ( "genlock_offset %i %i\n" )
#define CMD_SYNC_GENLOCK_OFFSET             ( "genlock_offset " )
#define CMD_GET_GENLOCK_OFFSET_NO_PARMS     ( 2 )
#define CMD_SET_GENLOCK_OFFSET_TMO          ( 15000 )

/******************************************************************************
 * @brief command "genlock_term" 
 *****************************************************************************/
#define CMD_GET_GENLOCK_TERM                ( "genlock_term\n" )
#define CMD_SET_GENLOCK_TERM                ( "genlock_term %i\n" )
#define CMD_SYNC_GENLOCK_TERM               ( "genlock_term " )
#define CMD_GET_GENLOCK_TERM_NO_PARMS       ( 1 )
#define CMD_SET_GENLOCK_TERM_TMO            ( 15000 )

/******************************************************************************
 * @brief command "timecode" 
 *****************************************************************************/
#define CMD_GET_TIMECODE                    ( "timecode\n" )
#define CMD_SET_TIMECODE                    ( "timecode %i %i %i\n" )
#define CMD_SYNC_TIMECODE                   ( "timecode " )
#define CMD_GET_TIMECODE_NO_PARAMS          ( 3 )

/******************************************************************************
 * @brief command "timecode_hold"
 *****************************************************************************/
#define CMD_GET_TIMECODE_HOLD               ( "timecode_hold\n" )
#define CMD_SET_TIMECODE_HOLD               ( "timecode_hold %i\n" )
#define CMD_SYNC_TIMECODE_HOLD              ( "timecode_hold " )
#define CMD_GET_TIMECODE_HOLD_NO_PARAMS     ( 1 )

/******************************************************************************
 * @brief command "audio_enable"
 *****************************************************************************/
#define CMD_GET_AUDIO_ENABLE                ( "audio_enable\n" )
#define CMD_SET_AUDIO_ENABLE                ( "audio_enable %i\n" )
#define CMD_SYNC_AUDIO_ENABLE               ( "audio_enable " )
#define CMD_GET_AUDIO_ENABLE_NO_PARAMS      ( 1 )

/******************************************************************************
 * @brief command "copy_settings" 
 *****************************************************************************/
#define CMD_COPY_SETTINGS                   ( "copy_settings %i %i\n" )

/******************************************************************************
 * get_output_chain - Gets current selected video chain
 *****************************************************************************/
static int get_output_chain
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             chain
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !chain )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_OUT, CMD_SYNC_OUT, CMD_SET_OUT, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_OUT_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *chain = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_output_chain - select the video chain, next commands should applied to
 *****************************************************************************/
static int set_output_chain
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               chain
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_OUT, INT( chain ) ) );
}

/******************************************************************************
 * get_video_mode - Gets the output video mode of both output channel
 *****************************************************************************/
static int get_video_mode
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
            CMD_GET_VIDEO_MODE, CMD_SYNC_VIDEO_MODE, CMD_SET_VIDEO_MODE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_VIDEO_MODE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_video_mode - Set the output video mode of both output channel
 *****************************************************************************/
static int set_video_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode
)
{
    (void) ctx;

    return ( set_param_int_X_with_tmo( channel, CMD_SET_VIDEO_MODE, CMD_SET_VIDEO_MODE_TMO, INT( mode ) ) );
}

/******************************************************************************
 * get_raw_mode - gets the RAW output mode
 *****************************************************************************/
static int get_raw_mode
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
            CMD_GET_RAW, CMD_SYNC_RAW, CMD_SET_RAW, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_RAW_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_raw_mode - Set the RAW output mode
 *****************************************************************************/
static int set_raw_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_RAW, INT( mode ) ) );
}

/******************************************************************************
 * get_sdi2_mode - gets the SDI-2 output mode
 *****************************************************************************/
static int get_sdi2_mode
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
            CMD_GET_SDI2, CMD_SYNC_SDI2, CMD_SET_SDI2, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_SDI2_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_sdi2_mode - Set the SDI-2 output mode
 *****************************************************************************/
static int set_sdi2_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode
)
{
    (void) ctx;

    return ( set_param_int_X_with_tmo( channel, CMD_SET_SDI2, CMD_SET_SDI2_TMO, INT( mode ) ) );
}

/******************************************************************************
 * get_downscale_mode - gets the downscale mode
 *****************************************************************************/
static int get_downscale_mode
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

    ctrl_protocol_downscale_enable_t * enable;

    int res;

    // parameter check
    if ( !no || !values || (sizeof(*enable) != no) )
    {
        return ( -EINVAL );
    }

    enable = (ctrl_protocol_downscale_enable_t *)values;

    // clear command buffer
    memset( command, 0, sizeof(command) );

    sprintf( command, CMD_GET_DOWNSCALE, enable->id );

    // send command to COM port
    ctrl_channel_send_request( channel, (uint8_t *)command, strlen(command) );

    // read response from provideo device
    res = evaluate_get_response( channel, data, sizeof(data) );
    if ( !res )
    {
        char * s = strstr( data, CMD_SYNC_DOWNSCALE );
        if ( s )
        {
            int v0, v1, v2;
            res = sscanf( s, CMD_SET_DOWNSCALE, &v0, &v1, &v2 );
            if ( (res == CMD_GET_DOWNSCALE_NO_PARMS) && (UINT8(v0) == enable->id) )
            {
                enable->downscale = UINT8( v1 );
                enable->interlace = UINT8( v2 );
                return ( 0 );
            }
            else
            {
                return ( -EINVAL );
            }
        }
    }
    else
    {
        res = evaluate_error_response( data, res );
    }

    if ( res < 0 )
    {
        return ( res );
    }

    return ( 0 );
}

/******************************************************************************
 * set_downscale_mode - Set the downscale mode
 *****************************************************************************/
static int set_downscale_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    ctrl_protocol_downscale_enable_t * enable;

    // parameter check
    if ( !no || !values || (sizeof(*enable) != no) )
    {
        return ( -EINVAL );
    }

    enable = (ctrl_protocol_downscale_enable_t *)values;

    return ( set_param_int_X( channel,
                CMD_SET_DOWNSCALE, INT( enable->id ), INT( enable->downscale ), INT( enable->interlace ) ) );
}

/******************************************************************************
 * get_flip - gets the flip-mode
 *****************************************************************************/
static int get_flip_mode
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
            CMD_GET_FLIP, CMD_SYNC_FLIP, CMD_SET_FLIP, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_FLIP_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_flip - sets the flip mode
 *****************************************************************************/
static int set_flip_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_FLIP, INT( mode ) ) );
}

/******************************************************************************
 * get_genlock_mode - gets the genlock mode
 *****************************************************************************/
static int get_genlock_mode
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
            CMD_GET_GENLOCK, CMD_SYNC_GENLOCK, CMD_SET_GENLOCK, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_GENLOCK_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_genlock_mode - set genlock mode
 *****************************************************************************/
static int set_genlock_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode
)
{
    (void) ctx;

    return ( set_param_int_X_with_tmo( channel, CMD_SET_GENLOCK, CMD_SET_GENLOCK_TMO, INT( mode ) ) );
}

/******************************************************************************
 * get_genlock_offset - gets the genlock offset 
 *****************************************************************************/
static int get_genlock_offset
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    int16_t * const             values
)
{
    (void) ctx;

    int vertical, horizontal;
    int res;

    // parameter check
    if ( !values )
    {
        return ( -EINVAL );
    }

    if ( no != CMD_GET_GENLOCK_OFFSET_NO_PARMS )
    {
        // return -EFAULT if number of parameter not matching
        return ( -EFAULT );
    }

    // command call to get 2 parameters from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_GENLOCK_OFFSET, CMD_SYNC_GENLOCK_OFFSET, CMD_SET_GENLOCK_OFFSET,
            &vertical, &horizontal );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_GENLOCK_OFFSET_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    values[0] = UINT16( vertical );
    values[1] = UINT16( horizontal );

    return ( 0 );
}

/******************************************************************************
 * set_genlock_offset - sets genlock offset
 *****************************************************************************/
static int set_genlock_offset
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    int16_t * const             values
)
{
    (void) ctx;

    if ( no != CMD_GET_GENLOCK_OFFSET_NO_PARMS )
    {
        // return -EFAULT if number of parameter not matching
        return ( -EFAULT );
    }

    return ( set_param_int_X_with_tmo( channel, CMD_SET_GENLOCK_OFFSET, CMD_SET_GENLOCK_OFFSET_TMO,
                                       INT( values[0] ), INT( values[1] ) ) );
}

/******************************************************************************
 * get_genlock_termination - gets the genlock termination
 *****************************************************************************/
static int get_genlock_termination
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
            CMD_GET_GENLOCK_TERM, CMD_SYNC_GENLOCK_TERM, CMD_SET_GENLOCK_TERM, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_GENLOCK_TERM_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_genlock_termination - set genlock termination
 *****************************************************************************/
static int set_genlock_termination
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode
)
{
    (void) ctx;

    return ( set_param_int_X_with_tmo( channel, CMD_SET_GENLOCK_TERM, CMD_SET_GENLOCK_TERM_TMO, INT( mode ) ) );
}

/******************************************************************************
 * get_sdi_range - Gets the YUV legalizer mode for the SDI output.
 *****************************************************************************/
static int get_sdi_range
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
            CMD_GET_SDI_RANGE, CMD_SYNC_SDI_RANGE, CMD_SET_SDI_RANGE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_SDI_RANGE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_sdi_range - Sets the YUV legalizer mode for the SDI output.
 *****************************************************************************/
static int set_sdi_range
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_SDI_RANGE, INT( mode ) ) );
}

/******************************************************************************
 * get_sdi_black - Gets the Y offset for the SDI output.
 *****************************************************************************/
static int get_sdi_black
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int8_t * const              offset
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
            CMD_GET_SDI_BLACK, CMD_SYNC_SDI_BLACK, CMD_SET_SDI_BLACK, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_SDI_BLACK_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *offset = INT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_sdi_black - Sets the Y offset for the SDI output.
 *****************************************************************************/
static int set_sdi_black
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int8_t const                offset
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_SDI_BLACK, INT( offset ) ) );
}

/******************************************************************************
 * get_sdi_white - Gets the Y offset for the SDI output.
 *****************************************************************************/
static int get_sdi_white
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int8_t * const              offset
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
            CMD_GET_SDI_WHITE, CMD_SYNC_SDI_WHITE, CMD_SET_SDI_WHITE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_SDI_WHITE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *offset = INT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_sdi_white - Sets the Y offset for the SDI output.
 *****************************************************************************/
static int set_sdi_white
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int8_t const                offset
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_SDI_WHITE, INT( offset ) ) );
}

/******************************************************************************
 * get_timecode - Gets current timecode
 *****************************************************************************/
static int get_timecode
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

    if ( no != CMD_GET_TIMECODE_NO_PARAMS )
    {
        // return -EFAULT if number of parameter not matching
        return ( -EFAULT );
    }

    // command call to get 3 parameters from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_TIMECODE, CMD_SYNC_TIMECODE, CMD_SET_TIMECODE,
            &values[0], &values[1], &values[2]
          );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }
    
    // return -EFAULT if number of returned parameters not matching
    else if ( res != CMD_GET_TIMECODE_NO_PARAMS )
    {
        return ( -EFAULT );
    }

    return ( 0 );
}

/******************************************************************************
 * set_timecode - Sets timecode
 *****************************************************************************/
static int set_timecode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no, 
    int32_t * const             values
)
{
    (void) ctx;

    // parameter check
    if ( !values )
    {
        return ( -EINVAL );
    }

    if ( no != CMD_GET_TIMECODE_NO_PARAMS )
    {
        // return -EFAULT if number of parameter not matching
        return ( -EFAULT );
    }

    return ( set_param_int_X( channel, CMD_SET_TIMECODE, values[0], values[1], values[2] ) );
}

/******************************************************************************
 * get_timecode_hold - Gets the timecode hold flag.
 *****************************************************************************/
static int get_timecode_hold
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
            CMD_GET_TIMECODE_HOLD, CMD_SYNC_TIMECODE_HOLD, CMD_SET_TIMECODE_HOLD, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_TIMECODE_HOLD_NO_PARAMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *enable = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_sdi_white - Sets the timecode hold flag.
 *****************************************************************************/
static int set_timecode_hold
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               enable
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_TIMECODE_HOLD, INT( enable ) ) );
}

/******************************************************************************
 * get_audio_enable - Gets the audio enable flag.
 *****************************************************************************/
static int get_audio_enable
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
            CMD_GET_AUDIO_ENABLE, CMD_SYNC_AUDIO_ENABLE, CMD_SET_AUDIO_ENABLE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_AUDIO_ENABLE_NO_PARAMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *enable = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_audio_enable - Sets the audio enable flag.
 *****************************************************************************/
static int set_audio_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               enable
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_AUDIO_ENABLE, INT( enable ) ) );
}

/******************************************************************************
 * CHAIN protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_chain_drv_t provideo_chain_drv = 
{
    .get_output_chain        = get_output_chain,
    .set_output_chain        = set_output_chain,
    .get_video_mode          = get_video_mode,
    .set_video_mode          = set_video_mode,
    .get_raw_mode            = get_raw_mode,
    .set_raw_mode            = set_raw_mode,
    .get_sdi2_mode           = get_sdi2_mode,
    .set_sdi2_mode           = set_sdi2_mode,
    .get_downscale_mode      = get_downscale_mode,
    .set_downscale_mode      = set_downscale_mode,
    .get_flip_mode           = get_flip_mode,
    .set_flip_mode           = set_flip_mode,
    .get_sdi_range           = get_sdi_range,
    .set_sdi_range           = set_sdi_range,
    .get_sdi_black           = get_sdi_black,
    .set_sdi_black           = set_sdi_black,
    .get_sdi_white           = get_sdi_white,
    .set_sdi_white           = set_sdi_white,
    .get_genlock_mode        = get_genlock_mode,
    .set_genlock_mode        = set_genlock_mode,
    .get_genlock_offset      = get_genlock_offset,
    .set_genlock_offset      = set_genlock_offset,
    .get_genlock_termination = get_genlock_termination,
    .set_genlock_termination = set_genlock_termination,
    .get_timecode            = get_timecode,
    .set_timecode            = set_timecode,
    .get_timecode_hold       = get_timecode_hold,
    .set_timecode_hold       = set_timecode_hold,
    .get_audio_enable        = get_audio_enable,
    .set_audio_enable        = set_audio_enable
};

/******************************************************************************
 * provideo_protocol_chain_init
 *****************************************************************************/
int provideo_protocol_chain_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_chain_register( handle, ctx, &provideo_chain_drv ) );
}

