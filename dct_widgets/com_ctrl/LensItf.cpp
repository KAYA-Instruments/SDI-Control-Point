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
 * @file    LensItf.cpp
 *
 * @brief   Implementation of Lens Control Interface
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <ctrl_protocol/ctrl_protocol_lens.h>

#include "common.h"
#include "LensItf.h"

#include <QtDebug>

/******************************************************************************
 * LensItf::resync()
 *****************************************************************************/
void LensItf::resync()
{
    GetLensSettings();
//    GetLensActive();

//    GetLensFocusPosition();
//    GetLensZoomPosition();
//    GetLensIrisPosition();
//    GetLensFilterPosition();

//    GetLensFocusSettings();
//    GetLensZoomSettings();
//    GetLensIrisSettings();
//    GetLensFilterSettings();
}

/******************************************************************************
 * LensItf::GetLensSettings
 *****************************************************************************/
void LensItf::GetLensSettings()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensSettingsChanged(QVector<int>))) > 0 )
    {
        int32_t value[NO_VALUES_LENS_SETTINGS];
    
        // get lens settings from device
        int res = ctrl_protocol_get_lens_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_SETTINGS, &value[0] );
        HANDLE_ERROR( res );

        QVector<int> v_values( NO_VALUES_LENS_SETTINGS );
        for ( int i = 0; i < NO_VALUES_LENS_SETTINGS; i++ ) {
            v_values[i] = value[i];
        }

        // emit a IrisSetupChanged signal
        emit LensSettingsChanged( v_values );
    }
}

/******************************************************************************
 * LensItf::onIrisSetupChange
 *****************************************************************************/
void LensItf::onLensSettingsChange( QVector<int> values )
{
    int value_array[NO_VALUES_LENS_SETTINGS];
    for ( int i = 0; i < NO_VALUES_LENS_SETTINGS; i++ ) {
        value_array[i] = values[i];
    }

    int res = ctrl_protocol_set_lens_settings( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_SETTINGS, &value_array[0]
    );

    HANDLE_ERROR( res );
}

///******************************************************************************
// * LensItf::GetIrisApt
// *****************************************************************************/
//void LensItf::GetIrisApt()
//{
//    // Is there a signal listener
//    if ( receivers(SIGNAL(IrisAptChanged( int ))) > 0 )
//    {
//        int value;
//        int res = ctrl_protocol_get_iris_apt(
//                GET_PROTOCOL_INSTANCE(this),
//                GET_CHANNEL_INSTANCE(this),
//                &value
//        );
//        if( res ) {
//            emit IrisAptError();
//        }
//        HANDLE_ERROR( res );

//        // emit a IrisAptChanged signal
//        emit IrisAptChanged( value );
//    }
//}

//void LensItf::onIrisAptChange( int pos )
//{
//    int res = ctrl_protocol_set_iris_apt(
//            GET_PROTOCOL_INSTANCE(this),
//            GET_CHANNEL_INSTANCE(this),
//            pos
//    );
//
//    HANDLE_ERROR( res );
//}

