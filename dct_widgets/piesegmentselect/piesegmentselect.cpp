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
 * @file    piesegmentselect.cpp
 *
 * @brief   Class implementation of customized qdail
 *
 *****************************************************************************/
#include "piesegmentselect.h"

#include <QtMath>

#include <QDebug>

const double PI = 3.14159265358979323846264338327950288419717;
const double TWOPI = 2.0*PI;

/******************************************************************************
 * PieSegmentSelect::PrivateData
 *****************************************************************************/
class PieSegmentSelect::PrivateData
{
public:
    PrivateData()
        : m_background( new QPixmap( ":/images/hue/hue_grey_center.png" ) )
        , m_cacheBackground( new QPixmap )
        , m_segmentBorderColor( QColor(0, 0, 0, 80) )
        , m_segmentHighLightColor( QColor(255, 255, 255, 127) )
        , m_paintFlags( QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing )
        , m_cacheWidth( 0 )
        , m_cacheHeight( 0 )
        , m_cacheIndex( -1 )
        , m_outerRadius( 0 )
        , m_noSegments( DEFAULT_NO_SEGMENTS )
        , m_redAngleOffset( 90 )
    {
        m_indexOffset = (m_noSegments * m_redAngleOffset) / 360;
        m_selIndex    = m_indexOffset;
    };
   
    ~PrivateData()
    {
        delete m_background;
    };

    QPixmap *               m_background;
    QPixmap *               m_cacheBackground;
    QColor                  m_segmentBorderColor;
    QColor                  m_segmentHighLightColor;
    QPainter::RenderHints   m_paintFlags;

    int                     m_cacheWidth;
    int                     m_cacheHeight;
    int                     m_cacheIndex;
    int                     m_outerRadius;

    QPoint                  m_lastMousePos;
    int                     m_noSegments;
    int                     m_redAngleOffset;

    int                     m_indexOffset;
    int                     m_selIndex;
};

/******************************************************************************
 * PieSegmentSelect::PieSegmentSelect
 *****************************************************************************/
PieSegmentSelect::PieSegmentSelect( QWidget * parent )
    : QWidget( parent )
{
    // create private data container
    d_data = new PrivateData;

    setMouseTracking( true );
}

/******************************************************************************
 * PieSegmentSelect::~PieSegmentSelect
 *****************************************************************************/
PieSegmentSelect::~PieSegmentSelect()
{
    delete d_data;
}

/******************************************************************************
 * PieSegmentSelect::backgroundImage
 *****************************************************************************/
QPixmap PieSegmentSelect::backgroundImage() const
{
    return ( *d_data->m_background );
}

/******************************************************************************
 * PieSegmentSelect::setBackgroundImage
 *****************************************************************************/
void PieSegmentSelect::setBackgroundImage( QPixmap p_img )
{
    *d_data->m_background = p_img;
    d_data->m_cacheHeight = 0;
    d_data->m_cacheWidth  = 0;
    repaint();
}

/******************************************************************************
 * PieSegmentSelect::angleOffsetRed
 *****************************************************************************/
int PieSegmentSelect::angleOffsetRed() const
{
    return ( d_data->m_redAngleOffset );
}

/******************************************************************************
 * PieSegmentSelect::setAngleOffsetRed
 *****************************************************************************/
void PieSegmentSelect::setAngleOffsetRed( int value )
{
    if ( d_data->m_redAngleOffset != value )
    {
        d_data->m_redAngleOffset = value;
        d_data->m_cacheHeight = 0;
        d_data->m_cacheWidth  = 0;
        repaint();
    }
}

/******************************************************************************
 * PieSegmentSelect::noSegments
 *****************************************************************************/
int PieSegmentSelect::noSegments() const
{
    return ( d_data->m_noSegments );
}

/******************************************************************************
 * PieSegmentSelect::setNoSegments
 *****************************************************************************/
void PieSegmentSelect::setNoSegments( int value )
{
    if ( d_data->m_noSegments != value )
    {
        d_data->m_indexOffset = (value * d_data->m_redAngleOffset) / 360;
        d_data->m_noSegments  = value;
        d_data->m_cacheHeight = 0;
        d_data->m_cacheWidth  = 0;
        repaint();
    }
}

/******************************************************************************
 * PieSegmentSelect::segmentBorderColor
 *****************************************************************************/
QColor PieSegmentSelect::segmentBorderColor() const
{
    return ( d_data->m_segmentBorderColor );
}

/******************************************************************************
 * PieSegmentSelect::setSegmentBorderColorm_segmentSize
 *****************************************************************************/
void PieSegmentSelect::setSegmentBorderColor( QColor color )
{
    d_data->m_segmentBorderColor = color;
    d_data->m_cacheHeight = 0;
    d_data->m_cacheWidth  = 0;
    repaint();
}

/******************************************************************************
 * PieSegmentSelect::segmentHighLightColor
 *****************************************************************************/
QColor PieSegmentSelect::segmentHighLightColor() const
{
    return ( d_data->m_segmentHighLightColor );
}

/******************************************************************************
 * PieSegmentSelect::setSegmentHighLightColor
 *****************************************************************************/
void PieSegmentSelect::setSegmentHighLightColor( QColor color )
{
    d_data->m_segmentHighLightColor = color;
    d_data->m_cacheHeight = 0;
    d_data->m_cacheWidth  = 0;
    repaint();
}

/******************************************************************************
 * PieSegmentSelect::selectedIndex
 *****************************************************************************/
int PieSegmentSelect::selectedIndex() const
{
    // take red-angle offset into account
    int idx = d_data->m_selIndex - d_data->m_indexOffset;
    if ( idx < 0 )
    {
        idx += d_data->m_noSegments;
    }

    return ( idx );
}

/******************************************************************************
 * PieSegmentSelect::selectedIndex
 *****************************************************************************/
void PieSegmentSelect::setSelectedIndex( int idx )
{
    // take red-angle offset into account
    d_data->m_selIndex = idx + d_data->m_indexOffset;
}

/******************************************************************************
 * PieSegmentSelect::paintEvent
 *****************************************************************************/
void PieSegmentSelect::paintEvent( QPaintEvent * evt )
{
    QPainter painter(this);

    QRect eventRect = evt->rect();

    int height = eventRect.height();
    int width  = eventRect.width();
    
    if ( (height != d_data->m_cacheHeight) || (width != d_data->m_cacheWidth) )
    {
        if ( width >= height )
        {
            if ( height <= d_data->m_background->height() )
            {
                *d_data->m_cacheBackground = d_data->m_background->scaledToHeight( height, Qt::SmoothTransformation );
                d_data->m_outerRadius = (height >> 1) + 1;
            }
        }
        else
        {
            if ( width <= d_data->m_background->width() )
            {
                *d_data->m_cacheBackground = d_data->m_background->scaledToWidth( width, Qt::SmoothTransformation );
                d_data->m_outerRadius = (width >> 1) + 1;
            }
        }

        d_data->m_cacheHeight = height;
        d_data->m_cacheWidth  = width;
    }

    int leftPoint = (width  - d_data->m_cacheBackground->width() ) / 2;
    int topPoint  = (height - d_data->m_cacheBackground->height()) / 2;
    
    QPoint startPix( leftPoint, topPoint );
    painter.setRenderHints( d_data->m_paintFlags );
    painter.drawPixmap( startPix, *d_data->m_cacheBackground );

    int spanAngle = (360 * 16) / d_data->m_noSegments;
    int startAngle = - ( spanAngle / 2);
    QRect rect = QRect( leftPoint, topPoint, d_data->m_cacheBackground->width(), d_data->m_cacheBackground->height() );

    for ( int i = 0; i < d_data->m_noSegments; ++i )
    {
        painter.setPen( d_data->m_segmentBorderColor );
        if ( (d_data->m_cacheIndex > -1) && (d_data->m_cacheIndex == i) )
        {
            painter.setBrush( d_data->m_segmentHighLightColor );
        }
        else
        {
            painter.setBrush( QColor(0, 0, 0, 0) );
        }

        painter.drawPie( rect, startAngle, spanAngle );

        // increment start-angle
        startAngle += spanAngle;
    }

    if ( d_data->m_selIndex > -1 )
    {
        painter.setPen( QColor(255, 255, 255, 255) );

        if ( (d_data->m_cacheIndex > -1) && (d_data->m_cacheIndex == d_data->m_selIndex) )
        {
            painter.setBrush( d_data->m_segmentHighLightColor );
        }
        else
        {
            painter.setBrush( QColor(0, 0, 0, 0) );
        }

        startAngle = (d_data->m_selIndex * spanAngle) - ( spanAngle / 2);
        painter.drawPie( rect, startAngle, spanAngle );
    }
}

/******************************************************************************
 * PieSegmentSelect::mouseMoveEvent
 *****************************************************************************/
void PieSegmentSelect::mouseMoveEvent( QMouseEvent * evt )
{
    d_data->m_lastMousePos = mapFromGlobal( evt->globalPos() );
    double mouseRad = radiusAt( d_data->m_lastMousePos );

    if ( mouseRad < d_data->m_outerRadius )
    {
        double angle = angleAt( d_data->m_lastMousePos );
        int index = angleToIndex( angle );

        if ( d_data->m_cacheIndex != index )
        {
            d_data->m_cacheIndex = index;
            repaint();

            // take red-angle offset into account
            int idx = index - d_data->m_indexOffset;
            if ( idx < 0 )
            {
                idx += d_data->m_noSegments;
            }

            emit moved( index );
        }
    }
} 

/******************************************************************************
 * PieSegmentSelect::mouseReleaseEvent
 *****************************************************************************/
void PieSegmentSelect::mouseReleaseEvent( QMouseEvent * evt )
{
    d_data->m_lastMousePos = mapFromGlobal( evt->globalPos() );
    double mouseRad = radiusAt( d_data->m_lastMousePos );

    if ( mouseRad < d_data->m_outerRadius )
    {
        double angle = angleAt( d_data->m_lastMousePos );
        int index = angleToIndex( angle );

        if ( d_data->m_selIndex != index )
        {
            d_data->m_selIndex = index;
            repaint();

            // take red-angle offset into account
            int idx = index - d_data->m_indexOffset;
            if ( idx < 0 )
            {
                idx += d_data->m_noSegments;
            }

            emit clicked( idx );
        }
    }
} 

/******************************************************************************
 * PieSegmentSelect::leaveEvent
 *****************************************************************************/
void PieSegmentSelect::leaveEvent( QEvent * evt )
{
    (void)evt;
    d_data->m_cacheIndex = -1;
    repaint();
} 

/******************************************************************************
 * PieSegmentSelect::radiusAt
 *****************************************************************************/
double PieSegmentSelect::radiusAt( const QPoint &pos ) const
{
    double mousexdist = pos.x() - rect().center().x();
    double mouseydist = pos.y() - rect().center().y();
    return ( qSqrt(mousexdist * mousexdist + mouseydist * mouseydist) );
}

/******************************************************************************
 * PieSegmentSelect::radiusAt
 *****************************************************************************/
double PieSegmentSelect::angleAt( const QPoint &pos ) const
{
    double mousexdist = pos.x() - rect().center().x();
    double mouseydist = pos.y() - rect().center().y();
    double mouserad = qSqrt(mousexdist * mousexdist + mouseydist * mouseydist);
    double angle = qAcos( mousexdist / mouserad );

    if ( mouseydist >= 0 )
    {
        angle = TWOPI - angle;
    }
    
    return ( angle );
}

/******************************************************************************
 * PieSegmentSelect::radiusAt
 *****************************************************************************/
int PieSegmentSelect::angleToIndex( const double angle ) const
{
    if ( angle <= 0.0001 )
    {
        return ( 0 );
    }

    // segments spans from -(2*PI/d_data->m_noSegments) to +(2*PI/d_data->m_noSegments)
    int index = (int)(((angle + (PI/d_data->m_noSegments)) * d_data->m_noSegments) / TWOPI);
    if ( index == d_data->m_noSegments )
    {
        index = 0;
    }

    return ( index );
}


