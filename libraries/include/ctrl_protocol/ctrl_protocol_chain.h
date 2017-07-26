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
 * @file    ctrl_protocol_chain.h
 *
 * @brief   Generic chain control protocol functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_CHAIN_H__
#define __CTRL_PROTOCOL_CHAIN_H__

#include <stdint.h>

#include <ctrl_channel/ctrl_channel.h>
#include <ctrl_protocol/ctrl_protocol.h>

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @brief Gets currently selected video chain.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  chidx    currently selected video output chain on device
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_output_chain
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              chidx
);

/**************************************************************************//**
 * @brief Selects the video chain.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   chidx    video output chain to select
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_output_chain
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                chidx
);

/**************************************************************************//**
 * @brief video mode 
 *****************************************************************************/
enum VIDEO_MODE
{
    VIDEO_MODE_FIRST        = 1,
    VIDEO_MODE_720p60       = 1,    /**<  720p, 60 fps    */
    VIDEO_MODE_720p50       = 2,    /**<  720p, 50 fps    */
    VIDEO_MODE_720p59_94    = 3,    /**<  720p, 59.94 fps */
    VIDEO_MODE_1080p30      = 4,    /**< 1080p, 30 fps    */
    VIDEO_MODE_1080p25      = 5,    /**< 1080p, 25 fps    */
    VIDEO_MODE_1080p24      = 6,    /**< 1080p, 24 fps    */
    VIDEO_MODE_1080p23_98   = 7,    /**< 1080p, 23.98 fps */
    VIDEO_MODE_1080p29_97   = 8,    /**< 1080p, 29.97 fps */
    VIDEO_MODE_1080p50      = 9,    /**< 1080p, 50 fps    */
    VIDEO_MODE_1080p60      = 10,   /**< 1080p, 60 fps    */
    VIDEO_MODE_1080i60      = 11,   /**< 1080i, 60 fps    */
    VIDEO_MODE_1080i50      = 12,   /**< 1080i, 50 fps    */
    VIDEO_MODE_1080i59_94   = 13,   /**< 1080i, 59.94 fps */
    VIDEO_MODE_MAX                  /**< number of normally supported modes (end marker) */
};

/**************************************************************************//**
 * @brief Gets the current video output mode.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  mode     current video mode
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_video_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode
);

/**************************************************************************//**
 * @brief Sets video output mode.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   mode     video output mode to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_video_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
);

/**************************************************************************//**
 * @brief Gets the current raw output mode.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  mode     current video mode
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_raw_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode
);

/**************************************************************************//**
 * @brief Sets raw output mode.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   mode     video output mode to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_raw_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
);

/**************************************************************************//**
 * @brief Gets the current sdi2 output mode.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  mode     current video mode
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_sdi2_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode
);

/**************************************************************************//**
 * @brief Sets sdi2 output mode.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   mode     video output mode to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_sdi2_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
);

/**************************************************************************//**
 * @brief Gets the FLIP mode.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  mode     current flip mode
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_flip_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode
);

/**************************************************************************//**
 * @brief Sets the FLIP mode.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   mode     enable/disable flip mode (0:off, ...)
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_flip_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
);

/**************************************************************************//**
 * @brief SDI output range
 *****************************************************************************/
enum SDI_RANGE
{
    SDI_RANGE_FIRST    = 0,
    SDI_RANGE_LEGAL    = 0, /**< legal range from 64 .. Y_max=940/C_max=960 on 10 Bits */
    SDI_RANGE_EXTENDED = 1, /**< extended range from 4 .. 1019 on 10 Bits */
    SDI_RANGE_MAX
};

/**************************************************************************//**
 * @brief Gets the YUV legalizer mode for the SDI output.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  mode     current mode
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_sdi_range
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode
);

/**************************************************************************//**
 * @brief Sets the YUV legalizer mode for the SDI output.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   mode     mode to set (0: legal, 1: extended)
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_sdi_range
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
);

/**************************************************************************//**
 * @brief Gets the SDI Y offset (lower border of Y)
 *
 * @note  In SDI legal mode (Y ranges from 64 to 940) the Y_min = 64 + Y_offset
 *        Effectless in SDI extended mode (Y ranges from 4 to 1019).
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] offset   current Y offset
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_sdi_black
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int8_t * const               offset
);

/**************************************************************************//**
 * @brief Sets the SDI Y offset (lower border of Y)
 *
 * @note  In SDI legal mode (Y ranges from 64 to 940) the Y_min = 64 + Y_offset
 *        Effectless in SDI extended mode (Y ranges from 4 to 1019).
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  offset   Y offset to set ( -60 <= offset <= +60 )
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_sdi_black
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int8_t const                 offset
);

/**************************************************************************//**
 * @brief Gets the SDI Y offset (upper border of Y)
 *
 * @note  In SDI legal mode (Y ranges from 64 to 940) the Y_max = 940 + Y_offset
 *        Effectless in SDI extended mode (Y ranges from 4 to 1019).
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  offset   current Y offset
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_sdi_white
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int8_t * const               offset
);

/**************************************************************************//**
 * @brief Sets the SDI Y offset (upper border of Y)
 *
 * @note  In SDI legal mode (Y ranges from 64 to 940) the Y_max = 940 + Y_offset
 *        Effectless in SDI extended mode (Y ranges from 4 to 1019).
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  offset   Y offset to set ( -80 <= offset <= 79 )
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_sdi_white
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int8_t const                 offset
);

/**************************************************************************//**
 * @brief Gets current gen-lock mode 
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  mode     current gen-lock mode
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_genlock_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode
);

/**************************************************************************//**
 * @brief Sets gen-lock mode
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  mode     gen-lock mode to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_genlock_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode
);

/**************************************************************************//**
 * @brief Gets current gen-lock offset 
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of array/list items
 * @param[out] values   array (memory pointer) to fill
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_genlock_offset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int16_t * const              values
);

/**************************************************************************//**
 * @brief Sets gen-lock offset
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of array/list items
 * @param[out] values   array (memory pointer) to fill
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_genlock_offset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int16_t * const              values
);

/**************************************************************************//**
 * @brief Gets current gen-lock termination status
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  flag     current gen-lock termination status 
 *                      (0: disabled, 1:enabled)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_genlock_termination
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              flag
);

/**************************************************************************//**
 * @brief Sets gen-lock termination status
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  flag gen-lock termination to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_genlock_termination
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                flag
);

/**************************************************************************//**
 * @brief Number of time code values
 *****************************************************************************/
#define NO_VALUES_TIMECODE             ( 3 ) 

/**************************************************************************//**
 * @brief Gets the current timecode
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of array/list items
 * @param[out]  values   array (memory pointer) to fill
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_timecode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no, 
    int32_t * const              values
);

/**************************************************************************//**
 * @brief Sets timecode
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of array/list items
 * @param[out]  values   array (memory pointer) to fill
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_timecode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no, 
    int32_t * const              values
);

/**************************************************************************//**
 * @brief CHAIN protocol driver implementation
 *****************************************************************************/
typedef struct ctrl_protocol_chain_drv_s
{
    ctrl_protocol_get_uint8_t    get_output_chain;
    ctrl_protocol_set_uint8_t    set_output_chain;
    ctrl_protocol_get_uint8_t    get_video_mode;
    ctrl_protocol_set_uint8_t    set_video_mode;
    ctrl_protocol_get_uint8_t    get_raw_mode;
    ctrl_protocol_set_uint8_t    set_raw_mode;
    ctrl_protocol_get_uint8_t    get_sdi2_mode;
    ctrl_protocol_set_uint8_t    set_sdi2_mode;
    ctrl_protocol_get_uint8_t    get_flip_mode;
    ctrl_protocol_set_uint8_t    set_flip_mode;
    ctrl_protocol_get_uint8_t    get_genlock_mode;
    ctrl_protocol_set_uint8_t    set_genlock_mode;
    ctrl_protocol_int16_array_t get_genlock_offset;
    ctrl_protocol_int16_array_t set_genlock_offset;
    ctrl_protocol_get_uint8_t    get_genlock_termination;
    ctrl_protocol_set_uint8_t    set_genlock_termination;
    ctrl_protocol_get_uint8_t    get_sdi_range;
    ctrl_protocol_set_uint8_t    set_sdi_range;
    ctrl_protocol_get_int8_t     get_sdi_black;
    ctrl_protocol_set_int8_t     set_sdi_black;
    ctrl_protocol_get_int8_t     get_sdi_white;
    ctrl_protocol_set_int8_t     set_sdi_white;
    ctrl_protocol_int32_array_t  get_timecode;
    ctrl_protocol_int32_array_t  set_timecode;
} ctrl_protocol_chain_drv_t;

/**************************************************************************//**
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_chain_register
(
    ctrl_protocol_handle_t const        handle,
    void * const                        ctx,
    ctrl_protocol_chain_drv_t * const   drv
);

/**************************************************************************//**
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_chain_unregister
(
    ctrl_protocol_handle_t const handle
);

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_CHAIN_H__ */

