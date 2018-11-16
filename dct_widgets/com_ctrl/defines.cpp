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
 * @file    defines.cpp
 *
 * @brief   
 *
 *****************************************************************************/
#include "defines.h"

/******************************************************************************
 * DeviceIsKnown
 *****************************************************************************/
bool DeviceIsKnown( const QString & deviceName )
{
    for ( int i = KnownDeviceFirst; i < KnownDeviceMax; i++ )
    {
        if ( deviceName.contains(QString(gKnownDevices[i])) )
        {
            return ( true );
        }
    }

    return ( false );
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

        case VideoMode2kp59_94:
            return ( QString(VIDEO_MODE_NAME_2kp59_94) );

        case VideoMode2kp48:
            return ( QString(VIDEO_MODE_NAME_2kp48) );

        case VideoMode2kp47_96:
            return ( QString(VIDEO_MODE_NAME_2kp47_96) );

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
 * GetExposureTime()
 *****************************************************************************/
int GetExposureTime( ExposureTime exposureIndex )
{
    switch ( exposureIndex )
    {
        case ExposureTime1To8000:
            return 125;
        case ExposureTime1To4000:
            return 250;
        case ExposureTime1To2000:
            return 500;
        case ExposureTime1To1000:
            return 1000;
        case ExposureTime1To500:
            return 2000;
        case ExposureTime1To250:
            return 4000;
        case ExposureTime1To125:
            return 8000;
        case ExposureTime1To120:
            return 8333;
        case ExposureTime1To100:
            return 10000;
        case ExposureTime1To60:
            return 16667;
        case ExposureTime1To50:
            return 20000;
        case ExposureTime1To33:
            return 30000;       // Rounded from 30303
        case ExposureTime1To30:
            return 33333;
        case ExposureTime1To25:
            return 40000;
        default:
            return 0;
    }
}

/******************************************************************************
 * GetExposureTimeString()
 *****************************************************************************/
QString GetExposureTimeString( ExposureTime exposureIndex )
{
    switch ( exposureIndex )
    {
        case ExposureTime1To8000:
            return QString("1/%1").arg(8000);
        case ExposureTime1To4000:
            return QString("1/%1").arg(4000);
        case ExposureTime1To2000:
            return QString("1/%1").arg(2000);
        case ExposureTime1To1000:
            return QString("1/%1").arg(1000);
        case ExposureTime1To500:
            return QString("1/%1").arg(500);
        case ExposureTime1To250:
            return QString("1/%1").arg(250);
        case ExposureTime1To125:
            return QString("1/%1").arg(125);
        case ExposureTime1To120:
            return QString("1/%1").arg(120);
        case ExposureTime1To100:
            return QString("1/%1").arg(100);
        case ExposureTime1To60:
            return QString("1/%1").arg(60);
        case ExposureTime1To50:
            return QString("1/%1").arg(50);
        case ExposureTime1To33:
            return QString("1/%1").arg(33);
        case ExposureTime1To30:
            return QString("1/%1").arg(30);
        case ExposureTime1To25:
            return QString("1/%1").arg(25);
        default:
            return QString("");
    }
}

/******************************************************************************
 * GetIsoValue()
 *****************************************************************************/
int GetIsoValue( IsoValue isoIndex )
{
    switch ( isoIndex )
    {
        case IsoValue80:
            return 80;
        case IsoValue100:
            return 100;
        case IsoValue125:
            return 125;
        case IsoValue160:
            return 160;
        case IsoValue200:
            return 200;
        case IsoValue250:
            return 250;
        case IsoValue320:
            return 320;
        case IsoValue400:
            return 400;
        case IsoValue500:
            return 500;
        case IsoValue640:
            return 640;
        case IsoValue800:
            return 800;
        case IsoValue1000:
            return 1000;
        case IsoValue1250:
            return 1250;
        case IsoValue1600:
            return 1600;
        case IsoValue2000:
            return 2000;
        case IsoValue2500:
            return 2500;
        case IsoValue3200:
            return 3200;
        case IsoValue4000:
            return 4000;
        case IsoValue5000:
            return 5000;
        case IsoValue6400:
            return 6400;
        case IsoValue8000:
            return 8000;
        case IsoValue10000:
            return 10000;
        case IsoValue12800:
            return 12800;
        case IsoValue16000:
            return 16000;
        case IsoValue20000:
            return 20000;
        case IsoValue25600:
            return 25600;
        default:
            return 0;
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
 * GetDownscaleModeName()
 *****************************************************************************/
QString GetDownscaleModeName( DownscaleMode mode )
{
    switch ( mode )
    {
        case DownscaleModeOff:
            return ( QString(DOWNSCALE_MODE_OFF) );
        case DownscaleModeOn:
            return ( QString(DOWNSCALE_MODE_ON) );
        case DownscaleModeOnAndInterlace:
            return ( QString(DOWNSCALE_MODE_ON_AND_INTERLACE) );
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
