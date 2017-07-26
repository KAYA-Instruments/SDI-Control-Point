/******************************************************************************
 *
 * Copyright 2016, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
/**
 * @file    CamItf.cpp
 *
 * @brief   Implementation of Color Processing Interface 
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <ctrl_protocol/ctrl_protocol_cam.h>

#include "common.h"
#include "CamItf.h"

#include <QtDebug>

/******************************************************************************
 * CamItf::resync()
 *****************************************************************************/
void CamItf::resync()
{
    // sync camera configuration and information
    GetCameraInfo();
    GetCameraGain();
    GetCameraExposure();
}

/******************************************************************************
 * CamItf::GetCameraInfo
 *****************************************************************************/
void CamItf::GetCameraInfo()
{
    // Is there at least one signal listener
    if ( receivers(SIGNAL(CameraInfoChanged(int, int, int, int))) > 0 )
    {
        ctrl_protocol_cam_info_t i;

        // get camera info structure from device
        int res = ctrl_protocol_get_cam_info( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), sizeof(i), (uint8_t *)&i );
        HANDLE_ERROR( res );

        // emit a CameraInfoChanged signal
        emit CameraInfoChanged(
                i.min_gain, i.max_gain, i.min_exposure_time, i.max_exposure_time );
    }
}

/******************************************************************************
 * CamItf::GetCameraGain
 *****************************************************************************/
void CamItf::GetCameraGain()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(CameraGainChanged(int))) > 0 )
    {
        uint16_t value;
    
        // get camera gain from device
        int res = ctrl_protocol_get_cam_gain( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a CameraGainChanged signal
        emit CameraGainChanged( (int)value );
    }
}

/******************************************************************************
 * CamItf::GetCameraExposure
 *****************************************************************************/
void CamItf::GetCameraExposure()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(CameraExposureChanged(int))) > 0 )
    {
        uint32_t value;
    
        // get camera exposure time from device
        int res = ctrl_protocol_get_cam_exposure( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

        // emit a CameraExposureChanged signal
        emit CameraExposureChanged( (int)value );
    }
}

/******************************************************************************
 * CamItf::onCameraGainChange
 *****************************************************************************/
void CamItf::onCameraGainChange( int value )
{
    // set camera gain on device
    int res = ctrl_protocol_set_cam_gain( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), (uint16_t)value );
    HANDLE_ERROR( res );

    emit NotifyCameraGainChanged();
}

/******************************************************************************
 * CamItf::onCameraExposureChange
 *****************************************************************************/
void CamItf::onCameraExposureChange( int value )
{
    // set camera exposure time on device
    int res = ctrl_protocol_set_cam_exposure( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), (uint32_t)value );
    HANDLE_ERROR( res );
    
    emit NotifyCameraExposureTimeChanged();
}

/******************************************************************************
 * CamItf::onNotifyVideoModeChange
 *****************************************************************************/
void CamItf::onNotifyVideoModeChange()
{
    GetCameraInfo();        // to get new gain/exposure time ranges
    GetCameraGain();        // to get new gain
    GetCameraExposure();    // to get new exposure
}

