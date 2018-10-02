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

    explicit SkinnedDial( QWidget * parent = nullptr );
    SkinnedDial( QPixmap * c_back, QPixmap * c_needle, int c_angle, QWidget * parent = nullptr );
    ~SkinnedDial() Q_DECL_OVERRIDE;

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
