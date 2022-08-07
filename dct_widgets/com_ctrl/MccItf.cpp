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

    // blink period
    GetMccPhaseSelectionBlink();
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
            // designators in struct initialization are not supported by MSVC
            //= { .id = (uint8_t)id, .saturation = 0u, .hue = 0 }; struct designators fix:
            = { (uint8_t)id, 0u, 0 };

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
                //= { .id = (uint8_t)id, .saturation = 0u, .hue = 0 };struct designators fix:
                = { (uint8_t)id, 0u, 0 };

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
void MccItf::onMccPhaseSelectionChange( int id, int period )
{
    uint32_t value = ((id >= 0) && (id<32)) ? ((uint32_t)(1 << id)) : 0u;

    // convert to array
    uint32_t values[2];
    values[0] = (uint32_t)value;
    values[1] = (uint32_t)period;

    // set mcc saturation blink mode on device
    int res = ctrl_protocol_set_mcc_blink( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), 2, values );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * MccItf::GetMccPhaseSelectionBlink
 *****************************************************************************/
void MccItf::GetMccPhaseSelectionBlink()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(MccPhaseSelectionBlinkChanged(int, int))) > 0 )
    {
        uint32_t values[2];

        // read mcc saturation blink mode on device
        int res = ctrl_protocol_get_mcc_blink( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), 2, (uint32_t *)values );
        HANDLE_ERROR( res );

        // emit a ChainGenlockOffsetChanged signal
        emit MccPhaseSelectionBlinkChanged( (int)values[0], (int)values[1] );
    }
}

/******************************************************************************
 * MccItf::onMccPhaseChange
 *****************************************************************************/
void MccItf::onMccPhaseChange( int id, int saturation, int hue )
{
    ctrl_protocol_mcc_phase_t phase 
        //= { .id = (uint8_t)id, .saturation = (uint16_t)saturation, .hue = (int16_t)hue };struct designators fix:
        = { (uint8_t)id, (uint32_t)saturation, (int32_t)hue };

    // set mcc phase configuration on device
    int res = ctrl_protocol_set_mcc_phase( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), sizeof(phase), (uint8_t *)&phase );
    HANDLE_ERROR( res );
}

