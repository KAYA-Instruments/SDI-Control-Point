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

























