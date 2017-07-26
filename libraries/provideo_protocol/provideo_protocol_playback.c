/******************************************************************************
 *
 * Copyright 2015, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
/**
 * @file    provideo_protocol_playback.c
 *
 * @brief   Implementation of playback- and record engine command functions 
 *          for a provideo host controller.
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_playback.h>

#include "provideo_protocol_common.h"

/******************************************************************************
 * @brief command "buffer_default"
 *****************************************************************************/
#define CMD_SET_BUFFER_DEFAULT              ( "buffer_default %i\n" )

/******************************************************************************
 * @brief command "buffer_set"
 *****************************************************************************/
#define CMD_SET_BUFFER_SET                  ( "buffer_set %i %i %i\n" )

/******************************************************************************
 * @brief command "buffer_list"
 *****************************************************************************/
#define CMD_GET_BUFFER_LISTn                ( "buffer_list\n" )
#define CMD_GET_BUFFER_LIST                 ( "buffer_list %i\n" )
#define CMD_GET_BUFFER_LIST_BUFFERn         ( "fb%i: %i..%i S:%s\n" )
#define CMD_GET_BUFFER_LIST_NO_PARMS        ( 4 )
#define CMD_SYNC_BUFFER_LIST_BUFFERn        ( "fb%i" )
#define CMD_SYNC_MAX_FRAMES                 ( "Max fames:" )
#define CMD_GET_MAX_FRAMES                  ( "Max fames:%i\n" )
#define CMD_MAX_FRAMES_NO_PARMS             ( 1 )

/******************************************************************************
 * @brief command "buffer_size"
 *****************************************************************************/
#define CMD_GET_BUFFER_SIZE                 ( "buffer_size %i\n" )
#define CMD_BUFFER_SIZE                     ( "%i\n" )
#define CMD_BUFFER_NO_PARM                  ( 1 )

/******************************************************************************
 * @brief command "play_mode" 
 *****************************************************************************/
#define CMD_GET_PLAY_MODE                   ( "play_mode\n" )
#define CMD_SET_PLAY_MODE                   ( "play_mode %i\n" )
#define CMD_SYNC_PLAY_MODE                  ( "play_mode " )
#define CMD_PLAY_MODE_NO_PARMS              ( 1 )

/******************************************************************************
 * @brief command "rec_mode" 
 *****************************************************************************/
#define CMD_GET_REC_MODE                    ( "rec_mode\n" )
#define CMD_SET_REC_MODE                    ( "rec_mode %i\n" )
#define CMD_SYNC_REC_MODE                   ( "rec_mode " )
#define CMD_REC_MODE_NO_PARMS               ( 1 )

/******************************************************************************
 * @brief command "rec_auto_live" 
 *****************************************************************************/
#define CMD_GET_REC_AUTO_LIVE               ( "rec_auto_live\n" )
#define CMD_SET_REC_AUTO_LIVE               ( "rec_auto_live %i\n" )
#define CMD_SYNC_REC_AUTO_LIVE              ( "rec_auto_live " )
#define CMD_REC_AUTO_LIVE_NO_PARMS          ( 1 )

/******************************************************************************
 * @brief command "rec" 
 *****************************************************************************/
#define CMD_GET_REC                         ( "rec\n" )
#define CMD_SET_REC                         ( "rec %i\n" )
#define CMD_SYNC_REC                        ( "rec " )
#define CMD_REC_NO_PARMS                    ( 1 )

/******************************************************************************
 * @brief command "rec_stop" 
 *****************************************************************************/
#define CMD_SET_REC_STOP                    ( "rec_stop\n" )

/******************************************************************************
 * @brief command "play" 
 *****************************************************************************/
#define CMD_GET_PLAY                        ( "play\n" )
#define CMD_SET_PLAY                        ( "play %i\n" )
#define CMD_SYNC_PLAY                       ( "play " )
#define CMD_PLAY_NO_PARMS                   ( 1 )

/******************************************************************************
 * @brief command "stop" 
 *****************************************************************************/
#define CMD_SET_PLAY_STOP                   ( "stop\n" )

/******************************************************************************
 * @brief command "pos" 
 *****************************************************************************/
#define CMD_GET_POS                         ( "pos\n" )
#define CMD_SET_POS                         ( "pos %i\n" )
#define CMD_SYNC_POS                        ( "pos " )
#define CMD_POS_NO_PARMS                    ( 1 )

/******************************************************************************
 * @brief command "pos" 
 *****************************************************************************/
#define CMD_SET_PAUSE                       ( "pause\n" )

/******************************************************************************
 * @brief command "paused" 
 *****************************************************************************/
#define CMD_GET_PAUSED                      ( "paused\n" )
#define CMD_SET_PAUSED                      ( "paused %i\n" )
#define CMD_SYNC_PAUSED                     ( "paused " )
#define CMD_PAUSED_NO_PARMS                 ( 1 )

/******************************************************************************
 * @brief command "forw" 
 *****************************************************************************/
#define CMD_SET_FORW                        ( "forw %i\n" )

/******************************************************************************
 * @brief command "rew" 
 *****************************************************************************/
#define CMD_SET_REW                         ( "rew %i\n" )

/******************************************************************************
 * @brief command "seek" 
 *****************************************************************************/
#define CMD_SET_SEEK                        ( "seek %i\n" )

/******************************************************************************
 * @brief command "live" 
 *****************************************************************************/
#define CMD_SET_LIVE                        ( "live\n" )

/******************************************************************************
 * @brief command "marker_out" 
 *****************************************************************************/
#define CMD_GET_MARKER_OUT                  ( "marker_out\n" )
#define CMD_SET_MARKER_OUT                  ( "marker_out %i\n" )
#define CMD_SYNC_MARKER_OUT                 ( "marker_out " )
#define CMD_MARKER_OUT_NO_PARMS             ( 1 )

/******************************************************************************
 * get_max_frames - Return maximal number of frames available on device
 *****************************************************************************/
static int get_max_frames
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            no
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !no )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_BUFFER_LISTn, CMD_SYNC_MAX_FRAMES, CMD_GET_MAX_FRAMES, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_MAX_FRAMES_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *no = UINT32( value );

    return ( 0 );
}

/******************************************************************************
 * set_buffer_default - Configure 'n' sequent frame buffers of equal size.
 *****************************************************************************/
static int set_buffer_default
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               no
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_BUFFER_DEFAULT, INT( no ) ) );
}

/******************************************************************************
 * get_buffer - Get frame buffer of specified ID
 *****************************************************************************/
static int get_buffer
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    char command[CMD_SINGLE_LINE_COMMAND_SIZE];
    char sync[CMD_SINGLE_LINE_COMMAND_SIZE];
    
    int res;

    int  id;
    int  first;
    int  last;
    char desc[40];
    
    ctrl_protocol_fb_t * fb;

    // parameter check
    if ( !no || !values || (no != sizeof(*fb)) )
    {
        return ( -EINVAL );
    }

    fb = (ctrl_protocol_fb_t *)values;

    // clear command buffer
    memset( command, 0, sizeof(command) );
    memset( sync, 0, sizeof(sync) );

    // create command to send
    sprintf( command, CMD_GET_BUFFER_LIST, fb->id );
    sprintf( sync, CMD_SYNC_BUFFER_LIST_BUFFERn, fb->id );

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            command, sync, CMD_GET_BUFFER_LIST_BUFFERn,
            &id, &first, &last, &desc );
    
    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_BUFFER_LIST_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // check buffer id
    if ( id != fb->id )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    fb->first = UINT32( first );
    fb->last  = UINT32( last );

    return ( 0 );
}

/******************************************************************************
 * set_buffer - Set frame buffer of specified ID
 *****************************************************************************/
static int set_buffer
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    ctrl_protocol_fb_t * fb;

    // parameter check
    if ( !no || !values || (no != sizeof(*fb)) )
    {
        return ( -EINVAL );
    }

    fb = (ctrl_protocol_fb_t *)values;

    return ( set_param_int_X( channel,
                CMD_SET_BUFFER_SET, INT( fb->id ), INT( fb->first ), INT( fb->last ) ) );
}

/******************************************************************************
 * get_buffer_size - Reports the buffer size (number of frames) for the
 *                   given frame-buffer
 *****************************************************************************/
int get_buffer_size
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    char command[CMD_SINGLE_LINE_COMMAND_SIZE];
    char data[CMD_SINGLE_LINE_RESPONSE_SIZE];

    int res;

    ctrl_protocol_fb_size_t * fb_size;

    // parameter check
    if ( !no || !values || (no != sizeof(*fb_size)) )
    {
        return ( -EINVAL );
    }

    fb_size = (ctrl_protocol_fb_size_t *)values;

    // clear command buffer
    memset( command, 0, sizeof(command) );

    // create command to send
    sprintf( command, CMD_GET_BUFFER_SIZE, fb_size->id );

    // send get-command to COM port
    ctrl_channel_send_request( channel, (uint8_t *)command, strlen(command) );

    // clear data buffer
    memset( data, 0, sizeof(data) );

    // read response from device
    res = evaluate_get_response( channel, data, sizeof(data) );
    if ( !res )
    {
        int v;

        res = sscanf( data, CMD_BUFFER_SIZE, &v );
        if ( res == CMD_BUFFER_NO_PARM )
        {
            fb_size->size = UINT32(v);
            return ( 0 );
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
 * get_play_mode - Gets the playback speed.
 *****************************************************************************/
static int get_play_mode
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
            CMD_GET_PLAY_MODE, CMD_SYNC_PLAY_MODE, CMD_SET_PLAY_MODE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_PLAY_MODE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_play_mode - Sets the playback speed.
 *****************************************************************************/
static int set_play_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_PLAY_MODE, INT( mode ) ) );
}

/******************************************************************************
 * get_record_mode - Gets the record mode.
 *****************************************************************************/
static int get_record_mode
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
            CMD_GET_REC_MODE, CMD_SYNC_REC_MODE, CMD_SET_REC_MODE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_REC_MODE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_record_mode - Set the record mode.
 *****************************************************************************/
static int set_record_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_REC_MODE, INT( mode ) ) );
}

/******************************************************************************
 * get_auto_live - Get auto live mode.
 *****************************************************************************/
static int get_auto_live
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
            CMD_GET_REC_AUTO_LIVE, CMD_SYNC_REC_AUTO_LIVE, CMD_SET_REC_AUTO_LIVE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_REC_AUTO_LIVE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_auto_live - Set auto live mode
 *****************************************************************************/
static int set_auto_live
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode 
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_REC_AUTO_LIVE, INT( mode ) ) );
}

/******************************************************************************
 * get_marker_out - Get out marker position (frame number)
 *****************************************************************************/
static int get_marker_out
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            pos
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !pos )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_MARKER_OUT, CMD_SYNC_MARKER_OUT, CMD_SET_MARKER_OUT, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_MARKER_OUT_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *pos = UINT32( value );

    return ( 0 );
}

/******************************************************************************
 * set_marker_out - Set out marker position (frame number)
 *****************************************************************************/
static int set_marker_out
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t const              pos
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_MARKER_OUT, INT( pos ) ) );
}

/******************************************************************************
 * get_play - Gets the current playback buffer.
 *****************************************************************************/
static int get_play
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             buf_id
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !buf_id )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_PLAY, CMD_SYNC_PLAY, CMD_SET_PLAY, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_PLAY_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *buf_id = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_play - Sets the playback from buffer.
 *****************************************************************************/
static int set_play
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               buf_id
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_PLAY, INT( buf_id ) ) );
}

/******************************************************************************
 * get_record - Gets the record buffer.
 *****************************************************************************/
static int get_record
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             buf_id
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !buf_id )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_REC, CMD_SYNC_REC, CMD_SET_REC, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_REC_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *buf_id = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_record - Starts a new recrod into buffer.
 *****************************************************************************/
static int set_record
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               buf_id
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_REC, INT( buf_id ) ) );
}

/******************************************************************************
 * stop_record - Stops a running record.
 *****************************************************************************/
static int stop_record
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0( channel, CMD_SET_REC_STOP ) );
}

/******************************************************************************
 * stop_play - Stops a running playback.
 *****************************************************************************/
static int stop_play
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0( channel, CMD_SET_PLAY_STOP ) );
}

/******************************************************************************
 * set_pause - Pauses a running playback.
 *****************************************************************************/
static int set_pause
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0( channel, CMD_SET_PAUSE ) );
}

/******************************************************************************
 * get_paused - Gets paused state of a playback
 *****************************************************************************/
static int get_paused
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
            CMD_GET_PAUSED, CMD_SYNC_PAUSED, CMD_SET_PAUSED, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_PAUSED_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *flag = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_live - Switches playback to current record
 *****************************************************************************/
static int set_live 
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0( channel, CMD_SET_LIVE ) );
}

/**************************************************************************//**
 * set_forward - Forwards play-back by specified number of frames
 *****************************************************************************/
static int set_forward
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t const              frames
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_FORW, INT( frames ) ) );
}

/**************************************************************************//**
 * set_rewind - Rewinds play-back by specified number of frames
 *****************************************************************************/
static int set_rewind
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t const              frames
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_REW, INT( frames ) ) );
}

/**************************************************************************//**
 * set_seek - Jumps to specified absolute position in play-back
 *****************************************************************************/
static int set_seek
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t const              pos
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_SEEK, INT( pos ) ) );
}

/**************************************************************************//**
 * get_position - Gets current play-back position (frame-number)
 *****************************************************************************/
static int get_position
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            pos
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !pos )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_POS, CMD_SYNC_POS, CMD_SET_POS, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_POS_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *pos = UINT32( value );

    return ( 0 );
}

/******************************************************************************
 * Playback protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_playback_drv_t provideo_playback_drv = 
{
    .get_max_frames     = get_max_frames,
    .set_buffer_default = set_buffer_default,
    .get_buffer         = get_buffer,
    .set_buffer         = set_buffer,
    .get_buffer_size    = get_buffer_size,
    .get_play_mode      = get_play_mode,
    .set_play_mode      = set_play_mode,
    .get_record_mode    = get_record_mode,
    .set_record_mode    = set_record_mode,
    .get_auto_live      = get_auto_live,
    .set_auto_live      = set_auto_live,
    .get_marker_out     = get_marker_out,
    .set_marker_out     = set_marker_out,
    .get_play           = get_play,
    .set_play           = set_play,
    .get_record         = get_record,
    .set_record         = set_record,
    .stop_record        = stop_record,
    .stop_play          = stop_play,
    .pause              = set_pause,
    .paused             = get_paused,
    .live               = set_live,
    .forward            = set_forward,
    .rewind             = set_rewind,
    .seek               = set_seek,
    .position           = get_position,
};

/******************************************************************************
 * provideo_protocol_playback_init
 *****************************************************************************/
int provideo_protocol_playback_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_playback_register( handle, ctx, &provideo_playback_drv ) );
}

