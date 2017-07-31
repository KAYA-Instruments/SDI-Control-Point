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
 * @file    KneeItf.h
 *
 * @brief   Knee Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _KNEE_INTERFACE_H_
#define _KNEE_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

class KneeItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit KneeItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }

    // resync all settings
    void resync() override;

    // configuration
    void GetKneeConfig();

signals:
    // enable status
    void KneeConfigChanged( int, int, int, int );

public slots:
    // enable status
    void onKneeConfigChange( int, int, int, int );
};

#define CONNECT_KNEE_INTERFACE(x, y)                                    \
{                                                                       \
    QObject::connect( x, SIGNAL(KneeConfigChanged(int,int,int,int)),    \
                      y, SLOT(onKneeConfigChange(int,int,int,int)) );   \
}

#endif // _KNEE_INTERFACE_H_

