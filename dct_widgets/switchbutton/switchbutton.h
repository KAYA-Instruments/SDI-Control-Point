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
 * @file    switchbutton.h
 *
 * @brief   Class definition of a switch button
 *
 * @note    This is a button which can be switched on and off. It looks
 *          similar as the on/off switches of Android.
 *
 *****************************************************************************/
#ifndef __SWITCHBUTTON_H__
#define __SWITCHBUTTON_H__

#include <QPushButton>

class SwitchButton : public QPushButton
{
    Q_OBJECT

    Q_PROPERTY( QString onLabel  READ onLabel  WRITE setOnLabel  DESIGNABLE true )
    Q_PROPERTY( QString offLabel READ offLabel WRITE setOffLabel DESIGNABLE true )
    Q_PROPERTY( QColor  onColor  READ onColor  WRITE setOnColor  DESIGNABLE true )
    Q_PROPERTY( QColor  offColor READ offColor WRITE setOffColor DESIGNABLE true )


public:
    explicit SwitchButton(QWidget* parent = 0);
    ~SwitchButton();

    QString onLabel() const;
    void setOnLabel( QString on );
    
    QString offLabel() const;
    void setOffLabel( QString on );
    
    QColor onColor() const;
    void setOnColor( QColor color = Qt::blue );
    
    QColor offColor() const;
    void setOffColor( QColor color = Qt::gray );
 
    void setColors( const QColor on=Qt::blue, const QColor off=Qt::gray );
    void setLabels( const QString on=QString(tr("on")), const QString off=QString(tr("off")) );

protected:
    void paintEvent( QPaintEvent * paint );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __SWITCHBUTTON_H__

























