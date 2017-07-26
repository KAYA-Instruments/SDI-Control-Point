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
 * @file    playbackbox.h
 *
 * @brief   Class definition of a playback control box 
 *
 *****************************************************************************/
#ifndef __PLAYBACK_BOX_H__
#define __PLAYBACK_BOX_H__

#include <dct_widgets_base.h>

/******************************************************************************
 * PLayback Control Box Widget
 *****************************************************************************/
class PlayBackBox : public DctWidgetBox
{
    Q_OBJECT

public:
    explicit PlayBackBox( QWidget * parent = 0 );
    ~PlayBackBox();

protected:
    void prepareMode( const Mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

signals:
    void BankDefaultChanged( int value );

    void RecordChanged( int value );
    void RecordStopped();
    void RecordStoppedPlayPause( int value );

    void PlayStopped();
    void PlayPaused();
    void PlayChanged( int value );
    void LiveChanged();

    void ForwardChanged( int value );
    void RewindChanged( int value );
    void SeekChanged( int value );

    void PositionUpdate();

public slots:
    void onCameraFramerateChange( int value );
    void onChainVideoModeChange( int value );
    
    void onMaxFramesChange( int value );
    void onBankChange(int id, int start, int end );
    
    void onAutoLiveModeChange( int flag );
    
    void onPlayChange( int id );
    void onRecordChange( int id );
    void onPausedChange( bool flag );
    void onPositionChange( int pos );

protected:
    void enterEvent(QEvent * ) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent * ) Q_DECL_OVERRIDE;

private slots:
    void onBankDefaultChange( int value );

    void onBankClick( int id );

    void onRecordClick();
    void onLiveClick();
    void onPlayClick();
    void onStopClick();
    void onFirstClick();
    void onLastClick();
    void onForwardClick();
    void onRewindClick();

    void onRotationChange( int value );
    void calcRotationDirection( int value );
    void onPosTimeOut();
    
private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __PLAYBACK_BOX_H__

