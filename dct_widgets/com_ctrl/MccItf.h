/******************************************************************************
 *
 * Copyright 2015, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
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

signals:
    // enable status
    void MccEnableChanged( int value );

    // operation mode 
    void MccOperationModeChanged( int mode, int no_phases );
    
    // number of color phases
    void MccNoPhasesChanged( int value );
    
    // number of color phases
    void MccPhaseChanged( int id, int saturation, int hue );

public slots:
    // enable status
    void onMccEnableChange( int value );

    // operation mode 
    void onMccOperationModeChange( int mode, int no_phases );
    
    // enable saturation blink for seletced phase
    void onMccPhaseSelectionChange( int id );
    
    // number of color phases
    void onMccPhaseChange( int id, int saturation, int hue );
};

#define CONNECT_MCC_INTERFACE(x, y)                                 \
{                                                                   \
    QObject::connect( x, SIGNAL(MccEnableChanged(int)),             \
                      y, SLOT(onMccEnableChange(int)) );            \
    QObject::connect( x, SIGNAL(MccOperationModeChanged(int,int)),  \
                      y, SLOT(onMccOperationModeChange(int,int)) ); \
    QObject::connect( x, SIGNAL(MccNoPhasesChanged(int)),           \
                      y, SLOT(onMccNoPhasesChange(int)) );          \
    QObject::connect( x, SIGNAL(MccPhaseChanged(int,int,int)),      \
                      y, SLOT(onMccPhaseChange(int,int,int)) );     \
}

#endif // _MCC_INTERFACE_H_

