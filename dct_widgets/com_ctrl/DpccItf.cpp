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
 * @file    DpccItf.cpp
 *
 * @brief   Implementation of DPCC Interface
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <ctrl_protocol/ctrl_protocol_dpcc.h>

#include "common.h"
#include "DpccItf.h"

#include <QtDebug>
#include <QApplication>

/******************************************************************************
 * DpccItf::resync()
 *****************************************************************************/
void DpccItf::resync()
{
    // enable status
    GetDpccEnable();

    // correction mode
    GetDpccCorrectionMode();
    
    // detection level
    GetDpccDetectionLevel();

    // defect pixel table
    GetDpccTable();

    // test mode
    GetDpccTestMode();
}

/******************************************************************************
 * DpccItf::GetDpccEnable
 *****************************************************************************/
void DpccItf::GetDpccEnable()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(DpccEnableChanged(int))) > 0 )
    {
        uint8_t value;
    
        // read dpcc enable from device
        int res = ctrl_protocol_get_dpcc_enable( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &value );
        HANDLE_ERROR( res );
        
        // emit a DpccEnableChanged signal
        emit DpccEnableChanged( (int)value );
    }
}

/******************************************************************************
 * DpccItf::GetDpccCorrectionMode
 *****************************************************************************/
void DpccItf::GetDpccCorrectionMode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(DpccCorrectionModeChanged(int))) > 0 )
    {
        uint8_t mode;
    
        // read dpcc correction mode from device
        int res = ctrl_protocol_get_dpcc_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &mode );
        HANDLE_ERROR( res );
        
        // emit a DpccCorrectionModeChanged signal
        emit DpccCorrectionModeChanged( (int)mode );
    }
}

/******************************************************************************
 * DpccItf::GetDpccDetectionLevel
 *****************************************************************************/
void DpccItf::GetDpccDetectionLevel()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(DpccDetectionLevelChanged(int))) > 0 )
    {
        uint8_t level;
    
        // read dpcc correction mode from device
        int res = ctrl_protocol_get_dpcc_level( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &level );
        HANDLE_ERROR( res );
        
        // emit a DpccDetectionLevelChanged signal
        emit DpccDetectionLevelChanged(level );
    }
}

/******************************************************************************
 * DpccItf::GetDpccTable
 *****************************************************************************/
void DpccItf::GetDpccTable()
{
    if ( receivers(SIGNAL(DpccTableChanged(QVector<int>,QVector<int>))) > 0 )
    {
        uint16_t pixelX[MAX_DPCC_NO_PIXEL];
        uint16_t pixelY[MAX_DPCC_NO_PIXEL];
        ctrl_protocol_dpcc_table_t table;

        memset( pixelX, 0, sizeof(pixelX) );
        memset( pixelY, 0, sizeof(pixelY) );
        
        table.no   = 0u;
        table.size = MAX_DPCC_NO_PIXEL;
        table.x    = pixelX;
        table.y    = pixelY;

        // read defect pixel table from device
        int res = ctrl_protocol_get_dpcc_table( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), sizeof(table), (uint32_t *)&table );
        HANDLE_ERROR( res );

        QVector<int> x( table.no );
        QVector<int> y( table.no );

        for( unsigned i=0u; i<table.no; i++ )
        {
            x[i] = pixelX[i];
            y[i] = pixelY[i];
        }

        // emit a DpccTableChanged signal
        emit DpccTableChanged( x, y );
    }
}

/******************************************************************************
 * DpccItf::GetDpccTable
 *****************************************************************************/
void DpccItf::SetDpccTable(QVector<int> & x, QVector<int> & y)
{
    int res;
    uint16_t px[2u];

    // clear table on device
    res = ctrl_protocol_clear_dpcc_table( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );

    // add all pixels to the table on the device
    if (x.length() != y.length())
    {
        return;
    }

    for (int i = 0; i < x.length() && i < (int)MAX_DPCC_NO_PIXEL; i++)
    {
        px[0] = (uint16_t)x[i];
        px[1] = (uint16_t)y[i];
        res = ctrl_protocol_add_pixel( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), 2u, px );
        HANDLE_ERROR( res );
    }
}

/******************************************************************************
 * DpccItf::GetDpccTestMode
 *****************************************************************************/
void DpccItf::GetDpccTestMode()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(DpccTestModeChanged(int))) > 0 )
    {
        uint8_t mode;

        // read dpcc test mode from device
        int res = ctrl_protocol_get_dpcc_test_mode( GET_PROTOCOL_INSTANCE(this),
            GET_CHANNEL_INSTANCE(this), &mode );
        HANDLE_ERROR( res );

        // emit a DpccTestModeChanged signal
        emit DpccTestModeChanged( (int)mode );
    }
}

/******************************************************************************
 * DpccItf::onDpccEnableChange
 *****************************************************************************/
void DpccItf::onDpccEnableChange( int value )
{
    // set dpcc enable state on device
    int res = ctrl_protocol_set_dpcc_enable( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), ( value ? 1u : 0u ) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * DpccItf::onDpccCorrectionModeChange
 *****************************************************************************/
void DpccItf::onDpccCorrectionModeChange( int mode )
{
    // set dpcc correction mode on device
    int res = ctrl_protocol_set_dpcc_mode( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint8_t)mode );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * DpccItf::onDpccDetectionLevelChange
 *****************************************************************************/
void DpccItf::onDpccDetectionLevelChange( int level )
{
    // change detection level
    int res = ctrl_protocol_set_dpcc_level( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint8_t)level );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * DpccItf::onDpccAddPixel
 *****************************************************************************/
void DpccItf::onDpccAddPixel( int x, int y )
{
    uint16_t px[2u] = { (uint16_t)x, (uint16_t)y };

    // add pixel to table
    int res = ctrl_protocol_add_pixel( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), 2u, px );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * DpccItf::onDpccSetTable
 *****************************************************************************/
void DpccItf::onDpccSetTable(QVector<int> & x, QVector<int> & y)
{
    SetDpccTable(x, y);
}

/******************************************************************************
 * DpccItf::onDpccGetTable
 *****************************************************************************/
void DpccItf::onDpccGetTable()
{
    GetDpccTable();
}


/******************************************************************************
 * DpccItf::onDpccClearTable
 *****************************************************************************/
void DpccItf::onDpccClearTable()
{
    // clear table on device
    int res = ctrl_protocol_clear_dpcc_table( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * DpccItf::onDpccSaveTable
 *****************************************************************************/
void DpccItf::onDpccSaveTable()
{
    // save table in device Flash
    int res = ctrl_protocol_save_dpcc_table( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * DpccItf::onDpccLoadTable
 *****************************************************************************/
void DpccItf::onDpccLoadTable()
{
    // restore table from device Flash
    int res = ctrl_protocol_load_dpcc_table( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * DpccItf::onDpccAutoLoadTable
 *****************************************************************************/
void DpccItf::onDpccAutoLoadTable()
{
    // automatically fill table
    int res = ctrl_protocol_auto_load_dpcc_table( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * DpccItf::onDpccCorrectionModeChange
 *****************************************************************************/
void DpccItf::onDpccTestModeChange( int mode )
{
    // set dpcc test mode on device
    int res = ctrl_protocol_set_dpcc_test_mode( GET_PROTOCOL_INSTANCE(this),
        GET_CHANNEL_INSTANCE(this), (uint8_t)mode );
    HANDLE_ERROR( res );
}
