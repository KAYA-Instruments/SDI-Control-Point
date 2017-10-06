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
 * @file    provideo_protocol_dpcc.c
 *
 * @brief   Implementation of provideo protocol dpcc functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_dpcc.h>

#include <provideo_protocol/provideo_protocol.h>
#include <provideo_protocol/provideo_protocol_common.h>

/******************************************************************************
 * @brief command "dpc" 
 *****************************************************************************/
#define CMD_GET_DPCC                        ( "dpc\n" )
#define CMD_SET_DPCC                        ( "dpc %i\n" )
#define CMD_SET_DPCC_WITH_COPY_FLAG         ( "dpc %i %i\n" )
#define CMD_SYNC_DPCC                       ( "dpc " )
#define CMD_GET_DPCC_NO_PARMS               ( 1 )

/******************************************************************************
 * @brief command "dpc_mode" 
 *****************************************************************************/
#define CMD_GET_DPCC_MODE                   ( "dpc_mode\n" )
#define CMD_SET_DPCC_MODE                   ( "dpc_mode %i\n" )
#define CMD_SET_DPCC_MODE_WITH_COPY_FLAG    ( "dpc_mode %i %i\n" )
#define CMD_SYNC_DPCC_MODE                  ( "dpc_mode " )
#define CMD_GET_DPCC_MODE_NO_PARMS          ( 1 )

/******************************************************************************
 * @brief command "dpc_level" 
 *****************************************************************************/
#define CMD_GET_DPCC_LEVEL                  ( "dpc_level\n" )
#define CMD_SET_DPCC_LEVEL                  ( "dpc_level %i\n" )
#define CMD_SET_DPCC_LEVEL_WITH_COPY_FLAG   ( "dpc_level %i %i\n" )
#define CMD_SYNC_DPCC_LEVEL                 ( "dpc_level " )
#define CMD_GET_DPCC_LEVEL_NO_PARMS         ( 1 )

/******************************************************************************
 * @brief command "dpc_add_px" 
 *****************************************************************************/
#define CMD_GET_DPCC_PIXEL                  ( "dpc_add_px\n" )
#define CMD_SET_DPCC_PIXEL                  ( "dpc_add_px %i %i\n" )
#define CMD_SET_DPCC_PIXELn                 ( "dpc_add_px %i %i\n%n" )
#define CMD_SET_DPCC_PIXEL_WITH_COPY_FLAG   ( "dpc_add_px %i %i %i\n" )
#define CMD_SYNC_DPCC_PIXEL                 ( "dpc_add_px " )
#define CMD_GET_DPCC_PIXEL_NO_PARMS         ( 2 )
/* Usually transmitting one coordinate does not take long (well below the 100ms
 * default timeout), but as up to 2048 values can be transmitted the device
 * sometimes hangs for some ms (probably due to a bad concatenation of
 * interrupts) thus we define a timeout of 300ms to make sure all values get
 * transmitted correctly. */
#define CMD_SET_DPCC_PIXEL_TMO              ( 300 )

/******************************************************************************
 * @brief command "dpc_del_px" 
 *****************************************************************************/
#define CMD_DEL_DPCC_PIXEL                  ( "dpc_del_px %i\n" )
#define CMD_DEL_DPCC_PIXEL_WITH_COPY_FLAG   ( "dpc_del_px %i %i %i\n" )

/******************************************************************************
 * @brief command "dpc_del_px" 
 *****************************************************************************/
#define CMD_CLEAR_DPCC_TABLE                ( "dpc_del_px\n" )
#define CMD_CLEAR_DPCC_TABLE_TMO            ( 1000 )

/******************************************************************************
 * @brief command "dpc_save" 
 *****************************************************************************/
#define CMD_SAVE_DPCC_TABLE                 ( "dpc_save\n" )
#define CMD_SAVE_DPCC_TABLE_TMO             ( 30000 )

/******************************************************************************
 * @brief command "load_settings" 
 *****************************************************************************/
#define CMD_LOAD_DPCC_TABLE                 ( "dpc_load\n" )
#define CMD_LOAD_DPCC_TABLE_TMO             ( 30000 )

/******************************************************************************
 * @brief command "load_settings"
 *****************************************************************************/
#define CMD_AUTO_LOAD_DPCC_TABLE            ( "dpc_auto_load\n" )
#define CMD_AUTO_LOAD_DPCC_TABLE_TMO        ( 500 )

/******************************************************************************
 * @brief command "dpc_test_mode"
 *****************************************************************************/
#define CMD_GET_DPCC_TEST_MODE                  ( "dpc_test_mode\n" )
#define CMD_SET_DPCC_TEST_MODE                  ( "dpc_test_mode %i\n" )
#define CMD_SET_DPCC_TEST_MODE_WITH_COPY_FLAG   ( "dpc_test_mode %i %i\n" )
#define CMD_SYNC_DPCC_TEST_MODE                 ( "dpc_test_mode " )
#define CMD_GET_DPCC_TEST_MODE_NO_PARMS         ( 1 )
#define CMD_SAVE_DPCC_TEST_MODE_TMO             ( 20000 )

/******************************************************************************
 * get_dpcc_enable - Get enable status of defect pixel cluster correction
 *****************************************************************************/
static int get_dpcc_enable
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
            CMD_GET_DPCC, CMD_SYNC_DPCC, CMD_SET_DPCC, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_DPCC_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *enable = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_dpcc_enable - Set enable status of defect pixel cluster correction
 *****************************************************************************/
static int set_dpcc_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               flag 
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_DPCC_WITH_COPY_FLAG,
                                    INT( flag ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_DPCC, INT( flag ) ) );
}

/******************************************************************************
 * get_dpcc_mode - Get correction mode of defect pixel cluster correction
 *****************************************************************************/
static int get_dpcc_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             mode 
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !mode )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_DPCC_MODE, CMD_SYNC_DPCC_MODE, CMD_SET_DPCC_MODE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_DPCC_MODE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_dpcc_mode - Set correction mode of defect pixel cluster correction
 *****************************************************************************/
static int set_dpcc_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode 
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_DPCC_MODE_WITH_COPY_FLAG,
                                    INT( mode ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_DPCC_MODE, INT( mode ) ) );
}

/******************************************************************************
 * get_dpcc_level - Get detection level of defect pixel cluster correction
 *****************************************************************************/
static int get_dpcc_level
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             mode 
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !mode )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_DPCC_LEVEL, CMD_SYNC_DPCC_LEVEL, CMD_SET_DPCC_LEVEL, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_DPCC_LEVEL_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_dpcc_level - Set detection level of defect pixel cluster correction
 *****************************************************************************/
static int set_dpcc_level
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               level 
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X( channel, CMD_SET_DPCC_LEVEL_WITH_COPY_FLAG,
                                    INT( level ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X( channel, CMD_SET_DPCC_LEVEL, INT( level ) ) );
}

/******************************************************************************
 * add_dpcc_pixel - Add a pixel into device dpcc table
 *****************************************************************************/
static int add_dpcc_pixel
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint16_t * const            values
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( !no || !values || (no != 2) )
    {
        return ( -EINVAL );
    }

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X_with_tmo( channel, CMD_SET_DPCC_PIXEL_WITH_COPY_FLAG,
                                           CMD_SET_DPCC_PIXEL_TMO, INT( values[0] ),
                                           INT( values[1] ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X_with_tmo( channel, CMD_SET_DPCC_PIXEL, CMD_SET_DPCC_PIXEL_TMO,
                                       INT( values[0] ), INT( values[1] ) ) );
}

/******************************************************************************
 * get_dpcc_table - read complete dpcc table
 *****************************************************************************/
static int get_dpcc_table
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint32_t * const            values
)
{
    (void) ctx;

    char command[CMD_SINGLE_LINE_COMMAND_SIZE];
    char buf[CMD_SINGLE_LINE_RESPONSE_SIZE];
    char data[CMD_SINGLE_LINE_RESPONSE_SIZE*2];

    ctrl_protocol_dpcc_table_t * table;

    int loop = 1;
    int cnt = 0;

    if ( (no != sizeof(ctrl_protocol_dpcc_table_t)) || !values )
    {
        return ( -EINVAL );
    }
    
    table = (ctrl_protocol_dpcc_table_t *)values;
    if ( !table->size || !table->x || !table->y )
    {
        return ( -EINVAL );
    }

    // clear command buffer
    memset( command, 0, sizeof(command) );

    // create command to send
    sprintf( command, CMD_GET_DPCC_PIXEL );

    // send get-command to control channel
    ctrl_channel_send_request( channel, (uint8_t *)command, strlen(command) );

    // clear data buffer
    memset( data, 0, sizeof(data) );
    data[0] = '\0';

    // counter to check for buffer overflow
    unsigned int data_count = 0;

    // start timer
    time_t start = time(NULL);;

    // wait for answer from COM-Port
    while ( loop )
    {
        int n;

        // poll for data (NOTE: reserve last byte for '\0')
        n = ctrl_channel_receive_response( channel, (uint8_t *)buf, (sizeof(buf) - 1u) );

        // evaluate number of received data
        if ( n > 0 )
        {
            // always put a "null" at the end of a string
            buf[n] = '\0';

            // increment data counter and check for overflow
            data_count += n;
            if ( data_count >= sizeof(data) )
            {
                return ( -ENOMEM );
            }

            // append poll buffer to receive buffer
            strcat( data, buf );

            // consume all commands from buffer
            // (get start position of first command therefoe)
            char * s = strstr( data, CMD_SYNC_DPCC_PIXEL );
            while ( s )
            {
                int x =0, y = 0, offset = 0;

                // parse command
                int res = sscanf( s, CMD_SET_DPCC_PIXELn, &x, &y, &offset );
                if ( (res == CMD_GET_DPCC_PIXEL_NO_PARMS) && (s[offset-1] == '\n') )
                {
                    table->x[cnt] = UINT16( x );
                    table->y[cnt] = UINT16( y );
                    if ( cnt++ >= table->size ) 
                    {
                        return ( -ENOMEM );
                    }

                    // move out the processed command
                    // Note: Use memmove instead of strncpy, because dst and src overlap!
                    memmove( data, (s+offset), sizeof(data) - (s+offset-data) );

                    // decrement data counter by bytes taken out
                    data_count -= offset;
           
                    // search next command in buffer
                    s = strstr( data, CMD_SYNC_DPCC_PIXEL );
                }
                else
                {
                    s = NULL;
                }
            }

            // check if string is complete and valid
            if ( strstr( data, CMD_OK ) )
            {
                table->no = cnt;
                return ( 0 );
            }

            // check if string is complete and error-message
            else if ( strstr( data, CMD_FAIL ) )
            {
                return ( evaluate_error_response( data, -EINVAL ) );
            }

            // reset timer
            start = time(NULL);;
        }
        else
        {
            // timeout handling, if device does not send new pixel positions after
            // 1 second, assume table has been transmitted completely
            int dt = (int)difftime( time(NULL), start );
            loop = ((int)dt > 1 ) ? 0 : 1;
        }
    }

    return ( -EILSEQ );
}

/******************************************************************************
 * clear_dpcc_table - clear current dpc table to camera flash
 *****************************************************************************/
static int clear_dpcc_table
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0_with_tmo( channel, CMD_CLEAR_DPCC_TABLE, CMD_CLEAR_DPCC_TABLE_TMO ) );
}

/******************************************************************************
 * save_dpcc_table - save current dpc table to camera flash
 *****************************************************************************/
static int save_dpcc_table
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0_with_tmo( channel, CMD_SAVE_DPCC_TABLE, CMD_SAVE_DPCC_TABLE_TMO ) );
}

/******************************************************************************
 * load_dpcc_table - load saved dpc table from camera flash
 *****************************************************************************/
static int load_dpcc_table
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0_with_tmo( channel, CMD_LOAD_DPCC_TABLE, CMD_LOAD_DPCC_TABLE_TMO ) );
}

/******************************************************************************
 * load_dpcc_table - automatically load dpcc table
 *****************************************************************************/
static int auto_load_dpcc_table
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0_with_tmo( channel, CMD_AUTO_LOAD_DPCC_TABLE, CMD_AUTO_LOAD_DPCC_TABLE_TMO ) );
}

/******************************************************************************
 * get_dpcc_test_mode - Get test mode of defect pixel cluster correction
 *****************************************************************************/
static int get_dpcc_test_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             mode
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !mode )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_DPCC_TEST_MODE, CMD_SYNC_DPCC_TEST_MODE, CMD_SET_DPCC_TEST_MODE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_DPCC_TEST_MODE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_dpcc_test_mode - Set test mode of defect pixel cluster correction
 *****************************************************************************/
static int set_dpcc_test_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode
)
{
    provideo_protocol_user_ctx_t * user = (provideo_protocol_user_ctx_t *)ctx;

    if ( user  && user->use_copy_flag )
    {
        return ( set_param_int_X_with_tmo( channel, CMD_SET_DPCC_TEST_MODE_WITH_COPY_FLAG,
                                           CMD_SAVE_DPCC_TEST_MODE_TMO, INT( mode ), INT( user->use_copy_flag ) ) );
    }

    return ( set_param_int_X_with_tmo( channel, CMD_SET_DPCC_TEST_MODE, CMD_SAVE_DPCC_TEST_MODE_TMO, INT( mode ) ) );
}

/******************************************************************************
 * DPCC protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_dpcc_drv_t provideo_dpcc_drv = 
{
    .get_dpcc_enable      = get_dpcc_enable,
    .set_dpcc_enable      = set_dpcc_enable,
    .get_dpcc_mode        = get_dpcc_mode,
    .set_dpcc_mode        = set_dpcc_mode,
    .get_dpcc_level       = get_dpcc_level,
    .set_dpcc_level       = set_dpcc_level,
    .add_dpcc_pixel       = add_dpcc_pixel,
    .get_dpcc_table       = get_dpcc_table,
    .clear_dpcc_table     = clear_dpcc_table,
    .save_dpcc_table      = save_dpcc_table,
    .load_dpcc_table      = load_dpcc_table,
    .auto_load_dpcc_table = auto_load_dpcc_table,
    .get_dpcc_test_mode   = get_dpcc_test_mode,
    .set_dpcc_test_mode   = set_dpcc_test_mode,
};

/******************************************************************************
 * provideo_protocol_dpcc_init
 *****************************************************************************/
int provideo_protocol_dpcc_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_dpcc_register( handle, ctx, &provideo_dpcc_drv ) );
}

