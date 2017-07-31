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
 * @file    provideo_protocol_auto.c
 *
 * @brief   Implementation of provideo protocol auto functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_auto.h>

#include <provideo_protocol/provideo_protocol_common.h>

/******************************************************************************
 * @brief command "aec" 
 *****************************************************************************/
#define CMD_GET_AEC_ENABLE                  ( "aec\n" )
#define CMD_SET_AEC_ENABLE                  ( "aec %i\n" )
#define CMD_SYNC_AEC_ENABLE                 ( "aec " )
#define CMD_GET_AEC_ENABLE_NO_PARAMS        ( 1 )

#define CMD_GET_AEC_SETUP                   ( "aec\n" )
#define CMD_SET_AEC_SETUP                   ( "aec %i %i %i %i %i %i %i %i\n" )
#define CMD_SYNC_AEC_SETUP                  ( "aec " )
#define CMD_GET_AEC_SETUP_NO_PARAMS         ( 8 )

/******************************************************************************
 * @brief command "stat_ae" 
 *****************************************************************************/
#define CMD_GET_STAT_AE                     ( "stat_ae\n" )
#define CMD_SET_STAT_AE                     ( "stat_ae %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i\n" )
#define CMD_SYNC_STAT_AE                    ( "stat_ae " )
#define CMD_GET_STAT_AE_NO_PARMS            ( 15 )

/******************************************************************************
 * @brief command "awb"
 *****************************************************************************/
#define CMD_GET_AWB                         ( "awb\n" )
#define CMD_SET_AWB                         ( "awb %i\n" )
#define CMD_SYNC_AWB                        ( "awb " )
#define CMD_GET_AWB_NO_PARAMS               ( 1 )

/******************************************************************************
 * @brief command "awb_speed"
 *****************************************************************************/
#define CMD_GET_AWB_SPEED                   ( "awb_speed\n" )
#define CMD_SET_AWB_SPEED                   ( "awb_speed %i\n" )
#define CMD_SYNC_AWB_SPEED                  ( "awb_speed " )
#define CMD_GET_AWB_SPEED_NO_PARAMS         ( 1 )

/******************************************************************************
 * @brief command "wb"
 *****************************************************************************/
#define CMD_WB                              ( "wb\n" )

/******************************************************************************
 * @brief command "wb_preset"
 *****************************************************************************/
#define CMD_GET_WB_PRESET                   ( "wb_preset\n" )
#define CMD_SET_WB_PRESET                   ( "wb_preset %i\n" )
#define CMD_SYNC_WB_PRESET                  ( "%i= " )
#define CMD_REGEXP_WB_PRESET                ( "%i= %[^(](%iK)\n" )

/******************************************************************************
 * @brief command "stat_exp"
 *****************************************************************************/
#define CMD_GET_STAT_EXP                    ( "stat_exp\n" )
#define CMD_SET_STAT_EXP                    ( "stat_exp %i\n" )
#define CMD_SYNC_STAT_EXP                   ( "stat_exp " )
#define CMD_GET_STAT_EXP_MASK               ( "stat_exp %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i\n" )
#define CMD_GET_STAT_EXP_NO_PARMS           ( 26 )

/******************************************************************************
 * @brief command "stat_rgb"
 *****************************************************************************/
#define CMD_GET_STAT_RGB                    ( "stat_rgb\n" )
#define CMD_SET_STAT_RGB                    ( "stat_rgb %i %i %i\n" )
#define CMD_SYNC_STAT_RGB                   ( "stat_rgb " )
#define CMD_GET_STAT_RGB_NO_PARMS           ( 3 )

/******************************************************************************
 * @brief command "stat_hist"
 *****************************************************************************/
#define CMD_GET_STAT_HIST                   ( "stat_hist\n" )
#define CMD_SET_STAT_HIST                   ( "stat_hist %i\n" )
#define CMD_SYNC_STAT_HIST                  ( "stat_hist " )
#define CMD_GET_STAT_HIST_MASK              ( "stat_hist %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i\n" )
#define CMD_GET_STAT_HIST_NO_PARMS          ( 17 )

/******************************************************************************
 * @brief command "stat_xyz"
 *****************************************************************************/
#define CMD_GET_STAT_XYZ                    ( "stat_xyz\n" )
#define CMD_SYNC_STAT_XYZ                   ( "stat_xyz " )
#define CMD_GET_STAT_XYZ_MASK               ( "stat_xyz %i %i %i\n" )
#define CMD_GET_STAT_XYZ_NO_PARMS           ( 3 )

/******************************************************************************
 * @brief command "color_xyz"
 *****************************************************************************/
#define CMD_GET_COLOR_XYZ                   ( "color_xyz\n" )
#define CMD_SYNC_COLOR_XYZ                  ( "color_xyz " )
#define CMD_GET_COLOR_XYZ_MASK              ( "color_xyz %i %i %i %i %i %i %i %i %i\n" )
#define CMD_GET_COLOR_XYZ_NO_PARMS          ( 9 )

/******************************************************************************
 * get_aec_enable - Gets AEC enable state
 *****************************************************************************/
static int get_aec_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             enable 
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !enable )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_AEC_ENABLE, CMD_SYNC_AEC_ENABLE, CMD_SET_AEC_ENABLE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_AEC_ENABLE_NO_PARAMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *enable = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_aec_enable - Sets AEC enable state
 *****************************************************************************/
static int set_aec_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               enable 
)
{
    (void) ctx;
    return ( set_param_int_X( channel, CMD_SET_AEC_ENABLE, INT( enable ) ) );
}

/******************************************************************************
 * get_stat_exposure_enable - Gets enable status of exposure statistic measurment
 *****************************************************************************/
static int get_aec_setup
(
    void * const                ctx,
    ctrl_channel_handle_t const channel, 
    int const                   no, 
    int32_t * const             values
)
{
    (void) ctx;
    (void) no;

    int res;

    // parameter check
    if ( !values )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_AEC_SETUP,
            CMD_SYNC_AEC_SETUP, CMD_SET_AEC_SETUP,
            &values[0], &values[1], &values[2], &values[3],
            &values[4], &values[5], &values[6], &values[7] ); 

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_AEC_SETUP_NO_PARAMS )
    {
        return ( -EFAULT );
    }

    return ( 0 );
}

/******************************************************************************
 * set_aec_setup - Sets enable status of exposure statistic measurment
 *****************************************************************************/
static int set_aec_setup
(
    void * const                ctx,
    ctrl_channel_handle_t const channel, 
    int const                   no, 
    int32_t * const             values
)
{
    (void) ctx;

    if ( no != CMD_GET_AEC_SETUP_NO_PARAMS )
    {
        return ( -EFAULT );
    }

    return ( set_param_int_X( channel, CMD_SET_AEC_SETUP,
        INT( values[0] ), INT( values[1] ), INT( values[2] ), INT( values[3] ),
        INT( values[4] ), INT( values[5] ), INT( values[6] ), INT( values[7] ) ) );
}

/******************************************************************************
 * get_awb_enable - Gets current AWB enable state
 *****************************************************************************/
static int get_awb_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             enable 
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !enable )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_AWB, CMD_SYNC_AWB, CMD_SET_AWB, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_AWB_NO_PARAMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *enable = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_awb_enable - Sets AWB enable state
 *****************************************************************************/
static int set_awb_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               enable 
)
{
    (void) ctx;
    return ( set_param_int_X( channel, CMD_SET_AWB, INT( enable ) ) );
}

/******************************************************************************
 * get_awb_speed - Gets current AWB speed 
 *****************************************************************************/
static int get_awb_speed
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             speed
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !speed )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_AWB_SPEED, CMD_SYNC_AWB_SPEED, CMD_SET_AWB_SPEED, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_AWB_SPEED_NO_PARAMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *speed = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_awb_speed - Sets AWB enable state
 *****************************************************************************/
static int set_awb_speed
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               speed
)
{
    (void) ctx;
    return ( set_param_int_X( channel, CMD_SET_AWB_SPEED, INT( speed ) ) );
}

/******************************************************************************
 * run_wb - run auto white-balance once
 *****************************************************************************/
static int run_wb 
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;
    
    return ( set_param_0( channel, CMD_WB ) );
}

/******************************************************************************
 * get_no_wbpresets - Gets the number of white-balance presets
 *****************************************************************************/
static int get_no_wbpresets
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             no 
)
{
    (void)ctx;
    
    char data[(10*CMD_SINGLE_LINE_RESPONSE_SIZE)];

    (void) ctx;

    int res;

    // parameter check
    if ( !no )
    {
        return ( -EINVAL );
    }

    // send get-command to control channel
    ctrl_channel_send_request( channel, (uint8_t *)CMD_GET_WB_PRESET, strlen(CMD_GET_WB_PRESET) );

    // read response from provideo device
    res = evaluate_get_response( channel, data, sizeof(data) );
    if ( !res )
    {
        int n = 0;

        char * s = strtok( data, CMD_NEW_LINE );
        char * d = strstr( s, CMD_OK );

        while ( s && !d )
        {
            int                 v0; // id
            wb_preset_name_t    v1; // preset name
            int                 v2; // color-temperature

            v0 = 0;
            memset( v1, 0, sizeof(v1) );
            v2 = 0;

            res = sscanf( s, CMD_REGEXP_WB_PRESET, &v0, &v1[0], &v2  );
            if ( res != 3 )
            {
                return ( res );
            }

            n++;

            s = strtok( NULL, CMD_NEW_LINE );
            d = strstr( s, CMD_OK );
        }

        if ( d )
        {
            *no = UINT8( n );
            res = 0;
        }
        else
        {
            res = -EILSEQ;
        }
    }
    else
    {
        res = evaluate_error_response( data, res );
    }

    return ( res );
}

/******************************************************************************
 * get_wbpresets - Gets the white-balance presets
 *****************************************************************************/
static int get_wbpresets
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void)ctx;
    
    char data[(10*CMD_SINGLE_LINE_RESPONSE_SIZE)];

    (void) ctx;

    int res;

    // parameter check
    if ( !no || !values )
    {
        return ( -EINVAL );
    }

    // send get-command to control channel
    ctrl_channel_send_request( channel, (uint8_t *)CMD_GET_WB_PRESET, strlen(CMD_GET_WB_PRESET) );

    // read response from provideo device
    res = evaluate_get_response( channel, data, sizeof(data) );
    if ( !res )
    {
        int n = 0;

        int no_of_items = no / sizeof(ctrl_protocol_wb_preset_t) ;

        ctrl_protocol_wb_preset_t * v = (ctrl_protocol_wb_preset_t *)values;

        char * s = strtok( data, CMD_NEW_LINE );
        char * d = strstr( s, CMD_OK );

        while ( s && (n < no_of_items) && !d )
        {
            int                 v0; // id
            wb_preset_name_t    v1; // preset name
            int                 v2; // color-temperature

            memset( v, 0, sizeof(*v) );

            v0 = 0;
            memset( v1, 0, sizeof(v1) );
            v2 = 0;

            res = sscanf( s, CMD_REGEXP_WB_PRESET, &v0, &v1[0], &v2  );
            if ( res != 3 )
            {
                return ( res );
            }

            v->id = UINT8( v0 );
            strncpy( (char *)v->name, v1, sizeof(wb_preset_name_t) );
            v->ct = UINT16( v2 );

            n++;
            v++;

            s = strtok( NULL, CMD_NEW_LINE );
            d = strstr( s, CMD_OK );
        }

        if ( d )
        {
            res = 0;
        }
        else
        {
            res = -EILSEQ;
        }
    }
    else
    {
        res = evaluate_error_response( data, res );
    }

    return ( res );
}

/******************************************************************************
 * set_wbpreset - Sets a white-balance preset
 *****************************************************************************/
static int set_wbpreset
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               id
)
{
    (void)ctx;
    
    return ( set_param_int_X( channel, CMD_SET_WB_PRESET, INT( id ) ) );
}
 
/******************************************************************************
 * get_stat_rgb - Gets white balance statistic
 *****************************************************************************/
static int get_stat_rgb
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint16_t * const            values
)
{
    (void) ctx;

    int v[CMD_GET_STAT_RGB_NO_PARMS];
    int res;

    // parameter check
    if ( !no || !values || (no != CMD_GET_STAT_RGB_NO_PARMS) )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_STAT_RGB,
            CMD_SYNC_STAT_RGB, CMD_SET_STAT_RGB,  &v[0u], &v[1u], &v[2u] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_STAT_RGB_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    values[0] = UINT16( v[0u] );
    values[1] = UINT16( v[1u] );
    values[2] = UINT16( v[2u] );

    return ( 0 );
}

/******************************************************************************
 * get_stat_histogram_enable - Gets enable status of histogram statistic measurment
 *****************************************************************************/
static int get_stat_histogram_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel, 
    uint8_t * const             flag
)
{
    (void) ctx;

    int v[CMD_GET_STAT_HIST_NO_PARMS];

    int res;

    // parameter check
    if ( !flag )
    {
        return ( -EINVAL );
    }

    // clear values
    memset( v, 0, sizeof( v ) );

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_STAT_HIST,
            CMD_SYNC_STAT_HIST, CMD_GET_STAT_HIST_MASK,
            &v[ 0],
            &v[ 1], &v[ 2], &v[ 3], &v[ 4], &v[ 5], &v[ 6], &v[ 7], &v[ 8],
            &v[ 9], &v[10], &v[11], &v[12], &v[13], &v[14], &v[15], &v[16] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_STAT_HIST_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *flag = UINT8( v[ 0] );

    return ( 0 );
}

/******************************************************************************
 * set_stat_histogram_enable - Sets enable status of histogram statistic measurment
 *****************************************************************************/
static int set_stat_histogram_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel, 
    uint8_t const               flag
)
{
    (void) ctx;
    
    return ( set_param_int_X( channel, CMD_SET_STAT_HIST, INT( flag ) ) );
}

/******************************************************************************
 * get_stat_exposure - Gets the histogram statistic from device
 *****************************************************************************/
static int get_stat_histogram
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint32_t * const            values
)
{
    (void) ctx;

    int v[CMD_GET_STAT_HIST_NO_PARMS];

    int res;
    unsigned i;

    // parameter check (no, values, no of items (command delivers 'enable state' too) 
    if ( (no <= 0) || !values || (no != (CMD_GET_STAT_HIST_NO_PARMS-1)) )
    {
        return ( -EINVAL );
    }

    // clear values
    memset( v, 0, sizeof( v ) );

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_STAT_HIST,
            CMD_SYNC_STAT_HIST, CMD_GET_STAT_HIST_MASK,
            &v[ 0],
            &v[ 1], &v[ 2], &v[ 3], &v[ 4], &v[ 5], &v[ 6], &v[ 7], &v[ 8],
            &v[ 9], &v[10], &v[11], &v[12], &v[13], &v[14], &v[15], &v[16] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_STAT_HIST_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range (1st parameter is 'enable state' => skip here)
    for ( i = 1; i < CMD_GET_STAT_HIST_NO_PARMS; i++ )
    {
        values[i-1] = UINT32( v[i] );
    }

    return ( 0 );
}

/******************************************************************************
 * get_stat_exposure_enable - Gets enable status of exposure statistic measurment
 *****************************************************************************/
static int get_stat_exposure_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel, 
    uint8_t * const             flag
)
{
    (void) ctx;

    int v[CMD_GET_STAT_EXP_NO_PARMS];

    int res;

    // parameter check
    if ( !flag )
    {
        return ( -EINVAL );
    }

    // clear values
    memset( v, 0, sizeof( v ) );

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_STAT_EXP,
            CMD_SYNC_STAT_EXP, CMD_GET_STAT_EXP_MASK,
            &v[ 0],
            &v[ 1], &v[ 2], &v[ 3], &v[ 4], &v[ 5],
            &v[ 6], &v[ 7], &v[ 8], &v[ 9], &v[10],
            &v[11], &v[12], &v[13], &v[14], &v[15],
            &v[16], &v[17], &v[18], &v[19], &v[20],
            &v[21], &v[22], &v[23], &v[24], &v[25] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_STAT_EXP_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *flag = UINT8( v[ 0] );
    
    return ( 0 );
}

/******************************************************************************
 * set_stat_exposure_enable - Sets enable status of exposure statistic measurment
 *****************************************************************************/
static int set_stat_exposure_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel, 
    uint8_t const               flag
)
{
    (void) ctx;
    
    return ( set_param_int_X( channel, CMD_SET_STAT_EXP, INT( flag ) ) );
}

/******************************************************************************
 * get_stat_exposure - Gets the exposure statistic from device
 *****************************************************************************/
static int get_stat_exposure
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint16_t * const            values
)
{
    (void) ctx;

    int v[CMD_GET_STAT_EXP_NO_PARMS];

    int res;
    unsigned i;

    // parameter check (no, values, no of items (command delivers 'enable state' too) 
    if ( (no <= 0) || !values || (no != (CMD_GET_STAT_EXP_NO_PARMS-1)) )
    {
        return ( -EINVAL );
    }

    // clear values
    memset( v, 0, sizeof( v ) );

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_STAT_EXP,
            CMD_SYNC_STAT_EXP, CMD_GET_STAT_EXP_MASK,
            &v[ 0],
            &v[ 1], &v[ 2], &v[ 3], &v[ 4], &v[ 5],
            &v[ 6], &v[ 7], &v[ 8], &v[ 9], &v[10],
            &v[11], &v[12], &v[13], &v[14], &v[15],
            &v[16], &v[17], &v[18], &v[19], &v[20],
            &v[21], &v[22], &v[23], &v[24], &v[25] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_STAT_EXP_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range (1st parameter is 'enable state' => skip here)
    for ( i = 1; i < CMD_GET_STAT_EXP_NO_PARMS; i++ )
    {
        values[i-1] = UINT16( v[i] );
    }
    
    return ( 0 );
}

/******************************************************************************
 * get_stat_xyz - Gets the WB statistics in XYZ
 *****************************************************************************/
static int get_stat_xyz
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    int32_t * const             values
)
{
    (void) ctx;

    int v[CMD_GET_STAT_XYZ_NO_PARMS];

    int res;

    // parameter check (no, values, no of items (command delivers 'enable state' too) 
    if ( !values || (no != CMD_GET_STAT_XYZ_NO_PARMS) )
    {
        return ( -EINVAL );
    }

    // clear values
    memset( v, 0, sizeof( v ) );

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_STAT_XYZ,
            CMD_SYNC_STAT_XYZ, CMD_GET_STAT_XYZ_MASK,
            &v[ 0], &v[ 1], &v[ 2] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_STAT_XYZ_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range (1st parameter is 'enable state' => skip here)
    values[0] = INT32( v[0] );
    values[1] = INT32( v[1] );
    values[2] = INT32( v[2] );

    return ( 0 );
}

/******************************************************************************
 * get_color_xyz - Gets the XYZ conversion matrix
 *****************************************************************************/
static int get_color_xyz
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    int32_t * const             values
)
{
    (void) ctx;

    int v[CMD_GET_COLOR_XYZ_NO_PARMS];

    int res;

    // parameter check (no, values, no of items (command delivers 'enable state' too) 
    if ( !values || (no != CMD_GET_COLOR_XYZ_NO_PARMS) )
    {
        return ( -EINVAL );
    }

    // clear values
    memset( v, 0, sizeof( v ) );

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, CMD_GET_COLOR_XYZ,
            CMD_SYNC_COLOR_XYZ, CMD_GET_COLOR_XYZ_MASK,
            &v[ 0], &v[ 1], &v[ 2],
            &v[ 3], &v[ 4], &v[ 5],
            &v[ 6], &v[ 7], &v[ 8] );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_COLOR_XYZ_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range (1st parameter is 'enable state' => skip here)
    values[0] = INT32( v[0] );
    values[1] = INT32( v[1] );
    values[2] = INT32( v[2] );
    values[3] = INT32( v[3] );
    values[4] = INT32( v[4] );
    values[5] = INT32( v[5] );
    values[6] = INT32( v[6] );
    values[7] = INT32( v[7] );
    values[8] = INT32( v[8] );

    return ( 0 );
}

/******************************************************************************
 * AUTO protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_auto_drv_t provideo_auto_drv = 
{
    .get_aec_enable             = get_aec_enable,
    .set_aec_enable             = set_aec_enable,
    .get_aec_setup              = get_aec_setup,
    .set_aec_setup              = set_aec_setup,
    .get_awb_enable             = get_awb_enable,
    .set_awb_enable             = set_awb_enable,
    .get_awb_speed              = get_awb_speed,
    .set_awb_speed              = set_awb_speed,
    .run_wb                     = run_wb,
    .get_no_wbpresets           = get_no_wbpresets,
    .get_wbpresets              = get_wbpresets,
    .set_wbpreset               = set_wbpreset,
    .get_stat_rgb               = get_stat_rgb,
    .get_stat_histogram_enable  = get_stat_histogram_enable,
    .set_stat_histogram_enable  = set_stat_histogram_enable,
    .get_stat_histogram         = get_stat_histogram,
    .get_stat_exposure_enable   = get_stat_exposure_enable,
    .set_stat_exposure_enable   = set_stat_exposure_enable,
    .get_stat_exposure          = get_stat_exposure,
    .get_stat_xyz               = get_stat_xyz,
    .get_color_xyz              = get_color_xyz,
};

/******************************************************************************
 * provideo_protocol_auto_init
 *****************************************************************************/
int provideo_protocol_auto_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_auto_register( handle, ctx, &provideo_auto_drv ) );
}

