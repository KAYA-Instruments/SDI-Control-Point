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
 * @brief downscale enable struct
 *****************************************************************************/
typedef struct ctrl_protocol_downscale_enable_s
{
    uint8_t     id;         /**< sdi out identifier */
    uint8_t     downscale;  /**< downscale enable status */
    uint8_t     interlace;  /**< interlace enable status */
} ctrl_protocol_downscale_enable_t;

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
 * @brief Gets current downscale mode
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of array/list items
 * @param[out] values   array (memory pointer) to fill
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_downscale_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Sets downscale mode
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of array/list items
 * @param[out] values   array (memory pointer) to fill
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_downscale_mode
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
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
 * @brief Gets current gen-lock status
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  mode     current gen-lock status
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_genlock_status
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              status
);

/**************************************************************************//**
 * @brief Gets current gen-lock crosslock settings
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of array/list items
 * @param[out] values   array (memory pointer) to fill
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_genlock_crosslock
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const                vmode
);

/**************************************************************************//**
 * @brief Sets gen-lock crosslock settings
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of array/list items
 * @param[out] values   array (memory pointer) to fill
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_genlock_crosslock
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                vmode
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
 * @brief Gets current gen-lock offset info
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of array/list items
 * @param[out] values   array (memory pointer) to fill
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_genlock_offset_info
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
 * @brief Gets current gen-lock loss of link filter
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  value    current gen-lock loss of link filter
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_genlock_loss_of_link_filter
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint16_t * const              value
);

/**************************************************************************//**
 * @brief Sets gen-lock loss of link filter
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  value gen-lock loss of link filter
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_genlock_loss_of_link_filter
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint16_t const              value
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
 * @brief Gets timecode hold flag
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  enable   timecode hold enable status
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_timecode_hold
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              enable
);

/**************************************************************************//**
 * @brief Sets timecode hold flag
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   enable   timecode hold enable status
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_timecode_hold
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                enable
);

/**************************************************************************//**
 * @brief Gets audio enable flag
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  enable   audio enable status
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_audio_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              enable
);

/**************************************************************************//**
 * @brief Sets audio enable flag
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   enable   audio enable status
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_audio_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                enable
);

/**************************************************************************//**
 * @brief Gets audio gain level
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  gain     audio gain level
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_audio_gain
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint16_t * const             gain
);

/**************************************************************************//**
 * @brief Sets audio gain level
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   gain     audio gain level
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_audio_gain
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint16_t const               gain
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
    ctrl_protocol_uint8_array_t  get_downscale_mode;
    ctrl_protocol_uint8_array_t  set_downscale_mode;
    ctrl_protocol_get_uint8_t    get_flip_mode;
    ctrl_protocol_set_uint8_t    set_flip_mode;
    ctrl_protocol_get_uint8_t    get_genlock_mode;
    ctrl_protocol_set_uint8_t    set_genlock_mode;
    ctrl_protocol_get_uint8_t    get_genlock_status;
    ctrl_protocol_get_uint8_t    get_genlock_crosslock;
    ctrl_protocol_set_uint8_t    set_genlock_crosslock;
    ctrl_protocol_int16_array_t  get_genlock_offset;
    ctrl_protocol_int16_array_t  set_genlock_offset;
    ctrl_protocol_int16_array_t  get_genlock_offset_info;
    ctrl_protocol_get_uint8_t    get_genlock_termination;
    ctrl_protocol_set_uint8_t    set_genlock_termination;
    ctrl_protocol_get_uint16_t   get_genlock_loss_of_link_filter;
    ctrl_protocol_set_uint16_t   set_genlock_loss_of_link_filter;
    ctrl_protocol_get_uint8_t    get_sdi_range;
    ctrl_protocol_set_uint8_t    set_sdi_range;
    ctrl_protocol_get_int8_t     get_sdi_black;
    ctrl_protocol_set_int8_t     set_sdi_black;
    ctrl_protocol_get_int8_t     get_sdi_white;
    ctrl_protocol_set_int8_t     set_sdi_white;
    ctrl_protocol_int32_array_t  get_timecode;
    ctrl_protocol_int32_array_t  set_timecode;
    ctrl_protocol_get_uint8_t    get_timecode_hold;
    ctrl_protocol_set_uint8_t    set_timecode_hold;
    ctrl_protocol_get_uint8_t    get_audio_enable;
    ctrl_protocol_set_uint8_t    set_audio_enable;
    ctrl_protocol_get_uint16_t   get_audio_gain;
    ctrl_protocol_set_uint16_t   set_audio_gain;
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

