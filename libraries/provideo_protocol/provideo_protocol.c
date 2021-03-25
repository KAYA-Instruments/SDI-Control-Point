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
 * @file    provideo_protocol.c
 *
 * @brief   Implementation of provideo protocol
 *
 *****************************************************************************/
#include <stdio.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>

#include <provideo_protocol/provideo_protocol_common.h>
#include <provideo_protocol/provideo_protocol_system.h>
#include <provideo_protocol/provideo_protocol_cam.h>
#include <provideo_protocol/provideo_protocol_isp.h>
#include <provideo_protocol/provideo_protocol_cproc.h>
#include <provideo_protocol/provideo_protocol_auto.h>
#include <provideo_protocol/provideo_protocol_mcc.h>
#include <provideo_protocol/provideo_protocol_chain.h>
#include <provideo_protocol/provideo_protocol_lut.h>
#include <provideo_protocol/provideo_protocol_tflt.h>
#include <provideo_protocol/provideo_protocol_fpnc.h>
#include <provideo_protocol/provideo_protocol_playback.h>
#include <provideo_protocol/provideo_protocol_osd.h>
#include <provideo_protocol/provideo_protocol_iris.h>
#include <provideo_protocol/provideo_protocol_lens.h>
#include <provideo_protocol/provideo_protocol_knee.h>
#include <provideo_protocol/provideo_protocol_roi.h>
#include <provideo_protocol/provideo_protocol_dpcc.h>

/******************************************************************************
 * provideo_protocol_init
 *****************************************************************************/
int provideo_protocol_init
(
    ctrl_protocol_handle_t const handle
)
{
    int res = provideo_protocol_sys_init( handle );
    if ( res )
    {
        return ( res );
    }

    res = provideo_protocol_isp_init( handle );
    if ( res )
    {
        return ( res );
    }
    
    res = provideo_protocol_cproc_init( handle );
    if ( res )
    {
        return ( res );
    }

    res = provideo_protocol_auto_init( handle );
    if ( res )
    {
        return ( res );
    }

    res = provideo_protocol_cam_init( handle );
    if ( res )
    {
        return ( res );
    }

    res = provideo_protocol_mcc_init( handle );
    if ( res )
    {
        return ( res );
    }

    res = provideo_protocol_chain_init( handle );
    if ( res )
    {
        return ( res );
    }

    res = provideo_protocol_lut_init( handle );
    if ( res )
    {
        return ( res );
    }

    res = provideo_protocol_tflt_init( handle );
    if ( res )
    {
        return ( res );
    }

    res = provideo_protocol_fpnc_init( handle );
    if ( res )
    {
        return ( res );
    }

    res = provideo_protocol_playback_init( handle );
    if ( res )
    {
        return ( res );
    }

    res = provideo_protocol_osd_init( handle );
    if ( res )
    {
        return ( res );
    }

    res = provideo_protocol_iris_init( handle );
    if ( res )
    {
        return ( res );
    }

    res = provideo_protocol_lens_init( handle );
    if ( res )
    {
        return ( res );
    }
 
    res = provideo_protocol_knee_init( handle );
    if ( res )
    {
        return ( res );
    }

    res = provideo_protocol_roi_init( handle );
    if ( res )
    {
        return ( res );
    }

    res = provideo_protocol_dpcc_init( handle );
    if ( res )
    {
        return ( res );
    }

    return ( 0 );
}

