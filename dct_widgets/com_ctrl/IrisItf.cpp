/******************************************************************************
 *
 * Copyright 2016, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
/**
 * @file    IrisItf.cpp
 *
 * @brief   Implementation of Iris Processing Interface 
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <ctrl_protocol/ctrl_protocol_iris.h>

#include "common.h"
#include "IrisItf.h"

#include <QtDebug>

/******************************************************************************
 * IrisItf::resync()
 *****************************************************************************/
void IrisItf::resync()
{
    GetIrisSetup();
    GetIrisApt();
}

/******************************************************************************
 * IrisItf::GetIrisSetup
 *****************************************************************************/
void IrisItf::GetIrisSetup()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(IrisSetupChanged(QVector<int>))) > 0 )
    {
        int32_t value[NO_VALUES_IRIS_SETUP];
    
        // get auto processing number of white balance presets from device
        int res = ctrl_protocol_get_iris_setup( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_IRIS_SETUP, &value[0] );
        HANDLE_ERROR( res );

        QVector<int> v_values( NO_VALUES_IRIS_SETUP );
        for ( int i = 0; i < NO_VALUES_IRIS_SETUP; i++ ) {
            v_values[i] = value[i];
        }

        // emit a IrisSetupChanged signal
        emit IrisSetupChanged( v_values );
    }
}

/******************************************************************************
 * IrisItf::onIrisSetupChange
 *****************************************************************************/
void IrisItf::onIrisSetupChange( QVector<int> value )
{
    int value_array[NO_VALUES_IRIS_SETUP];
    for ( int i = 0; i < NO_VALUES_IRIS_SETUP; i++ ) {
        value_array[i] = value[i];
    }

    int res = ctrl_protocol_set_iris_setup( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_IRIS_SETUP, &value_array[0]
    );

    HANDLE_ERROR( res );
}


/******************************************************************************
 * IrisItf::GetIrisApt
 *****************************************************************************/
void IrisItf::GetIrisApt()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(IrisAptChanged( int ))) > 0 )
    {
        int value;
        int res = ctrl_protocol_get_iris_apt(
                GET_PROTOCOL_INSTANCE(this),
                GET_CHANNEL_INSTANCE(this),
                &value
        );
        if( res ) {
            emit IrisAptError();
        }
        HANDLE_ERROR( res );

        // emit a IrisAptChanged signal
        emit IrisAptChanged( value );
    }
}

void IrisItf::onIrisAptChange( int pos )
{
    int res = ctrl_protocol_set_iris_apt(
            GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this),
            pos
    );

    HANDLE_ERROR( res );
}

