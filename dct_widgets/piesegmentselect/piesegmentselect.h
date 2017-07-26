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
