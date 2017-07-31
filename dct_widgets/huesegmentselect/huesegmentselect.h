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
 * @file    huesegmentselect.h
 *
 * @brief   Class definition of customized color segment/phase selection
 *
 *****************************************************************************/
#ifndef __HUE_SEGMENT_SELECT_H__
#define __HUE_SEGMENT_SELECT_H__

#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QTransform>
#include <QWidget>

#define DEFAULT_NO_SEGMENTS     ( 24 )

class HueSegmentSelect : public QWidget
{
    Q_OBJECT

    // background image
    // hue range runs from Red (0°) to Red- (360°) 
    // For a smoothly movin create a background image with threefold size by copying the image 3 times
    // (see "ressource/images/hue/hue_grey.png" the original images is 2400 pixel in width)
    // +--------------------+--------------------+--------------------+
    // |       image        |       image        |       image        |
    // +--------------------+--------------------+--------------------+
    //                      ^-- red offset
    Q_PROPERTY( QPixmap backgroundImage READ backgroundImage WRITE setBackgroundImage DESIGNABLE true )
   
    // offset of red inside the background image
    Q_PROPERTY( int offsetPixel READ offsetPixel WRITE setOffsetPixel DESIGNABLE true )

    // number of hue segements ( mcc hw-module supports 12, 16, 24, 32 )
    Q_PROPERTY( int noSegments READ noSegments WRITE setNoSegments DESIGNABLE true )
 
    // current segment index to view
    Q_PROPERTY( int segmentIndex READ segmentIndex WRITE setSegmentIndex DESIGNABLE true )

    // min hue offset 
    // in mcc the offset is a 16 bit signed value
    // => value ranges from -32768 to 32767
    // => angle ranges from -180° to 179.994506836 (=32767/32768*180)
    Q_PROPERTY( int minimum READ minValue WRITE setMinValue DESIGNABLE true )
    Q_PROPERTY( int maximum READ maxValue WRITE setMaxValue DESIGNABLE true )

    // comma position is the number of bits for the fractional part
    // hue = 180° is for a value of 0x8000 => comma pos = 15
    Q_PROPERTY( int comma   READ commaPos WRITE setCommaPos DESIGNABLE true )

    // display multiplier is the value to convert into angle range
    // angle offset(x) = (x * displayMultiplier) / (1<<commaPos) 
    // => i.e: offset = (x * 180) / (1<<15) 
    Q_PROPERTY( int displayMultiplier READ displayMultiplier WRITE setDisplayMultiplier DESIGNABLE true )

public:
    explicit HueSegmentSelect( QWidget * parent = 0 );
    ~HueSegmentSelect();

    QPixmap backgroundImage() const;
    void setBackgroundImage( QPixmap p_img );
    
    int offsetPixel () const;
    void setOffsetPixel( int value );

    int noSegments() const;
    void setNoSegments( int value );

    int segmentIndex() const;
    void setSegmentIndex( int value );

    int minValue() const;
    void setMinValue( int min );
    
    int maxValue() const;
    void setMaxValue( int min );
    
    int commaPos() const;
    void setCommaPos( int pos );

    int displayMultiplier() const;
    void setDisplayMultiplier( int factor );

public slots:
    void onHueChange( int value );
    void onSegmentChange( int value );
    
protected:
    void paintEvent( QPaintEvent * ) Q_DECL_OVERRIDE;

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __HUE_SEGMENT_SELECT_H__
