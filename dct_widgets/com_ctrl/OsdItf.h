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
 * @file    OsdItf.h
 *
 * @brief   Osd Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _OSD_INTERFACE_H_
#define _OSD_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

class OsdItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit OsdItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }

    // resync all settings
    void resync() override;

    // test pattern
    void GetTestPattern();

signals:
    // test pattern
    void TestPatternChanged( int pattern );

public slots:
    // test pattern
    void onTestPatternChange( int pattern );
};

#endif // _OSD_INTERFACE_H_

