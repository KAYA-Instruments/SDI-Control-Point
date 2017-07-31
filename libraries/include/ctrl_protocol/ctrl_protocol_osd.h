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
 * @file    ctrl_protocol_osd.h
 *
 * @brief   Generic osd control protocol functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_OSD_H__
#define __CTRL_PROTOCOL_OSD_H__

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
 * @brief Get test pattern enable status.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  status      current test-pattern enable status
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_test_pattern
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t * const                 enable
);

/**************************************************************************//**
 * @brief Get test pattern enable status.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  status      test-pattern enable status to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_test_pattern
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                enable 
);

/**************************************************************************//**
 * @brief Get center marker mode.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  mode        current center marker mode 
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_center_marker
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t * const                 mode 
);

/**************************************************************************//**
 * @brief Set center marker mode.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   mode        center marker mode to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_center_marker
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode 
);

/**************************************************************************//**
 * @brief Get logo mode.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  id          current logo id (0: disable, 1: DCT, ...)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_logo
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t * const                 id 
);

/**************************************************************************//**
 * @brief Set logo mode. 
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   id          logo id to set (0: disable, 1: DCT, ... )
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_logo
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                id 
);

/**************************************************************************//**
 * @brief OSD zebra configuration
 *****************************************************************************/
typedef struct ctrl_protocol_osd_zebra_s
{
    uint8_t     mode;           /**< 0: disable, 1: enable */
    uint16_t    min;            /**< min Luma (Y) value */
    uint16_t    max;            /**< max Luma (Y) value */
} ctrl_protocol_osd_zebra_t;

/**************************************************************************//**
 * @brief Get zebra configuration.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   no          number of bytes in buffer
 * @param[out]  buf         data buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_zebra
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    int const                       no,
    uint8_t * const                 buf 
);

/**************************************************************************//**
 * @brief Set zebra configuration. 
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   no          number of bytes in buffer
 * @param[in]   buf         data buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_zebra
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                       no,
    uint8_t * const                 buf 
);

/**************************************************************************//**
 * @brief OSD protocol driver implementation
 *****************************************************************************/
typedef struct ctrl_protocol_osd_drv_s
{
    ctrl_protocol_get_uint8_t   get_test_pattern;
    ctrl_protocol_set_uint8_t   set_test_pattern;
    ctrl_protocol_get_uint8_t   get_center_marker;
    ctrl_protocol_set_uint8_t   set_center_marker;
    ctrl_protocol_get_uint8_t   get_logo;
    ctrl_protocol_set_uint8_t   set_logo;
    ctrl_protocol_uint8_array_t get_zebra;
    ctrl_protocol_uint8_array_t set_zebra;
} ctrl_protocol_osd_drv_t;

/******************************************************************************
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_osd_register
(
    ctrl_protocol_handle_t const    handle,
    void * const                    ctx,
    ctrl_protocol_osd_drv_t * const drv
);

/******************************************************************************
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_osd_unregister
(
    ctrl_protocol_handle_t const handle
);

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_OSD_H__ */

