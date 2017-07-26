/******************************************************************************
 *
 * Copyright 2015, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
/**
 * @file    ctrl_channel_rs232.c
 *
 * @brief   Implementation of a RS232/RSUSB command interface
 *
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <ctrl_channel/ctrl_channel.h>
#include <ctrl_channel/ctrl_channel_serial.h>

#include <rs232/ctrl_channel_rs232.h>

// header files of system specific implementation
#include "rs232.h"

/******************************************************************************
 * ctrl_channel_rs232_get_no_ports - Return the number of available serial 
 * ports on the system
 *****************************************************************************/
static int ctrl_channel_rs232_get_no_ports( void * const handle )
{
    (void) handle;

    // call system implementation 
    return ( rs232_get_no_ports() );
}

/******************************************************************************
 * ctrl_channel_rs232_get_port_name - Return a system name/description of the 
 * index port
 *****************************************************************************/
static int ctrl_channel_rs232_get_port_name
(
    void * const        handle,
    int const           idx,
    ctrl_channel_name_t name
)
{
    (void) handle;

    // call system implementation 
    return ( rs232_get_port_name( idx, name ) );
}


/******************************************************************************
 * ctrl_channel_rs232_open - open a control channel to a serial connected device
 *****************************************************************************/
static int ctrl_channel_rs232_open
(
    void * const    handle,
    void * const    param,
    int const       size
)
{
    // type cast open configuration
    ctrl_channel_rs232_open_config_t * conf  = (ctrl_channel_rs232_open_config_t *)param;
    
    // type cast internal context
    ctrl_channel_rs232_context_t * ctx = (ctrl_channel_rs232_context_t *)handle;

    int res;

    // parameter check
    if ( !param || (sizeof(ctrl_channel_rs232_open_config_t) != size) )
    {
        return ( -EINVAL );
    }

    // default settings if not configured
    if ( conf->baudrate == 0u )
    {
        conf->baudrate = CTRL_CHANNEL_BAUDRATE_DEFAULT;
    }

    if ( conf->data == 0u )
    {
        conf->data = CTRL_CHANNEL_DATA_BITS_DEFAULT;
    }

    if ( conf->parity == 0u )
    {
        conf->parity = CTRL_CHANNEL_PARITY_DEFAULT;
    }

    if ( conf->stop == 0u )
    {
        conf->stop = CTRL_CHANNEL_STOP_BITS_DEFAULT;
    }
      

    // call system implementation 
    res = rs232_open( conf->idx, conf->baudrate, conf->data, conf->parity, conf->stop );
    if ( res )
    {
        return ( res );
    }

    // update internal context
    ctx->idx = conf->idx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_channel_rs232_close - close a existing control channel
 *****************************************************************************/
static int ctrl_channel_rs232_close( void * const handle )
{
    // type cast internal context
    ctrl_channel_rs232_context_t * ctx = (ctrl_channel_rs232_context_t *)handle;

    // call system implementation 
    rs232_close( ctx->idx );
        
    // update internal context
    ctx->idx    = 0;

    return ( 0 );
}

/******************************************************************************
 * ctrl_channel_rs232_send_request - send a command request data buffer to a 
 * connected device
 *****************************************************************************/
static int ctrl_channel_rs232_send_request
(
    void * const    handle,
    uint8_t * const data,
    int const       len
)
{
    // type cast internal context
    ctrl_channel_rs232_context_t * ctx = (ctrl_channel_rs232_context_t *)handle;

    // parameter check
    if ( !data || (len <= 0) )
    {
        return ( -EINVAL );
    }

    // call system implementation 
    return ( rs232_send( ctx->idx, (char *)data, len ) );
}

/******************************************************************************
 * ctrl_channel_rs232_poll - receive response data from a connected device
 *****************************************************************************/
static int ctrl_channel_rs232_receive_response
(
    void * const    handle,
    uint8_t * const data,
    int const       len
)
{
    // type cast internal context
    ctrl_channel_rs232_context_t * ctx = (ctrl_channel_rs232_context_t *)handle;

    // parameter check
    if ( !data || (len <= 0) )
    {
        return ( -EINVAL );
    }

    // call system implementation 
    return ( rs232_poll( ctx->idx, (char *)data, len ) );
}

/******************************************************************************
 * ctrl_channel_rs232_init - init serial control channel interface
 *****************************************************************************/
int ctrl_channel_rs232_init
(
    ctrl_channel_handle_t const             ch,
    ctrl_channel_rs232_context_t * const    ctx
)
{
    int res = ctrl_channel_register( ch, ctx, 
                            ctrl_channel_rs232_get_no_ports,
                            ctrl_channel_rs232_get_port_name,
                            ctrl_channel_rs232_open,
                            ctrl_channel_rs232_close,
                            ctrl_channel_rs232_send_request,
                            ctrl_channel_rs232_receive_response );
    if ( res )
    {
        return ( res );
    }

    // call system implementation 
    return ( rs232_init( ) );
}


