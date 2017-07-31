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
 * @file    ctrl_protocol_fpnc.h
 *
 * @brief   Generic fpnc control protocol functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_FPNC_H__
#define __CTRL_PROTOCOL_FPNC_H__

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
 * @brief fpn correction data per column
 *****************************************************************************/
#define FPNC_DATA_PER_COLUMN    ( 16u )

/**************************************************************************//**
 * @brief Get enable status of fix pattern noise correction.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  mode     current enable status of FPNC
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_fpnc_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              mode 
);

/**************************************************************************//**
 * @brief Set enable status of fix pattern noise correction.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   mode     status to set (1: enable or 0: disable)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_fpnc_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                mode 
);

/**************************************************************************//**
 * @brief inverse gain settings
 *****************************************************************************/
typedef struct ctrl_protocol_fpnc_inv_gains_s
{
    uint16_t    inv_gain_0;     /**< inverse gain of bayer channel A */
    uint16_t    inv_gain_1;     /**< inverse gain of bayer channel B */
    uint16_t    inv_gain_2;     /**< inverse gain of bayer channel C */
    uint16_t    inv_gain_3;     /**< inverse gain of bayer channel D */
} ctrl_protocol_fpnc_inv_gains_t;

/**************************************************************************//**
 * @brief Get inverse gain values (pre-correction gains)
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values, shall be equal to sizeof(ctrl_protocol_fpnc_inv_gains_t)
 * @param[out]  values   inverse gain values
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_fpnc_inv_gains
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Set inverse gain values (pre-correction gains)
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values, shall be equal to sizeof(ctrl_protocol_fpnc_inv_gains_t)
 * @param[in]   values   inverse gain values
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_fpnc_inv_gains
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief gain settings
 *****************************************************************************/
typedef struct ctrl_protocol_fpnc_gains_s
{
    uint16_t    gain_0;     /**< gain of bayer channel A */
    uint16_t    gain_1;     /**< gain of bayer channel B */
    uint16_t    gain_2;     /**< gain of bayer channel C */
    uint16_t    gain_3;     /**< gain of bayer channel D */
} ctrl_protocol_fpnc_gains_t;

/**************************************************************************//**
 * @brief Get gain values (post-correction gains)
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values, shall be equal to sizeof(ctrl_protocol_fpnc_inv_gains_t)
 * @param[out]  values   inverse gain values
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_fpnc_gains
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Set gain values (post-correction gains)
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values, shall be equal to sizeof(ctrl_protocol_fpnc_inv_gains_t)
 * @param[in]   values   inverse gain values
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_fpnc_gains
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief fpnc correction data
 *
 * There are 2 correction data RAMs (one for Red/Green columns and a 2nd one
 * for the green/blue columns) available in the FPNC module. This means both 
 * correction RAMs hold 960 columns for a Full-HD resolution. Every Column 
 * has a set of 16 24-bit correction value. The upper 12 Bits are for the 
 * first bayer channel in the column (i.e. red in a RED/GREEN column) and 
 * the lower 12 Bits are for the other bayer-channel (i.e. green in a RED/GREEN
 * column).
 *****************************************************************************/
typedef struct ctrl_protocol_fpnc_data_s
{
    uint32_t    page;       /**< correction ram selector */
    uint32_t    column;     /**< image column */
    uint32_t    offset;     /**< segment offset */
    uint32_t    v0;         /**< 24-bit correction value 0 */
    uint32_t    v1;         /**< 24-bit correction value 1 */
    uint32_t    v2;         /**< 24-bit correction value 2 */
    uint32_t    v3;         /**< 24-bit correction value 3 */
} ctrl_protocol_fpnc_data_t;

/**************************************************************************//**
 * @brief Get 4 24-bit fpnc values from selected Correction-Data-RAM
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values, shall be equal to sizeof(ctrl_protocol_fpnc_data_t)
 * @param[out]  values   correction data
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_fpnc_correction_data
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Set 4 24-bit fpnc values in selected Correction-Data-RAM
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values, shall be equal to sizeof(ctrl_protocol_fpnc_data_t)
 * @param[in]   values   correction data to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_fpnc_correction_data
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief FPNC protocol driver implementation
 *****************************************************************************/
typedef struct ctrl_protocol_fpnc_drv_s
{
    ctrl_protocol_get_uint8_t   get_fpnc_enable;
    ctrl_protocol_set_uint8_t   set_fpnc_enable;
    ctrl_protocol_uint8_array_t get_fpnc_inv_gains;
    ctrl_protocol_uint8_array_t set_fpnc_inv_gains;
    ctrl_protocol_uint8_array_t get_fpnc_gains;
    ctrl_protocol_uint8_array_t set_fpnc_gains;
    ctrl_protocol_run_t         set_fpnc_calibrate;
    ctrl_protocol_uint8_array_t get_fpnc_correction_data;
    ctrl_protocol_uint8_array_t set_fpnc_correction_data;
} ctrl_protocol_fpnc_drv_t;

/******************************************************************************
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_fpnc_register
(
    ctrl_protocol_handle_t const        handle,
    void * const                        ctx,
    ctrl_protocol_fpnc_drv_t * const    drv
);

/******************************************************************************
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_fpnc_unregister
(
    ctrl_protocol_handle_t const handle
);

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_FPNC_H__ */

