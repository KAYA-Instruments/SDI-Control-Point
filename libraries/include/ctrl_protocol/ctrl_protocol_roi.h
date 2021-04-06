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
 * @file    ctrl_protocol_roi.h
 *
 * @brief   Generic ROI control protocol functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_ROI_H__
#define __CTRL_PROTOCOL_ROI_H__

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
 * @brief Camera ROI offset information type
 *****************************************************************************/
typedef struct ctrl_protocol_stat_roi_info_s
{
    uint32_t    max_width;      /**< maximum width of the camera ROI */
    uint32_t    max_height;     /**< maximum height of the camera ROI */
    uint32_t    width_step;     /**< width step of the camera ROI */
    uint32_t    height_step;    /**< height step of the camera ROI */
} ctrl_protocol_stat_roi_info_t;

/**************************************************************************//**
 * @brief Get camera-device ROI information
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  no       number of bytes, shall be equal to sizeof(ctrl_protocol_stat_roi_info_t)
 * @param[out]  values   array pointer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_stat_roi_info
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Camera ROI offset type
 *****************************************************************************/
typedef struct ctrl_protocol_stat_roi_s
{
    uint32_t    width;        /**< actuel width of the camera ROI  */
    uint32_t    height;       /**< actuel height of the camera ROI */
    uint32_t    offset_x;     /**< actuel offset x of the camera ROI */
    uint32_t    offset_y;     /**< actuel offset y of the camera ROI */
} ctrl_protocol_stat_roi_t;

/**************************************************************************//**
 * @brief Get camera stat ROI
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  no       number of bytes, shall be equal to sizeof(ctrl_protocol_stat_roi_t)
 * @param[out]  values   array pointer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_stat_roi
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Set camera stat ROI
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  no       number of bytes, shall be equal to sizeof(ctrl_protocol_stat_roi_t)
 * @param[out]  gain     analogue gain to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_stat_roi
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint32_t * const             values
);

/**************************************************************************//**
 * @brief CAM protocol driver implementation
 *****************************************************************************/
typedef struct ctrl_protocol_roi_drv_s
{
    ctrl_protocol_uint8_array_t     get_stat_roi_info;
    ctrl_protocol_uint8_array_t     get_stat_roi;
    ctrl_protocol_uint8_array_t     set_stat_roi;
} ctrl_protocol_roi_drv_t;

/******************************************************************************
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_roi_register
(
    ctrl_protocol_handle_t const    handle,
    void * const                    ctx,
    ctrl_protocol_roi_drv_t * const drv
);

/******************************************************************************
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_roi_unregister
(
    ctrl_protocol_handle_t const handle
);

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_ROI_H__ */

