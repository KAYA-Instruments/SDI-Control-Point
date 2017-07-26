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
 * @file    skinneddial.h
 *
 * @brief   Class definition of customized qdail
 *
 *****************************************************************************/
#ifndef __SKINNEDDIAL_H__
#define __SKINNEDDIAL_H__

#include <QDial>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QTransform>

// #define SKINNED_DIAL_DIRECTION_DETECTION

class SkinnedDial : public QDial
{
    Q_OBJECT

    Q_PROPERTY( QPixmap backgroundImage READ backgroundImage WRITE setBackgroundImage DESIGNABLE true )
    Q_PROPERTY( QPixmap needleImage READ needleImage WRITE setNeedleImage DESIGNABLE true )
    Q_PROPERTY( int maxAngle READ maxAngle WRITE setMaxAngle DESIGNABLE true )

public:
#if defined(SKINNED_DIAL_DIRECTION_DETECTION)
    enum Direction { Unknown=0, CounterClockWise=1, ClockWise=2 };
#endif // SKINNED_DIAL_DIRECTION_DETECTION

    explicit SkinnedDial( QWidget * parent = 0 );
    SkinnedDial( QPixmap * c_back, QPixmap * c_needle, int c_angle, QWidget * parent = 0);
    ~SkinnedDial();

    QPixmap backgroundImage() const;
    void setBackgroundImage(QPixmap p_img);

    QPixmap needleImage() const;
    void setNeedleImage(QPixmap p_img);

    int maxAngle() const;
    void setMaxAngle(int p_angle);

#if defined(SKINNED_DIAL_DIRECTION_DETECTION)
    Direction direction() const;
#endif // SKINNED_DIAL_DIRECTION_DETECTION

protected:
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent( QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent( QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent( QMouseEvent *) Q_DECL_OVERRIDE;

    double radiusAt(const QPoint &pos) const;

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __SKINNEDDIAL_H__
