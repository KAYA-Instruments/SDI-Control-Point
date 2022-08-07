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
 * @file    MccItf.h
 *
 * @brief   MCC Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _MCC_INTERFACE_H_
#define _MCC_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

class MccItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit MccItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }

    // resync all settings
    void resync() override;

    // enable state 
    void GetMccEnable();

    // operation mode 
    void GetMccOperationMode();
    
    // number of color phases
    void GetMccNoPhases( int mode );

    // color phase setting
    void GetMccPhase( int id );
    
    // color phase setting
    void GetMccPhases( int mode );

    // color phase blink
    void GetMccPhaseSelectionBlink();

signals:
    // enable status
    void MccEnableChanged( int value );

    // operation mode 
    void MccOperationModeChanged( int mode, int no_phases );
    
    // number of color phases
    void MccNoPhasesChanged( int value );
    
    // number of color phases
    void MccPhaseChanged( int id, int saturation, int hue );

    // number of color phases
    void MccPhaseSelectionBlinkChanged( int id, int period );

public slots:
    // enable status
    void onMccEnableChange( int value );

    // operation mode 
    void onMccOperationModeChange( int mode, int no_phases );
    
    // enable saturation blink for seletced phase
    void onMccPhaseSelectionChange( int id, int period );
    
    // number of color phases
    void onMccPhaseChange( int id, int saturation, int hue );
};

#endif // _MCC_INTERFACE_H_

