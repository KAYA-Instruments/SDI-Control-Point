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
 * @file    ctrl_protocol_dpcc.h
 *
 * @brief   Generic dpcc control protocol functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_DPCC_H__
#define __CTRL_PROTOCOL_DPCC_H__

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
 * @brief Max number of supported defect pixel coordinates 
 *****************************************************************************/
#define MAX_DPCC_NO_PIXEL           ( 2048u )

/**************************************************************************//**
 * @brief Gets enable status of defect pixel cluster correction
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  status      current enable status of DPCC 
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_dpcc_enable
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t * const                 enable
);

/**************************************************************************//**
 * @brief Sets enable status of defect pixel cluster correction
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   status      status to set (enable or disable)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_dpcc_enable
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t const                   enable
);

/**************************************************************************//**
 * @brief Gets correction mode of defect pixel cluster correction
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  mode        current correction mode of DPCC
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_dpcc_mode
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t * const                 mode
);

/**************************************************************************//**
 * @brief Sets correction mode of defect pixel cluster correction
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   mode        correction mode to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_dpcc_mode
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t const                   mode
);

/**************************************************************************//**
 * @brief Gets detection level of defect pixel cluster correction
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  status      current enable status of DPCC 
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_dpcc_level
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t * const                 enable
);

/**************************************************************************//**
 * @brief Sets detection level of defect pixel cluster correction
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   status      status to set (enable or disable)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_dpcc_level
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t const                   enable
);

/**************************************************************************//**
 * @brief       Add a pixel into dpcc table
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of bytes in buffer
 * @param[in]   buf      data buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_add_pixel
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    int const                       no,
    uint16_t * const                buf 
);

/**************************************************************************//**
 * @brief pixel table
 *****************************************************************************/
typedef struct ctrl_protocol_dpcc_table_s
{
    uint16_t    size;   /**< number of defect pixel in table */
    uint16_t    no;     /**< number of defect pixel in table */
    uint16_t *  x;      /**< pointer to x coordinates */
    uint16_t *  y;      /**< pointer to y coordinates */
} ctrl_protocol_dpcc_table_t;

/**************************************************************************//**
 * @brief Gets current defect pixel table
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of bytes in buffer
 * @param[out]  buf      data buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_dpcc_table
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    int const                       no,
    uint32_t * const                buf 
);

/**************************************************************************//**
 * @brief Clears current defect pixel table
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_clear_dpcc_table
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel
);

/**************************************************************************//**
 * @brief Save current defect pixel table
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_save_dpcc_table
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel
);

/**************************************************************************//**
 * @brief Load defect pixel table
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_load_dpcc_table
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel
);

/**************************************************************************//**
 * @brief automatically load dpcc table
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_auto_load_dpcc_table
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel
);

/**************************************************************************//**
 * @brief Gets test mode of defect pixel cluster correction
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  mode        current test mode of DPCC
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_dpcc_test_mode
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel,
    uint8_t * const                 mode
);

/**************************************************************************//**
 * @brief Sets test mode of defect pixel cluster correction
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   mode        test mode to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_dpcc_test_mode
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel,
    uint8_t const                   mode
);

/**************************************************************************//**
 * @brief DPCC protocol driver implementation
 *****************************************************************************/
typedef struct ctrl_protocol_dpcc_drv_s
{
    ctrl_protocol_get_uint8_t       get_dpcc_enable;
    ctrl_protocol_set_uint8_t       set_dpcc_enable;
    ctrl_protocol_get_uint8_t       get_dpcc_mode;
    ctrl_protocol_set_uint8_t       set_dpcc_mode;
    ctrl_protocol_get_uint8_t       get_dpcc_level;
    ctrl_protocol_set_uint8_t       set_dpcc_level;
    ctrl_protocol_uint16_array_t    add_dpcc_pixel;
    ctrl_protocol_uint32_array_t    get_dpcc_table;
    ctrl_protocol_run_t             clear_dpcc_table;
    ctrl_protocol_run_t             save_dpcc_table;
    ctrl_protocol_run_t             load_dpcc_table;
    ctrl_protocol_run_t             auto_load_dpcc_table;
    ctrl_protocol_get_uint8_t       get_dpcc_test_mode;
    ctrl_protocol_set_uint8_t       set_dpcc_test_mode;
} ctrl_protocol_dpcc_drv_t;

/**************************************************************************//**
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
extern int ctrl_protocol_dpcc_register
(
    ctrl_protocol_handle_t const    handle,
    void * const                    ctx,
    ctrl_protocol_dpcc_drv_t * const drv
);

/**************************************************************************//**
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_dpcc_unregister
(
    ctrl_protocol_handle_t const handle
);

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_DPCC_H__ */

