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
 * @file    MccItf.cpp
 *
 * @brief   Implementation of MCC Interface 
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <ctrl_protocol/ctrl_protocol_mcc.h>

#include "common.h"
#include "MccItf.h"

#include <QtDebug>

/******************************************************************************
 * MccItf::resync()
 *****************************************************************************/
void MccItf::resync()
{
    // enable status
    GetMccEnable();

    // operation mode 
    GetMccOperationMode();
}

/******************************************************************************
 * MccItf::GetMccEnable
 *****************************************************************************/
void MccItf::GetMccEnable()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(MccEnableChanged(int))) > 0 )
    {
        uint8_t value;
    
        // read mcc enable from device
        int res = ctrl_protocol_get_mcc_enable( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a MccOperationModeChanged signal
        emit MccEnableChanged( (int)value );
    }
}

/******************************************************************************
 * MccItf::GetMccOperationMode
 *****************************************************************************/
void MccItf::GetMccOperationMode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(MccOperationModeChanged(int,int))) > 0 )
    {
        uint8_t mode;
        uint8_t phases;
    
        // read mcc operation mode from device
        int res = ctrl_protocol_get_mcc_opmode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &mode );
        HANDLE_ERROR( res );
        
        res = ctrl_protocol_get_mcc_no_phases( mode, &phases );
        HANDLE_ERROR( res );

        // emit a MccOperationModeChanged signal
        emit MccOperationModeChanged( (int)mode, (int) phases );

        // sync all phases
        GetMccPhases( (int)mode );
    }
}

/******************************************************************************
 * MccItf::GetMccNoPhases
 *****************************************************************************/
void MccItf::GetMccNoPhases( int mode )
{
    // Is there a signal listener
    if ( receivers(SIGNAL(MccNoPhasesChanged(int))) > 0 )
    {
        uint8_t phases;

        // get number of phases regarding operation mode
        int res = ctrl_protocol_get_mcc_no_phases( mode, &phases );
        HANDLE_ERROR( res );

        // emit a MccOperationModeChanged signal
        emit MccNoPhasesChanged( (int)phases ); 
    }
}

/******************************************************************************
 * MccItf::GetMccPhase
 *****************************************************************************/
void MccItf::GetMccPhase( int id )
{
    // Is there a signal listener
    if ( receivers(SIGNAL(MccPhaseChanged(int,int,int))) > 0 )
    {
        ctrl_protocol_mcc_phase_t phase 
            = { .id = (uint8_t)id, .saturation = 0u, .hue = 0 };

        int res = ctrl_protocol_get_mcc_phase( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(phase), (uint8_t *)&phase );
        HANDLE_ERROR( res );

        // emit a MccOperationModeChanged signal
        emit MccPhaseChanged( (int)phase.id, (int)phase.saturation, (int)phase.hue ); 
    }
}

/******************************************************************************
 * MccItf::GetMccPhases
 *****************************************************************************/
void MccItf::GetMccPhases( int mode )
{
    // Is there a signal listener
    if ( receivers(SIGNAL(MccPhaseChanged(int,int,int))) > 0 )
    {
        uint8_t phases;

        int res = ctrl_protocol_get_mcc_no_phases( mode, &phases );
        HANDLE_ERROR( res );

        for ( unsigned id = 0u; id<phases; id++ )
        {
            ctrl_protocol_mcc_phase_t phase
                = { .id = (uint8_t)id, .saturation = 0u, .hue = 0 };

            res = ctrl_protocol_get_mcc_phase( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), sizeof(phase), (uint8_t *)&phase );
            HANDLE_ERROR( res );

            // emit a MccOperationModeChanged signal
            emit MccPhaseChanged( (int)phase.id, (int)phase.saturation, (int)phase.hue ); 
        }
    }
}

/******************************************************************************
 * MccItf::onMccEnableChange
 *****************************************************************************/
void MccItf::onMccEnableChange( int value )
{
    // set mcc enable state on device
    int res = ctrl_protocol_set_mcc_enable( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), ( value ? 1u : 0u ) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * MccItf::onMccOperationModeChange
 *****************************************************************************/
void MccItf::onMccOperationModeChange( int mode, int no_phases )
{
    ( void )no_phases;

    // set mcc operation mode on device
    int res = ctrl_protocol_set_mcc_opmode( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint8_t)mode );
    HANDLE_ERROR( res );

    // sync number of phases
    GetMccNoPhases( mode );

    // sync all phases
    GetMccPhases( mode );
}

/******************************************************************************
 * MccItf::onMccPhaseSelectionChange
 *****************************************************************************/
void MccItf::onMccPhaseSelectionChange( int id )
{
    uint32_t value = ((id >= 0) && (id<32)) ? ((uint32_t)(1 << id)) : 0u;

    // set mcc saturation blink mode on device
    int res = ctrl_protocol_set_mcc_blink( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * MccItf::onMccPhaseChange
 *****************************************************************************/
void MccItf::onMccPhaseChange( int id, int saturation, int hue )
{
    ctrl_protocol_mcc_phase_t phase 
        = { .id = (uint8_t)id, .saturation = (uint16_t)saturation, .hue = (int16_t)hue };

    // set mcc phase configuration on device
    int res = ctrl_protocol_set_mcc_phase( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), sizeof(phase), (uint8_t *)&phase );
    HANDLE_ERROR( res );
}

