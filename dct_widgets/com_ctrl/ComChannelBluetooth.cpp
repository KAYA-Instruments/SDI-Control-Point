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
 * @file    ComChannelBluetooth.cpp
 *
 * @brief   
 *
 *****************************************************************************/
#include <cerrno>

#include <QtDebug>

#include "common.h"
#include "ComChannelBluetooth.h"

/******************************************************************************
 * ctrl_channel_qtbluetooth_open
 *****************************************************************************/
static int ctrl_channel_qtbluetooth_open
(
    void * const    handle,
    void * const    param,
    int const       size
)
{
    // type cast context
    ComChannelBluetooth * com = (ComChannelBluetooth *)handle;
   
    // type cast open configuration
    ctrl_channel_generic_config_t * conf  = (ctrl_channel_generic_config_t *)param;

    // check parameter
    if ( !com || !conf || (size != sizeof(ctrl_channel_generic_config_t)) )
    {
        return ( -EINVAL );
    }

    // set port instance
    com->setSocket( new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol) );

    return ( 0 );
}

/******************************************************************************
 * ctrl_channel_qtbluetooth_close
 *****************************************************************************/
static int ctrl_channel_qtbluetooth_close( void * const handle )
{
    // type cast context
    ComChannelBluetooth * com = (ComChannelBluetooth *)handle;
    if ( com )
    {
        QBluetoothSocket * socket = com->getSocket();
        if ( socket )
        {
            socket->disconnectFromService();
            delete socket;
        }
    }
    return ( 0 );
}

/******************************************************************************
 * ctrl_channel_qtbluetooth_send_request
 *****************************************************************************/
static int ctrl_channel_qtbluetooth_send_request
(
    void * const    handle,
    uint8_t * const data,
    int const       len
)
{
    // type cast context
    ComChannelBluetooth * com = (ComChannelBluetooth *)handle;
    if ( com )
    {
        QBluetoothSocket * socket = com->getSocket();
        if ( socket )
        {
            return ( socket->write( (char *)data, len ) );
        }
    }
    return ( 0 );
}

/******************************************************************************
 * ctrl_channel_qtbluetooth_receive_response
 *****************************************************************************/
static int ctrl_channel_qtbluetooth_receive_response
(
    void * const    handle,
    uint8_t * const data,
    int const       len
)
{
    // type cast context
    ComChannelBluetooth * com = (ComChannelBluetooth *)handle;
    if ( com )
    {
        QBluetoothSocket * socket = com->getSocket();
        if ( socket && socket->canReadLine() )
        {
            return ( socket->read( (char *)data, len ) );
        }
    }
    return ( 0 );
}

/******************************************************************************
 * ComChannelBluetooth::ComChannelBluetooth
 *****************************************************************************/
ComChannelBluetooth::ComChannelBluetooth( void )
{
    // register functions 
    int res = ctrl_channel_register( GetInstance(), this,
                    NULL, NULL,
                    ctrl_channel_qtbluetooth_open,
                    ctrl_channel_qtbluetooth_close,
                    ctrl_channel_qtbluetooth_send_request,
                    ctrl_channel_qtbluetooth_receive_response );
    if ( res )
    {
        showError( res, __FILE__, __FUNCTION__, __LINE__ ); 
        return;
    }
}


