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
 * @file    ROIItf.cpp
 *
 * @brief   Implementation of ROI Interface
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <ctrl_protocol/ctrl_protocol_roi.h>

#include "common.h"
#include "ROIItf.h"

#include <QtDebug>

/******************************************************************************
 * ROIItf::resync()
 *****************************************************************************/
void ROIItf::resync()
{
    // enable status
    GetStatROIInfo();
    GetStatROI();
}

/******************************************************************************
 * ROIItf::GetStatROIInfo
 *****************************************************************************/
void ROIItf::GetStatROIInfo()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(StatROIInfoChanged(int, int, int, int))) > 0 )
    {
        ctrl_protocol_stat_roi_info_t i;

        memset( &i, 0, sizeof(i) );

        // get ROI info structure from device
        int res = ctrl_protocol_get_stat_roi_info( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(i), (uint8_t *)&i );
        HANDLE_ERROR( res );

        // emit a KneeEnableChanged signal
        emit StatROIInfoChanged(
            int(i.max_width), int(i.max_height), int(i.width_step), int(i.height_step) );
    }
}

/******************************************************************************
 * ROIItf::GetStatROI
 *****************************************************************************/
void ROIItf::GetStatROI()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(StatROIChanged(int, int, int, int))) > 0 )
    {
        ctrl_protocol_stat_roi_info_t i;

        memset( &i, 0, sizeof(i) );
    
        // get ROI info structure from device
        int res = ctrl_protocol_get_stat_roi( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(i), (uint8_t *)&i );
        HANDLE_ERROR( res );
        
        // emit a KneeEnableChanged signal
        emit StatROIChanged(
            int(i.max_width), int(i.max_height), int(i.width_step), int(i.height_step) );
    }
}

/******************************************************************************
 * ROIItf::onStatROIChange
 *****************************************************************************/
void ROIItf::onStatROIChange( int width, int height, int offset_x, int offset_y )
{
    // convert to array
    uint32_t values[4];
    values[0] = uint32_t(width);
    values[1] = uint32_t(height);
    values[2] = uint32_t(offset_x);
    values[3] = uint32_t(offset_y);

    int res = ctrl_protocol_set_stat_roi( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), 4, values );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ROIItf::onROIVideoModeChanged
 *****************************************************************************/
void ROIItf::onROIVideoModeChanged()
{
    resync();
}

