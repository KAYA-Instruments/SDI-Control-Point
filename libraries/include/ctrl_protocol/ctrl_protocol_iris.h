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
 * @file    ctrl_protocol_iris.h
 *
 * @brief   Generic time control functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_IRIS_H__
#define __CTRL_PROTOCOL_IRIS_H__

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
 * @brief Number of iris setup values
 *****************************************************************************/
#define NO_VALUES_IRIS_SETUP             ( 10 ) 

/**************************************************************************//**
 * @brief Gets the current iris setup
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of array/list items
 * @param[out]  values   array (memory pointer) to fill
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_iris_setup
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no, 
    int32_t * const              values
);

/**************************************************************************//**
 * @brief Sets iris setup
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of array/list items
 * @param[out]  values   array (memory pointer) to fill
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_iris_setup
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no, 
    int32_t * const              values
);

/**************************************************************************//**
 * @brief Gets the current iris aperture
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  pos      pointer to iris aperture value
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_iris_apt
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int32_t * const              pos
);

/**************************************************************************//**
 * @brief Sets iris aperture
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  pos      new iris aperture position
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_iris_apt
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int32_t const                pos
);

typedef struct ctrl_protocol_iris_drv_s
{
    ctrl_protocol_int32_array_t     get_iris_setup;
    ctrl_protocol_int32_array_t     set_iris_setup;
    ctrl_protocol_get_int32_t       get_iris_apt;
    ctrl_protocol_set_int32_t       set_iris_apt;
} ctrl_protocol_iris_drv_t;

/******************************************************************************
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_iris_register
(
    ctrl_protocol_handle_t const     handle,
    void * const                     ctx,
    ctrl_protocol_iris_drv_t * const drv
);

/******************************************************************************
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_iris_unregister
(
    ctrl_protocol_handle_t const handle
);

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_IRIS_H__ */


