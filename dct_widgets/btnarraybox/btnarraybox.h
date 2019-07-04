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
 * @file    btnarraybox.h
 *
 * @brief   Class definition of a button array group-box
 *
 *****************************************************************************/
#ifndef __BTN_ARRAY_BOX_H__
#define __BTN_ARRAY_BOX_H__

#include <QGroupBox>

class ButtonArrayBox : public QGroupBox
{
    Q_OBJECT
  
    Q_PROPERTY( int maxColumns READ maxColumns WRITE setMaxColumns DESIGNABLE true )

public:
    explicit ButtonArrayBox( QWidget * parent = 0 );
    ~ButtonArrayBox();

    int maxColumns() const;
    void setMaxColumns( int max );
 
    void addButton( QString label, bool newRow = false );
    void addButton( QString label, QPixmap px, bool newRow = false );
    void deleteButtons();

    void setButtonPixmap( int id, QPixmap px );

signals:
    void buttonClicked( int id );

private:
    class CustomToolButton;
    class PrivateData;
    PrivateData * d_data;
};

#endif // __BTN_ARRAY_BOX_H__

