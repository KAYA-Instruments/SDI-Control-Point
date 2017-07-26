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
 * @file    PlayBackItf.cpp
 *
 * @brief   Implementation of Playback Interface
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <QtDebug>

#include <ctrl_protocol/ctrl_protocol_playback.h>

#include "common.h"
#include "PlayBackItf.h"

/******************************************************************************
 * PlayBackItf::resync()
 *****************************************************************************/
void PlayBackItf::resync()
{
    // sync max number of frames
    GetMaxFrames();

    // sync bakn/fram-buffer configuration
    GetBank( 0 );
    GetBank( 1 );
    GetBank( 2 );
    GetBank( 3 );

    // sync play-mode
    GetPlayMode();

    // sync record-mode
    GetRecordMode();

    // sync autolive-mode
    GetAutoLiveMode();

    // sync marker-out
    GetMarkerOut();
    
    // sync record buffer
    GetRecord();
    
    // sync playback buffer
    GetPlay();
    
    // sync paused state
    GetPaused();
    
    // sync position
    GetPosition();
}

/******************************************************************************
 * PlayBackItf::GetMaxFrames
 *****************************************************************************/
void PlayBackItf::GetMaxFrames()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(MaxFramesChanged(int))) > 0 )
    {
        uint32_t value = 0u;
    
        // get play-mode from device
        int res = ctrl_protocol_get_max_frames( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a PlayModeChanged signal
        emit MaxFramesChanged( (int)value );
    }
}

/******************************************************************************
 * PlayBackItf::GetBank
 *****************************************************************************/
void PlayBackItf::GetBank( int id )
{
    // Is there a signal listener
    if ( receivers(SIGNAL(BankChanged(int,int,int))) > 0 )
    {
        ctrl_protocol_fb_t v;

        memset( &v, 0, sizeof(v) );
        v.id = (uint8_t)id;
    
        // get play-mode from device
        int res = ctrl_protocol_get_buffer( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
        HANDLE_ERROR( res );

        // emit a PlayModeChanged signal
        emit BankChanged( (int)v.id, (int)v.first, (int)v.last );
    }
}

/******************************************************************************
 * PlayBackItf::GetPlayMode
 *****************************************************************************/
void PlayBackItf::GetPlayMode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(PlayModeChanged(int))) > 0 )
    {
        uint8_t value = 0u;
    
        // get play-mode from device
        int res = ctrl_protocol_get_play_mode( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a PlayModeChanged signal
        emit PlayModeChanged( (int)value );
    }
}

/******************************************************************************
 * PlayBackItf::GetRecordMode
 *****************************************************************************/
void PlayBackItf::GetRecordMode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(RecordModeChanged(int))) > 0 )
    {
        uint8_t value = 0u;
    
        // get play-mode from device
        int res = ctrl_protocol_get_record_mode( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a RecordModeChanged signal
        emit RecordModeChanged( (int)value );
    }
}

/******************************************************************************
 * PlayBackItf::GetAutoLiveMode
 *****************************************************************************/
void PlayBackItf::GetAutoLiveMode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(AutoLiveModeChanged(int))) > 0 )
    {
        uint8_t value = 0u;
    
        // get play-mode from device
        int res = ctrl_protocol_get_auto_live( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a AutoLiveModeChanged signal
        emit AutoLiveModeChanged( (int)value );
    }
}

/******************************************************************************
 * PlayBackItf::GetMarkerOut
 *****************************************************************************/
void PlayBackItf::GetMarkerOut()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(MarkerOutChanged(int))) > 0 )
    {
        uint32_t value = 0u;
    
        // get play-mode from device
        int res = ctrl_protocol_get_marker_out( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a MarkerOutChanged signal
        emit MarkerOutChanged( (int)value );
    }
}

/******************************************************************************
 * PlayBackItf::GetPlay
 *****************************************************************************/
void PlayBackItf::GetPlay()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(PlayChanged(int))) > 0 )
    {
        int value = 0;
    
        // get play-mode from device
        int res = ctrl_protocol_get_play( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), ((uint8_t *)&value) );
        if ( -EINVAL == res )
        {
            // overrule value, because a playback isn't running
            value = -1;
        }
        else
        {
            HANDLE_ERROR( res );
        }

        // emit a PlayChanged signal
        emit PlayChanged( value );
    }
}

/******************************************************************************
 * PlayBackItf::GetRecord
 *****************************************************************************/
void PlayBackItf::GetRecord()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(RecordChanged(int))) > 0 )
    {
        int value = 0;
    
        // get play-mode from device
        int res = ctrl_protocol_get_record( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), ((uint8_t *)&value) );
        if ( -EINVAL == res )
        {
            // overrule value, because a record isn't running
            value = -1;
        }
        else
        {
            HANDLE_ERROR( res );
        }

        // emit a RecordChanged signal
        emit RecordChanged( value );
    }
}

/******************************************************************************
 * PlayBackItf::GetPaused
 *****************************************************************************/
void PlayBackItf::GetPaused()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(PausedChanged(bool))) > 0 )
    {
        uint8_t value = 0u;
    
        // get play-mode from device
        int res = ctrl_protocol_paused( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a PausedChanged signal
        emit PausedChanged( value ? true : false );
    }
}

/******************************************************************************
 * PlayBackItf::GetPosition
 *****************************************************************************/
void PlayBackItf::GetPosition()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(PositionChanged(int))) > 0 )
    {
        uint32_t value = 0u;
    
        // get play-mode from device
        int res = ctrl_protocol_position( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a PositionChanged signal
        emit PositionChanged( value );
    }
}

/******************************************************************************
 * PlayBackItf::onBankDefaultChange
 *****************************************************************************/
void PlayBackItf::onBankDefaultChange( int value )
{
    // set play-mode on device
    int res = ctrl_protocol_set_buffer_default( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );

    GetBank( 0 );
    GetBank( 1 );
    GetBank( 2 );
    GetBank( 3 );
}

/******************************************************************************
 * PlayBackItf::onPlayModeChange
 *****************************************************************************/
void PlayBackItf::onPlayModeChange( int value )
{
    // set play-mode on device
    int res = ctrl_protocol_set_play_mode( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * PlayBackItf::onRecordModeChange
 *****************************************************************************/
void PlayBackItf::onRecordModeChange( int value )
{
    // set record-mode on device
    int res = ctrl_protocol_set_record_mode( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * PlayBackItf::onAutoLiveModeChange
 *****************************************************************************/
void PlayBackItf::onAutoLiveModeChange( int value )
{
    // set autolive-mode on device
    int res = ctrl_protocol_set_auto_live( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * PlayBackItf::onMarkerOutChange
 *****************************************************************************/
void PlayBackItf::onMarkerOutChange( int value )
{
    // set marker-out on device
    int res = ctrl_protocol_set_marker_out( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (uint32_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * PlayBackItf::onPlayChange
 *****************************************************************************/
void PlayBackItf::onPlayChange( int value )
{
    // set play-back on device
    int res = ctrl_protocol_set_play( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
    
    GetRecord();
    GetPlay();
    GetPaused();
    GetPosition(); 
}

/******************************************************************************
 * PlayBackItf::onRecordChange
 *****************************************************************************/
void PlayBackItf::onRecordChange( int value )
{
    // set play-back on device
    int res = ctrl_protocol_set_record( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );

    GetRecord();
    GetPlay();
    GetPaused();
}

/******************************************************************************
 * PlayBackItf::onStopRecordChange
 *****************************************************************************/
void PlayBackItf::onStopRecordChange()
{
    // set record stop on device
    int res = ctrl_protocol_stop_record( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );

    // resync record buffer
    GetRecord();
}

/******************************************************************************
 * PlayBackItf::onStopRecordPlayPauseChange
 *****************************************************************************/
void PlayBackItf::onStopRecordPlayPauseChange( int value )
{
    // set play-back on device
    int res = ctrl_protocol_stop_record( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );
    
    // set play-back on device
    res = ctrl_protocol_set_play( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
    
    // set pause playback on device
    res = ctrl_protocol_pause( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );

    // resync playback buffer
    GetRecord();
    GetPlay();
    GetPaused();
    GetPosition();
}

/******************************************************************************
 * PlayBackItf::onStopPlayChange
 *****************************************************************************/
void PlayBackItf::onStopPlayChange()
{
    // set play-back on device
    int res = ctrl_protocol_stop_play( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );

    // resync play-back buffer
    GetPlay();
}

/******************************************************************************
 * PlayBackItf::onPauseChange
 *****************************************************************************/
void PlayBackItf::onPauseChange()
{
    // pause play-back on device
    int res = ctrl_protocol_pause( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );

    // resync pause-state 
    GetPlay();
    GetPaused();
    GetPosition();
}

/******************************************************************************
 * PlayBackItf::onLiveChange
 *****************************************************************************/
void PlayBackItf::onLiveChange()
{
    // pause play-back on device
    int res = ctrl_protocol_live( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );

    // resync play-back buffer
    GetPlay();
    GetPaused();
}

/******************************************************************************
 * PlayBackItf::onForwardChange
 *****************************************************************************/
void PlayBackItf::onForwardChange( int value )
{
    // pause play-back on device
    int res = ctrl_protocol_forward( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (uint32_t)value );
    HANDLE_ERROR( res );

    // resync play-back position
    GetPosition();
}

/******************************************************************************
 * PlayBackItf::onRewindChange
 *****************************************************************************/
void PlayBackItf::onRewindChange( int value )
{
    // pause play-back on device
    int res = ctrl_protocol_rewind( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (uint32_t)value );
    HANDLE_ERROR( res );

    // resync play-back position
    GetPosition();
}

/******************************************************************************
 * PlayBackItf::onSeekChange
 *****************************************************************************/
void PlayBackItf::onSeekChange( int value )
{
    // pause play-back on device
    int res = ctrl_protocol_seek( GET_PROTOCOL_INSTANCE(this), GET_CHANNEL_INSTANCE(this), (uint32_t)value );
    HANDLE_ERROR( res );

    // resync play-back position
    GetPosition();
}

/******************************************************************************
 * PlayBackItf::onPositionUpdate
 *****************************************************************************/
void PlayBackItf::onPositionUpdate()
{
    // resync play-back position
    GetPosition();
}


