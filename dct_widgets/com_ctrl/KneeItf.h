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
 * @file    KneeItf.h
 *
 * @brief   Knee Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _KNEE_INTERFACE_H_
#define _KNEE_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

class KneeItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit KneeItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }

    // resync all settings
    void resync() override;

    // configuration
    void GetKneeConfig();

signals:
    // enable status
    void KneeConfigChanged( int, int, int, int );

public slots:
    // enable status
    void onKneeConfigChange( int, int, int, int );
};

#define CONNECT_KNEE_INTERFACE(x, y)                                    \
{                                                                       \
    QObject::connect( x, SIGNAL(KneeConfigChanged(int,int,int,int)),    \
                      y, SLOT(onKneeConfigChange(int,int,int,int)) );   \
}

#endif // _KNEE_INTERFACE_H_

