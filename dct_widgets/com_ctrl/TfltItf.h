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
 * @file    TfltItf.h
 *
 * @brief   Temporal Filter Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _TFLT_INTERFACE_H_
#define _TFLT_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

class TfltItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit TfltItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }

    // resync all settings
    void resync() override;

    // enable state 
    void GetTfltEnable();
    
    // denoise level
    void GetTfltDenoiseLevel();
    
    // min/max range
    void GetTfltMinMax();

signals:
    // enable status
    void TfltEnableChanged( int value );
    
    // denoise level
    void TfltDenoiseLevelChanged( int value );
    
    // min/max range
    void TfltMinMaxChanged( int min, int max );

public slots:
    // enable status
    void onTfltEnableChange( int value );
    
    // denoise level
    void onTfltDenoiseLevelChange( int value );
    
    // min/max range
    void onTfltMinMaxChange( int min, int max );
};

#define CONNECT_TFLT_INTERFACE(x, y)                            \
{                                                               \
    QObject::connect( x, SIGNAL(TfltEnableChanged(int)),        \
                      y, SLOT(onTfltEnableChange(int)) );       \
    QObject::connect( x, SIGNAL(TfltDenoiseLevelChanged(int)),  \
                      y, SLOT(onTfltDenoiseLevelChange(int)) ); \
    QObject::connect( x, SIGNAL(TfltMinMaxChanged(int,int)),    \
                      y, SLOT(onTfltMinMaxChange(int,int)) );   \
}


#endif // _TFLT_INTERFACE_H_

