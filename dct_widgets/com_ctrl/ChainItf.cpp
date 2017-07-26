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
 * @file    ChainItf.cpp
 *
 * @brief   Implementation of CHAIN Interface 
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <ctrl_protocol/ctrl_protocol_chain.h>

#include "common.h"
#include "defines.h"

#include "ChainItf.h"

#include <QtDebug>

/******************************************************************************
 * ChainItf::resync()
 *****************************************************************************/
void ChainItf::resync()
{
    GetChainSelectedChain();
    GetChainVideoMode();
    GetChainRawMode();
    GetChainSdi2Mode();
    GetChainFlipMode();
    GetChainSdiMode();
    GetChainSdiBlackLevel();
    GetChainSdiWhiteLevel();
    GetChainGenlockMode();
    GetChainGenlockOffset();
    GetChainGenlockTermination();
    GetTimecode();
}

/******************************************************************************
 * ChainItf::GetSelectedChain
 *****************************************************************************/
void ChainItf::GetChainSelectedChain()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ChainSelectedChainChanged(int))) > 0 )
    {
        uint8_t value;
    
        // read selected chain from device
        int res = ctrl_protocol_get_output_chain( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a SelectedChainChanged signal
        emit ChainSelectedChainChanged( (int)value );
    }
}

/******************************************************************************
 * ChainItf::GetChainVideoMode
 *****************************************************************************/
void ChainItf::GetChainVideoMode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ChainVideoModeChanged(int))) > 0 )
    {
        uint8_t value;
    
        // read video-mode from device
        int res = ctrl_protocol_get_video_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a SelectedChainChanged signal
        emit ChainVideoModeChanged( (int)value );
    }
}

/******************************************************************************
 * ChainItf::GetChainRawMode
 *****************************************************************************/
void ChainItf::GetChainRawMode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ChainRawModeChanged(int))) > 0 )
    {
        uint8_t value;
    
        // read video-mode from device
        int res = ctrl_protocol_get_raw_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a SelectedChainChanged signal
        emit ChainRawModeChanged( (int)value );
    }
}

/******************************************************************************
 * ChainItf::GetChainSdi2Mode
 *****************************************************************************/
void ChainItf::GetChainSdi2Mode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ChainSdi2ModeChanged(int))) > 0 )
    {
        uint8_t value;
    
        // read video-mode from device
        int res = ctrl_protocol_get_sdi2_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a SelectedChainChanged signal
        emit ChainSdi2ModeChanged( (int)value );
    }
}

/******************************************************************************
 * ChainItf::GetChainFlipMode
 *****************************************************************************/
void ChainItf::GetChainFlipMode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ChainFlipModeChanged(int))) > 0 )
    {
        uint8_t value;
    
        // read raw-mode from device
        int res = ctrl_protocol_get_flip_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a ChainFlipModeChanged signal
        emit ChainFlipModeChanged( (int)value );
    }
}

/******************************************************************************
 * ChainItf::GetChainSdiMode
 *****************************************************************************/
void ChainItf::GetChainSdiMode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ChainSdiModeChanged(int))) > 0 )
    {
        uint8_t value;
    
        // read SDI mode from device
        int res = ctrl_protocol_get_sdi_range( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a ChainSdiModeChanged signal
        emit ChainSdiModeChanged( (int)value );
    }
}

/******************************************************************************
 * ChainItf::GetChainSdiBlackLevel
 *****************************************************************************/
void ChainItf::GetChainSdiBlackLevel()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ChainSdiBlackLevelChanged(int))) > 0 )
    {
        int8_t value;
    
        // read SDI black level from device
        int res = ctrl_protocol_get_sdi_black( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a ChainSdiBlackLevelChanged signal
        emit ChainSdiBlackLevelChanged( (int)value );
    }
}

/******************************************************************************
 * ChainItf::GetChainSdiWhiteLevel
 *****************************************************************************/
void ChainItf::GetChainSdiWhiteLevel()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ChainSdiWhiteLevelChanged(int))) > 0 )
    {
        int8_t value;
    
        // read SDI white level from device
        int res = ctrl_protocol_get_sdi_white( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a ChainSdiWhiteLevelChanged signal
        emit ChainSdiWhiteLevelChanged( (int)value );
    }
}

/******************************************************************************
 * ChainItf::GetChainGenlockMode
 *****************************************************************************/
void ChainItf::GetChainGenlockMode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ChainGenlockModeChanged(int))) > 0 )
    {
        uint8_t value;
    
        // read genlock mode from device
        int res = ctrl_protocol_get_genlock_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a ChainGenlockModeChanged signal
        emit ChainGenlockModeChanged( (int)value );
    }
}

/******************************************************************************
 * ChainItf::GetChainGenlockOffset
 *****************************************************************************/
void ChainItf::GetChainGenlockOffset()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ChainGenlockOffsetChanged(int, int))) > 0 )
    {
        int16_t values[2];
    
        // read genlock offset from device
        int res = ctrl_protocol_get_genlock_offset( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), 2, values );
        HANDLE_ERROR( res );
        
        // emit a ChainGenlockOffsetChanged signal
        emit ChainGenlockOffsetChanged( (int)values[0], (int)values[1] );
    }
}

/******************************************************************************
 * ChainItf::GetChainGenlockTermination
 *****************************************************************************/
void ChainItf::GetChainGenlockTermination()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ChainGenlockTerminationChanged(int))) > 0 )
    {
        uint8_t value;
    
        // read genlock mode from device
        int res = ctrl_protocol_get_genlock_termination( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a ChainGenlockTerminationChanged signal
        emit ChainGenlockTerminationChanged( (int)value );
    }
}

/******************************************************************************
 * ChainItf::GetTimecode
 *****************************************************************************/
void ChainItf::GetTimecode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(TimecodeChanged(QVector<int>))) > 0 )
    {
        int32_t value[NO_VALUES_TIMECODE];
    
        // get auto processing number of white balance presets from device
        int res = ctrl_protocol_get_timecode( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_TIMECODE, &value[0] );
        HANDLE_ERROR( res );

        QVector<int> v( NO_VALUES_TIMECODE );
        for ( int i = 0; i < NO_VALUES_TIMECODE; i++ )
        {
            v[i] = value[i];
        }

        // emit a TimecodeChanged signal
        emit TimecodeChanged( v );
    }
}

/******************************************************************************
 * ChainItf::onChainSelectedChainChange
 *****************************************************************************/
void ChainItf::onChainSelectedChainChange( int value )
{
    // select chain on device
    int res = ctrl_protocol_set_output_chain( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ChainItf::onChainVideoModeChange
 *****************************************************************************/
void ChainItf::onChainVideoModeChange( int value )
{
    // set video-mode on device
    int res = ctrl_protocol_set_video_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );

    emit NotifyVideoModeChanged();
}

/******************************************************************************
 * ChainItf::onChainRawModeChange
 *****************************************************************************/
void ChainItf::onChainRawModeChange( int value )
{
    // set RAW output mode on device
    int res = ctrl_protocol_set_raw_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ChainItf::onChainSdi2ModeChange
 *****************************************************************************/
void ChainItf::onChainSdi2ModeChange( int value )
{
    // set SDI-2 output mode on device
    int res = ctrl_protocol_set_sdi2_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ChainItf::onChainFlipModeChange
 *****************************************************************************/
void ChainItf::onChainFlipModeChange( int value )
{
    // set raw mode on device
    int res = ctrl_protocol_set_flip_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ChainItf::onChainSdiModeChange
 *****************************************************************************/
void ChainItf::onChainSdiModeChange( int value )
{
    // set sdi range mode on device
    int res = ctrl_protocol_set_sdi_range( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ChainItf::onChainSdiBlackLevelChange
 *****************************************************************************/
void ChainItf::onChainSdiBlackLevelChange( int value )
{
    // set sdi black level on device
    int res = ctrl_protocol_set_sdi_black( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ChainItf::onChainSdiWhiteLevelChange
 *****************************************************************************/
void ChainItf::onChainSdiWhiteLevelChange( int value )
{
    // set sdi white level on device
    int res = ctrl_protocol_set_sdi_white( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ChainItf::onChainGenlockModeChange
 *****************************************************************************/
void ChainItf::onChainGenlockModeChange( int value )
{
    // set genlock mode on device
    int res = ctrl_protocol_set_genlock_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), (uint8_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ChainItf::onChainGenlockOffsetChange
 *****************************************************************************/
void ChainItf::onChainGenlockOffsetChange( int vertical, int horizontal )
{
    // convert to array
    int16_t values[2];
    values[0] = (int16_t)vertical;
    values[1] = (int16_t)horizontal;

    // set genlock offset on device
    int res = ctrl_protocol_set_genlock_offset( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), 2, values );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ChainItf::onChainGenlockTerminationChange
 *****************************************************************************/
void ChainItf::onChainGenlockTerminationChange( int value )
{
    // set genlock termination on device
    int res = ctrl_protocol_set_genlock_termination( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), (uint16_t)value );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ChainItf::onTimecodeChange
 *****************************************************************************/
void ChainItf::onTimecodeChange( QVector<int> value )
{
    int v[NO_VALUES_TIMECODE];

    for ( int i = 0; i < NO_VALUES_TIMECODE; i++ )
    {
        v[i] = value[i];
    }

    int res = ctrl_protocol_set_timecode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_TIMECODE, &v[0] );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ChainItf::onTimecodeGetRequest
 *****************************************************************************/
void ChainItf::onTimecodeGetRequest( )
{
    GetTimecode();
}

