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
 * @file    huesegmentselect.cpp
 *
 * @brief   Class implementation of customized qdail
 *
 *****************************************************************************/
#include "huesegmentselect.h"

#include <QDebug>

/******************************************************************************
 * definitions
 *****************************************************************************/
#define DEFAULT_MIN                 ( -32768 )  /**< default minimum value */
#define DEFAULT_MAX                 (  32767 )  /**< default maximum value */
#define DEFAULT_COMMA_POSITION      (     15 )  /**< comma position of 2^0 */
#define DEFAULT_FACTOR              (    180 )

#define DEFAULT_BACKGROUND          ( ":/images/hue/hue_grey.png" )
#define DEFAULT_PIXEL_OFFSET        ( 2400 )

/******************************************************************************
 * HueSegmentSelect::PrivateData
 *****************************************************************************/
class HueSegmentSelect::PrivateData
{
public:
    PrivateData()
        : m_background( new QPixmap( DEFAULT_BACKGROUND ) )
        , m_segmentBorderColor( QColor(255, 255, 255, 255) )
        , m_paintFlags( QPainter::RenderHint(QPainter::Antialiasing | QPainter::SmoothPixmapTransform || QPainter::HighQualityAntialiasing) )
        , m_offsetPixel( DEFAULT_PIXEL_OFFSET )
        , m_noSegments( DEFAULT_NO_SEGMENTS )
        , m_factor( DEFAULT_FACTOR )
        , m_min( DEFAULT_MIN )
        , m_max( DEFAULT_MAX )
        , m_pos( DEFAULT_COMMA_POSITION )
    {
        m_segmentSize  = (m_background->width() - (m_offsetPixel<<1)) / m_noSegments;
        m_segmentAngle = 360 / m_noSegments;
        m_hue          = 0;
        m_hue_offset   = 0;
        m_segmentIndex = 0;
    };
   
    ~PrivateData()
    {
        delete m_background;
    };

    QPixmap *               m_background;
    QColor                  m_segmentBorderColor;
    QPainter::RenderHint    m_paintFlags;
    
    int                     m_offsetPixel;      /**< red offset in pixel */
    int                     m_noSegments;       /**< number of currently used segments */

    int                     m_factor;           /**< display factor */
    int                     m_min;              /**< minimal value */
    int                     m_max;              /**< maximal value */
    int                     m_pos;              /**< position of comma (meaning (2^m_pos)) */

    int                     m_segmentSize;
    float                   m_segmentAngle;
    float                   m_hue;
    int                     m_hue_offset;
    int                     m_segmentIndex;
};

/******************************************************************************
 * HueSegmentSelect::HueSegmentSelect
 *****************************************************************************/
HueSegmentSelect::HueSegmentSelect( QWidget * parent )
    : QWidget( parent )
{
    // create private data container
    d_data = new PrivateData;

    setMaximumHeight( d_data->m_background->height() );
}

/******************************************************************************
 * HueSegmentSelect::~HueSegmentSelect
 *****************************************************************************/
HueSegmentSelect::~HueSegmentSelect()
{
    delete d_data;
}

/******************************************************************************
 * HueSegmentSelect::backgroundImage
 *****************************************************************************/
QPixmap HueSegmentSelect::backgroundImage() const
{
    return ( *d_data->m_background );
}

/******************************************************************************
 * HueSegmentSelect::setBackgroundImage
 *****************************************************************************/
void HueSegmentSelect::setBackgroundImage( QPixmap p_img )
{
    *d_data->m_background = p_img;
    setMaximumHeight( p_img.height() );
    repaint();
}

/******************************************************************************
 * HueSegmentSelect::offsetPixel
 *****************************************************************************/
int HueSegmentSelect::offsetPixel() const
{
    return ( d_data->m_offsetPixel );
}

/******************************************************************************
 * HueSegmentSelect::setOffsetPixel
 *****************************************************************************/
void HueSegmentSelect::setOffsetPixel( int value )
{
    if ( d_data->m_offsetPixel != value )
    {
        d_data->m_offsetPixel = value;
        repaint();
    }
}

/******************************************************************************
 * HueSegmentSelect::noSegments
 *****************************************************************************/
int HueSegmentSelect::noSegments() const
{
    return ( d_data->m_noSegments );
}

/******************************************************************************
 * HueSegmentSelect::setNoSegments
 *****************************************************************************/
void HueSegmentSelect::setNoSegments( int value )
{
    if ( d_data->m_noSegments != value )
    {
        d_data->m_segmentSize  = (d_data->m_background->width() - (d_data->m_offsetPixel<<1)) / value;
        d_data->m_segmentAngle = 360 / value;
        d_data->m_noSegments   = value;
        repaint();
    }
}

/******************************************************************************
 * HueSegmentSelect::segmentIndex
 *****************************************************************************/
int HueSegmentSelect::segmentIndex() const
{
    return ( d_data->m_segmentIndex );
}

/******************************************************************************
 * HueSegmentSelect::setSegmentIndex
 *****************************************************************************/
void HueSegmentSelect::setSegmentIndex( int value )
{
    if ( (value < d_data->m_noSegments) && (d_data->m_segmentIndex != value) )
    {
        d_data->m_segmentIndex = value;
        d_data->m_hue          = (value * 360) / d_data->m_noSegments;
        d_data->m_hue_offset   = 0;
        repaint();
    }
}

/******************************************************************************
 * HueSegmentSelect::minValue
 *****************************************************************************/
int HueSegmentSelect::minValue() const
{
    return ( d_data->m_min );
}

/******************************************************************************
 * HueSegmentSelect::setMinValue
 *****************************************************************************/
void HueSegmentSelect::setMinValue( int min )
{
    if ( (d_data->m_max > min) && (d_data->m_min != min) )
    {
        d_data->m_min = min;
        repaint();
    }
}

/******************************************************************************
 * HueSegmentSelect::maxValue
 *****************************************************************************/
int HueSegmentSelect::maxValue() const
{
    return ( d_data->m_max );
}

/******************************************************************************
 * HueSegmentSelect::setMaxValue
 *****************************************************************************/
void HueSegmentSelect::setMaxValue( int max )
{
    if ( (max > d_data->m_min) && ( d_data->m_max != max ) )
    {
        d_data->m_max = max;
        repaint();
    }
}

/******************************************************************************
 * HueSegmentSelect::commaPos
 *****************************************************************************/
int HueSegmentSelect::commaPos() const
{
    return ( d_data->m_pos );
}

/******************************************************************************
 * HueSegmentSelect::setCommaPos
 *****************************************************************************/
void HueSegmentSelect::setCommaPos( int pos )
{
    if ( (pos >= 0) && (pos < 31) && (d_data->m_pos != pos) )
    {
        d_data->m_pos = pos;
        repaint();
    }       
}

/******************************************************************************
 * HueSegmentSelect::displayMultiplier
 *****************************************************************************/
int HueSegmentSelect::displayMultiplier() const
{
    return ( d_data->m_factor );
}

/******************************************************************************
 * HueSegmentSelect::setCommaPos
 *****************************************************************************/
void HueSegmentSelect::setDisplayMultiplier( int factor )
{
    if ( (factor > 0) && (d_data->m_factor != factor) )
    {
        d_data->m_factor = factor;
        repaint();
    }
}

/******************************************************************************
 * HueSegmentSelect::onHueChange
 *****************************************************************************/
void HueSegmentSelect::onHueChange( int value )
{
    if ( d_data->m_hue_offset != value )
    {
        d_data->m_hue_offset = value;
        repaint();
    }
}

/******************************************************************************
 * HueSegmentSelect::onSegmentChange
 *****************************************************************************/
void HueSegmentSelect::onSegmentChange( int value )
{
    setSegmentIndex( value );
}

/******************************************************************************
 * HueSegmentSelect::paintEvent
 *****************************************************************************/
void HueSegmentSelect::paintEvent( QPaintEvent * evt )
{
    // QWidget::paintEvent( evt );
    //
    QPainter painter(this);

    QRect eventRect = evt->rect();

    int height = eventRect.height();
    int width  = eventRect.width();
    
    float offset = (float)(d_data->m_hue_offset * d_data->m_factor) / (float)(1<<d_data->m_pos);
    int x = (int)(( (d_data->m_hue + offset) * d_data->m_segmentSize ) / d_data->m_segmentAngle);
    x +=  d_data->m_offsetPixel;
    
    QRect target( 0, 0, width, height );
    QRect source( (x - (width>>1)), 0, width, height );

    painter.setRenderHints( d_data->m_paintFlags );
    painter.drawPixmap( target, *d_data->m_background, source );

    painter.setPen( d_data->m_segmentBorderColor );
    painter.drawRect( ((width>>1)-(d_data->m_segmentSize>>1)), 0, d_data->m_segmentSize, height );
}


