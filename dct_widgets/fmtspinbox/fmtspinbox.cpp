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
 * @file    fmtspinbox.cpp
 *
 * @brief   Class implementation of a formated spinbox
 *
 *****************************************************************************/
#include <QtDebug>
#include <QProxyStyle>

#include "fmtspinbox.h"

/******************************************************************************
 * overrule unconfigureable timer implementation in qt.
 *****************************************************************************/
class FmtSpinBoxStyle : public QProxyStyle
{
public:
    FmtSpinBoxStyle( QStyle *style = 0 ) : QProxyStyle(style) { }

    int styleHint
    (
        StyleHint hint,
        const QStyleOption * option = 0,
        const QWidget * widget = 0,
        QStyleHintReturn * returnData = 0
    ) const
    {
        switch ( hint )
        {
            case QStyle::SH_SpinBox_KeyPressAutoRepeatRate:
            case QStyle::SH_SpinBox_ClickAutoRepeatRate:
                return ( 1000 );

            case QStyle::SH_SpinBox_ClickAutoRepeatThreshold:
                return ( 5000 );

            default:
                return ( QProxyStyle::styleHint(hint, option, widget, returnData) );
        }
    }
};

/******************************************************************************
 * FmtSpinBox::PrivateData
 *****************************************************************************/
class FmtSpinBox::PrivateData
{
public:
    PrivateData()
    {
        m_fmt = "%-2.2f";
    }

    ~PrivateData()
    {
    }

    QString m_fmt;
};

/******************************************************************************
 * FmtSpinBox::FmtSpinBox
 *****************************************************************************/
FmtSpinBox::FmtSpinBox( QWidget * parent )
    : QDoubleSpinBox( parent )
{
    // create private data container
    d_data = new PrivateData();
    setStyle( new FmtSpinBoxStyle() );
}

/******************************************************************************
 * FmtSpinBox::~FmtSpinBox
 *****************************************************************************/
FmtSpinBox::~FmtSpinBox()
{
    delete d_data;
}

/******************************************************************************
 * FmtSpinBox::getFmt
 *****************************************************************************/
QString FmtSpinBox::getFmt() const
{
    return ( d_data->m_fmt );
}

/******************************************************************************
 * FmtSpinBox::setFmt
 *****************************************************************************/
void FmtSpinBox::setFmt( const QString & fmt )
{
    d_data->m_fmt = fmt;
}

/******************************************************************************
 * FmtSpinBox::textFromValue
 *****************************************************************************/
double FmtSpinBox::valueFromText( const QString &text) const
{
    return ( QDoubleSpinBox::valueFromText( QString(text).replace( QLatin1Char('.'), QLocale().decimalPoint() ) ) );
    //double value = text.toDouble();
    //return ( value );
}

/******************************************************************************
 * FmtSpinBox::textFromValue
 *****************************************************************************/
QString FmtSpinBox::textFromValue( double value ) const
{
    QString s;
    s.sprintf( d_data->m_fmt.toLatin1().data(), value );
    s.replace( QLocale().decimalPoint(), QLatin1Char('.') );
    return ( s );
}


/******************************************************************************
 * FmtSpinBox::inputMethodQuery
 *****************************************************************************/
QVariant FmtSpinBox::inputMethodQuery ( Qt::InputMethodQuery ) const
{
    return ( QVariant() );
}
