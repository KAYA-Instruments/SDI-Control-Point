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

