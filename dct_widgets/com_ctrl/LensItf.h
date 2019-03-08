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
 * @file    LensItf.h
 *
 * @brief   Lens Control Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _LENS_INTERFACE_H_
#define _LENS_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"

class LensItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit LensItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p )
    { }
 
    // resync all settings
    void resync() override;

    void GetLensSettings();
    void GetLensActive();

    void GetLensFocusPosition();
    void GetLensZoomPosition();
    void GetLensIrisPosition();
    void GetLensFilterPosition();

    void GetLensFocusSettings();
    void GetLensZoomSettings();
    void GetLensIrisSettings();
    void GetLensFilterSettings();

signals:
    void LensSettingsChanged( QVector<int> values );
    void LensActiveChanged( bool active );

    void LensFocusPositionChanged( int pos );
    void LensZoomPositionChanged( int pos );
    void LensIrisPositionChanged( int pos );
    void LensFilterPositionChanged( int pos );

    void LensFocusSettingsChanged( QVector<int> values );
    void LensZoomSettingsChanged( QVector<int> values );
    void LensIrisSettingsChanged( QVector<int> values );
    void LensFilterSettingsChanged( QVector<int> values );

public slots:
    void onLensSettingsChange( QVector<int> values );
    void onLensActiveChange( bool active );

    void onLensFocusPositionChange( int pos );
    void onLensZoomPositionChange( int pos );
    void onLensIrisPositionChange( int pos );
    void onLensFilterPositionChange( int pos );

    void onLensFocusSettingsChange( QVector<int> values );
    void onLensZoomSettingsChange( QVector<int> values );
    void onLensIrisSettingsChange( QVector<int> values );
    void onLensFilterSettingsChange( QVector<int> values );

    void onSmallResyncRequest(void);
};

#endif // _LENS_INTERFACE_H_



