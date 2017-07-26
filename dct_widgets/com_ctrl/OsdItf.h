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
 * @file    OsdItf.h
 *
 * @brief   Osd Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _OSD_INTERFACE_H_
#define _OSD_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

class OsdItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit OsdItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }

    // resync all settings
    void resync() override;

    // test pattern
    void GetTestPattern();

signals:
    // test pattern
    void TestPatternChanged( int pattern );

public slots:
    // test pattern
    void onTestPatternChange( int pattern );
};

#define CONNECT_OSD_INTERFACE(x, y)                                    \
{                                                                       \
    QObject::connect( x, SIGNAL(TestPatternChanged(int)),    \
                      y, SLOT(onTestPatternChanged(int)) );   \
}

#endif // _OSD_INTERFACE_H_

