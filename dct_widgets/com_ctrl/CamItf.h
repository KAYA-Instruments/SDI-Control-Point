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
    void resyncAEC();

    // cam setting 
    void GetCameraInfo();
    void GetCameraGain();
    void GetCameraExposure();
    void GetCameraRoiOffsetInfo();
    void GetCameraRoiOffset();

signals:
    // cam configuration
    void CameraInfoChanged( int, int, int, int, int );
    void CameraGainChanged( int value );
    void CameraExposureChanged( int value );
    void CameraRoiOffsetInfoChanged(int, int, int, int);
    void CameraRoiOffsetChanged(int, int);

    // new camera analogue gain set
    // This is used to synchronize other interface components.
    void NotifyCameraGainChanged();

public slots:
    // cam configuration
    void onCameraGainChange( int );
    void onCameraExposureChange( int );

    void onAnalogGainResyncRequest();
    void onMaxExposureResyncRequest();

    // notifier slot for video-mode change
    void onNotifyVideoModeChange();

    // notifier slot for LOG mode change
    void onNotifyLogModeChange();

    // notifier slot for Roi Offset change
    void onCameraRoiOffsetChange( int offset_x, int offset_y );

    // notifier slot for Downscaler change
    void onDownscalerChange();
};

#endif // _CAM_INTERFACE_H_
