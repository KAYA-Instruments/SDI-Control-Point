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
 * @file    ctrl_protocol_tflt.h
 *
 * @brief   Generic tflt control protocol functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_TFLT_H__
#define __CTRL_PROTOCOL_TFLT_H__

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
 * @brief Get enable status of temporal filtering.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   mode     current enable status of TFLT
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_tflt_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode 
);

/**************************************************************************//**
 * @brief Set enable status of temporal filtering.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   mode     status to set (1: enable or 0: disable)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_tflt_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode 
);

/**************************************************************************//**
 * @brief Border indices of tflt module in coefficient LUT
 *****************************************************************************/
typedef struct ctrl_protocol_tflt_index_s
{
    uint8_t min;    /**< min index in coefficient LUT */
    uint8_t max;    /**< max index in coefficient LUT */
} ctrl_protocol_tflt_index_t;

/**************************************************************************//**
 * @brief Get temporal filter stage min/max coefficient level.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of bytes in buffer
 * @param[out]  buf      data buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_tflt_min_max
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    int const                       no,
    uint8_t * const                 buf 
);

/**************************************************************************//**
 * @brief Set temporal filter stage min/max coefficient level.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of bytes in buffer
 * @param[in]   buf      data buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_tflt_min_max
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    int const                       no,
    uint8_t * const                 buf 
);

/**************************************************************************//**
 * @brief Get denoise level of tflt stage
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   level    current denoise level
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_tflt_denoise_level
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t * const                 level
);

/**************************************************************************//**
 * @brief Set denoise level of tflt stage
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   level    denoise level to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_tflt_denoise_level
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t const                   level
);

/**************************************************************************//**
 * @brief TFLT protocol driver implementation
 *****************************************************************************/
typedef struct ctrl_protocol_tflt_drv_s
{
    ctrl_protocol_get_uint8_t   get_tflt_enable;
    ctrl_protocol_set_uint8_t   set_tflt_enable;
    ctrl_protocol_uint8_array_t get_tflt_min_max;
    ctrl_protocol_uint8_array_t set_tflt_min_max;
    ctrl_protocol_get_uint8_t   get_tflt_denoise_level;
    ctrl_protocol_set_uint8_t   set_tflt_denoise_level;
} ctrl_protocol_tflt_drv_t;


/******************************************************************************
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_tflt_register
(
    ctrl_protocol_handle_t const    handle,
    void * const                    ctx,
    ctrl_protocol_tflt_drv_t * const drv
);

/******************************************************************************
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_tflt_unregister
(
    ctrl_protocol_handle_t const handle
);

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_TFLT_H__ */

