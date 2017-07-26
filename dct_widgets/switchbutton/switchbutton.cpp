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
 * @file    switchbutton.cpp
 *
 * @brief   Class implementation of a switch button
 *
 *****************************************************************************/
#include <QtDebug>
 
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>

#include "switchbutton.h"

/******************************************************************************
 * SwitchButton::PrivateData
 *****************************************************************************/
class SwitchButton::PrivateData
{
public:
    PrivateData()
    {
        m_onColor  = Qt::blue;
        m_offColor = Qt::gray;
        m_onLabel  = QString(tr("on"));
        m_offLabel = QString(tr("off"));
    }

    ~PrivateData()
    {
    }

    QColor  m_onColor;
    QColor  m_offColor;
    
    QString m_onLabel;
    QString m_offLabel;
    
    QColor  m_onLabelColor;
    QColor  m_offLabelColor;
};

/******************************************************************************
 * SwitchButton::SwitchButton
 *****************************************************************************/
SwitchButton::SwitchButton( QWidget * parent )
    : QPushButton(parent)
{
    // create private data container
    d_data = new PrivateData();

    setCheckable( true );

    // Set default colors and labels
    setColors();
    setLabels();
}

/******************************************************************************
 * SwitchButton::~SwitchButton
 *****************************************************************************/
SwitchButton::~SwitchButton()
{
    delete d_data;
}

/******************************************************************************
 * SwitchButton::onLabel
 *****************************************************************************/
QString SwitchButton::onLabel() const
{
    return ( d_data->m_onLabel );
}

/******************************************************************************
 * SwitchButton::setOnLabel
 *****************************************************************************/
void SwitchButton::setOnLabel( QString on )
{
    d_data->m_onLabel = on;
}

/******************************************************************************
 * SwitchButton::offLabel
 *****************************************************************************/
QString SwitchButton::offLabel() const
{
    return ( d_data->m_offLabel );
}

/******************************************************************************
 * SwitchButton::setOffLabel
 *****************************************************************************/
void SwitchButton::setOffLabel( QString on )
{
    d_data->m_offLabel = on;
}

/******************************************************************************
 * SwitchButton::onColor
 *****************************************************************************/
QColor SwitchButton::onColor() const
{
    return ( d_data->m_onColor );
}

/******************************************************************************
 * SwitchButton::setOnColor
 *****************************************************************************/
void SwitchButton::setOnColor( QColor color )
{
    d_data->m_onColor = color;
}

/******************************************************************************
 * SwitchButton::offColor
 *****************************************************************************/
QColor SwitchButton::offColor() const
{
    return ( d_data->m_offColor );
}

/******************************************************************************
 * SwitchButton::setOffColor
 *****************************************************************************/
void SwitchButton::setOffColor( QColor color )
{
    d_data->m_offColor = color;
}

/******************************************************************************
 * SwitchButton::setColors
 *****************************************************************************/
void SwitchButton::setColors( const QColor on, const QColor off )
{
    d_data->m_onColor  = on;
    d_data->m_offColor = off;
    
    if ( (on.red() + on.green() + on.blue()) > 500 )
    {
        d_data->m_onLabelColor = Qt::black;
    }
    else
    {
        d_data->m_onLabelColor = Qt::white;
    }

    if ( (off.red() + off.green() + off.blue()) > 500 )
    {
        d_data->m_offLabelColor = Qt::black;
    }
    else
    {
        d_data->m_offLabelColor = Qt::white;
    }
}

/******************************************************************************
 * SwitchButton::setLabels
 *****************************************************************************/
void SwitchButton::setLabels( const QString on, const QString off )
{
    d_data->m_onLabel  = on;
    d_data->m_offLabel = off;
    setMinimumWidth( fontMetrics().width(off) + fontMetrics().width(on) + fontMetrics().height()*2 );
}

/******************************************************************************
 * SwitchButton::paintEvent
 *****************************************************************************/
void SwitchButton::paintEvent( QPaintEvent * paint )
{
    QPushButton::paintEvent( paint );
    QPainterPath path;

    QPainter p( this );
    p.save();

    int rectWidth = paint->rect().width()/2;

#ifdef Q_OS_ANDROID
    // nn Android, the buttons are much smaller than paint->rect().
    int rectMargin = 10;
#else
    int rectMargin = 4;
#endif

    if ( isChecked() )
    {
        QRect rect = paint->rect().adjusted( rectWidth, rectMargin, -rectMargin, -rectMargin );
        QPoint textPosi   = rect.center() - QPoint( fontMetrics().width(d_data->m_onLabel)/2, 1-fontMetrics().ascent()/2 );
        
        QRect rect1( rect );
        rect1.setWidth( rect.width() >> 1 );
        
        path.setFillRule( Qt::WindingFill );
        path.addRoundedRect( rect, 5, 5 );
        path.addRect( rect1 );

        p.fillPath( path.simplified(), QBrush(d_data->m_onColor) );
        p.setPen( d_data->m_onLabelColor );
        p.drawText( textPosi, d_data->m_onLabel );
    }
    else
    {
        QRect rect      = paint->rect().adjusted( rectMargin, rectMargin, -rectWidth, -rectMargin );
        QPoint textPosi = rect.center() - QPoint( fontMetrics().width(d_data->m_offLabel)/2, 1-fontMetrics().ascent()/2 );

        QRect rect1( rect );
        //rect1.setWidth( rect.width() >> 1 );
        rect1.setX( (rect.width() >> 1) + rect.x() );
        rect1.setWidth( ((rect.width() >> 1) + 1) );

        path.setFillRule( Qt::WindingFill );
        path.addRoundedRect( rect, 5, 5 );
        path.addRect( rect1 );

        p.fillPath( path.simplified(), QBrush(d_data->m_offColor) );
        p.setPen( d_data->m_offLabelColor );
        p.drawText( textPosi, d_data->m_offLabel );
    }

    p.restore();
}
