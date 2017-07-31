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
 * @file    PlayBackItf.h
 *
 * @brief   Playback Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _PLAYBACK_INTERFACE_H_
#define _PLAYBACK_INTERFACE_H_

#include <QObject>
#include <QVector>

#include "ProVideoItf.h"

class PlayBackItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit PlayBackItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }

    // resync all settings
    void resync() override;

    // max frames
    void GetMaxFrames();
    
    // bank configurations
    void GetBank( int id );

    // play-mode
    void GetPlayMode();
    
    // record-mode
    void GetRecordMode();
    
    // autolive-mode
    void GetAutoLiveMode();
    
    // marker-out
    void GetMarkerOut();
    
    // playback buffer
    void GetPlay();
    
    // record buffer
    void GetRecord();
    
    // paused state
    void GetPaused();
    
    // play-back position
    void GetPosition();

signals:
    void MaxFramesChanged( int value );
    void BankChanged(int id, int start, int end );

    void PlayModeChanged( int value );
    void RecordModeChanged( int value );
    void AutoLiveModeChanged( int value );
    void MarkerOutChanged( int value );
    void PlayChanged( int value );
    void RecordChanged( int value );
    void PausedChanged( bool flag );
    void PositionUpdate();
    void PositionChanged( int value );
    
public slots:
    void onBankDefaultChange( int value );

    void onPlayModeChange( int value );
    void onRecordModeChange( int value );
    void onAutoLiveModeChange( int value );
    void onMarkerOutChange( int value );
    void onPlayChange( int value );
    void onRecordChange( int value );
    void onStopRecordChange();
    void onStopRecordPlayPauseChange( int value);
    void onStopPlayChange();
    void onPauseChange();
    void onLiveChange();
    void onForwardChange( int value );
    void onRewindChange( int value );
    void onSeekChange( int value );
    void onPositionUpdate();
};

#endif // _PLAYBACK_INTERFACE_H_

