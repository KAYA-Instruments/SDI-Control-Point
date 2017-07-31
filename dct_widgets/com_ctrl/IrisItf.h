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
 * @file    IrisItf.h
 *
 * @brief   Iris Processing Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _IRIS_INTERFACE_H_
#define _IRIS_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

class IrisItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit IrisItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }
 
    // resync all settings
    void resync() override;

    void GetIrisSetup();
    void GetIrisApt();


signals:
    void IrisSetupChanged( QVector<int> values );
    void IrisAptChanged( int pos );
    void IrisAptError( void );

public slots:
    void onIrisSetupChange( QVector<int> values );
    void onIrisAptChange( int );

};

#endif // _IRIS_INTERFACE_H_



