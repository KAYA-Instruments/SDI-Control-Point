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
 * @file    IrisItf.h
 *
 * @brief   Iris Processing Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _IRIS_INTERFACE_H_
#define _IRIS_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

class IrisItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit IrisItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }
 
    // resync all settings
    void resync() override;

    void GetIrisSetup();
    void GetIrisApt();


signals:
    void IrisSetupChanged( QVector<int> values );
    void IrisAptChanged( int pos );
    void IrisAptError( void );

public slots:
    void onIrisSetupChange( QVector<int> values );
    void onIrisAptChange( int );

};

#endif // _IRIS_INTERFACE_H_



