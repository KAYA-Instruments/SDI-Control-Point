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
 * @file    ctrl_protocol.h
 *
 * @brief   Control protocol definitions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_H__
#define __CTRL_PROTOCOL_H__

#include <stdint.h>

#include <ctrl_channel/ctrl_channel.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * @defgroup ctrl_protocol_layer Generic Control Protocol Layer implementation
 * @{
 *****************************************************************************/

/**************************************************************************//**
 * @brief Control protocol instance handle
 *****************************************************************************/
typedef struct ctrl_protocol_s * ctrl_protocol_handle_t;

/**************************************************************************//**
 * list of funtion pointer types:
 *
 * action/event function types
 * ctrl_protocol_run_t              - parameter less
 *
 * scalar function types
 * ctrl_protocol_get_uint8_t        - get a single uint8_t parameter
 * ctrl_protocol_set_uint8_t        - set a single uint8_t parameter
 * ctrl_protocol_get_int8_t         - get a single int8_t parameter
 * ctrl_protocol_set_int8_t         - set a single int8_t parameter
 * ctrl_protocol_get_uint16_t       - get a single uint16_t parameter
 * ctrl_protocol_set_uint16_t       - set a single uint16_t parameter
 * ctrl_protocol_get_int16_t        - get a single int16_t parameter 
 * ctrl_protocol_set_int16_t        - set a single int16_t parameter
 * ctrl_protocol_get_uint32_t       - get a single uint32_t parameter 
 * ctrl_protocol_set_uint32_t       - set a single uint32_t parameter
 * ctrl_protocol_get_int32_t        - get a single int32_t parameter 
 * ctrl_protocol_set_int32_t        - set a single int32_t parameter
 *
 * vector or buffer function types
 * ctrl_protocol_uint8_array_t      - get a set of uint8_t values
 * ctrl_protocol_int8_array_t       - get a set of int8_t values
 * ctrl_protocol_uint16_array_t     - get a set of uint16_t values
 * ctrl_protocol_int16_array_t      - get a set of int16_t values
 * ctrl_protocol_uint32_array_t     - get a set of uint32_t values
 * ctrl_protocol_int32_array_t      - get a set of int32_t values
 *****************************************************************************/

/**************************************************************************//**
 * @brief function pointer type to run a command on device
 *
 * @param[in]   ctx     private instance context of protocol implementation
 * @param[in]   channel control channel for device communication
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_run_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
);

/**************************************************************************//**
 * @brief function pointer type to get/read one unsigned 8 bit value
 *
 * @param[in]   ctx     private instance context of protocol implementation
 * @param[in]   channel control channel for device communication
 * @param[out]  value   currently configured value
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_get_uint8_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             value
);

/**************************************************************************//**
 * @brief function pointer type to set/write one unsigned 8 bit value
 *
 * @param[in]   ctx     private instance context of protocol implementation
 * @param[in]   channel control channel for device communication
 * @param[in]   value   value to set
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_set_uint8_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               value
);

/**************************************************************************//**
 * @brief function pointer type to get/read one signed 8 bit value
 *
 * @param[in]   ctx     private instance context of protocol implementation
 * @param[in]   channel control channel for device communication
 * @param[out]  value   currently configured value
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_get_int8_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int8_t * const              value
);

/**************************************************************************//**
 * @brief function pointer type to set/write one signed 8 bit value
 *
 * @param[in]   ctx     private instance context of protocol implementation
 * @param[in]   channel control channel for device communication
 * @param[in]   value   value to set
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_set_int8_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int8_t const                value
);

/**************************************************************************//**
 * @brief function pointer type to get/read one unsigned 16 bit value
 *
 * @param[in]   ctx     private instance context of protocol implementation
 * @param[in]   channel control channel for device communication
 * @param[out]  value   currently configured value
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_get_uint16_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t * const            value
);

/**************************************************************************//**
 * @brief function pointer type to set/write one unsigned 16 bit value
 *
 * @param[in]   ctx     private instance context of protocol implementation
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_set_uint16_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t const              value
);

/**************************************************************************//**
 * @brief function pointer type to get/read one signed 16 bit value
 *
 * @param[in]   ctx     private instance context of protocol implementation
 * @param[in]   channel control channel for device communication
 * @param[out]  value   currently configured value
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_get_int16_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int16_t * const             value
);

/**************************************************************************//**
 * @brief function pointer type to set/write one signed 16 bit value
 *
 * @param[in]   ctx     private instance context of protocol implementation
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_set_int16_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int16_t const               value
);

/**************************************************************************//**
 * @brief function pointer type to get/read one unsigned 32 bit value
 *
 * @param[in]   ctx     private instance context of protocol implementation
 * @param[in]   channel control channel for device communication
 * @param[out]  value   currently configured value
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_get_uint32_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            value
);

/**************************************************************************//**
 * @brief function pointer type to set/write one unsigned 32 bit value
 *
 * @param[in]   ctx     private instance context of protocol implementation
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_set_uint32_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t const              value
);

/**************************************************************************//**
 * @brief function pointer type to get/read one signed 32 bit value
 *
 * @param[in]   ctx     private instance context of protocol implementation
 * @param[in]   channel control channel for device communication
 * @param[out]  value   currently configured value
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_get_int32_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int32_t * const             value
);

/**************************************************************************//**
 * @brief function pointer type to set/write one signed 32 bit value
 *
 * @param[in]   ctx     private instance context of protocol implementation
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_set_int32_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int32_t const               value
);

/**************************************************************************//**
 * @brief function pointer type to get/read or set/write a number of 
 *        unsigned 8 bit values
 *
 * @param[in]   ctx     private instance context of protocol implementation
 * @param[in]   channel control channel for device communication
 * @param[in]   no      number of values to get/read
 * @param[out]  value   values
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_uint8_array_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             value
);

/**************************************************************************//**
 * @brief function pointer type to get/read or set/write a number of 
 *        signed 8 bit values
 *
 * @param[in]   ctx     private instance context of protocol implementation
 * @param[in]   channel control channel for device communication
 * @param[in]   no      number of values to get/read
 * @param[out]  value   values
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_int8_array_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    int8_t * const              value
);

/**************************************************************************//**
 * @brief function pointer type to get/read or set/write a number of 
 *        unsigned 16 bit values
 *
 * @param[in]   ctx     private instance context of protocol implementation
 * @param[in]   channel control channel for device communication
 * @param[in]   no      number of values to get/read
 * @param[out]  value   values
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_uint16_array_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint16_t * const            value
);

/**************************************************************************//**
 * @brief function pointer type to get/read or set/write a number of 
 *        signed 16 bit values
 *
 * @param[in]   ctx     private instance context of protocol implementation
 * @param[in]   channel control channel for device communication
 * @param[in]   no      number of values to get/read
 * @param[out]  value   values
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_int16_array_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    int16_t * const             value
);

/**************************************************************************//**
 * @brief function pointer type to get/read or set/write a number of 
 *        unsigned 32 bit values
 *
 * @param[in]   ctx     private instance context of protocol implementation
 * @param[in]   channel control channel for device communication
 * @param[in]   no      number of values to get/read
 * @param[out]  value   values
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_uint32_array_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint32_t * const            value
);

/**************************************************************************//**
 * @brief function pointer type to get/read or set/write a number of 
 *        signed 32 bit values
 *
 * @param[in]   ctx     private instance context of protocol implementation
 * @param[in]   channel control channel for device communication
 * @param[in]   no      number of values to get/read
 * @param[out]  value   values
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_protocol_int32_array_t)
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    int32_t * const             value
);

/**************************************************************************//**
 * @brief      Returns the size of a control protocol instance 
 *
 * @param      none
 *
 * @return     size of a control channel instance
 *****************************************************************************/
int ctrl_protocol_get_instance_size( void );

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_H__ */

