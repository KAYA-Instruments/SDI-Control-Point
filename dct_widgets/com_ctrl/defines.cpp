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
 * @file    defines.cpp
 *
 * @brief   
 *
 *****************************************************************************/
#include "defines.h"

/******************************************************************************
 * GetKnownDeviceName()
 *****************************************************************************/
QString GetKnownDeviceName( KnownDevice device )
{
    switch ( device )
    {
        case KnownDeviceXbow:
            return ( QString(KNOWN_DEVICE_XBOW) );

        case KnownDeviceCondor4k:
            return ( QString(KNOWN_DEVICE_CONDOR4K) );

        case KnownDeviceCondor4kMini:
            return ( QString(KNOWN_DEVICE_CONDOR4K_MINI) );

        case KnownDeviceCondorZeiss:
            return ( QString(KNOWN_DEVICE_CONDOR_ZEISS) );

        case KnownDeviceAdam:
            return ( QString(KNOWN_DEVICE_ADAM) );

        case KnownDeviceAdamImx:
            return ( QString(KNOWN_DEVICE_ADAM_IMX) );

        case KnownDeviceCameleon:
            return ( QString(KNOWN_DEVICE_CAMELEON) );

        default:
            return ( QString::null );
    }
}

/******************************************************************************
 * DeviceIsKnown
 *****************************************************************************/
bool DeviceIsKnown(const QString & deviceName)
{
    for (int i = KnownDeviceFirst; i < KnownDeviceMax; i++)
    {
        if (deviceName == GetKnownDeviceName((KnownDevice)i))
        {
            return ( true );
        }
    }

    return ( false );
}

/******************************************************************************
 * GetLscSegmentationModeName()
 *****************************************************************************/
QString GetLscSegmentationModeName( LscSegmentationMode mode )
{
    switch ( mode )
    {
        case LscSegmentationLinSeg33:
            return ( QString(LSC_SEGM_MODE_LIN_SEG_33) );

        case LscSegmentationLinSeg32:
            return ( QString(LSC_SEGM_MODE_LIN_SEG_32) );

        case LscSegmentation3Seg32:
            return ( QString(LSC_SEGM_MODE_3_SEG_32) );

        case LscSegmentation5Seg32:
            return ( QString(LSC_SEGM_MODE_5_SEG_32) );

        case LscSegmentationLinSeg128:
            return ( QString(LSC_SEGM_MODE_LIN_SEG_128) );

        case LscSegmentation3Seg128:
            return ( QString(LSC_SEGM_MODE_3_SEG_128) );

        default:
            return ( QString::null );
    }
}

/******************************************************************************
 * GetBayerPatternName()
 *****************************************************************************/
QString GetBayerPatternName( BayerPattern pattern )
{
    switch ( pattern )
    {
        case BayerPatternRGGB:
            return ( QString(BAYER_PATTERN_RGGB) );

        case BayerPatternGRBG:
            return ( QString(BAYER_PATTERN_GRBG) );

        case BayerPatternGBRG:
            return ( QString(BAYER_PATTERN_GBRG) );

        case BayerPatternBGGR:
            return ( QString(BAYER_PATTERN_BGGR) );

        default:
            return ( QString::null );
    }
}

/******************************************************************************
 * GetVideoModeName()
 *****************************************************************************/
QString GetVideoModeName( VideoMode mode )
{
    switch ( mode )
    {
        case VideoMode720p60:
            return ( QString(VIDEO_MODE_NAME_720p60) );

        case VideoMode720p50:
            return ( QString(VIDEO_MODE_NAME_720p50) );

        case VideoMode720p59_94:
            return ( QString(VIDEO_MODE_NAME_720p59_94) );

        case VideoMode1080p30:
            return ( QString(VIDEO_MODE_NAME_1080p30) );

        case VideoMode1080p25:
            return ( QString(VIDEO_MODE_NAME_1080p25) );

        case VideoMode1080p24:
            return ( QString(VIDEO_MODE_NAME_1080p24) );

        case VideoMode1080p23_98:
            return ( QString(VIDEO_MODE_NAME_1080p23_98) );

        case VideoMode1080p29_97:
            return ( QString(VIDEO_MODE_NAME_1080p29_97) );

        case VideoMode1080p50:
            return ( QString(VIDEO_MODE_NAME_1080p50) );

        case VideoMode1080p60:
            return ( QString(VIDEO_MODE_NAME_1080p60) );

        case VideoMode1080i60:
            return ( QString(VIDEO_MODE_NAME_1080i60) );

        case VideoMode1080i50:
            return ( QString(VIDEO_MODE_NAME_1080i50) );

        case VideoMode1080i59_94:
            return ( QString(VIDEO_MODE_NAME_1080i59_94) );

        case VideoMode1080p59_94:
            return ( QString(VIDEO_MODE_NAME_1080p59_94) );
        
        case VideoMode2kp30:
            return ( QString(VIDEO_MODE_NAME_2kp30) );
        case VideoMode2kp25:
            return ( QString(VIDEO_MODE_NAME_2kp25) );

        case VideoMode2kp24:
            return ( QString(VIDEO_MODE_NAME_2kp24) );

        case VideoMode2kp23_98:
            return ( QString(VIDEO_MODE_NAME_2kp23_98) );

        case VideoMode2kp29_97:
            return ( QString(VIDEO_MODE_NAME_2kp29_97) );

        case VideoMode2kp50:
            return ( QString(VIDEO_MODE_NAME_2kp50) );

        case VideoMode2kp60:
            return ( QString(VIDEO_MODE_NAME_2kp60) );

        case VideoMode2ki50:
            return ( QString(VIDEO_MODE_NAME_2ki50) );

        case VideoMode2ki60:
            return ( QString(VIDEO_MODE_NAME_2ki60) );

        case VideoModeUHDp30:
            return ( QString(VIDEO_MODE_NAME_UHDp30) );

        case VideoModeUHDp25:
            return ( QString(VIDEO_MODE_NAME_UHDp25) );

        case VideoModeUHDp24:
            return ( QString(VIDEO_MODE_NAME_UHDp24) );

        case VideoModeUHDp23_98:
            return ( QString(VIDEO_MODE_NAME_UHDp23_98) );

        case VideoModeUHDp29_97:
            return ( QString(VIDEO_MODE_NAME_UHDp29_97) );

        case VideoModeUHDp50:
            return ( QString(VIDEO_MODE_NAME_UHDp50) );

        case VideoModeUHDp60:
            return ( QString(VIDEO_MODE_NAME_UHDp60) );

        case VideoModeUHDp59_94:
            return ( QString(VIDEO_MODE_NAME_UHDp59_94) );

        case VideoModeUHDp48:
            return ( QString(VIDEO_MODE_NAME_UHDp48) );

        case VideoModeUHDp47_96:
            return ( QString(VIDEO_MODE_NAME_UHDp47_96) );

        case VideoMode4Kp30:
            return ( QString(VIDEO_MODE_NAME_4Kp30) );

        case VideoMode4Kp25:
            return ( QString(VIDEO_MODE_NAME_4Kp25) );

        case VideoMode4Kp24:
            return ( QString(VIDEO_MODE_NAME_4Kp24) );

        case VideoMode4Kp23_98:
            return ( QString(VIDEO_MODE_NAME_4Kp23_98) );

        case VideoMode4Kp29_97:
            return ( QString(VIDEO_MODE_NAME_4Kp29_97) );

        case VideoMode4Kp50:
            return ( QString(VIDEO_MODE_NAME_4Kp50) );

        case VideoMode4Kp60:
            return ( QString(VIDEO_MODE_NAME_4Kp60) );

        case VideoMode4Kp59_94:
            return ( QString(VIDEO_MODE_NAME_4Kp59_94) );

        case VideoMode4Kp48:
            return ( QString(VIDEO_MODE_NAME_4Kp48) );

        case VideoMode4Kp47_96:
            return ( QString(VIDEO_MODE_NAME_4Kp47_96) );

        default:
            return ( QString::null );
    }
}

/******************************************************************************
 * GetVideoModeSpeed()
 *****************************************************************************/
int GetVideoModeSpeed( VideoMode mode )
{
    switch ( mode )
    {
        case VideoMode720p60:
            return ( 60000 );

        case VideoMode720p50:
            return ( 50000 );

        case VideoMode720p59_94:
            return ( 59940 );

        case VideoMode1080p30:
            return ( 30000 );

        case VideoMode1080p25:
            return ( 25000 );

        case VideoMode1080p24:
            return ( 24000 );

        case VideoMode1080p23_98:
            return ( 23980 );

        case VideoMode1080p29_97:
            return ( 29970 );

        case VideoMode1080p50:
            return ( 50000 );

        case VideoMode1080p60:
            return ( 60000 );

        case VideoMode1080i60:
            return ( 60000 );

        case VideoMode1080i50:
            return ( 50000 );

        case VideoMode1080i59_94:
            return ( 59940 );

        default:
            return ( 0 );
    }
}

/******************************************************************************
 * GetVideoModeResolution()
 *****************************************************************************/
void GetVideoModeResolution(const VideoMode mode, int & xRes, int & yRes )
{
    if (mode >= VideoModeFirstHD && mode <= VideoModeLastHD)
    {
        xRes = 1280;
        yRes = 720;
    }
    else if (mode >= VideoModeFirstFullHD && mode <= VideoModeLastFullHD)
    {
        xRes = 1920;
        yRes = 1080;
    }
    else if (mode >= VideoModeFirst2K && mode <= VideoModeLast2K)
    {
        xRes = 2048;
        yRes = 1080;
    }
    else if (mode >= VideoModeFirstUHD && mode <= VideoModeLastUHD)
    {
        xRes = 3840;
        yRes = 2160;
    }
    else if (mode >= VideoModeFirst4K && mode <= VideoModeLast4K)
    {
        xRes = 4096;
        yRes = 2160;
    }
    else    // Default: Full HD
    {
        xRes = 1920;
        yRes = 1080;
    }
}

/******************************************************************************
 * GetSdiModeName()
 *****************************************************************************/
QString GetSdiModeName( SdiMode mode )
{
    switch ( mode )
    {
        case SdiModeLegal:
            return ( QString(SDI_MODE_NAME_LEGAL) );

        case SdiModeExtended:
            return ( QString(SDI_MODE_NAME_EXTENDED) );

        default:
            return ( QString::null );
    }
}

/******************************************************************************
 * GetCsModeName()
 *****************************************************************************/
QString GetColorSpaceModeName( ColorSpaceMode mode )
{
    switch ( mode )
    {
        case ColorSpaceModeYUV:
            return ( QString(COLOR_SPACE_MODE_NAME_YUV) );

        case ColorSpaceModeRAW10:
            return ( QString(COLOR_SPACE_MODE_NAME_RAW10) );

        case ColorSpaceModeRAW12:
            return ( QString(COLOR_SPACE_MODE_NAME_RAW12) );

        default:
            return ( QString::null );
    }
}

/******************************************************************************
 * GetCamDepthName()
 *****************************************************************************/
QString GetCamDepthName( CamDepth depth )
{
    switch ( depth )
    {
        case CamDepth8b:
            return ( QString(CAM_DEPTH_NAME_BIT_8) );
        case CamDepth10b:
            return ( QString(CAM_DEPTH_NAME_BIT_10) );
        case CamDepth12b:
            return ( QString(CAM_DEPTH_NAME_BIT_12) );
        default:
            return ( QString::null );
    }
}

/******************************************************************************
 * GetGenlockModeName()
 *****************************************************************************/
QString GetGenlockModeName( GenLockMode mode )
{
    switch ( mode )
    {
        case GenLockModeDisabled:
            return ( QString(GENLOCK_MODE_DISABLE) );
        case GenLockModeSlave:
            return ( QString(GENLOCK_MODE_SLAVE) );
        case GenLockModeMaster:
            return ( QString(GENLOCK_MODE_MASTER) );
        default:
            return ( QString::null );
    }
}

/******************************************************************************
 * GetSdi2ModeName()
 *****************************************************************************/
QString GetSdi2ModeName( Sdi2Mode mode )
{
    switch ( mode )
    {
        case Sdi2ModeSdi1:
            return ( QString(SDI2_MODE_SDI1) );
        case Sdi2ModeLogC:
            return ( QString(SDI2_MODE_LOGC) );
        case Sdi2ModeRAW10:
            return ( QString(SDI2_MODE_RAW10) );
        case Sdi2ModeRAW12:
            return ( QString(SDI2_MODE_RAW12) );
        default:
            return ( QString::null );
    }
}

/******************************************************************************
 * GetFlipModeName()
 *****************************************************************************/
QString GetFlipModeName( FlipMode mode )
{
    switch ( mode )
    {
        case FlipModeOff:
            return ( QString(FLIP_MODE_OFF) );
        case FlipModeVertical:
            return ( QString(FLIP_MODE_VERTICAL) );
        case FlipModeHorizontal:
            return ( QString(FLIP_MODE_HORIZONTAL) );
        case FlipModeRotated:
            return ( QString(FLIP_MODE_ROTATED) );
        default:
            return ( QString::null );
    }
}

