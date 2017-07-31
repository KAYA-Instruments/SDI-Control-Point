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
 * @file    dct_widgets_base.h
 *
 * @brief   Definition of base class for DCT widgets 
 *
 *****************************************************************************/
#ifndef __DCT_WIDGETS_BASE_H__
#define __DCT_WIDGETS_BASE_H__

#include <inttypes.h>

#include <QWidget>
#include <QSettings>
#include <QApplication>

class DctWidgetBox : public QWidget
{
    Q_OBJECT

    Q_ENUMS( Mode )
    Q_PROPERTY( Mode mode READ mode WRITE setMode DESIGNABLE true )

public:
    enum Mode
    {
        Normal   = 0,
        Advanced,
    };

    explicit DctWidgetBox( QWidget * parent = 0 )
        : QWidget( parent )
        , m_mode( Normal )
    {
        // do nothing here
    }

    Mode mode() const
    {
        return ( m_mode );
    }

    void setMode( const Mode mode )
    {
        if ( m_mode != mode )
        {
            prepareMode( mode );
            m_mode = mode;
        }
    }

    void load( QSettings & s )
    {
        loadSettings( s );
    }

    void save( QSettings & s )
    {
        saveSettings( s );
    }

    void apply( void )
    {
        applySettings();
    }

    void setWaitCursor()
    {
        QApplication::setOverrideCursor( Qt::WaitCursor );
    }

    void setNormalCursor()
    {
        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }

protected:
    virtual void prepareMode( const Mode ) = 0;
    virtual void loadSettings( QSettings & ) = 0;
    virtual void saveSettings( QSettings & ) = 0;
    virtual void applySettings( void ) = 0;

private:
    Mode    m_mode;
};

#endif // __DCT_WIDGETS_BASE_H__


