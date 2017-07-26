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
 * @file    fmtspinbox.cpp
 *
 * @brief   Class implementation of a button array box
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
