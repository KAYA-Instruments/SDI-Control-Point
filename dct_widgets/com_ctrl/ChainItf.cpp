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

#include <QApplication>
#include <QMessageBox>
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

    // Downscale Mode for each output
    /* Note: On some devices not all chains will be available, in this case GetDownscale
     * will report an error, but that is no problem */
    for ( int i = 1; i <= MAX_NUM_CHAINS; i++ )
    {
        GetChainDownscaleMode( i );
    }

    GetChainFlipMode();
    GetChainSdiMode();
    GetChainSdiBlackLevel();
    GetChainSdiWhiteLevel();
    GetChainGenlockMode();
    GetChainGenlockOffset();
    GetChainGenlockTermination();
    GetChainTimecode();
    GetChainTimecodeHold();
    GetChainAudioEnable();
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
 * ChainItf::GetChainDownscaleMode
 *****************************************************************************/
void ChainItf::GetChainDownscaleMode( int id )
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ChainDownscaleModeChanged(int, bool, bool))) > 0 )
    {
        ctrl_protocol_downscale_enable_t v = { .id = (uint8_t)id, .downscale = 0u, .interlace = 0u };

        // read enable state from device
        int res = ctrl_protocol_get_downscale_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(v), (uint8_t *)&v );
        HANDLE_ERROR( res );

        // emit a ChainGenlockOffsetChanged signal
        emit ChainDownscaleModeChanged( (int)v.id, (bool)v.downscale, (bool)v.interlace );
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
 * ChainItf::GetChainTimecode
 *****************************************************************************/
void ChainItf::GetChainTimecode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ChainTimecodeChanged(QVector<int>))) > 0 )
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
        emit ChainTimecodeChanged( v );
    }
}

/******************************************************************************
 * ChainItf::GetChainTimecodeHold
 *****************************************************************************/
void ChainItf::GetChainTimecodeHold()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ChainTimecodeHoldChanged(bool))) > 0 )
    {
        uint8_t enable;

        // get timecode hold flag from device
        int res = ctrl_protocol_get_timecode_hold( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &enable );
        HANDLE_ERROR( res );

        // emit a TimecodeHoldChanged signal
        emit ChainTimecodeHoldChanged( enable > 0 ? true : false );
    }
}

/******************************************************************************
 * ChainItf::GetChainAudioEnable
 *****************************************************************************/
void ChainItf::GetChainAudioEnable()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ChainAudioEnableChanged(bool))) > 0 )
    {
        uint8_t enable;

        // get audio enable flag from device
        int res = ctrl_protocol_get_audio_enable( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &enable );
        HANDLE_ERROR( res );

        // emit a TimecodeHoldChanged signal
        emit ChainAudioEnableChanged( enable > 0 ? true : false );
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

    /* In case swtich was successful, or if a genlock error occured,
     * notify video mode was changed to get new exposure range. */
    if ( res == 0 || res == -ENOSYS )
    {
        emit NotifyVideoModeChanged();

        // If a genlock error occured, display a message box
        if ( res == -ENOSYS )
        {
            QApplication::setOverrideCursor( Qt::ArrowCursor );

            QMessageBox msgBox;
            msgBox.setWindowTitle("Genlock Error");
            msgBox.setText("The device can not lock to the supplied sync source.\n\n"
                           "Please make sure you have attached a valid sync signal "
                           "that matches the currently setup video mode.");
            msgBox.exec();
        }
    }

    HANDLE_ERROR( res );
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

    /* Get downscale mode of the second sdi output as it gets disabled
     * after switchting to RAW mode */
    GetChainDownscaleMode( 2 );
}

/******************************************************************************
 * ChainItf::onChainDownscaleModeChange
 *****************************************************************************/
void ChainItf::onChainDownscaleModeChange( int sdi_out_idx, bool downscale, bool interlace )
{
    // convert to array
    uint8_t values[3];
    values[0] = (uint8_t)sdi_out_idx;
    values[1] = (uint8_t)downscale;
    values[2] = (uint8_t)interlace;

    // set downscale mode on device
    int res = ctrl_protocol_set_downscale_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), 3, values );
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

    // check if a genlock error occured and display a message box
    if ( res == -ENOSYS )
    {
        QApplication::setOverrideCursor( Qt::ArrowCursor );

        QMessageBox msgBox;
        msgBox.setWindowTitle("Genlock Error");
        msgBox.setText("The device can not lock to the supplied sync source.\n\n"
                       "Please make sure you have attached a valid sync signal "
                       "that matches the currently setup video mode.");
        msgBox.exec();
    }

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
 * ChainItf::onChainTimecodeChange
 *****************************************************************************/
void ChainItf::onChainTimecodeChange( QVector<int> value )
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
 * ChainItf::onChainTimecodeGetRequest
 *****************************************************************************/
void ChainItf::onChainTimecodeGetRequest( )
{
    GetChainTimecode();
}

/******************************************************************************
 * ChainItf::onChainTimecodeHoldChange
 *****************************************************************************/
void ChainItf::onChainTimecodeHoldChange( bool enable )
{
    int res = ctrl_protocol_set_timecode_hold( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), (uint8_t)enable );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ChainItf::onChainAudioEnableChange
 *****************************************************************************/
void ChainItf::onChainAudioEnableChange( bool enable )
{
    int res = ctrl_protocol_set_audio_enable( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), (uint8_t)enable );
    HANDLE_ERROR( res );
}
