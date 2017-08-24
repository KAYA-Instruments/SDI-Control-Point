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
 * @file    ctrl_channel.c
 *
 * @brief   Implementation of generic command interface
 *
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <ctrl_channel/ctrl_channel.h>

/******************************************************************************
 * helper macros
 *****************************************************************************/
#ifndef EOPNOTSUPP
/* taken from asm-generic/errno.h */
#define EOPNOTSUPP  95  
#endif

#define CHECK_HANDLE( h )                       \
{                                               \
    if ( !h )                                   \
    {                                           \
        return ( -ENODEV );                     \
    }                                           \
}

#define CHECK_HANDLE_AND_STATE( h, s )          \
{                                               \
    CHECK_HANDLE( h );                          \
    if ( h->state != s )                        \
    {                                           \
        return ( -EFAULT );                     \
    }                                           \
}

#define CHECK_HANDLE_AND_STATES( h, s0, s1 )    \
{                                               \
    CHECK_HANDLE( h );                          \
    if ( (h->state != s0) && (h->state != s1) ) \
    {                                           \
        return ( -EFAULT );                     \
    }                                           \
}

#define CHECK_API_FUNC( func )                  \
{                                               \
    if ( NULL == (func) )                       \
    {                                           \
        return ( -EOPNOTSUPP );                 \
    }                                           \
}

/**************************************************************************//**
 * @brief Command interface to transfer commands to provideo device
 *****************************************************************************/
typedef struct ctrl_channel_s
{
    // get system interformation
    ctrl_channel_get_no_ports_t     get_no_ports;       /**< get available number of system-ports of this port-type */
    ctrl_channel_get_port_name_t    get_port_name;      /**< get a port description */

    // transfer function
    ctrl_channel_open_t             open;               /**< instance specific implementation of open function */
    ctrl_channel_close_t            close;              /**< instance specific implementation of close function */
    ctrl_channel_send_request_t     send_request;       /**< instance specific implementation of send function */
    ctrl_channel_receive_response_t receive_response;   /**< instance specific implementation of poll function */

    ctrl_channel_state_t            state;              /**< control channel state */
    void *                          priv;               /**< pointer to internal context */
} ctrl_channel_t;

/******************************************************************************
 * ctrl_channel_get_instance_size - returns the size of a control channel instance
 *****************************************************************************/
int ctrl_channel_get_instance_size( void )
{
    return ( sizeof( ctrl_channel_t ) );
}

/******************************************************************************
 * ctrl_channel_get_state - returns state of control channel instance
 *****************************************************************************/
ctrl_channel_state_t ctrl_channel_get_state( ctrl_channel_handle_t const ch )
{
    if ( ch )
    {
        return ( ch->state );
    }

    return ( CTRL_CHANNEL_STATE_INVALID );
}

/******************************************************************************
 * ctrl_channel_get_no_ports - get number of available ports in system
 *****************************************************************************/
int ctrl_channel_get_no_ports( ctrl_channel_handle_t const ch )
{
    CHECK_HANDLE_AND_STATES( ch, CTRL_CHANNEL_STATE_INIT, CTRL_CHANNEL_STATE_CONNECTED );

    CHECK_API_FUNC( ch->get_no_ports );

    return ( ch->get_no_ports( ch->priv ) );
}

/******************************************************************************
 * ctrl_channel_get_port_name - returns the portname description
 *****************************************************************************/
int ctrl_channel_get_port_name
(
    ctrl_channel_handle_t const ch,
    int const                   idx,
    ctrl_channel_name_t         name
)
{
    CHECK_HANDLE_AND_STATES( ch, CTRL_CHANNEL_STATE_INIT, CTRL_CHANNEL_STATE_CONNECTED );

    CHECK_API_FUNC( ch->get_port_name );

    return ( ch->get_port_name( ch->priv, idx, name ) );
}

/******************************************************************************
 * ctrl_channel_open - open a connection to a device
 *****************************************************************************/
int ctrl_channel_open
(
    ctrl_channel_handle_t const ch,
    void * const                param,
    int const                   size
)
{
    int res;

    CHECK_HANDLE_AND_STATES( ch, CTRL_CHANNEL_STATE_INIT, CTRL_CHANNEL_STATE_CONNECTED );

    CHECK_API_FUNC( ch->open );

    res = ch->open( ch->priv, param, size );
    if ( !res )
    {
        ch->state = CTRL_CHANNEL_STATE_CONNECTED;
    }

    return ( res );
}

/******************************************************************************
 * ctrl_channel_close - close an existing connection to a device 
 *****************************************************************************/
int ctrl_channel_close( ctrl_channel_handle_t const ch )
{
    int res;

    CHECK_HANDLE_AND_STATE( ch, CTRL_CHANNEL_STATE_CONNECTED );

    CHECK_API_FUNC( ch->close );

    res = ch->close( ch->priv );
    if ( !res )
    {
        ch->state = CTRL_CHANNEL_STATE_INIT;
    }

    return ( res );
}

/******************************************************************************
 * ctrl_channel_send_request - send a command request data buffer to a 
 * connected device
 *****************************************************************************/
int ctrl_channel_send_request
(
    ctrl_channel_handle_t const ch,
    uint8_t * const             data,
    int const                   len
)
{
    CHECK_HANDLE_AND_STATE( ch, CTRL_CHANNEL_STATE_CONNECTED );
    
    CHECK_API_FUNC( ch->send_request );

    return ( ch->send_request( ch->priv, data, len ) );
}

/******************************************************************************
 * ctrl_channel_receive_response - receive response data from a connected device
 *****************************************************************************/
int ctrl_channel_receive_response
(
    ctrl_channel_handle_t const ch,
    uint8_t * const             data,
    int const                   len
)
{
    CHECK_HANDLE_AND_STATE( ch, CTRL_CHANNEL_STATE_CONNECTED );
    
    CHECK_API_FUNC( ch->receive_response );

    return ( ch->receive_response( ch->priv, data, len ) );
}

/******************************************************************************
 * ctrl_channel_register - register a control channel driver functions
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
)
{
    if ( !ch )
    {
        return ( -ENODEV );
    }

    if ( ch->state != CTRL_CHANNEL_STATE_INVALID )
    {
        return ( -EBUSY );
    }

    // set context pointer
    ch->priv = priv;

    // set system interformation handler
    ch->get_no_ports     = get_no_ports;
    ch->get_port_name    = get_port_name;

    // set functional handler
    ch->open             = open;
    ch->close            = close;
    ch->send_request     = send_request;
    ch->receive_response = receive_response;

    ch->state = CTRL_CHANNEL_STATE_INIT;

    return ( 0 );
}

/******************************************************************************
 * ctrl_channel_unregister - unregister a control channel driver functions
 *****************************************************************************/
int ctrl_channel_unregister( ctrl_channel_handle_t const ch )
{
    if ( !ch )
    {
        return ( -ENODEV );
    }

    memset( ch, 0, sizeof(ctrl_channel_t) );

    ch->state = CTRL_CHANNEL_STATE_INVALID;

    return ( 0 );
}

