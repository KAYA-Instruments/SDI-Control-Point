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
 * @file    ctrl_protocol_playback.h
 *
 * @brief   Generic playback- and record engine control protocol functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_PLAYBACK_H__
#define __CTRL_PROTOCOL_PLAYBACK_H__

#include <stdint.h>

#include <ctrl_channel/ctrl_channel.h>
#include <ctrl_protocol/ctrl_protocol.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * @defgroup ctrl_protocol_layer Generic Control Protocol Layer implementation
 * @{
 *****************************************************************************/

/**************************************************************************//**
 * @brief Returns the maximal/available number of frames on device. 
 *        (size of capture memory in frames)
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] no       number of frames
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_max_frames
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             no 
);

/**************************************************************************//**
 * @brief Configures 'no' sequent frame buffers of equal size. 
 *
 * @note Start- and end/last-frame are device internally computed.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of frame buffers to use
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_buffer_default
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                no 
);

/**************************************************************************//**
 * @brief Frame buffer type
 *****************************************************************************/
typedef struct ctrl_protocol_fb_s
{
    uint8_t     id;                 /**< frame buffer identifier */
    uint32_t    first;              /**< start frame */
    uint32_t    last;               /**< end/last frame */
    char        desc[40];           /**< ussage description */
} ctrl_protocol_fb_t;

/**************************************************************************//**
 * @brief Get current configuration of the frame buffer.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] no       number of bytes, shall be equal to
 *                      sizeof(ctrl_protocol_fb_t)
 * @param[out] values   array pointer
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_buffer
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Set configuration of the frame buffer.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] no       number of bytes, shall be equal to
 *                      sizeof(ctrl_protocol_fb_t)
 * @param[out] values   array pointer
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_buffer
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Frame buffer type
 *****************************************************************************/
typedef struct ctrl_protocol_fb_size_s
{
    uint8_t     id;                 /**< frame buffer identifier */
    uint32_t    size;               /**< size inf number of frames */
} ctrl_protocol_fb_size_t;

/**************************************************************************//**
 * @brief Get current configuration of the frame buffer.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] no       number of bytes, shall be equal to
 *                      sizeof(ctrl_protocol_fb_size_t)
 * @param[out] values   array pointer
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_buffer_size
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Get play mode of playback engine.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   mode     current playback mode
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_play_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode 
);

/**************************************************************************//**
 * @brief Set play mode of record engine.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   mode     play mode to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_play_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode 
);

/**************************************************************************//**
 * @brief Get record mode of record engine.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   mode     current recording mode
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_record_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode 
);

/**************************************************************************//**
 * @brief Set record mode of record engine.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   mode     mode 0 => Record once (stops at end of buffer)
 *                       mode 1 => Record loop (ring buffer)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_record_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode 
);

/**************************************************************************//**
 * @brief Get auto live mode.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   mode     current auto-live mode
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_auto_live
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode 
);

/**************************************************************************//**
 * @brief Set auto live mode.
 *
 * @note Auto-live switches play-back engine to a new record automatically
 *       (without any interaction).
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   mode     mode 0 => disable auto-live
 *                       mode 1 => enable auto-live
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_auto_live
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode 
);

/**************************************************************************//**
 * @brief Gets out marker position (frame number)
 *
 * @note An out marker is used to automatically pause a play-back at a 
 *       specific frame-number. The out marker is a one-shot marker, which 
 *       means it's cleared after hitting.
 * 
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   pos      current out marker position
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_marker_out
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             pos
);

/**************************************************************************//**
 * @brief Set out marker position (frame number)
 *
 * @note An out marker is used to automatically pause a play-back at a 
 *       specific frame-number. The out marker is a one-shot marker, which 
 *       means it's cleared after hitting.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   pos      out marker position to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_marker_out
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               pos
);

/**************************************************************************//**
 * @brief Gets the current playback buffer, fails if no playback running. 
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   buf_id   current playback buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_play
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              buf_id 
);

/**************************************************************************//**
 * @brief Starts a new playback from selected buffer.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   buf_id   playback buffer to play from
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_play
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                buf_id
);

/**************************************************************************//**
 * @brief Gets the current record buffer, fails if no record running.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   buf_id   current record buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_record
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              buf_id 
);

/**************************************************************************//**
 * @brief Starts a new record in selected buffer.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   buf_id   record buffer to use
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_record
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                buf_id
);

/**************************************************************************//**
 * @brief Stops a running record.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_stop_record
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Stops a running playback.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_stop_play
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Pauses a running playback.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_pause
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Gets paused state of a running playback.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  flag     paused-state (1: paused, 0: running)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_paused
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              flag
);

/**************************************************************************//**
 * @brief Switches playback to a running record.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_live
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Forwards play-back by specified number of frames
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   frames   number of frames to forward 
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_forward
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               frames
);

/**************************************************************************//**
 * @brief Rewinds play-back by specified number of frames
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   frames   number of frames to rewind 
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_rewind
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               frames
);

/**************************************************************************//**
 * @brief Jumps to specified absolute position in play-back
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   pos      jump position (frame number)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_seek
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               pos
);

/**************************************************************************//**
 * @brief Gets current play-back position (frame-number)
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   pos      current play-back position
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_position
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             pos
);

/**************************************************************************//**
 * @brief Playback engine protocol driver implementation
 *****************************************************************************/
typedef struct ctrl_protocol_playback_drv_s
{
    // buffer control
    ctrl_protocol_get_uint32_t  get_max_frames;
    ctrl_protocol_set_uint8_t   set_buffer_default;
    ctrl_protocol_uint8_array_t get_buffer;
    ctrl_protocol_uint8_array_t set_buffer;
    ctrl_protocol_uint8_array_t get_buffer_size;
    // playback/record control
    ctrl_protocol_get_uint8_t   get_play_mode;
    ctrl_protocol_set_uint8_t   set_play_mode;
    ctrl_protocol_get_uint8_t   get_record_mode;
    ctrl_protocol_set_uint8_t   set_record_mode;
    ctrl_protocol_get_uint8_t   get_auto_live;
    ctrl_protocol_set_uint8_t   set_auto_live;
    ctrl_protocol_get_uint32_t  get_marker_out;
    ctrl_protocol_set_uint32_t  set_marker_out;
    ctrl_protocol_get_uint8_t   get_play;
    ctrl_protocol_set_uint8_t   set_play;
    ctrl_protocol_get_uint8_t   get_record;
    ctrl_protocol_set_uint8_t   set_record;
    ctrl_protocol_run_t         stop_record;
    ctrl_protocol_run_t         stop_play;
    ctrl_protocol_run_t         pause;
    ctrl_protocol_get_uint8_t   paused;
    ctrl_protocol_run_t         live;
    ctrl_protocol_set_uint32_t  forward;
    ctrl_protocol_set_uint32_t  rewind;
    ctrl_protocol_set_uint32_t  seek;
    ctrl_protocol_get_uint32_t  position;
} ctrl_protocol_playback_drv_t;

/**************************************************************************//**
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_playback_register
(
    ctrl_protocol_handle_t const            handle,
    void * const                            ctx,
    ctrl_protocol_playback_drv_t * const    drv
);

/**************************************************************************//**
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_playback_unregister
(
    ctrl_protocol_handle_t const handle
);

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_PLAYBACK_H__ */

