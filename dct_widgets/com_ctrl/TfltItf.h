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
 * @file    TfltItf.h
 *
 * @brief   Temporal Filter Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _TFLT_INTERFACE_H_
#define _TFLT_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

class TfltItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit TfltItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }

    // resync all settings
    void resync() override;

    // enable state 
    void GetTfltEnable();
    
    // denoise level
    void GetTfltDenoiseLevel();
    
    // min/max range
    void GetTfltMinMax();

signals:
    // enable status
    void TfltEnableChanged( int value );
    
    // denoise level
    void TfltDenoiseLevelChanged( int value );
    
    // min/max range
    void TfltMinMaxChanged( int min, int max );

public slots:
    // enable status
    void onTfltEnableChange( int value );
    
    // denoise level
    void onTfltDenoiseLevelChange( int value );
    
    // min/max range
    void onTfltMinMaxChange( int min, int max );
};

#define CONNECT_TFLT_INTERFACE(x, y)                            \
{                                                               \
    QObject::connect( x, SIGNAL(TfltEnableChanged(int)),        \
                      y, SLOT(onTfltEnableChange(int)) );       \
    QObject::connect( x, SIGNAL(TfltDenoiseLevelChanged(int)),  \
                      y, SLOT(onTfltDenoiseLevelChange(int)) ); \
    QObject::connect( x, SIGNAL(TfltMinMaxChanged(int,int)),    \
                      y, SLOT(onTfltMinMaxChange(int,int)) );   \
}


#endif // _TFLT_INTERFACE_H_

