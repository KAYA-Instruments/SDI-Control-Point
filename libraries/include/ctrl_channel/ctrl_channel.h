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
 * @file    ctrl_channel.h
 *
 * @brief   Control channel to transfer commands to a provideo device
 *
 *****************************************************************************/
#ifndef __CTRL_CHANNEL_H__
#define __CTRL_CHANNEL_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * @defgroup ctrl_channel Generic control channel interface definitions
 * @{
 *****************************************************************************/

/**************************************************************************//**
 * @brief      Control channel instance handle
 *****************************************************************************/
typedef struct ctrl_channel_s * ctrl_channel_handle_t;

/**************************************************************************//**
 * @brief      Control channel descriptor type, typically used as port-name
 *****************************************************************************/
typedef char ctrl_channel_name_t[80];

/**************************************************************************//**
 * @brief      Control channel states
 *****************************************************************************/
typedef enum ctrl_channel_state_e
{
    CTRL_CHANNEL_STATE_INVALID   = 0,   /**< invalid, startup state */
    CTRL_CHANNEL_STATE_INIT      = 1,   /**< initialized, but not connected */
    CTRL_CHANNEL_STATE_CONNECTED = 2,   /**< connected to provideo device */
    CTRL_CHANNEL_STATE_MAX              /**< for max range check */
} ctrl_channel_state_t;

/**************************************************************************//**
 * @brief function pointer type to get the number of available serial ports
 *
 * @param[in]  handle   private channel context handle
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_channel_get_no_ports_t)( void * const handle );

/**************************************************************************//**
 * @brief function pointer type to get number of ports
 *
 * @param[in]  handle   private channel context handle
 * @param[in]  idx      number/index of port
 * @param[out] name     port-name
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_channel_get_port_name_t)
(
    void * const            handle,
    int const               idx,
    ctrl_channel_name_t     name
);

/**************************************************************************//**
 * @brief function pointer type to open a control channel.
 *
 * @param[in]  handle   private channel context handle
 * @param[in]  param    open configuration to open the control port
 * @param[in]  size     size of parameter to evaluate given open configuration
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_channel_open_t)
(
    void * const    handle,
    void * const    param,
    int const       size
);

/**************************************************************************//**
 * @brief function pointer type to close an existing connection to a device.
 *
 * @param[in]  handle   private channel context handle
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_channel_close_t)
(
    void * const    handle
);

/**************************************************************************//**
 * @brief function pointer type to send control command packet (request) 
 *        to a connected provideo device.
 *
 * @param[in]  handle   private channel context handle
 * @param[in]  data     control command packet (packetized data buffer) to send 
 * @param[in]  len      sizeof data buffer
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_channel_send_request_t)
(
    void * const    handle,
    uint8_t * const data,
    int const       len
);

/**************************************************************************//**
 * @brief function pointer type to receive response data from device.
 *
 * @param[in]  handle   private channel context handle
 * @param[in]  data     data buffer to record received data
 * @param[in]  len      sizeof data buffer
 *
 * @return      0 on sucess, error-code otherwise
 *****************************************************************************/
typedef int (* ctrl_channel_receive_response_t)
(
    void * const    handle,
    uint8_t * const data,
    int const       len
);

/**************************************************************************//**
 * @brief      Returns the size of a control channel instance
 *
 * @param      none
 *
 * @return     size of a control channel instance
 *****************************************************************************/
int ctrl_channel_get_instance_size( void );

/**************************************************************************//**
 * @brief      Get state of control channel instance
 *
 * @param[in]  ch   control channel handle
 *
 * @return     state of control channel instance
 *****************************************************************************/
ctrl_channel_state_t ctrl_channel_get_state( ctrl_channel_handle_t const ch );

/**************************************************************************//**
 * @brief      Get number of available interface ports of this port-type
 *
 * @note       for example:
 *
 *             when using serial interface implementation (RS232, RS485, ... )
 *             it returns the number of available serial interface ports 
 *
 *             when using ethernet interface implementation (eth0, ... )
 *             it returns the number of ethernet devices
 *
 * @param[in]  ch   control channel handle
 *
 * @return     >=0 number of available ports, < 0 error-code
 *****************************************************************************/
int ctrl_channel_get_no_ports( ctrl_channel_handle_t const  ch );

/**************************************************************************//**
 * @brief      Get the system description of an interface port
 *
 * @param[in]  ch       control channel handle
 * @param[in]  idx      system interface index
 * @param[out] name     description (name) of the interface port
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_channel_get_port_name
(
    ctrl_channel_handle_t const ch,
    int const                   idx,
    ctrl_channel_name_t         name
);

/**************************************************************************//**
 * @brief      Open a control channel port.
 *
 * @param[in]  ch       control channel handle
 * @param[in]  param    open configuration to open the control port
 * @param[in]  size     size of parameter to evaluate given open configuration
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_channel_open
( 
    ctrl_channel_handle_t const ch,
    void * const                param,
    int const                   size
);

/**************************************************************************//**
 * @brief      Close an opened control channel port.
 *
 * @param[in]  ch       channel handle
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_channel_close( ctrl_channel_handle_t const ch );

/**************************************************************************//**
 * @brief      Send a control command packet to a connected provideo device.
 *
 * @param[in]  ch       control channel handle, has to be connected before using
 * @param[in]  data     control command packet (packetized data buffer) to send 
 * @param[in]  len      sizeof data buffer
 *
 * @return     >=0 number of transfered bytes, error-code otherwise
 *****************************************************************************/
int ctrl_channel_send_request
(
    ctrl_channel_handle_t const ch,
    uint8_t * const             data,
    int const                   len
);

/**************************************************************************//**
 * @brief      Receive response data from command interface.
 *
 * @param[in]  ch       control channel handle used for receiving response data
 * @param[in]  data     data buffer to record received data
 * @param[in]  len      sizeof data buffer
 *
 * @return     >=0 number of received bytes, error-code otherwise
 *****************************************************************************/
int ctrl_channel_receive_response
( 
    ctrl_channel_handle_t const ch,
    uint8_t * const             data,
    int const                   len
);

/**************************************************************************//**
 * @brief      Register function handlers at control channel instance
 *
 * @param[in]  ch               control channel instance handle
 * @param[in]  priv             private context pointer
 * @param[in]  get_no_ports     function pointer to implementation
 * @param[in]  get_port_name    function pointer to implementation
 * @param[in]  open             function pointer to implementation
 * @param[in]  close            function pointer to implementation
 * @param[in]  send_request     function pointer to implementation
 * @param[in]  receive_response function pointer to implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_channel_register
(
    ctrl_channel_handle_t const             ch,
    void * const                            priv,
    ctrl_channel_get_no_ports_t const       get_no_ports,
    ctrl_channel_get_port_name_t const      get_port_name,
    ctrl_channel_open_t const               open,
    ctrl_channel_close_t const              close,
    ctrl_channel_send_request_t const       send_request,
    ctrl_channel_receive_response_t const   receive_response
);

int ctrl_channel_unregister( ctrl_channel_handle_t const ch );

/* @} ctrl_channel */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_CHANNEL_H__ */

