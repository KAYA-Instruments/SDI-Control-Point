
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
 * @file    CamItf.h
 *
 * @brief   Camera Processing Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _CAM_INTERFACE_H_
#define _CAM_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

class CamItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit CamItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }

    // resync all settings
    void resync() override;

    // cam setting 
    void GetCameraInfo();
    void GetCameraGain();
    void GetCameraExposure();

signals:
    // cam configuration
    void CameraInfoChanged( int, int, int, int );
    void CameraGainChanged( int value );
    void CameraExposureChanged( int value );

    // new camera analogue gain set
    // This is used to synchronize other interface components.
    void NotifyCameraGainChanged();
    void NotifyCameraExposureTimeChanged();

public slots:
    // cam configuration
    void onCameraGainChange( int );
    void onCameraExposureChange( int );

    // notifier slot for video-mode change
    void onNotifyVideoModeChange();
};

#define CONNECT_CAM_INTERFACE(x, y)                                                                             \
{                                                                                                               \
    QObject::connect( x, SIGNAL(CameraInfoChanged(int,int,int,int)),                                            \
                      y, SLOT(onCameraInfoChange(int,int,int,int)) );                                           \
    QObject::connect( x, SIGNAL(CameraGainChanged(int)),                                                        \
                      y, SLOT(onCameraGainChange(int)) );                                                       \
    QObject::connect( x, SIGNAL(CameraExposureChanged(int)),                                                    \
                      y, SLOT(onCameraExposureChange(int)) );                                                   \
}

#endif // _CAM_INTERFACE_H_
