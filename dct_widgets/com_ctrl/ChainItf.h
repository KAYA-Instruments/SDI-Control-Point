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
 * @file    ChainItf.h
 *
 * @brief   CHAIN Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _CHAIN_INTERFACE_H_
#define _CHAIN_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

/******************************************************************************
 * ChainItf
 *****************************************************************************/
class ChainItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit ChainItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }

    // resync all settings
    void resync() override;

    // chain selection state 
    // Note: on muli video-chain systems only the selected chain
    // gets configuration changes (like gains, blacklevel, etc)
    void GetChainSelectedChain();

    // video mode
    void GetChainVideoMode();

    // raw output mode state
    void GetChainRawMode();

    // sdi2 output mode state
    void GetChainSdi2Mode();
    
    // flip mode state
    void GetChainFlipMode();

    // sdi range mode state
    void GetChainSdiMode();
    
    // sdi black level
    void GetChainSdiBlackLevel();
    
    // sdi white level
    void GetChainSdiWhiteLevel();
    
    // genlock mode
    void GetChainGenlockMode();
    
    // genlock offset
    void GetChainGenlockOffset();
    
    // genlock termination
    void GetChainGenlockTermination();
    
    // timecode
    void GetTimecode();
    void GetTimecodeHold();

signals:
    // chain selection status
    void ChainSelectedChainChanged( int value );
    
    // video mode
    void ChainVideoModeChanged( int value );
    
    // raw output mode
    void ChainRawModeChanged( int value );

    // sdi2 output mode
    void ChainSdi2ModeChanged( int value );
    
    // raw mode
    void ChainFlipModeChanged( int value );
    
    // sdi mode
    void ChainSdiModeChanged( int value );
    
    // sdi black level
    void ChainSdiBlackLevelChanged( int value );
    
    // sdi white level
    void ChainSdiWhiteLevelChanged( int value );
    
    // genlock mode
    void ChainGenlockModeChanged( int value );
    
    // genlock offset
    void ChainGenlockOffsetChanged( int vertical, int horzontal );
    
    // genlock termination 
    void ChainGenlockTerminationChanged( int value );

    // timecode
    void TimecodeChanged( QVector<int> value );
    void TimecodeHoldChanged( bool enable );

    // new video mode is set
    // This is used to synchronize other interface components.
    void NotifyVideoModeChanged();

public slots:
    void onChainSelectedChainChange( int value );
    void onChainVideoModeChange( int value );
    void onChainRawModeChange( int value );
    void onChainSdi2ModeChange( int value );
    void onChainFlipModeChange( int value );
    void onChainSdiModeChange( int value );
    void onChainSdiBlackLevelChange( int value );
    void onChainSdiWhiteLevelChange( int value );
    void onChainGenlockModeChange( int value );
    void onChainGenlockOffsetChange( int vertical, int horizontal );
    void onChainGenlockTerminationChange( int value );
    void onTimecodeChange( QVector<int> value );
    void onTimecodeGetRequest( );
    void onTimecodeHoldChange( bool enable );
};

#define CONNECT_CHAIN_INTERFACE(x, y)                               \
{                                                                   \
    QObject::connect( x, SIGNAL(ChainSelectedChainChanged(int)),    \
                      y, SLOT(onChainSelectedChainChange(int)) );   \
    QObject::connect( x, SIGNAL(ChainVideoModeChanged(int)),        \
                      y, SLOT(onChainVideoModeChange(int)) );       \
    QObject::connect( x, SIGNAL(ChainSdi2ModeChanged(int)),         \
                      y, SLOT(onChainSdi2ModeChange(int)) );        \
    QObject::connect( x, SIGNAL(ChainFlipModeChanged(int)),         \
                      y, SLOT(onChainFlipModeChange(int)) );        \
    QObject::connect( x, SIGNAL(ChainGenlockModeChanged(int)),      \
                      y, SLOT(onChainGenlockModeChange(int)) );     \
    QObject::connect( x, SIGNAL(ChainSdiModeChanged(int)),          \
                      y, SLOT(onChainSdiModeChange(int)) );         \
    QObject::connect( x, SIGNAL(ChainSdiBlackLevelChanged(int)),    \
                      y, SLOT(onChainSdiBlackLevelChange(int)) );   \
    QObject::connect( x, SIGNAL(ChainSdiWhiteLevelChanged(int)),    \
                      y, SLOT(onChainSdiWhiteLevelChange(int)) );   \
}

#endif // _CHAIN_INTERFACE_H_

