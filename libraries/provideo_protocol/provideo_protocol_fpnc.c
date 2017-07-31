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
 * @file    provideo_protocol_fpnc.c
 *
 * @brief   Implementation of provideo protocol fpnc functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_fpnc.h>

#include <provideo_protocol/provideo_protocol_common.h>

/******************************************************************************
 * @brief command "fpnc"
 *****************************************************************************/
#define CMD_GET_FPNC                        ( "fpnc\n" )
#define CMD_SET_FPNC                        ( "fpnc %i\n" )
#define CMD_SYNC_FPNC                       ( "fpnc " )
#define CMD_GET_FPNC_NO_PARMS               ( 1 )

/******************************************************************************
 * @brief command "fpnc_inv_gains"
 *****************************************************************************/
#define CMD_GET_FPNC_INV_GAINS              ( "fpnc_inv_gains\n" )
#define CMD_SET_FPNC_INV_GAINS              ( "fpnc_inv_gains %i %i %i %i\n" )
#define CMD_SYNC_FPNC_INV_GAINS             ( "fpnc_inv_gains " )
#define CMD_GET_FPNC_INV_GAINS_NO_PARMS     ( 4 )

/******************************************************************************
 * @brief command "fpnc_gains"
 *****************************************************************************/
#define CMD_GET_FPNC_GAINS                  ( "fpnc_gains\n" )
#define CMD_SET_FPNC_GAINS                  ( "fpnc_gains %i %i %i %i\n" )
#define CMD_SYNC_FPNC_GAINS                 ( "fpnc_gains " )
#define CMD_GET_FPNC_GAINS_NO_PARMS         ( 4 )

/******************************************************************************
 * @brief command "fpnc_calibrate"
 *****************************************************************************/
#define CMD_FPNC_CALIBRATE                  ( "fpnc_calibrate\n" )

/******************************************************************************
 * @brief command "fpnc_dump"
 *****************************************************************************/
#define CMD_GET_FPNC_DUMP                   ( "fpnc_dump %i\n" )
#define CMD_GET_FPNC_DUMP_LINE              ( "%i: %i %i %i %i" )
#define CMD_GET_FPNC_DUMP_NO_PARMS          ( 5 )

/******************************************************************************
 * @brief command "fpnc_get_values"
 *****************************************************************************/
#define CMD_GET_FPNC_VALUE                  ( "fpnc_get_values %i %i %i\n" )
#define CMD_GET_FPNC_VALUE_LINE             ( "fpnc_get_values %i %i %i %i\n" )
#define CMD_SYNC_FPNC_VALUE                 ( "fpnc_get_values " )
#define CMD_GET_FPNC_VALUE_LINE_NO_PARMS    ( 4 )

/******************************************************************************
 * @brief command "fpnc_set_values"
 *****************************************************************************/
#define CMD_SET_FPNC_VALUE                  ( "fpnc_set_values %i %i %i 0x%08x 0x%08x 0x%08x 0x%08x\n" )

/******************************************************************************
 * @brief command "fpnc_save"
 *****************************************************************************/
#define CMD_FPNC_SAVE                       ( "fpnc_save %i\n" )

/******************************************************************************
 * @brief command "fpnc_load"
 *****************************************************************************/
#define CMD_FPNC_LOAD                       ( "fpnc_load %i\n" )

/******************************************************************************
 * get_fpnc_enable - Get enable status of fix-pattern correction
 *****************************************************************************/
static int get_fpnc_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             flag
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !flag )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_FPNC, CMD_SYNC_FPNC, CMD_SET_FPNC, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_FPNC_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *flag = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_fpnc_enable - Set enable status of fix-pattern correction
 *****************************************************************************/
static int set_fpnc_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               flag
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_FPNC, INT( flag ) ) );
}

/******************************************************************************
 * get_fpnc_inv_gains - Get inverse gains of fix-pattern correction
 *****************************************************************************/
static int get_fpnc_inv_gains
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    int v[CMD_GET_FPNC_INV_GAINS_NO_PARMS];
    int res;

    ctrl_protocol_fpnc_inv_gains_t * inv_gains;

    if ( !no || !values || (no != sizeof(*inv_gains)) )
    {
        return ( -EINVAL );
    }

    // command call to get parameters from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_FPNC_INV_GAINS,
            CMD_SYNC_FPNC_INV_GAINS, CMD_SET_FPNC_INV_GAINS, &v[0u], &v[1u], &v[2u], &v[3] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_FPNC_INV_GAINS_NO_PARMS )
    {
        return ( -EFAULT );
    }


    inv_gains = (ctrl_protocol_fpnc_inv_gains_t *)values;
    inv_gains->inv_gain_0 = UINT16( v[0] );
    inv_gains->inv_gain_1 = UINT16( v[1] );
    inv_gains->inv_gain_2 = UINT16( v[2] );
    inv_gains->inv_gain_3 = UINT16( v[3] );
    
    return ( 0 );
}

/******************************************************************************
 * set_fpnc_inv_gains - Set inverse gains of fix-pattern correction
 *****************************************************************************/
static int set_fpnc_inv_gains
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    ctrl_protocol_fpnc_inv_gains_t * inv_gains;

    if ( !no || !values || (no != sizeof(*inv_gains)) )
    {
        return ( -EINVAL );
    }
    
    inv_gains = (ctrl_protocol_fpnc_inv_gains_t *)values;

    return ( set_param_int_X( channel, CMD_SET_FPNC_INV_GAINS,
                INT( inv_gains->inv_gain_0 ),
                INT( inv_gains->inv_gain_1 ),
                INT( inv_gains->inv_gain_2 ),
                INT( inv_gains->inv_gain_3 )) );
}

/******************************************************************************
 * get_fpnc_gains - Get gains of fix-pattern correction
 *****************************************************************************/
static int get_fpnc_gains
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    int v[CMD_GET_FPNC_GAINS_NO_PARMS];
    int res;

    ctrl_protocol_fpnc_gains_t * gains;

    if ( !no || !values || (no != sizeof(*gains)) )
    {
        return ( -EINVAL );
    }

    // command call to get parameters from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_FPNC_GAINS,
            CMD_SYNC_FPNC_GAINS, CMD_SET_FPNC_GAINS, &v[0u], &v[1u], &v[2u], &v[3] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_FPNC_GAINS_NO_PARMS )
    {
        return ( -EFAULT );
    }

    gains = (ctrl_protocol_fpnc_gains_t *)values;
    gains->gain_0 = UINT16( v[0] );
    gains->gain_1 = UINT16( v[1] );
    gains->gain_2 = UINT16( v[2] );
    gains->gain_3 = UINT16( v[3] );

    return ( 0 );
}

/******************************************************************************
 * set_fpnc_gains - Set gains of fix-pattern correction
 *****************************************************************************/
static int set_fpnc_gains
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    ctrl_protocol_fpnc_gains_t * gains;

    if ( !no || !values || (no != sizeof(*gains)) )
    {
        return ( -EINVAL );
    }
    
    gains = (ctrl_protocol_fpnc_gains_t *)values;

    return ( set_param_int_X( channel, CMD_SET_FPNC_GAINS,
                INT( gains->gain_0 ),
                INT( gains->gain_1 ),
                INT( gains->gain_2 ),
                INT( gains->gain_3 )) );
}

/******************************************************************************
 * set_fpnc_calibrate - do an fpnc calibration on target
 *****************************************************************************/
static int set_fpnc_calibrate
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;
    
    return ( set_param_0( channel, CMD_FPNC_CALIBRATE ) );
}

#if 0
/******************************************************************************
 * get_fpnc_dump - dumps the fpnc values and write into string
 *****************************************************************************/
int get_fpnc_dump
(
    cmd_itf_handle_t const  com,
    uint8_t const           pg,
    fpnc_line_t             values
)
{
    char command[CMD_SINGLE_LINE_COMMAND_SIZE];
    char data[FPNC_IMAGE_WIDTH/CMD_GET_FPNC_DUMP_NO_PARMS * CMD_SINGLE_LINE_RESPONSE_SIZE];

    int res;

    // parameter check
    if ( !values )
    {
        return ( -EINVAL );
    }
    
    // clear command buffer
    memset( command, 0, sizeof(command) );

    // create command to send
    sprintf( command, CMD_GET_FPNC_DUMP, pg );

    // send get-command to COM port
    cmd_itf_send( com, command, strlen(command) );

    // read response from provideo device
    res = evaluate_get_response( com, data, sizeof(data) );
    if ( !res )
    {
        unsigned i = 0u;
        char * line = strtok( strdup(data), "\n");
        while ( line )
        {
            uint32_t adr = 0u;
            uint32_t fpnc0 = 0u;
            uint32_t fpnc1 = 0u;
            uint32_t fpnc2 = 0u;
            uint32_t fpnc3 = 0u;

            int res = sscanf( line, CMD_GET_FPNC_DUMP_LINE, &adr, &fpnc0, &fpnc1, &fpnc2, &fpnc3);
            if ( res == CMD_GET_FPNC_DUMP_NO_PARMS )
            {
                values[i++] = FPNC_SAMPLE_TYPE(fpnc0);
                values[i++] = FPNC_SAMPLE_TYPE(fpnc1);
                values[i++] = FPNC_SAMPLE_TYPE(fpnc2);
                values[i++] = FPNC_SAMPLE_TYPE(fpnc3);
            }
            line  = strtok( NULL, "\n" );
        }
    }
    else
    {
        res = evaluate_error_response( data, res );
    }

    return ( res );
}
#endif

/******************************************************************************
 * get_fpnc_correction_data - read correction data
 *****************************************************************************/
static int get_fpnc_correction_data
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;
    
    char command[CMD_SINGLE_LINE_COMMAND_SIZE];
    char data[(2*CMD_SINGLE_LINE_RESPONSE_SIZE)];

    ctrl_protocol_fpnc_data_t * v;
    int res;

    if ( !no || !values || (no != sizeof(*v)) )
    {
        return ( -EINVAL );
    }
    
    v = (ctrl_protocol_fpnc_data_t *)values;

    // clear command buffer
    memset( command, 0, sizeof(command) );

    // create command to send
    sprintf( command, CMD_GET_FPNC_VALUE, v->page, v->column, v->offset );
    
    // send get-command to control channel
    ctrl_channel_send_request( channel, (uint8_t *)command, strlen(command) );

    // read response from provideo device
    res = evaluate_get_response( channel, data, sizeof(data) );
    if ( !res )
    {
        char * s = strstr( data, CMD_SYNC_FPNC_VALUE );
        if ( s )
        {
            uint32_t v0 = 0u;
            uint32_t v1 = 0u;
            uint32_t v2 = 0u;
            uint32_t v3 = 0u;

            res = sscanf( s, CMD_GET_FPNC_VALUE_LINE, &v0, &v1, &v2, &v3 );
            if ( res == CMD_GET_FPNC_VALUE_LINE_NO_PARMS )
            {
                v->v0 = UINT32( v0 );
                v->v1 = UINT32( v1 );
                v->v2 = UINT32( v2 );
                v->v3 = UINT32( v3 );
                return ( 0 );
            }
            else
            {
                return ( -EFAULT );
            }
        }
        else
        {
            return ( -EFAULT );
        }
    }
    else
    {
        res = evaluate_error_response( data, res );
    }

    return ( 0 );
}

/******************************************************************************
 * set_fpnc_correction_data - write correction data
 *****************************************************************************/
static int set_fpnc_correction_data
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    ctrl_protocol_fpnc_data_t * v;

    if ( !no || !values || (no != sizeof(*v)) )
    {
        return ( -EINVAL );
    }
    
    v = (ctrl_protocol_fpnc_data_t *)values;

    return ( set_param_int_X( channel, CMD_SET_FPNC_VALUE,
                INT( v->page ), INT( v->column ), INT( v->offset ),
                INT( v->v0 ), INT( v->v1 ), INT( v->v2 ), INT( v->v3 )) );
}

/******************************************************************************
 * ISP protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_fpnc_drv_t provideo_fpnc_drv = 
{
    .get_fpnc_enable            = get_fpnc_enable,
    .set_fpnc_enable            = set_fpnc_enable,
    .get_fpnc_inv_gains         = get_fpnc_inv_gains,
    .set_fpnc_inv_gains         = set_fpnc_inv_gains,
    .get_fpnc_gains             = get_fpnc_gains,
    .set_fpnc_gains             = set_fpnc_gains,
    .set_fpnc_calibrate         = set_fpnc_calibrate,
    .get_fpnc_correction_data   = get_fpnc_correction_data,
    .set_fpnc_correction_data   = set_fpnc_correction_data,
};

/******************************************************************************
 * provideo_protocol_fpnc_init
 *****************************************************************************/
int provideo_protocol_fpnc_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_fpnc_register( handle, ctx, &provideo_fpnc_drv ) );
}

