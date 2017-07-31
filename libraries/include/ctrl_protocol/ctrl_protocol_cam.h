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
 * @file    ctrl_protocol_cam.h
 *
 * @brief   Generic cam control protocol functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_CAM_H__
#define __CTRL_PROTOCOL_CAM_H__

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
 * @brief camera feature bit-mask (supported features of connected camera)
 *****************************************************************************/
enum CAM_FEATURE
{
    CAM_FEATURE_UNKNOWN             = 0x00000000u,   /**< reset value */
    CAM_FEATURE_HAS_AUTO_BLACK      = 0x00000001u,   /**< auto-black feature available */
    CAM_FEATURE_HAS_FPNC            = 0x00000002u,   /**< fix pattern correction feature available */
    CAM_FEATURE_HAS_FRAME_RATE_USER = 0x00000004u,   /**< user defined frame rate feature available */
    CAM_FEATURE_HAS_AWB             = 0x00000010u,   /**< camera has a auto white balance */
    CAM_FEATURE_HAS_CMD_ACCESS      = 0x40000000u,   /**< camera has a register command access functions */
    CAM_FEATURE_HAS_REG_ACCESS      = 0x80000000u,   /**< camera has a register read/write function */
};

#define IS_CAM_FEATURE_AUTO_BLACK_LEVEL(x)  ( x & CAM_FEATURE_HAS_AUTO_BLACK )
#define IS_CAM_FEATURE_FIX_PATTERN(x)       ( x & CAM_FEATURE_HAS_FPNC )
#define IS_CAM_FEATURE_FRAME_RATE_USER(x)   ( x & CAM_FEATURE_HAS_FRAME_RATE_USER )
#define IS_CAM_FEATURE_HAS_AWB(x)           ( x & CAM_FEATURE_HAS_AWB )
#define IS_CAM_FEATURE_HAS_CMD_ACCESS(x)    ( x & CAM_FEATURE_HAS_CMD_ACCESS )
#define IS_CAM_FEATURE_HAS_REG_ACCESS(x)    ( x & CAM_FEATURE_HAS_REG_ACCESS )

/**************************************************************************//**
 * @brief Camera information type
 *****************************************************************************/
typedef struct ctrl_protocol_cam_info_s
{
    uint32_t    min_gain;
    uint32_t    max_gain;
    uint32_t    min_exposure_time;
    uint32_t    max_exposure_time;
} ctrl_protocol_cam_info_t;

/**************************************************************************//**
 * @brief Get camera-device information
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  no       number of bytes, shall be equal to sizeof(camera_info_t)
 * @param[out]  values   array pointer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_cam_info
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Get camera analogue gain.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  gain     current camera analog gain
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_cam_gain
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             gain
);

/**************************************************************************//**
 * @brief Set camera analogue gain.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  gain     analogue gain to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_cam_gain
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               gain
);

/**************************************************************************//**
 * @brief Get exposure time in microseconds.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  exposure current camera exposure time
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_cam_exposure
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             exposure
);

/**************************************************************************//**
 * @brief Set exposure time in microseconds.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   exposure exposure time to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_cam_exposure
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               exposure
);

/**************************************************************************//**
 * @brief Get currently configured camera frame-rate.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  framerate   current frame-rate
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_cam_fps
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             framerate
);

/**************************************************************************//**
 * @brief Set camera frame-rate.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   framerate   frame-rate to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_cam_fps
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               framerate
);

/**************************************************************************//**
 * @brief Get currently configured camera depth.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  depth       current bit width
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_cam_depth
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              depth
);

/**************************************************************************//**
 * @brief Set camera depth.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   depth       bit width to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_cam_depth
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                depth
);

/**************************************************************************//**
 * @brief Camera digital gain vector type
 *****************************************************************************/
typedef struct ctrl_protocol_cam_dig_gain_s
{
    uint32_t    red;            /**< red gain of camera device */
    uint32_t    green;          /**< green gain of camera device */
    uint32_t    blue;           /**< blue gain of camera device */
} ctrl_protocol_cam_dig_gain_t;

/**************************************************************************//**
 * @brief Get camera digital gains. 
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   no          number of values, shall be equal to 
 *                          sizeof(ctrl_protocol_cam_dig_gain_t)
 * @param[in]   values      digital gain values
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_cam_dig_gains
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Set camera digital gains. 
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   no          number of values, shall be equal to 
 *                          sizeof(ctrl_protocol_cam_dig_gain_t)
 * @param[in]   values      digital gain values
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_cam_dig_gains
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Camera auto-black configuration.
 *****************************************************************************/
typedef struct ctrl_protocol_cam_auto_black_s
{
    uint8_t left;
    uint8_t right;
} ctrl_protocol_cam_auto_black_t;

/**************************************************************************//**
 * @brief Get camera auto-black configuration. 
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   no          number of values, shall be equal to
 *                          sizeof(ctrl_protocol_cam_auto_black_t)
 * @param[in]   values      digital gain values
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_cam_auto_black
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Set camera auto-black configuration. 
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   no          number of values, shall be equal to
 *                          sizeof(ctrl_protocol_cam_auto_black_t)
 * @param[in]   values      digital gain values
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_cam_auto_black
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief CAM protocol driver implementation
 *****************************************************************************/
typedef struct ctrl_protocol_cam_drv_s
{
    ctrl_protocol_uint8_array_t     get_cam_info;
    ctrl_protocol_get_uint16_t      get_cam_gain;
    ctrl_protocol_set_uint16_t      set_cam_gain;
    ctrl_protocol_get_uint32_t      get_cam_exposure;
    ctrl_protocol_set_uint32_t      set_cam_exposure;
} ctrl_protocol_cam_drv_t;

/**************************************************************************//**
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_cam_register
(
    ctrl_protocol_handle_t const    handle,
    void * const                    ctx,
    ctrl_protocol_cam_drv_t * const drv
);

/**************************************************************************//**
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_cam_unregister
(
    ctrl_protocol_handle_t const handle
);

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_CAM_H__ */

