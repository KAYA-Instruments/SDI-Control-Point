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
 * @file    fmtspinbox.h
 *
 * @brief   Class definition of a formated spinbox 
 *
 *****************************************************************************/
#ifndef __FMT_SPIN_BOX_H__
#define __FMT_SPIN_BOX_H__

#include <QDoubleSpinBox>

class FmtSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
    Q_PROPERTY( QString fmt READ getFmt WRITE setFmt DESIGNABLE true )

public:
    explicit FmtSpinBox( QWidget * parent = 0 );
    ~FmtSpinBox();

    QString getFmt() const;
    void setFmt( const QString & );

    double valueFromText( const QString & ) const;
    QString textFromValue( double value ) const;
    QVariant inputMethodQuery ( Qt::InputMethodQuery query ) const;

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __FMT_SPIN_BOX_H__


