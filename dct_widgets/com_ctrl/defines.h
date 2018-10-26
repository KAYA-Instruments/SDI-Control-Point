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
 * @file    Defines.h
 *
 * @brief   Definitions for User-Interfaces
 *
 *****************************************************************************/
#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <QString>

/******************************************************************************
 * general defines
 *****************************************************************************/
#define MAX_NUM_CHAINS                  ( 2 )
#define MAX_DEVICE_ID                   ( 99 )
#define SETTINGS_FILE_NAME              ( ".provideo-gui.ini" )

/******************************************************************************
 * lens shading correction segmentation mode
 *****************************************************************************/
#define LSC_SEGM_MODE_LIN_SEG_33        ( "Linear, 33 Nodes" )
#define LSC_SEGM_MODE_LIN_SEG_32        ( "Linear, 32 Nodes" )
#define LSC_SEGM_MODE_3_SEG_32          ( "3 Segments, 32 Nodes" )
#define LSC_SEGM_MODE_5_SEG_32          ( "5 Segments, 32 Nodes" )
#define LSC_SEGM_MODE_LIN_SEG_128       ( "Linear, 128 Nodes" )
#define LSC_SEGM_MODE_3_SEG_128         ( "3 Segments, 128 Nodes" )

/******************************************************************************
 * bayer pattern 
 *****************************************************************************/
#define BAYER_PATTERN_RGGB              ( "RG,GB" )
#define BAYER_PATTERN_GRBG              ( "GR,BG" )
#define BAYER_PATTERN_GBRG              ( "GB,RG" )
#define BAYER_PATTERN_BGGR              ( "BG,GR" )

/******************************************************************************
 * video mode names
 *****************************************************************************/
#define VIDEO_MODE_NAME_720p60          ( "720p60" )
#define VIDEO_MODE_NAME_720p50          ( "720p50" )
#define VIDEO_MODE_NAME_720p59_94       ( "720p59.94" )
#define VIDEO_MODE_NAME_1080p30         ( "1080p30" )
#define VIDEO_MODE_NAME_1080p25         ( "1080p25" )
#define VIDEO_MODE_NAME_1080p24         ( "1080p24" )
#define VIDEO_MODE_NAME_1080p23_98      ( "1080p23.98" )
#define VIDEO_MODE_NAME_1080p29_97      ( "1080p29.97" )
#define VIDEO_MODE_NAME_1080p50         ( "1080p50" )
#define VIDEO_MODE_NAME_1080p60         ( "1080p60" )
#define VIDEO_MODE_NAME_1080i60         ( "1080i60" )
#define VIDEO_MODE_NAME_1080i50         ( "1080i50" )
#define VIDEO_MODE_NAME_1080i59_94      ( "1080i59.94" )
#define VIDEO_MODE_NAME_1080p59_94      ( "1080p59.94" )

#define VIDEO_MODE_NAME_2kp30           ( "2kp30" )
#define VIDEO_MODE_NAME_2kp25           ( "2kp25" )
#define VIDEO_MODE_NAME_2kp24           ( "2kp24" ) 
#define VIDEO_MODE_NAME_2kp23_98        ( "2kp23.98" )
#define VIDEO_MODE_NAME_2kp29_97        ( "2kp29.97" )
#define VIDEO_MODE_NAME_2kp50           ( "2kp50" )
#define VIDEO_MODE_NAME_2kp60           ( "2kp60" )
#define VIDEO_MODE_NAME_2kp59_94        ( "2kp59.94" )
#define VIDEO_MODE_NAME_2kp48           ( "2kp48" )
#define VIDEO_MODE_NAME_2kp47_96        ( "2kp47.96" )

#define VIDEO_MODE_NAME_UHDp30          ( "2160p30" )
#define VIDEO_MODE_NAME_UHDp25          ( "2160p25" )
#define VIDEO_MODE_NAME_UHDp24          ( "2160p24" )
#define VIDEO_MODE_NAME_UHDp23_98       ( "2160p23.98" )
#define VIDEO_MODE_NAME_UHDp29_97       ( "2160p29.97" )
#define VIDEO_MODE_NAME_UHDp50          ( "2160p50" )
#define VIDEO_MODE_NAME_UHDp60          ( "2160p60" )
#define VIDEO_MODE_NAME_UHDp59_94       ( "2160p59.94" )
#define VIDEO_MODE_NAME_UHDp48          ( "2160p48" )
#define VIDEO_MODE_NAME_UHDp47_96       ( "2160p47.96" )

#define VIDEO_MODE_NAME_4Kp30           ( "4Kp30" )
#define VIDEO_MODE_NAME_4Kp25           ( "4Kp25" ) 
#define VIDEO_MODE_NAME_4Kp24           ( "4Kp24" )
#define VIDEO_MODE_NAME_4Kp23_98        ( "4Kp23.98" )
#define VIDEO_MODE_NAME_4Kp29_97        ( "4Kp29.97" )
#define VIDEO_MODE_NAME_4Kp50           ( "4Kp50" )
#define VIDEO_MODE_NAME_4Kp60           ( "4Kp60" )
#define VIDEO_MODE_NAME_4Kp59_94        ( "4Kp59.94" )
#define VIDEO_MODE_NAME_4Kp48           ( "4Kp48" )
#define VIDEO_MODE_NAME_4Kp47_96        ( "4Kp47.96" )

/******************************************************************************
 * sdi mode names
 *****************************************************************************/
#define SDI_MODE_NAME_LEGAL             ( "Legal Range" )
#define SDI_MODE_NAME_EXTENDED          ( "Extended Range" )

/******************************************************************************
 * color space output mode names
 *****************************************************************************/
#define COLOR_SPACE_MODE_NAME_YUV       ( "YUV" )
#define COLOR_SPACE_MODE_NAME_RGB       ( "RGB" )
#define COLOR_SPACE_MODE_NAME_RAW10     ( "RAW10" )
#define COLOR_SPACE_MODE_NAME_RAW12     ( "RAW12" )

/******************************************************************************
 * camera bitwidth mode names
 *****************************************************************************/
#define CAM_DEPTH_NAME_BIT_8            ( "8 Bit" )
#define CAM_DEPTH_NAME_BIT_10           ( "10 Bit" )
#define CAM_DEPTH_NAME_BIT_12           ( "12 Bit" )

/******************************************************************************
 * gen-lock mode names
 *****************************************************************************/
#define GENLOCK_MODE_DISABLE            ( "Disable" )
#define GENLOCK_MODE_SLAVE              ( "Slave" )
#define GENLOCK_MODE_MASTER             ( "Master" )

/******************************************************************************
 * SDI-2 mode names
 *****************************************************************************/
#define SDI2_MODE_SDI1                  ( "SDI-1 (mirror)" )
#define SDI2_MODE_LOGC                  ( "SDI-2 LUT" )
#define SDI2_MODE_RAW10                 ( "RAW-10" )
#define SDI2_MODE_RAW12                 ( "RAW-12" )

/******************************************************************************
 * Downscale mode names
 *****************************************************************************/
#define DOWNSCALE_MODE_OFF              ( "Disabled" )
#define DOWNSCALE_MODE_ON               ( "Downscale" )
#define DOWNSCALE_MODE_ON_AND_INTERLACE ( "Downscale and Interlace" )

/******************************************************************************
 * Flip mode names
 *****************************************************************************/
#define FLIP_MODE_OFF                   ( "off" )
#define FLIP_MODE_VERTICAL              ( "vertical" )
#define FLIP_MODE_HORIZONTAL            ( "horizontal" )
#define FLIP_MODE_ROTATED               ( "rotated" )

/******************************************************************************
 * known devices
 *****************************************************************************/
#define KNOWN_DEVICE_XBOW               ( "xbow" )
#define KNOWN_DEVICE_CONDOR4K           ( "condor4k" )
#define KNOWN_DEVICE_CONDOR4K_MINI      ( "condor4k_mini" )
#define KNOWN_DEVICE_COOPER             ( "cooper" )

enum KnownDevice
{
    KnownDeviceFirst = 0,
    KnownDeviceXbow,
    KnownDeviceCondor4k,
    KnownDeviceCondor4kMini,
    KnownDeviceCooper,
    KnownDeviceMax,
};

static QString gKnownDevices[KnownDeviceMax] =
{
    KNOWN_DEVICE_XBOW,
    KNOWN_DEVICE_CONDOR4K,
    KNOWN_DEVICE_CONDOR4K_MINI,
    KNOWN_DEVICE_COOPER
};

/******************************************************************************
 * bayer pattern 
 *****************************************************************************/
enum BayerPattern 
{
    BayerPatternFirst     = 0,
    BayerPatternRGGB      = 0,
    BayerPatternGRBG      = 1,
    BayerPatternGBRG      = 2,
    BayerPatternBGGR      = 3,
    BayerPatternMax,
};

/******************************************************************************
 * video modes
 *****************************************************************************/
enum VideoMode
{
    VideoModeFirst        = 1,
    VideoMode720p60       = 1,  /**<  720p, 60 fps      */
    VideoMode720p50       = 2,  /**<  720p, 50 fps      */
    VideoMode720p59_94    = 3,  /**<  720p, 59.94 fps   */
    VideoMode1080p30      = 4,  /**< 1080p, 30 fps      */
    VideoMode1080p25      = 5,  /**< 1080p, 25 fps      */
    VideoMode1080p24      = 6,  /**< 1080p, 24 fps      */
    VideoMode1080p23_98   = 7,  /**< 1080p, 23.98 fps   */
    VideoMode1080p29_97   = 8,  /**< 1080p, 29.97 fps   */
    VideoMode1080p50      = 9,  /**< 1080p, 50 fps      */
    VideoMode1080p60      = 10, /**< 1080p, 60 fps      */
    VideoMode1080i60      = 11, /**< 1080i, 60 fps      */
    VideoMode1080i50      = 12, /**< 1080i, 50 fps      */
    VideoMode1080i59_94   = 13, /**< 1080i, 59.94 fps   */
    VideoMode1080p59_94   = 14, /**< 1080p, 59.94 fps   */
    VideoMode2kp30        = 15, /**< 1080p2k, 30 fps    */
    VideoMode2kp25        = 16, /**< 1080p2k, 25 fps    */
    VideoMode2kp24        = 17, /**< 1080p2k, 24 fps    */
    VideoMode2kp23_98     = 18, /**< 1080p2k, 23.98 fps */
    VideoMode2kp29_97     = 19, /**< 1080p2k, 29.97 fps */
    VideoMode2kp50        = 20, /**< 1080p2k, 50 fps    */
    VideoMode2kp60        = 21, /**< 1080p2k, 60 fps    */
    VideoMode2kp59_94     = 22, /**< 1080p2k, 59.94 fps */
    VideoMode2kp48        = 23, /**< 1080p2k, 50 fps    */
    VideoMode2kp47_96     = 24, /**< 1080p2k, 60 fps    */
    VideoModeUHDp30       = 25, /**< UHD, 30 fps        */
    VideoModeUHDp25       = 26, /**< UHD, 25 fps        */
    VideoModeUHDp24       = 27, /**< UHD, 24 fps        */
    VideoModeUHDp23_98    = 28, /**< UHD, 23.98 fps     */
    VideoModeUHDp29_97    = 29, /**< UHD, 29.97 fps     */
    VideoModeUHDp50       = 30, /**< UHD, 50 fps        */
    VideoModeUHDp60       = 31, /**< UHD, 60 fps        */
    VideoModeUHDp59_94    = 32, /**< UHD, 59.94 fps     */
    VideoModeUHDp48       = 33, /**< UHD, 34 fps        */
    VideoModeUHDp47_96    = 34, /**< UHD, 47.96 fps     */
    VideoMode4Kp30        = 35, /**< 4K, 30 fps         */
    VideoMode4Kp25        = 36, /**< 4K, 25 fps         */
    VideoMode4Kp24        = 37, /**< 4K, 24 fps         */
    VideoMode4Kp23_98     = 38, /**< 4K, 23.98 fps      */
    VideoMode4Kp29_97     = 39, /**< 4K, 29.97 fps      */
    VideoMode4Kp50        = 40, /**< 4K, 50 fps         */
    VideoMode4Kp60        = 41, /**< 4K, 60 fps         */
    VideoMode4Kp59_94     = 42, /**< 4K, 59.94 fps      */
    VideoMode4Kp48        = 43, /**< 4K, 34 fps         */
    VideoMode4Kp47_96     = 44, /**< 4K, 47.96 fps      */
    VideoModeMax,               /**< number of normally supported modes (end marker) */

    VideoModeFirstHD      = (VideoMode720p60),
    VideoModeLastHD       = (VideoMode720p59_94),
    VideoModeNumHD        = (VideoMode720p59_94),                       /**< number of supported HD video modes */
    VideoModeFirstFullHD  = (VideoMode1080p30),
    VideoModeLastFullHD   = (VideoMode1080p59_94),
    VideoModeNumFullHD    = (VideoMode1080p59_94 - VideoModeNumHD),     /**< number of supported Full HD video modes */
    VideoModeFirst2K      = (VideoMode2kp30),
    VideoModeLast2K       = (VideoMode2kp47_96),
    VideoModeNum2K        = (VideoMode2kp47_96   - VideoModeNumFullHD), /**< number of supported 2K video modes */
    VideoModeFirstUHD     = (VideoModeUHDp30),
    VideoModeLastUHD      = (VideoModeUHDp47_96),
    VideoModeNumUHD       = (VideoModeUHDp47_96  - VideoModeUHDp30),     /**< number of supported UHD video modes */
    VideoModeFirst4K      = (VideoMode4Kp30),
    VideoModeLast4K       = (VideoMode4Kp47_96),
    VideoModeNum4K        = (VideoMode4Kp47_96   - VideoModeNum2K),     /**< number of supported 4K video modes */
};

/******************************************************************************
 * Fixed exposure times
 *****************************************************************************/
enum ExposureTime
{
    ExposureTimeFirst   = 0,
    ExposureTime1To8000 = 0,    /**< Exposure 1/8000 = 125us */
    ExposureTime1To4000 = 1,    /**< Exposure 1/4000 = 250us */
    ExposureTime1To2000 = 2,    /**< Exposure 1/2000 = 500us */
    ExposureTime1To1000 = 3,    /**< Exposure 1/1000 = 1000us */
    ExposureTime1To500  = 4,    /**< Exposure 1/500  = 2000us */
    ExposureTime1To250  = 5,    /**< Exposure 1/250  = 4000us */
    ExposureTime1To125  = 6,    /**< Exposure 1/125  = 8000us */
    ExposureTime1To120  = 7,    /**< Exposure 1/120  = 8333us */
    ExposureTime1To100  = 8,    /**< Exposure 1/100  = 10000us */
    ExposureTime1To60   = 9,    /**< Exposure 1/60   = 16667us */
    ExposureTime1To50   = 10,   /**< Exposure 1/50   = 20000us */
    ExposureTime1To33   = 11,   /**< Exposure 1/33   = 30303us = ~30000us */
    ExposureTime1To30   = 12,   /**< Exposure 1/30   = 33333us */
    ExposureTime1To25   = 13,   /**< Exposure 1/25   = 40000us */
    ExposureTimeMax             /**< number of normally supported exposure times (end marker) */
};

/******************************************************************************
 * Fixed ISO values
 *****************************************************************************/
enum IsoValue
{
    IsoValueFirst = 0,
    IsoValue80    = 0,          /**< ISO 80 */
    IsoValue100   = 1,          /**< ISO 100 */
    IsoValue125   = 2,          /**< ISO 125 */
    IsoValue160   = 3,          /**< ISO 160 */
    IsoValue200   = 4,          /**< ISO 200 */
    IsoValue250   = 5,          /**< ISO 250 */
    IsoValue320   = 6,          /**< ISO 320 */
    IsoValue400   = 7,          /**< ISO 400 */
    IsoValue500   = 8,          /**< ISO 500 */
    IsoValue640   = 9,          /**< ISO 640 */
    IsoValue800   = 10,         /**< ISO 800 */
    IsoValue1000  = 11,         /**< ISO 1000 */
    IsoValue1250  = 12,         /**< ISO 1250 */
    IsoValue1600  = 13,         /**< ISO 1600 */
    IsoValue2000  = 14,         /**< ISO 2000 */
    IsoValue2500  = 15,         /**< ISO 2500 */
    IsoValue3200  = 16,         /**< ISO 3200 */
    IsoValue4000  = 17,         /**< ISO 4000 */
    IsoValue5000  = 18,         /**< ISO 5000 */
    IsoValue6400  = 19,         /**< ISO 6400 */
    IsoValue8000  = 20,         /**< ISO 8000 */
    IsoValue10000 = 21,         /**< ISO 10000 */
    IsoValue12800 = 22,         /**< ISO 12800 */
    IsoValue16000 = 23,         /**< ISO 16000 */
    IsoValue20000 = 24,         /**< ISO 20000 */
    IsoValue25600 = 25,         /**< ISO 25600 */
    IsoValueMax                 /**< number of normally supported ISO values (end marker) */
};

/******************************************************************************
 * sdi modes
 *****************************************************************************/
enum SdiMode
{
    SdiModeFirst          = 0,
    SdiModeLegal          = 0,  /**< legal mode ranges from 64..940(Y) or 960(Cx) */
    SdiModeExtended       = 1,  /**< extended mode ranges from 4..1019 */
    SdiModeMax                  /**< number of normally supported modes (end marker) */
};

/******************************************************************************
 * color space mode
 *****************************************************************************/
enum ColorSpaceMode
{
    ColorSpaceModeFirst   = 0,
    ColorSpaceModeYUV     = 0,  /**< YUV output */
    ColorSpaceModeRAW10   = 1,  /**< RAW10 output */
    ColorSpaceModeRAW12   = 2,  /**< RAW12 output */
    ColorSpaceModeMax           /**< number of normally supported modes (end marker) */
};

/******************************************************************************
 * camera bitwidth mode
 *****************************************************************************/
enum CamDepth 
{
    CamDepth8b  = 8, 
    CamDepth10b = 10,
    CamDepth12b = 12
};

/******************************************************************************
 * camera genlock mode 
 *****************************************************************************/
enum GenLockMode
{
    GenLockModeFirst    = 0,
    GenLockModeDisabled = 0,
    GenLockModeSlave    = 1,
    GenLockModeMaster   = 2,
    GenLockModeMax
};

/******************************************************************************
 * sdi2 mode 
 *****************************************************************************/
enum Sdi2Mode
{
    Sdi2ModeFirst = 0,
    Sdi2ModeSdi1  = 0,  /**< SDI-2 mirrors SDI-1 */
    Sdi2ModeLogC  = 1,  /**< SDI-2 Log-C mode (means own LUT + YUV conversion)  */
    Sdi2ModeRAW12 = 2,  /**< RAW-12 */
    Sdi2ModeRAW10 = 3,  /**< RAW-10 */
    Sdi2ModeMax
};

/******************************************************************************
 * downscale mode
 *****************************************************************************/
enum DownscaleMode
{
    DownscaleModeFirst          = 0,
    DownscaleModeOff            = 0,  /**< Do not downscale (original resoltion) */
    DownscaleModeOn             = 1,  /**< Downscale 4k to 2k content, if possible  */
    DownscaleModeOnAndInterlace = 2,  /**< Downscale 4k to 2k content and interlace, if possible */
    DownscaleModeMax
};

/******************************************************************************
 * flip mode 
 *****************************************************************************/
enum FlipMode
{
    FlipModeFirst       = 0,
    FlipModeOff         = 0,    /**< off (readout starts at upper left corner) */
    FlipModeVertical    = 1,    /**< veritcal flipped (readout starts at lower left corner) */
    FlipModeHorizontal  = 2,    /**< horizontel flipped (readout starts at upper right corner) */
    FlipModeRotated     = 3,    /**< rotated by 180° (readout starts at lower right corner) */
    FlipModeMax
};

// return if the name belongs to a known device
bool DeviceIsKnown(const QString & deviceName);

// return corresponding bayer-pattern name
QString GetBayerPatternName( BayerPattern pattern );

// return corresponding video-mode name 
QString GetVideoModeName( VideoMode mode );

// return corresponding playout speed in [ms]
int GetVideoModeSpeed( VideoMode mode );

// return corresponding resolution
void GetVideoModeResolution(const VideoMode mode, int & xRes, int & yRes );

// return corresponding exposure time
int GetExposureTime( ExposureTime exposureIndex );

// return corresponding exposure time text string
QString GetExposureTimeString( ExposureTime exposureIndex );

// return corresponding ISO value
int GetIsoValue( IsoValue isoIndex );
    
// return corresponding sdi-mode name 
QString GetSdiModeName( SdiMode mode );
    
// return corresponding sdi-mode name 
QString GetColorSpaceModeName( ColorSpaceMode mode );

// return corresponding camera depth name 
QString GetCamDepthName( CamDepth depth );

// return corresponding genlock mode name 
QString GetGenlockModeName( GenLockMode mode );

// return corresponding sdi2 mode name 
QString GetSdi2ModeName( Sdi2Mode mode );

// return corresponding downscale mode name
QString GetDownscaleModeName( DownscaleMode mode );

// return corresponding flipe mode name 
QString GetFlipModeName( FlipMode mode );

#endif // _DEFINES_H_

