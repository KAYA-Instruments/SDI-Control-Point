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
 * @file    piesegmentselect.h
 *
 * @brief   Class definition of customized color segment/phase selection
 *
 *****************************************************************************/
#ifndef __PIE_SEGMENT_SELECT_H__
#define __PIE_SEGMENT_SELECT_H__

#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QTransform>
#include <QWidget>

#define DEFAULT_NO_SEGMENTS     ( 24 )

class PieSegmentSelect : public QWidget
{
    Q_OBJECT

    Q_PROPERTY( QPixmap backgroundImage READ backgroundImage WRITE setBackgroundImage DESIGNABLE true )

    // angleOffsetRed: Normally red is zero point (hue=0°), but if red not at 0°
    // in background image give an offset [°] here for computing a segment index offset
    Q_PROPERTY( int angleOffsetRed READ angleOffsetRed WRITE setAngleOffsetRed DESIGNABLE true )
    
    Q_PROPERTY( QColor segmentBorderColor READ segmentBorderColor WRITE setSegmentBorderColor DESIGNABLE true )
    Q_PROPERTY( QColor segmentHighLightColor READ segmentHighLightColor WRITE setSegmentHighLightColor DESIGNABLE true )
    Q_PROPERTY( int noSegments READ noSegments WRITE setNoSegments DESIGNABLE true )

public:
    explicit PieSegmentSelect( QWidget * parent = 0 );
    ~PieSegmentSelect();

    QPixmap backgroundImage() const;
    void setBackgroundImage( QPixmap p_img );
    
    int angleOffsetRed() const;
    void setAngleOffsetRed( int value );
    
    int noSegments() const;
    void setNoSegments( int no_segments );

    QColor segmentBorderColor() const;
    void setSegmentBorderColor( QColor color );
    
    QColor segmentHighLightColor() const;
    void setSegmentHighLightColor( QColor color );

    int selectedIndex() const;
    void setSelectedIndex( int );

protected:
    void paintEvent( QPaintEvent * ) Q_DECL_OVERRIDE;
    void mouseMoveEvent( QMouseEvent * ) Q_DECL_OVERRIDE;
    void mouseReleaseEvent( QMouseEvent * ) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent * ) Q_DECL_OVERRIDE;

    double radiusAt(const QPoint &pos) const;
    double angleAt( const QPoint &pos ) const;
    int angleToIndex( const double angle ) const;

signals:
    void clicked( int segment );
    void moved( int segment );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __PIE_SEGMENT_SELECT_H__
