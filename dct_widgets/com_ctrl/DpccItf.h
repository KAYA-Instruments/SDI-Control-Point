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
 * @file    DpccItf.h
 *
 * @brief   DPCC Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _DPCC_INTERFACE_H_
#define _DPCC_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

class DpccItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit DpccItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }

    // resync all settings
    void resync() override;

    // enable state 
    void GetDpccEnable();

    // operation mode 
    void GetDpccCorrectionMode();
    
    // operation mode 
    void GetDpccDetectionLevel();
    
    // defect pixel table
    void GetDpccTable();
    void SetDpccTable(QVector<int> & x, QVector<int> & y);

    // test mode
    void GetDpccTestMode();
    
signals:
    // enable status
    void DpccEnableChanged( int value );

    // operation mode 
    void DpccCorrectionModeChanged( int mode );
    
    // detection level
    void DpccDetectionLevelChanged( int level );
    
    // dpcc table
    void DpccTableChanged( QVector<int> x, QVector<int> y );

    // test mode
    void DpccTestModeChanged( int mode );
    
public slots:
    // enable status
    void onDpccEnableChange( int value );

    // operation mode 
    void onDpccCorrectionModeChange( int mode );
    
    // operation level 
    void onDpccDetectionLevelChange( int level );

    // add one pixel to dpcc table
    void onDpccAddPixel( int x, int y );

    // set complete dpcc table
    void onDpccSetTable(QVector<int> & x, QVector<int> & y);

    // get complete dpcc table
    void onDpccGetTable();

    // clear complete dpcc table
    void onDpccClearTable();
    
    // save dpcc table to cam-device's flash
    void onDpccSaveTable();
    
    // load dpcc table from cam-device's flash
    void onDpccLoadTable();

    // automatically load dpcc table
    void onDpccAutoLoadTable();

    // test mode
    void onDpccTestModeChange( int mode );
};

#endif // _DPCC_INTERFACE_H_

