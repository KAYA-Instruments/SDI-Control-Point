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

