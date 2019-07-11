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
    GetLensInvert();

    GetLensFocusPosition();
    GetLensFineFocus();
    GetLensZoomPosition();
    GetLensIrisPosition();
    GetLensFilterPosition();

    GetLensIrisSetup();

    GetLensFocusSettings();
    GetLensZoomSettings();
    GetLensIrisSettings();
    GetLensFilterSettings();
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


       bool active;

       if ( value == 1 && res == 0)
       {
           active = true;
       }
       else
       {
            active = false;
       }

        // emit a IrisSetupChanged signal
        emit LensActiveChanged( active );

        HANDLE_ERROR( res );
    }
}

/******************************************************************************
 * LensItf::GetLensSettings
 *****************************************************************************/
void LensItf::GetLensInvert()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensInvertChanged(QVector<int>))) > 0 )
    {
        int32_t value[NO_VALUES_LENS_INVERT];

        // get lens invert values from device
        int res = ctrl_protocol_get_lens_invert( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_INVERT, &value[0] );
        HANDLE_ERROR( res );

        QVector<int> v_values( NO_VALUES_LENS_INVERT );
        for ( int i = 0; i < NO_VALUES_LENS_INVERT; i++ ) {
            v_values[i] = value[i];
        }

        // emit a LensInvertChanged signal
        emit LensInvertChanged( v_values );
    }
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
 * LensItf::GetLensFineFocus
 *****************************************************************************/
void LensItf::GetLensFineFocus()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensFocusFineChanged(bool))) > 0 )
    {
        int32_t value;

        // get lens settings from device
        int res = ctrl_protocol_get_lens_fine_focus( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );



        // emit a LensFineFocusChanged signal
        emit LensFocusFineChanged( value );
    }
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
 * LensItf::GetLensIrisAperture
 *****************************************************************************/
void LensItf::GetLensIrisAperture()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensIrisApertureChanged(int))) > 0 )
    {
        int32_t value;

        // get lens settings from device
        int res = ctrl_protocol_get_lens_iris_aperture( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );



        // emit a IrisSetupChanged signal
        emit LensIrisApertureChanged( value );
    }
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
 * LensItf::GetLensFocusSettings
 *****************************************************************************/
void LensItf::GetLensFocusSettings()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensFocusSettingsChanged(QVector<int>))) > 0 )
    {
        int32_t value[NO_VALUES_LENS_FOCUS_SETTINGS];

        // get lens settings from device
        int res = ctrl_protocol_get_lens_focus_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_FOCUS_SETTINGS, &value[0] );
        HANDLE_ERROR( res );

        QVector<int> qValues;

        qValues.append(value[0]);
        qValues.append(value[1]);
        qValues.append(value[2]);


        // emit a IrisSetupChanged signal
        emit LensFocusSettingsChanged( qValues );
    }
}



/******************************************************************************
 * LensItf::GetLensZoomSettings
 *****************************************************************************/
void LensItf::GetLensZoomSettings()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensZoomSettingsChanged(QVector<int>))) > 0 )
    {
        int32_t value[NO_VALUES_LENS_ZOOM_SETTINGS];

        // get lens settings from device
        int res = ctrl_protocol_get_lens_zoom_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_ZOOM_SETTINGS, &value[0] );
        HANDLE_ERROR( res );

        QVector<int> qValues;

        qValues.append(value[0]);
        qValues.append(value[1]);
        qValues.append(value[2]);

        // emit a IrisSetupChanged signal
        emit LensZoomSettingsChanged( qValues );
    }
}



/******************************************************************************
 * LensItf::GetLensIrisSettings
 *****************************************************************************/
void LensItf::GetLensIrisSettings()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensIrisSettingsChanged(QVector<int>))) > 0 )
    {
        int32_t value[NO_VALUES_LENS_IRIS_SETTINGS];

        // get lens settings from device
        int res = ctrl_protocol_get_lens_iris_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_IRIS_SETTINGS, &value[0] );
        HANDLE_ERROR( res );

        QVector<int> qValues;

        qValues.append(value[0]);
        qValues.append(value[1]);
        qValues.append(value[2]);

        // emit a IrisSetupChanged signal
        emit LensIrisSettingsChanged( qValues );
    }
}

/******************************************************************************
 * LensItf::GetLensIrisSetup
 *****************************************************************************/
void LensItf::GetLensIrisSetup()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensIrisSetupChanged(QVector<int>))) > 0 )
    {
        int32_t value[NO_VALUES_LENS_IRIS_SETUP];

        // get lens settings from device
        int res = ctrl_protocol_get_lens_iris_setup( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_IRIS_SETUP, &value[0] );
        HANDLE_ERROR( res );

        QVector<int> qValues;

        for( int i = 0; i < NO_VALUES_LENS_IRIS_SETUP; i++)
        {
            qValues.append( value[i]);
        }


        // emit a IrisSetupChanged signal
        emit LensIrisSetupChanged( qValues );
    }
}


/******************************************************************************
 * LensItf::GetLensFilterSettings
 *****************************************************************************/
void LensItf::GetLensFilterSettings()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(LensFilterSettingsChanged(QVector<int>))) > 0 )
    {
        int32_t value[NO_VALUES_LENS_FILTER_SETTINGS];

        // get lens settings from device
        int res = ctrl_protocol_get_lens_filter_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_FILTER_SETTINGS, &value[0] );
        HANDLE_ERROR( res );

        QVector<int> qValues;

        qValues.append(value[0]);
        qValues.append(value[1]);
        qValues.append(value[2]);

        // emit a IrisSetupChanged signal
        emit LensFilterSettingsChanged( qValues );
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
            GET_CHANNEL_INSTANCE(this), value
    );

    if(res != 0)
    {
    // emit signal for indicating that lens could not connected
         emit LensActiveChanged( false );
    }
    else {
         emit LensActiveChanged( true );
    }

    HANDLE_ERROR( res );
}

/******************************************************************************
 * LensItf::onLensInvertChange
 *****************************************************************************/
void LensItf::onLensInvertChange( QVector<int> values )
{
    int value_array[NO_VALUES_LENS_INVERT];
    for ( int i = 0; i < NO_VALUES_LENS_INVERT; i++ ) {
        value_array[i] = values[i];
    }

    int res = ctrl_protocol_set_lens_invert( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_INVERT, &value_array[0]
    );

    HANDLE_ERROR( res );
}

/******************************************************************************
 * LensItf::onLensFocusPositionChange
 *****************************************************************************/
void LensItf::onLensFocusPositionChange( int value )
{


    int res = ctrl_protocol_set_lens_focus_position( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), value );

    HANDLE_ERROR( res );
}

/******************************************************************************
 * LensItf::onLensFineFocusChange
 *****************************************************************************/
void LensItf::onLensFocusFineChange( bool en )
{
    int value;

    if ( en == true)
    {
        value = 1;
    }
    else
    {
        value = 0;
    }

    int res = ctrl_protocol_set_lens_fine_focus( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), value
    );


    HANDLE_ERROR( res );
}

/******************************************************************************
 * LensItf::onLensZoomPositionChange
 *****************************************************************************/
void LensItf::onLensZoomPositionChange( int value )
{


    int res = ctrl_protocol_set_lens_zoom_position( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), value );

    HANDLE_ERROR( res );
}

/******************************************************************************
 * LensItf::onLensIrisPositionChange
 *****************************************************************************/
void LensItf::onLensIrisPositionChange( int value )
{


    int res = ctrl_protocol_set_lens_iris_position( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), value );

    HANDLE_ERROR( res );
}

/******************************************************************************
 * LensItf::onLensIrisApertureChange
 *****************************************************************************/
void LensItf::onLensIrisApertureChange( int value )
{


    int res = ctrl_protocol_set_lens_iris_aperture( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), value );

    HANDLE_ERROR( res );
}


/******************************************************************************
 * LensItf::onLensFilterPositionChange
 *****************************************************************************/
void LensItf::onLensFilterPositionChange( int value )
{


    int res = ctrl_protocol_set_lens_filter_position( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), value );

    HANDLE_ERROR( res );
}

/******************************************************************************
 * LensItf::onLensFocusSettingsChange
 *****************************************************************************/
void LensItf::onLensFocusSettingsChange( QVector<int> values )
{
    int value_array[NO_VALUES_LENS_FOCUS_SETTINGS];

    value_array[0] = values[0];
    value_array[1] = values[1];
    value_array[2] = values[2];



    int res = ctrl_protocol_set_lens_focus_settings( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_FOCUS_SETTINGS, &value_array[0]
    );

    HANDLE_ERROR( res );
}

/******************************************************************************
 * LensItf::onLensZoomSettingsChange
 *****************************************************************************/
void LensItf::onLensZoomSettingsChange( QVector<int> values )
{
    int value_array[NO_VALUES_LENS_ZOOM_SETTINGS];

    value_array[0] = values[0];
    value_array[1] = values[1];
    value_array[2] = values[2];



    int res = ctrl_protocol_set_lens_zoom_settings( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_ZOOM_SETTINGS, &value_array[0]
    );

    HANDLE_ERROR( res );
}

/******************************************************************************
 * LensItf::onLensIrisSettingsChange
 *****************************************************************************/
void LensItf::onLensIrisSettingsChange( QVector<int> values )
{
    int value_array[NO_VALUES_LENS_IRIS_SETTINGS];

    value_array[0] = values[0];
    value_array[1] = values[1];
    value_array[2] = values[2];



    int res = ctrl_protocol_set_lens_iris_settings( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_IRIS_SETTINGS, &value_array[0]
    );

    HANDLE_ERROR( res );
}

/******************************************************************************
 * LensItf::onLensIrisSetupChange
 *****************************************************************************/
void LensItf::onLensIrisSetupChange( QVector<int> values )
{
    int value_array[NO_VALUES_LENS_IRIS_SETUP];
    for ( int i = 0; i < NO_VALUES_LENS_IRIS_SETUP; i++ ) {
        value_array[i] = values[i];
    }

    int res = ctrl_protocol_set_lens_iris_setup( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_IRIS_SETUP, &value_array[0]
    );

    HANDLE_ERROR( res );
}


/******************************************************************************
 * LensItf::onLensFilterSettingsChange
 *****************************************************************************/
void LensItf::onLensFilterSettingsChange( QVector<int> values )
{
    int value_array[NO_VALUES_LENS_FILTER_SETTINGS];

    value_array[0] = values[0];
    value_array[1] = values[1];
    value_array[2] = values[2];



    int res = ctrl_protocol_set_lens_filter_settings( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), NO_VALUES_LENS_FILTER_SETTINGS, &value_array[0]
    );

    HANDLE_ERROR( res );
}

/******************************************************************************
 * LensItf::onSmallResyncRequest
 *****************************************************************************/
void LensItf::onSmallResyncRequest( void )
{
    GetLensFocusPosition();
    GetLensZoomPosition();
    GetLensIrisPosition();
    GetLensFilterPosition();

    GetLensFocusSettings();
    GetLensZoomSettings();
    GetLensIrisSettings();
    GetLensFilterSettings();
}





