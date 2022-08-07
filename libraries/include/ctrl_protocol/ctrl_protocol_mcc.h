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
 * @file    ctrl_protocol_mcc.h
 *
 * @brief   Generic mcc control protocol functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_MCC_H__
#define __CTRL_PROTOCOL_MCC_H__

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
 * @brief Max number of supported color phases
 *****************************************************************************/
#define MAX_MCC_NO_PHASES               ( 32u )

/**************************************************************************//**
 * @brief Max number of supported operation modes
 *****************************************************************************/
#define MAX_MCC_OPERATION_MODES         ( 4u )

/**************************************************************************//**
 * @brief Converts operation mode into number of color segments
 *
 * @param[in]  opmode       current opmode 
 * @param[out] no           number of supported color segments/phases
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_mcc_no_phases
(
    uint8_t const                   opmode,
    uint8_t * const                 no
);

/**************************************************************************//**
 * @brief Gets enable status of Multi Color Controller.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  status      current enable status of MCC 
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_mcc_enable
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t * const                 enable
);

/**************************************************************************//**
 * @brief Sets enable status of Multi Color Controller.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   status      status to set (enable or disable)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_mcc_enable
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t const                   enable
);

/**************************************************************************//**
 * @brief Gets operation mode of Multi Color Controller.
 *
 * @note  Operation mode means the number of color phases/segments.
 *              0: 12 color phases
 *              1: 16 color phases
 *              2: 24 color phases
 *              3: 32 color phases
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  mode        current operation mode of MCC 
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_mcc_opmode
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t * const                 mode
);

/**************************************************************************//**
 * @brief Sets operation mode of Multi Color Controller.
 *
 * @note  Operation mode means the number of color phases/segments.
 *              0: 12 color phases
 *              1: 16 color phases
 *              2: 24 color phases
 *              3: 32 color phases
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  mode        current operation mode of MCC 
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_mcc_opmode
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel,
    uint8_t const                   mode
);

/**************************************************************************//**
 * @brief Gets saturation blink mask of Multi Color Controller.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  mask        current operation mode of MCC 
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_mcc_blink
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint32_t * const             values
);

/**************************************************************************//**
 * @brief Sets saturation blink mask of Multi Color Controller.
 *
 * @note  Setting a bit to 1 enables saturation blinking for this color phase. 
 *        (max 32 to colorphases are supported)
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   mask        blink mask to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_mcc_blink
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint32_t * const             values
);

/**************************************************************************//**
 * @brief color phase definition
 *****************************************************************************/
typedef struct ctrl_protocol_mcc_phase_s
{
    uint8_t     id;             /**< color phase identifier */
    uint32_t    saturation;     /**< saturation of this color phase */
    int32_t     hue;            /**< hue of this color phase */
} ctrl_protocol_mcc_phase_t;

/**************************************************************************//**
 * @brief Gets saturation and hue of selected color phase
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of bytes in buffer
 * @param[out]  buf      data buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_mcc_phase
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    int const                       no,
    uint8_t * const                 buf 
);

/**************************************************************************//**
 * @brief      Sets saturation and hue for selected segment of multi color
 *             controller.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of bytes in buffer
 * @param[in]   buf      data buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_mcc_phase
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    int const                       no,
    uint8_t * const                 buf 
);

/**************************************************************************//**
 * @brief MCC protocol driver implementation
 *****************************************************************************/
typedef struct ctrl_protocol_mcc_drv_s
{
    ctrl_protocol_get_uint8_t   get_mcc_enable;
    ctrl_protocol_set_uint8_t   set_mcc_enable;
    ctrl_protocol_get_uint8_t   get_mcc_opmode;
    ctrl_protocol_set_uint8_t   set_mcc_opmode;
    ctrl_protocol_uint32_array_t  get_mcc_blink;
    ctrl_protocol_uint32_array_t set_mcc_blink;
    ctrl_protocol_uint8_array_t get_mcc_phase;
    ctrl_protocol_uint8_array_t set_mcc_phase;
} ctrl_protocol_mcc_drv_t;

/******************************************************************************
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_mcc_register
(
    ctrl_protocol_handle_t const    handle,
    void * const                    ctx,
    ctrl_protocol_mcc_drv_t * const drv
);

/******************************************************************************
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_mcc_unregister
(
    ctrl_protocol_handle_t const handle
);

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_MCC_H__ */

