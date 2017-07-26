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
 * @file    wbbox.h
 *
 * @brief   Class definition of a white balance box 
 *
 *****************************************************************************/
#ifndef __WB_BOX_H__
#define __WB_BOX_H__

#include <dct_widgets_base.h>

/******************************************************************************
 * Red Gain Definitions
 *****************************************************************************/
#define WB_RED_GAIN_MIN                         (        0 )
#define WB_RED_GAIN_MAX                         (     1023 )
#define WB_RED_GAIN_COMMA_POSITION              (        8 )
#define WB_RED_GAIN_BASE                        (        2 )
#define WB_RED_GAIN_DISPLAY_MULTIPLIER          (        1 )
#define WB_RED_GAIN_DISPLAY_MASK                (  "%1.3f" )

/******************************************************************************
 * Green Gain Definitions
 *****************************************************************************/
#define WB_GREEN_GAIN_MIN                       (        0 )
#define WB_GREEN_GAIN_MAX                       (     1023 )
#define WB_GREEN_GAIN_COMMA_POSITION            (        8 )
#define WB_GREEN_GAIN_BASE                      (        2 )
#define WB_GREEN_GAIN_DISPLAY_MULTIPLIER        (        1 )
#define WB_GREEN_GAIN_DISPLAY_MASK              (  "%1.3f" )

/******************************************************************************
 * Blue Gain Definitions
 *****************************************************************************/
#define WB_BLUE_GAIN_MIN                        (        0 )
#define WB_BLUE_GAIN_MAX                        (     1023 )
#define WB_BLUE_GAIN_COMMA_POSITION             (        8 )
#define WB_BLUE_GAIN_BASE                       (        2 )
#define WB_BLUE_GAIN_DISPLAY_MULTIPLIER         (        1 )
#define WB_BLUE_GAIN_DISPLAY_MASK               (  "%1.3f" )

/******************************************************************************
 * Color Temperature Definitions
 *****************************************************************************/
#define WB_COLOR_TEMPERATURE_MIN                (     2000 )
#define WB_COLOR_TEMPERATURE_MAX                (    10000 )
#define WB_COLOR_TEMPERATURE_COMMA_POSITION     (        0 )
#define WB_COLOR_TEMPERATURE_BASE               (       10 )
#define WB_COLOR_TEMPERATURE_DISPLAY_MULTIPLIER (        1 )
#define WB_COLOR_TEMPERATURE_DISPLAY_MASK       (  "%5.0f" )

/******************************************************************************
 * Hue Definitions
 *****************************************************************************/
#define WB_HUE_MIN                              (     -128 )
#define WB_HUE_MAX                              (      127 )
#define WB_HUE_COMMA_POSITION                   (        7 )
#define WB_HUE_BASE                             (        2 )
#define WB_HUE_DISPLAY_MULTIPLIER               (       90 )
#define WB_HUE_DISPLAY_MASK                     (  "%2.2f" )

/******************************************************************************
 * Saturation Definitions
 *****************************************************************************/
#define WB_SATURATION_MIN                       (        0 )
#define WB_SATURATION_MAX                       (      255 )
#define WB_SATURATION_COMMA_POSITION            (        7 )
#define WB_SATURATION_BASE                      (        2 )
#define WB_SATURATION_DISPLAY_MULTIPLIER        (        1 )
#define WB_SATURATION_DISPLAY_MASK              (  "%1.3f" )

/******************************************************************************
 * Brightness Definitions
 *****************************************************************************/
#define WB_BRIGHTNESS_MIN                       (     -128 )
#define WB_BRIGHTNESS_MAX                       (      127 )
#define WB_BRIGHTNESS_COMMA_POSITION            (        0 )
#define WB_BRIGHTNESS_BASE                      (        2 )
#define WB_BRIGHTNESS_DISPLAY_MULTIPLIER        (        1 )
#define WB_BRIGHTNESS_DISPLAY_MASK              (  "%1.2f" )

/******************************************************************************
 * Contrast Definitions
 *****************************************************************************/
#define WB_CONTRAST_MIN                         (        0 )
#define WB_CONTRAST_MAX                         (      255 )
#define WB_CONTRAST_COMMA_POSITION              (        7 )
#define WB_CONTRAST_BASE                        (        2 )
#define WB_CONTRAST_DISPLAY_MULTIPLIER          (        1 )
#define WB_CONTRAST_DISPLAY_MASK                (  "%1.3f" )

/******************************************************************************
 * White Balance Box Widget
 *****************************************************************************/
class WbBox : public DctWidgetBox
{
    Q_OBJECT

public:
    explicit WbBox( QWidget * parent = 0 );
    ~WbBox();

    bool AwbEnable() const;
    void setAwbEnable( const bool enable );
    int AwbSpeed() const;
    void setAwbSpeed( const int speed );

    int RedGain() const;
    void setRedGain( const int gain );
    int GreenGain() const;
    void setGreenGain( const int gain );
    int BlueGain() const;
    void setBlueGain( const int gain );

//    int ColorTemperature() const;

    int Hue() const;
    void setHue( const int hue );
    int Saturation() const;
    void setSaturation( const int saturation );
    int Brightness() const;
    void setBrightness( const int brightness );
    int Contrast() const;
    void setContrast( const int contrast );

    // Show or hide UI elements
    void setWhiteBalanceSettingsVisible( const bool awbVisible, const bool gainVisible, const bool greenGainVisible );

protected:
    void prepareMode( const Mode ) Q_DECL_OVERRIDE;

    void loadSettings( QSettings & ) Q_DECL_OVERRIDE;
    void saveSettings( QSettings & ) Q_DECL_OVERRIDE;
    void applySettings( void ) Q_DECL_OVERRIDE;

signals:
    void WbUpdate();
    void WbChanged();
    void WbPresetChanged( int value );

    void AwbEnableChanged( int value );
    void AwbSpeedChanged( int value );

    void StatisticChanged();

    void RedGainChanged( int value );
    void GreenGainChanged( int value );
    void BlueGainChanged( int value );
    void ColorTemperatureChanged( int value );

    void HueChanged( int value );
    void SaturationChanged( int value );
    void BrightnessChanged( int value );
    void ContrastChanged( int value );

public slots:
    void onNoWbPresetsChange( int value );
    void onWbPresetsChange( int id, QString name, int ct );

    void onAwbEnableChange( int value );
    void onAwbSpeedChange( int value );

    void onRedGainChange( int value );
    void onGreenGainChange( int value );
    void onBlueGainChange( int value );
//    void onColorTemperatureChange( int value );

    void onHueChange( int value );
    void onSaturationChange( int value );
    void onBrightnessChange( int value );
    void onContrastChange( int value );
   
    void onStatRGBChange( int red, int green, int blue );
    void onStatXYZChange( int x, int y, int z );
    void onColorXYZChange( int c0, int c1, int c2,
                           int c3, int c4, int c5,
                           int c6, int c7, int c8 );

protected:
    void enterEvent(QEvent * ) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent * ) Q_DECL_OVERRIDE;

private slots:
    void onWbButtonClick( int value );
    void onWbUpdate();
    void onAwbEnableClick( int value );
    void onAwbSpeedSliderChange( int value );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __WB_BOX_H__

