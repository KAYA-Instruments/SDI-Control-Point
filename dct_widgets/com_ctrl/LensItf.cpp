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
    GetLensActive();

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
 * LensItf::onLensSettingsChange
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


/******************************************************************************
 * LensItf::GetLensActive
 *****************************************************************************/
void LensItf::GetLensActive()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensActiveChanged(bool))) > 0 )
    {
        int32_t value;

        // get lens settings from device
        int res = ctrl_protocol_get_lens_active( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );

       bool active;

       if ( value == 1)
       {
           active = true;
       }
       else
       {
            active = false;
       }

        // emit a IrisSetupChanged signal
        emit LensActiveChanged( active );
    }
}

/******************************************************************************
 * LensItf::onLensActiveChange
 *****************************************************************************/
void LensItf::onLensActiveChange( bool active )
{
    int value;

    if ( active == true)
    {
        value = 1;
    }
    else
    {
        value = 0;
    }

    int res = ctrl_protocol_set_lens_active( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value
    );

    HANDLE_ERROR( res );
}

/******************************************************************************
 * LensItf::GetLensFocusPosition
 *****************************************************************************/
void LensItf::GetLensFocusPosition()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensFocusPositionChanged(int))) > 0 )
    {
        int32_t value;

        // get lens settings from device
        int res = ctrl_protocol_get_lens_focus_position( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );



        // emit a IrisSetupChanged signal
        emit LensFocusPositionChanged( value );
    }
}

/******************************************************************************
 * LensItf::onLensFocusPositionChange
 *****************************************************************************/
void LensItf::onLensFocusPositionChange( int value )
{


    int res = ctrl_protocol_set_lens_focus_position( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );

    HANDLE_ERROR( res );
}


/******************************************************************************
 * LensItf::GetLensZoomPosition
 *****************************************************************************/
void LensItf::GetLensZoomPosition()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensZoomPositionChanged(int))) > 0 )
    {
        int32_t value;

        // get lens settings from device
        int res = ctrl_protocol_get_lens_zoom_position( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );



        // emit a IrisSetupChanged signal
        emit LensZoomPositionChanged( value );
    }
}

/******************************************************************************
 * LensItf::onLensZoomPositionChange
 *****************************************************************************/
void LensItf::onLensZoomPositionChange( int value )
{


    int res = ctrl_protocol_set_lens_zoom_position( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );

    HANDLE_ERROR( res );
}


/******************************************************************************
 * LensItf::GetLensIrisPosition
 *****************************************************************************/
void LensItf::GetLensIrisPosition()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensIrisPositionChanged(int))) > 0 )
    {
        int32_t value;

        // get lens settings from device
        int res = ctrl_protocol_get_lens_iris_position( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );



        // emit a IrisSetupChanged signal
        emit LensIrisPositionChanged( value );
    }
}

/******************************************************************************
 * LensItf::onLensIrisPositionChange
 *****************************************************************************/
void LensItf::onLensIrisPositionChange( int value )
{


    int res = ctrl_protocol_set_lens_iris_position( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );

    HANDLE_ERROR( res );
}


/******************************************************************************
 * LensItf::GetLensFilterPosition
 *****************************************************************************/
void LensItf::GetLensFilterPosition()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensFilterPositionChanged(int))) > 0 )
    {
        int32_t value;

        // get lens settings from device
        int res = ctrl_protocol_get_lens_filter_position( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );



        // emit a IrisSetupChanged signal
        emit LensFilterPositionChanged( value );
    }
}

/******************************************************************************
 * LensItf::onLensFilterPositionChange
 *****************************************************************************/
void LensItf::onLensFilterPositionChange( int value )
{


    int res = ctrl_protocol_set_lens_filter_position( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );

    HANDLE_ERROR( res );
}


/******************************************************************************
 * LensItf::GetLensFocusSettings
 *****************************************************************************/
void LensItf::GetLensFocusSettings()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensFocusSettingsChanged(int speed, int stepMode, int torque))) > 0 )
    {
        int32_t value[NO_VALUES_LENS_FOCUS_SETTINGS];

        // get lens settings from device
        int res = ctrl_protocol_get_lens_focus_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_FOCUS_SETTINGS, &value[0] );
        HANDLE_ERROR( res );



        // emit a IrisSetupChanged signal
        emit LensFocusSettingsChanged( value[0],value[1],value[2] );
    }
}

/******************************************************************************
 * LensItf::onLensFocusSettingsChange
 *****************************************************************************/
void LensItf::onLensFocusSettingsChange( int speed, int stepMode, int torque )
{
    int value_array[NO_VALUES_LENS_FOCUS_SETTINGS];

    value_array[0] = speed;
    value_array[1] =stepMode;
    value_array[2] = torque;



    int res = ctrl_protocol_set_lens_focus_settings( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_FOCUS_SETTINGS, &value_array[0]
    );

    HANDLE_ERROR( res );
}

/******************************************************************************
 * LensItf::GetLensZoomSettings
 *****************************************************************************/
void LensItf::GetLensZoomSettings()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensZoomSettingsChanged(int speed, int stepMode, int torque))) > 0 )
    {
        int32_t value[NO_VALUES_LENS_ZOOM_SETTINGS];

        // get lens settings from device
        int res = ctrl_protocol_get_lens_zoom_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_ZOOM_SETTINGS, &value[0] );
        HANDLE_ERROR( res );



        // emit a IrisSetupChanged signal
        emit LensZoomSettingsChanged( value[0],value[1],value[2] );
    }
}

/******************************************************************************
 * LensItf::onLensZoomSettingsChange
 *****************************************************************************/
void LensItf::onLensZoomSettingsChange( int speed, int stepMode, int torque )
{
    int value_array[NO_VALUES_LENS_ZOOM_SETTINGS];

    value_array[0] = speed;
    value_array[1] =stepMode;
    value_array[2] = torque;



    int res = ctrl_protocol_set_lens_zoom_settings( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_ZOOM_SETTINGS, &value_array[0]
    );

    HANDLE_ERROR( res );
}

/******************************************************************************
 * LensItf::GetLensIrisSettings
 *****************************************************************************/
void LensItf::GetLensIrisSettings()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensIrisSettingsChanged(int speed, int stepMode, int torque))) > 0 )
    {
        int32_t value[NO_VALUES_LENS_IRIS_SETTINGS];

        // get lens settings from device
        int res = ctrl_protocol_get_lens_iris_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_IRIS_SETTINGS, &value[0] );
        HANDLE_ERROR( res );



        // emit a IrisSetupChanged signal
        emit LensIrisSettingsChanged( value[0],value[1],value[2] );
    }
}

/******************************************************************************
 * LensItf::onLensIrisSettingsChange
 *****************************************************************************/
void LensItf::onLensIrisSettingsChange( int speed, int stepMode, int torque )
{
    int value_array[NO_VALUES_LENS_IRIS_SETTINGS];

    value_array[0] = speed;
    value_array[1] =stepMode;
    value_array[2] = torque;



    int res = ctrl_protocol_set_lens_iris_settings( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_IRIS_SETTINGS, &value_array[0]
    );

    HANDLE_ERROR( res );
}

/******************************************************************************
 * LensItf::GetLensFilterSettings
 *****************************************************************************/
void LensItf::GetLensFilterSettings()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensFilterSettingsChanged(int speed, int stepMode, int torque))) > 0 )
    {
        int32_t value[NO_VALUES_LENS_FILTER_SETTINGS];

        // get lens settings from device
        int res = ctrl_protocol_get_lens_filter_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_FILTER_SETTINGS, &value[0] );
        HANDLE_ERROR( res );



        // emit a IrisSetupChanged signal
        emit LensFilterSettingsChanged( value[0],value[1],value[2] );
    }
}

/******************************************************************************
 * LensItf::onLensFilterSettingsChange
 *****************************************************************************/
void LensItf::onLensFilterSettingsChange( int speed, int stepMode, int torque )
{
    int value_array[NO_VALUES_LENS_FILTER_SETTINGS];

    value_array[0] = speed;
    value_array[1] =stepMode;
    value_array[2] = torque;



    int res = ctrl_protocol_set_lens_filter_settings( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_FILTER_SETTINGS, &value_array[0]
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

