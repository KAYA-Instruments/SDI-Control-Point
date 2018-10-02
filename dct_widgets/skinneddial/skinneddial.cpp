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
 * @file    skinneddial.cpp
 *
 * @brief   Class implementation of customized qdail
 *
 *****************************************************************************/
#include "skinneddial.h"

#include <QDebug>
#include <qmath.h>

#if defined(SKINNED_DIAL_DIRECTION_DETECTION)
static const qreal Q_PI   = qreal(3.14159265358979323846);   // pi
static const qreal Q_2PI  = qreal(6.28318530717958647693);   // 2*pi
#endif // SKINNED_DIAL_DIRECTION_DETECTION

/******************************************************************************
 * SkinnedDial::PrivateData
 *****************************************************************************/
class SkinnedDial::PrivateData
{
public:
    PrivateData()
        : m_background( new QPixmap( ":/images/needle/dial-back.png" ) )
        , m_needle( new QPixmap( ":/images/back/dial-pointer.png" ) )
        , m_paintFlags( QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing )
        , m_maxDeg( 150 )
        , m_cacheBackground( new QPixmap )
        , m_cacheNeedle( new QPixmap )
        , m_cacheNeedleRot( new QPixmap )
        , m_cacheVal( 0 )
        , m_cacheMin( 0 )
        , m_cacheMax( 0 )
        , m_cacheWidth( 0 )
        , m_cacheHeight( 0 )
        , m_outerRadius( 0 )
#if defined(SKINNED_DIAL_DIRECTION_DETECTION)
        , m_mousePoint( 0, 0 );
        , m_mouseAngle( 0.0 );
        , m_direction( Unknown );
#endif
    {
        // do nothing
    }
   
    PrivateData( QPixmap * c_back, QPixmap * c_needle, int c_angle )
        : m_background( new QPixmap( *c_back ) )
        , m_needle( new QPixmap( *c_needle ) )
        , m_paintFlags( QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing )
        , m_maxDeg( c_angle )
        , m_cacheBackground( new QPixmap )
        , m_cacheNeedle( new QPixmap )
        , m_cacheNeedleRot( new QPixmap )
        , m_cacheVal( 0 )
        , m_cacheMin( 0 )
        , m_cacheMax( 0 )
        , m_cacheWidth( 0 )
        , m_cacheHeight( 0 )
        , m_outerRadius( 0 )
#if defined(SKINNED_DIAL_DIRECTION_DETECTION)
        , m_mousePoint( 0, 0 );
        , m_mouseAngle( 0.0 );
        , m_direction( Unknown );
#endif
    {
        // do nothing
    }

    ~PrivateData()
    {
        delete m_background;
        delete m_needle;
        delete m_cacheBackground;
        delete m_cacheNeedle;
    }

    QPixmap *               m_background;
    QPixmap *               m_needle;
    QPainter::RenderHints   m_paintFlags;
    int                     m_maxDeg;

    QPixmap *               m_cacheBackground;
    QPixmap *               m_cacheNeedle;
    QPixmap *               m_cacheNeedleRot;

    int                     m_cacheVal;
    int                     m_cacheMin;
    int                     m_cacheMax;
    int                     m_cacheWidth;
    int                     m_cacheHeight;

    int                     m_outerRadius;

#if defined(SKINNED_DIAL_DIRECTION_DETECTION)
    QPoint                  m_mousePoint;
    double                  m_mouseAngle;
    SkinnedDial::Direction  m_direction;
#endif    

    QPixmap _rotatePix( QPixmap* p_pix, float p_deg, bool p_dir );
};

/******************************************************************************
 * SkinnedDial::PrivateData::_rotatePix
 *****************************************************************************/
QPixmap SkinnedDial::PrivateData::_rotatePix( QPixmap *p_pix, float p_deg, bool p_dir )
{
    // all rotation is CCW, so calculate for the "right" side of the knob
    if ( p_dir == false )
    {
        p_deg = 360.0f - p_deg;
    }

    // perform rotation, transforming around the center of the image
    QTransform trans;
    trans.translate(  p_pix->width()/2.0 , p_pix->height()/2.0 );
    trans.rotate( static_cast<qreal>(p_deg) );
    trans.translate( -p_pix->width()/2.0 , -p_pix->height()/2.0 );
    QPixmap outPix = p_pix->transformed( trans, Qt::SmoothTransformation );

    // re-crop to original size
    int xOffset = (outPix.width() - p_pix->width()) / 2;
    int yOffset = (outPix.height() - p_pix->height()) / 2;
    outPix = outPix.copy( xOffset, yOffset, p_pix->width(), p_pix->height() );

    return ( outPix );
}

/******************************************************************************
 * SkinnedDial::SkinnedDial
 *****************************************************************************/
SkinnedDial::SkinnedDial( QWidget * parent )
    : QDial( parent )
{
    // create private data container
    d_data = new PrivateData;

    setWrapping( false );
}

/******************************************************************************
 * SkinnedDial::SkinnedDial
 *****************************************************************************/
SkinnedDial::SkinnedDial( QPixmap * c_back, QPixmap * c_needle, int c_angle, QWidget * parent ) 
    : QDial( parent )
{
    // create private data container
    d_data = new PrivateData( c_back, c_needle, c_angle );
    
    setWrapping( false );
}

/******************************************************************************
 * SkinnedDial::~SkinnedDial
 *****************************************************************************/
SkinnedDial::~SkinnedDial()
{
    delete d_data;
}

/******************************************************************************
 * SkinnedDial::backgroundImage
 *****************************************************************************/
QPixmap SkinnedDial::backgroundImage() const
{
    return ( *d_data->m_background );
}

/******************************************************************************
 * SkinnedDial::setBackgroundImage
 *****************************************************************************/
void SkinnedDial::setBackgroundImage( QPixmap p_img )
{
    *d_data->m_background = p_img;
    d_data->m_cacheHeight = 0;
    d_data->m_cacheWidth  = 0;
    repaint();
}

/******************************************************************************
 * SkinnedDial::needleImage
 *****************************************************************************/
QPixmap SkinnedDial::needleImage() const 
{
    return ( *d_data->m_needle );
}

/******************************************************************************
 * SkinnedDial::setNeedleImage
 *****************************************************************************/
void SkinnedDial::setNeedleImage( QPixmap p_img )
{
    *d_data->m_needle = p_img;
    d_data->m_cacheHeight = 0;
    d_data->m_cacheWidth  = 0;
    repaint();
}

/******************************************************************************
 * SkinnedDial::maxAngle
 *****************************************************************************/
int SkinnedDial::maxAngle() const
{
    return ( d_data->m_maxDeg );
}

/******************************************************************************
 * SkinnedDial::setMaxAngle
 *****************************************************************************/
void SkinnedDial::setMaxAngle( int p_angle )
{
    d_data->m_maxDeg = p_angle;
}

/******************************************************************************
 * SkinnedDial::direction
 *****************************************************************************/
#if defined(SKINNED_DIAL_DIRECTION_DETECTION)
SkinnedDial::Direction SkinnedDial::direction() const
{
    return ( d_data->m_direction );
}
#endif // SKINNED_DIAL_DIRECTION_DETECTION

/******************************************************************************
 * SkinnedDial::mousePressEvent
 *****************************************************************************/
void SkinnedDial::mousePressEvent( QMouseEvent * evt )
{
    QPoint p = mapFromGlobal( evt->globalPos() );
    double mouseRad = radiusAt( p );
    if ( mouseRad < d_data->m_outerRadius )
    {
#if defined(SKINNED_DIAL_DIRECTION_DETECTION)
        int xx = p.x() - (width() / 2);
        int yy = p.y() - (height() / 2);
        double angle = qAtan2( yy, xx );
        
        d_data->m_mousePoint = QPoint( xx, yy );
        d_data->m_mouseAngle = angle;
#endif // SKINNED_DIAL_DIRECTION_DETECTION
        
        QDial::mousePressEvent( evt );
    }
    else
    {
        evt->ignore();
    }
}

/******************************************************************************
 * SkinnedDial::mouseReleaseEvent
 *****************************************************************************/
void SkinnedDial::mouseReleaseEvent( QMouseEvent * evt )
{
    double mouseRad = radiusAt( mapFromGlobal( evt->globalPos() ) );
    if ( mouseRad < d_data->m_outerRadius )
    {
        QDial::mouseReleaseEvent( evt );
    }
    else
    {
        evt->ignore();
    }
}

/******************************************************************************
 * SkinnedDial::mouseMoveEvent
 *****************************************************************************/
void SkinnedDial::mouseMoveEvent( QMouseEvent * evt )
{
    QPoint p = mapFromGlobal( evt->globalPos() );
    double mouseRad = radiusAt( p );
    if ( mouseRad < d_data->m_outerRadius ) 
    {
#if defined(SKINNED_DIAL_DIRECTION_DETECTION)
        int yy = (height() / 2) - p.y();
        int xx = p.x() - (width() / 2);

        if ( (xx == d_data->m_mousePoint.x()) || (yy == d_data->m_mousePoint.y()) )
        {
            evt->ignore();
            return;
        }

        double angle = qAtan2( yy, xx );
        double d_angle = (d_data->m_mouseAngle - angle);
        
        // range check for 4->1 quadrant crossing
        if ( d_angle > 1 )
        {
            d_angle -= Q_2PI;
        }
        else
        {
            // range check for 2->3 quadrant crossing
            if ( d_angle < -1 )
            {
                d_angle += Q_2PI;
            }
        }

        d_data->m_direction  = ( d_angle > 0 ) ? ClockWise : CounterClockWise;
        d_data->m_mousePoint = QPoint( xx, yy );
        d_data->m_mouseAngle = angle;
#endif // SKINNED_DIAL_DIRECTION_DETECTION
        
        QDial::mouseMoveEvent( evt );
    }
    else
    {
        evt->ignore();
    }
}

/******************************************************************************
 * SkinnedDial::resizeEvent
 *****************************************************************************/
void SkinnedDial::resizeEvent( QResizeEvent * evt )
{
    d_data->m_cacheHeight = 0;
    d_data->m_cacheWidth  = 0;
    QDial::resizeEvent( evt );
}

/******************************************************************************
 * SkinnedDial::paintEvent
 *****************************************************************************/
void SkinnedDial::paintEvent( QPaintEvent *pe )
{
    QPainter painter(this);
    QRect eventRect = pe->rect();

    bool cacheHit = true;

    // scale pixmap so that it fits within the boundaries of the event

    int height = eventRect.height();
    int width  = eventRect.width();
    
    // only perform scaling again if our event rectangle has changed

    if ( (height != d_data->m_cacheHeight) || (width != d_data->m_cacheWidth) )
    {
        if ( width >= height )
        {
            if ( height <= d_data->m_background->height() )
            {
                *d_data->m_cacheBackground = d_data->m_background->scaledToHeight( height, Qt::SmoothTransformation );
                *d_data->m_cacheNeedle     = d_data->m_needle->scaledToHeight( height, Qt::SmoothTransformation );
                *d_data->m_cacheNeedleRot  = *d_data->m_cacheNeedle;
                d_data->m_outerRadius = (height >> 1) + 1;
            }
        }
        else
        {
            if ( width <= d_data->m_background->width() )
            {
                *d_data->m_cacheBackground = d_data->m_background->scaledToWidth( width, Qt::SmoothTransformation );
                *d_data->m_cacheNeedle     = d_data->m_needle->scaledToWidth( width, Qt::SmoothTransformation );
                *d_data->m_cacheNeedleRot  = *d_data->m_cacheNeedle;
                d_data->m_outerRadius = (width >> 1) + 1;
            }
        }

        d_data->m_cacheHeight = height;
        d_data->m_cacheWidth  = width;
        cacheHit              = false;
    }

    // find top-left corner to start placing the pixmap for the background,
    // centering it in the total event area

    int leftPoint = (width  - d_data->m_cacheBackground->width() ) / 2;
    int topPoint  = (height - d_data->m_cacheBackground->height()) / 2;

    QPoint startPix( leftPoint, topPoint );

    painter.setRenderHints( d_data->m_paintFlags );
    painter.drawPixmap( startPix, *d_data->m_cacheBackground );

    // rotate the needle image and display it

    int curVal = this->value();
    int curMin = this->minimum();
    int curMax = this->maximum();

    QPixmap rotNeedle = *d_data->m_cacheNeedleRot;

    // only re-process the needle image if the position changes, or we had a size cache miss
    if ( (curVal != d_data->m_cacheVal) || (curMin != d_data->m_cacheMin) || (curMax != d_data->m_cacheMax) || (cacheHit == false) )
    {
        rotNeedle = *d_data->m_cacheNeedle;

        int median = ( this->maximum() - abs(this->minimum()) ) / 2;
        if ( curVal < median )
        {
            float pct = static_cast<float>(abs( curVal - median )) / static_cast<float>(abs( this->minimum() - median ));
            rotNeedle = d_data->_rotatePix( &rotNeedle, static_cast<float>(d_data->m_maxDeg) * pct, false );
        }
        else
        {
            float pct = static_cast<float>(abs( curVal - median )) / static_cast<float>(abs( this->maximum() - median ));
            rotNeedle = d_data->_rotatePix( &rotNeedle, static_cast<float>(d_data->m_maxDeg) * pct, true );
        }

        d_data->m_cacheVal = curVal;
        *d_data->m_cacheNeedleRot = rotNeedle;
    }

    painter.drawPixmap( startPix, rotNeedle );
}

/******************************************************************************
 * SkinnedDial::radiusAt
 *****************************************************************************/
double SkinnedDial::radiusAt( const QPoint &pos ) const
{
    double mousexdist = pos.x() - rect().center().x();
    double mouseydist = pos.y() - rect().center().y();
    return ( sqrt(mousexdist * mousexdist + mouseydist * mouseydist) );
}
