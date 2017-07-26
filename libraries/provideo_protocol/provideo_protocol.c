/******************************************************************************
 *
 * Copyright 2016, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
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

#include "provideo_protocol_common.h"
#include "provideo_protocol_system.h"
#include "provideo_protocol_cam.h"
#include "provideo_protocol_isp.h"
#include "provideo_protocol_cproc.h"
#include "provideo_protocol_auto.h"
#include "provideo_protocol_mcc.h"
#include "provideo_protocol_chain.h"
#include "provideo_protocol_lut.h"
#include "provideo_protocol_tflt.h"
#include "provideo_protocol_fpnc.h"
#include "provideo_protocol_playback.h"
#include "provideo_protocol_osd.h"
#include "provideo_protocol_iris.h"
#include "provideo_protocol_knee.h"
#include "provideo_protocol_dpcc.h"

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
 
    res = provideo_protocol_knee_init( handle );
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

