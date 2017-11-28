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
 * @file    provideo_protocol_gamma.c
 *
 * @brief   Implementation of provideo protocol gamma functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_lut.h>

#include <provideo_protocol/provideo_protocol_common.h>

/******************************************************************************
 * @brief command "lut_enable" 
 *****************************************************************************/
#define CMD_GET_LUT_ENABLE                      ( "lut_enable %i\n" )
#define CMD_SET_LUT_ENABLE                      ( "lut_enable %i %i\n" )
#define CMD_SYNC_LUT_ENABLE                     ( "lut_enable " )
#define CMD_GET_LUT_ENABLE_NO_PARMS             ( 2 )

/******************************************************************************
 * @brief command "lut_mode"
 *****************************************************************************/
#define CMD_GET_LUT_MODE                        ( "lut_mode \n" )
#define CMD_SET_LUT_MODE                        ( "lut_mode %i\n" )
#define CMD_SYNC_LUT_MODE                       ( "lut_mode " )
#define CMD_GET_LUT_MODE_NO_PARMS               ( 1 )
#define CMD_SET_LUT_MODE_TMO                    ( 5000 )

/******************************************************************************
 * @brief command "lut_fixed_mode"
 *****************************************************************************/
#define CMD_GET_LUT_FIXED_MODE                  ( "lut_fixed_mode \n" )
#define CMD_SET_LUT_FIXED_MODE                  ( "lut_fixed_mode %i\n" )
#define CMD_SYNC_LUT_FIXED_MODE                 ( "lut_fixed_mode " )
#define CMD_GET_LUT_FIXED_MODE_NO_PARMS         ( 1 )
#define CMD_SET_LUT_FIXED_MODE_TMO              ( 5000 )

/******************************************************************************
 * @brief command "lut_enable" 
 *****************************************************************************/
#define CMD_GET_LUT_PRESET                      ( "lut_preset \n" )
#define CMD_SET_LUT_PRESET                      ( "lut_preset %i\n" )
#define CMD_SYNC_LUT_PRESET                     ( "lut_preset " )
#define CMD_GET_LUT_PRESET_NO_PARMS             ( 1 )

/******************************************************************************
 * @brief command "lut_write_addr" 
 *****************************************************************************/
#define CMD_SET_LUT_WRITE_ADDR                  ( "lut_write_addr %i\n" )

/******************************************************************************
 * @brief command "lut_write_addr_red" 
 *****************************************************************************/
#define CMD_GET_LUT_WRITE_ADDR_RED              ( "lut_write_addr_red\n" )
#define CMD_SET_LUT_WRITE_ADDR_RED              ( "lut_write_addr_red %i\n" )
#define CMD_SYNC_LUT_WRITE_ADDR_RED             ( "lut_write_addr_red " )
#define CMD_GET_LUT_WRITE_ADDR_RED_NO_PARMS     ( 1 )

/******************************************************************************
 * @brief command "lut_write_addr_green" 
 *****************************************************************************/
#define CMD_GET_LUT_WRITE_ADDR_GREEN            ( "lut_write_addr_green\n" )
#define CMD_SET_LUT_WRITE_ADDR_GREEN            ( "lut_write_addr_green %i\n" )
#define CMD_SYNC_LUT_WRITE_ADDR_GREEN           ( "lut_write_addr_green " )
#define CMD_GET_LUT_WRITE_ADDR_GREEN_NO_PARMS   ( 1 )

/******************************************************************************
 * @brief command "lut_write_addr_blue" 
 *****************************************************************************/
#define CMD_GET_LUT_WRITE_ADDR_BLUE             ( "lut_write_addr_blue\n" )
#define CMD_SET_LUT_WRITE_ADDR_BLUE             ( "lut_write_addr_blue %i\n" )
#define CMD_SYNC_LUT_WRITE_ADDR_BLUE            ( "lut_write_addr_blue " )
#define CMD_GET_LUT_WRITE_ADDR_GREEN_NO_PARMS   ( 1 )

/******************************************************************************
 * @brief Min number of values to read if doing lut_read_XXX
 *****************************************************************************/
#define CMD_SYNC_LUT_READ                       ( "%i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i\n" )
#define CMD_GET_LUT_READ_NO_PARMS               ( 16 )

/******************************************************************************
 * @brief command "lut_read_red" 
 *****************************************************************************/
#define CMD_GET_LUT_READ_RED                    ( "lut_read_red\n" )
#define CMD_SYNC_LUT_READ_RED                   ( "lut_read_red " )
#define CMD_SET_LUT_READ_RED                    ( "lut_read_red %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i\n" )

/******************************************************************************
 * @brief command "lut_read_green" 
 *****************************************************************************/
#define CMD_GET_LUT_READ_GREEN                  ( "lut_read_green\n" )
#define CMD_SYNC_LUT_READ_GREEN                 ( "lut_read_green " )
#define CMD_SET_LUT_READ_GREEN                  ( "lut_read_green %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i\n" )

/******************************************************************************
 * @brief command "lut_read_blue" 
 *****************************************************************************/
#define CMD_GET_LUT_READ_BLUE                   ( "lut_read_blue\n" )
#define CMD_SYNC_LUT_READ_BLUE                  ( "lut_read_blue " )
#define CMD_SET_LUT_READ_BLUE                   ( "lut_read_blue %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i\n" )

/******************************************************************************
 * @brief command "lut_reset" 
 *****************************************************************************/
#define CMD_SET_LUT_RESET                       ( "lut_reset\n" )

/******************************************************************************
 * @brief command "lut_reset_red" 
 *****************************************************************************/
#define CMD_SET_LUT_RESET_RED                   ( "lut_reset_red\n" )

/******************************************************************************
 * @brief command "lut_reset_green" 
 *****************************************************************************/
#define CMD_SET_LUT_RESET_GREEN                 ( "lut_reset_green\n" )

/******************************************************************************
 * @brief command "lut_reset_blue" 
 *****************************************************************************/
#define CMD_SET_LUT_RESET_BLUE                  ( "lut_reset_blue\n" )

/******************************************************************************
 * @brief command "lut_reset_master" 
 *****************************************************************************/
#define CMD_SET_LUT_RESET_MASTER                ( "lut_reset_master\n" )

/******************************************************************************
 * @brief command "lut_sample" 
 *****************************************************************************/
#define CMD_SET_LUT_SAMPLE_8                    ( "lut_sample %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i\n" )
#define CMD_SET_LUT_SAMPLE_4                    ( "lut_sample %i %i %i %i %i %i %i %i\n" )
#define CMD_SET_LUT_SAMPLE_2                    ( "lut_sample %i %i %i %i\n" )
#define CMD_SET_LUT_SAMPLE_1                    ( "lut_sample  %i %i\n" )

/******************************************************************************
 * @brief command "lut_sample_red" 
 *****************************************************************************/
#define CMD_GET_LUT_SAMPLE_RED                  ( "lut_sample_red\n" )
#define CMD_SYNC_LUT_SAMPLE_RED                 ( "lut_sample_red " )
#define CMD_SET_LUT_SAMPLE_RED_8                ( "lut_sample_red %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i\n" )
#define CMD_SET_LUT_SAMPLE_RED_4                ( "lut_sample_red %i %i %i %i %i %i %i %i\n" )
#define CMD_SET_LUT_SAMPLE_RED_2                ( "lut_sample_red %i %i %i %i\n" )
#define CMD_SET_LUT_SAMPLE_RED_1                ( "lut_sample_red %i %i\n" )

/******************************************************************************
 * @brief command "lut_sample_green" 
 *****************************************************************************/
#define CMD_GET_LUT_SAMPLE_GREEN                ( "lut_sample_green\n" )
#define CMD_SYNC_LUT_SAMPLE_GREEN               ( "lut_sample_green " )
#define CMD_SET_LUT_SAMPLE_GREEN_8              ( "lut_sample_green %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i\n" )
#define CMD_SET_LUT_SAMPLE_GREEN_4              ( "lut_sample_green %i %i %i %i %i %i %i %i\n" )
#define CMD_SET_LUT_SAMPLE_GREEN_2              ( "lut_sample_green %i %i %i %i\n" )
#define CMD_SET_LUT_SAMPLE_GREEN_1              ( "lut_sample_green %i %i\n" )

/******************************************************************************
 * @brief command "lut_sample_blue" 
 *****************************************************************************/
#define CMD_GET_LUT_SAMPLE_BLUE                 ( "lut_sample_blue\n" )
#define CMD_SYNC_LUT_SAMPLE_BLUE                ( "lut_sample_blue " )
#define CMD_SET_LUT_SAMPLE_BLUE_8               ( "lut_sample_blue %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i\n" )
#define CMD_SET_LUT_SAMPLE_BLUE_4               ( "lut_sample_blue %i %i %i %i %i %i %i %i\n" )
#define CMD_SET_LUT_SAMPLE_BLUE_2               ( "lut_sample_blue %i %i %i %i\n" )
#define CMD_SET_LUT_SAMPLE_BLUE_1               ( "lut_sample_blue %i %i\n" )

/******************************************************************************
 * @brief command "lut_sample_master" 
 *****************************************************************************/
#define CMD_GET_LUT_SAMPLE_MASTER               ( "lut_sample_master\n" )
#define CMD_SYNC_LUT_SAMPLE_MASTER              ( "lut_sample_master " )
#define CMD_SET_LUT_SAMPLE_MASTER_8             ( "lut_sample_master %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i\n" )
#define CMD_SET_LUT_SAMPLE_MASTER_4             ( "lut_sample_master %i %i %i %i %i %i %i %i\n" )
#define CMD_SET_LUT_SAMPLE_MASTER_2             ( "lut_sample_master %i %i %i %i\n" )
#define CMD_SET_LUT_SAMPLE_MASTER_1             ( "lut_sample_master %i %i\n" )

/******************************************************************************
 * @brief command "lut_rec709" 
 *****************************************************************************/
#define CMD_SET_LUT_REC709                      ( "lut_fun_rec709 %i %i %i %i %i %i\n" )

/******************************************************************************
 * @brief command "lut_interpolate" 
 *****************************************************************************/
#define CMD_SET_LUT_INTERPOLATE                 ( "lut_interpolate\n" )
#define CMD_SET_LUT_INTERPOLATE_TMO             ( 5000 )

/******************************************************************************
 * @brief command "lut_interpolate" 
 *****************************************************************************/
#define CMD_SET_LUT_INTERPOLATE_RED             ( "lut_interpolate_red\n" )
#define CMD_SET_LUT_INTERPOLATE_RED_TMO         ( 2000 )

/******************************************************************************
 * @brief command "lut_interpolate" 
 *****************************************************************************/
#define CMD_SET_LUT_INTERPOLATE_GREEN           ( "lut_interpolate_green\n" )
#define CMD_SET_LUT_INTERPOLATE_GREEN_TMO       ( 2000 )

/******************************************************************************
 * @brief command "lut_interpolate" 
 *****************************************************************************/
#define CMD_SET_LUT_INTERPOLATE_BLUE            ( "lut_interpolate_blue\n" )
#define CMD_SET_LUT_INTERPOLATE_BLUE_TMO        ( 2000 )

/******************************************************************************
 * @brief command "lut_fast_gamma"
 *****************************************************************************/
#define CMD_GET_LUT_FAST_GAMMA                  ( "lut_fast_gamma \n" )
#define CMD_SET_LUT_FAST_GAMMA                  ( "lut_fast_gamma %i\n" )
#define CMD_SYNC_LUT_FAST_GAMMA                 ( "lut_fast_gamma " )
#define CMD_GET_LUT_FAST_GAMMA_NO_PARMS         ( 1 )
#define CMD_SET_LUT_FAST_GAMMA_TMO              ( 3000 )


/******************************************************************************
 * protocol helper function
 *****************************************************************************/

/******************************************************************************
 * @brief Gets the read/write index of a single LUT component. The gamma LUT
 *        component is given by the command-parameter.
 *
 * @note This index (offset) is used for a direct-read/write from/to LUT memory
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] index    current address index
 * @param[in]  cmd_get  command string to request settings
 * @param[in]  cmd_sync command string to synchronize response
 * @param[in]  cmd_set  command string to parse parameter in response
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_lut_write_index_internal
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t * const            index,
    char * const                cmd_get,
    char * const                cmd_set,
    char * const                cmd_sync
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !index )
    {
        return ( -EINVAL );
    }
    
    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2, cmd_get, cmd_sync, cmd_set, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_LUT_ENABLE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *index = UINT16( value );

    return ( 0 );
}

/******************************************************************************
 * @brief Direct-read out of a number of values from LUT component memory
 *
 * @note Start address-index has to be set with @see set_lut_write_addr, during
 *       reading the address-index gets auto-incremeted.
 *
 * @param[in]  com          command interface handle
 * @param[out] values       current LUT values
 * @param[in]  no           number of values to read
 * @param[in]  read         number of values read
 * @param[in]  cmd_get      command string to request settings
 * @param[in]  cmd_sync     command string to synchronize response
 * @param[in]  cmd_set      command string to parse parameters in response
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_lut_read_internal
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t * const            values,
    int const                   no,
    int * const                 read,
    char * const                cmd_get,
    char * const                cmd_sync,
    char * const                cmd_set
)
{
    (void) ctx;

    char data[2*CMD_SINGLE_LINE_RESPONSE_SIZE];

    int res;
    int n, i;

    // parameter check
    if ( !values || !no || (no < CMD_GET_LUT_READ_NO_PARMS) || !read )
    {
        return ( -EINVAL );
    }
    
    // round down to multiple of 16
    n = no / CMD_GET_LUT_READ_NO_PARMS;
    *read = 0;
    
    for ( i = 0; i<n; i++ )
    {
        // clear receive buffer
        memset( data, 0, sizeof(data) );
    
        // send command to channel port
        ctrl_channel_send_request( channel, (uint8_t *)cmd_get, strlen(cmd_get) );

        // read response from provideo device
        res = evaluate_get_response( channel, data, sizeof(data) );
        if ( !res )
        {
            char * s = strstr( data, cmd_sync );
            if ( s )
            {
                int v0,  v1,  v2,  v3,  v4,  v5,  v6,  v7;
                int v8,  v9, v10, v11, v12, v13, v14, v15;
                
                res = sscanf( s, cmd_set,
                                     &v0,  &v1,  &v2,  &v3,
                                     &v4,  &v5,  &v6,  &v7,
                                     &v8,  &v9, &v10, &v11,
                                    &v12, &v13, &v14, &v15 );
                if ( res == CMD_GET_LUT_READ_NO_PARMS )
                {
                    int j = 16 * i;
                    values[j+ 0] = UINT16( v0 );
                    values[j+ 1] = UINT16( v1 );
                    values[j+ 2] = UINT16( v2 );
                    values[j+ 3] = UINT16( v3 );
                    values[j+ 4] = UINT16( v4 );
                    values[j+ 5] = UINT16( v5 );
                    values[j+ 6] = UINT16( v6 );
                    values[j+ 7] = UINT16( v7 );
                    values[j+ 8] = UINT16( v8 );
                    values[j+ 9] = UINT16( v9 );
                    values[j+10] = UINT16( v10 );
                    values[j+11] = UINT16( v11 );
                    values[j+12] = UINT16( v12 );
                    values[j+13] = UINT16( v13 );
                    values[j+14] = UINT16( v14 );
                    values[j+15] = UINT16( v15 );
                    
                    *read += CMD_GET_LUT_READ_NO_PARMS;
                }
            }
            else
            {
                return ( -EFAULT );
            }
        }
        else
        {
            return ( evaluate_error_response( data, res ) );
        }
    }

    return ( 0 );
}

/******************************************************************************
 * @brief Gets/Reads out all sample points of a LUT component
 *
 * @note used for interpolation mode configuration of LUT component
 *
 * @param[in]  com          command interface handle
 * @param[out] x_i          current x values
 * @param[out] y_i          current y values
 * @param[in]  no           number of x_i/y_i-values
 * @param[out] read         number of sample points read
 * @param[in]  cmd_get      command string to request settings
 * @param[in]  cmd_sync     command string to synchronize response
 * @param[in]  cmd_set      command string to parse parameter in response
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_lut_sample_internal
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t * const            x_i,
    uint16_t * const            y_i,
    uint32_t const              no,
    uint32_t * const            read,
    char * const                cmd_get,
    char * const                cmd_set,
    char * const                cmd_sync
)
{
    (void) ctx;

    char data[MAX_NO_SAMPLE_POINTS * CMD_SINGLE_LINE_RESPONSE_SIZE / 8u];

    unsigned n = 0u;

    int res;

    // parameter check
    if ( !x_i || !y_i || !no || !read )
    {
        return ( -EINVAL );
    }

    // send command to COM port
    ctrl_channel_send_request( channel, (uint8_t *)cmd_get, strlen(cmd_get) );

    // read response from provideo device
    res = evaluate_get_response( channel, data, sizeof(data) );
    if ( !res )
    {

        char * s = strstr( data, cmd_sync );

        *read = 0;

        while ( s && (n < no) )
        {
            int x[8];
            int y[8];

            unsigned i;
            
            memset( x, 0, sizeof(x) );
            memset( y, 0, sizeof(y) );

            res = sscanf( s, cmd_set,
                    &x[0], &y[0], &x[1], &y[1], &x[2], &y[2], &x[3], &y[3],
                    &x[4], &y[4], &x[5], &y[5], &x[6], &y[6], &x[7], &y[7] );
            if ( (res >= 2) && !(res & 0x1) && ((n+((uint8_t)(res>>1))) <= no) )
            {
                for ( i=0; i<(unsigned)(res>>1); i++ )
                {
                    x_i[n+i] = (uint16_t)x[i];
                    y_i[n+i] = (uint16_t)y[i];
                }
                n += ((uint8_t)(res>>1));
            }
            else if ( !res )
            {
                // no match
                return ( -EFAULT );
            }
            else
            {
                // error during match
                return ( res );
            }

            s = strstr( (s+1), cmd_sync );
        }

        *read = n;
        return ( 0 );
    }
    else
    {
        res = evaluate_error_response( data, res );
    }
    
    return ( res );
}

/******************************************************************************
 * set_lut_sample_internal - Adds sample points to a LUT component
 *
 * @note used for interpolation mode configuration of LUT component
 *
 * @param[in]  com          command interface handle
 * @param[in]  x_i          x values to add/set
 * @param[in]  y_i          y values to add/set
 * @param[in]  no           number of x_i/y_i-values
 * @param[in]  cmd_set_8    command string to add/set 8 sample points
 * @param[in]  cmd_set_4    command string to add/set 4 sample points
 * @param[in]  cmd_set_2    command string to add/set 2 sample points
 * @param[in]  cmd_set_1    command string to add/set 1 sample point
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_lut_sample_internal
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t * const            x_i,
    uint16_t * const            y_i,
    uint32_t const              no,
    const char * const          cmd_8,
    const char * const          cmd_4,
    const char * const          cmd_2,
    const char * const          cmd_1
)
{
    (void) ctx;

    char command[CMD_SINGLE_LINE_COMMAND_SIZE];

    int res;

    unsigned n, i;

    // parameter check
    if ( !x_i || !y_i || !no )
    {
        return ( -EINVAL );
    }

    n = no;
    i = 0;

    // send 8 sample points
    while ( n >= 8 )
    {
        // clear command buffer
        memset( command, 0, sizeof(command) );

        // create command to send
        sprintf( command, cmd_8,
            x_i[i+0], y_i[i+0], x_i[i+1], y_i[i+1], x_i[i+2], y_i[i+2], x_i[i+3], y_i[i+3],
            x_i[i+4], y_i[i+4], x_i[i+5], y_i[i+5], x_i[i+6], y_i[i+6], x_i[i+7], y_i[i+7] );

        // send command to COM port
        ctrl_channel_send_request( channel, (uint8_t *)command, strlen(command) );

        // wait for response and evaluate
        res = evaluate_set_response( channel );
        if ( res )
        {
            return ( res );
        }

        n-=8;
        i+=8;
    }

    // send 4 sample points
    while ( n >= 4 )
    {
        // clear command buffer
        memset( command, 0, sizeof(command) );

        // create command to send
        sprintf( command, cmd_4,
            x_i[i+0], y_i[i+0], x_i[i+1], y_i[i+1], x_i[i+2], y_i[i+2], x_i[i+3], y_i[i+3] );

        // send command to COM port
        ctrl_channel_send_request( channel, (uint8_t *)command, strlen(command) );

        // wait for response and evaluate
        res = evaluate_set_response( channel );
        if ( res )
        {
            return ( res );
        }

        n-=4;
        i+=4;
    }

    // send 2 sample points
    while ( n >= 2 )
    {
        // clear command buffer
        memset( command, 0, sizeof(command) );

        // create command to send
        sprintf( command, cmd_2,
            x_i[i+0], y_i[i+0], x_i[i+1], y_i[i+1] );
        
        // send command to COM port
        ctrl_channel_send_request( channel, (uint8_t *)command, strlen(command) );

        // wait for response and evaluate
        res = evaluate_set_response( channel );
        if ( res )
        {
            return ( res );
        }

        n-=2;
        i+=2;
    }

    // send a single sample point
    for ( n=i; n<no; n++ )
    {
        // clear command buffer
        memset( command, 0, sizeof(command) );

        // create command to send
        sprintf( command, cmd_1, x_i[n+0], y_i[n+0] );
        
        // send command to COM port
        ctrl_channel_send_request( channel, (uint8_t *)command, strlen(command) );

        // wait for response and evaluate
        res = evaluate_set_response( channel );
        if ( res )
        {
            return ( res );
        }
    }

    return ( 0 );
}



/******************************************************************************
 * protocol function 
 *****************************************************************************/

/**************************************************************************//**
 * @brief Gets the enable status of gamma LUT module 
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[in]  flag     current enable status
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_lut_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    char command[CMD_SINGLE_LINE_COMMAND_SIZE];
    char data[2*CMD_SINGLE_LINE_RESPONSE_SIZE];

    ctrl_protocol_enable_t * enable;

    int res;

    // parameter check
    if ( !no || !values || (sizeof(*enable) != no) )
    {
        return ( -EINVAL );
    }

    enable = (ctrl_protocol_enable_t *)values;

    // clear command buffer
    memset( command, 0, sizeof(command) );

    sprintf( command, CMD_GET_LUT_ENABLE, enable->id ); 

    // send command to COM port
    ctrl_channel_send_request( channel, (uint8_t *)command, strlen(command) );

    // read response from provideo device
    res = evaluate_get_response( channel, data, sizeof(data) );
    if ( !res )
    {
        char * s = strstr( data, CMD_SYNC_LUT_ENABLE );
        if ( s )
        {
            int v0, v1;
            res = sscanf( s, CMD_SET_LUT_ENABLE, &v0, &v1 );
            if ( (res == CMD_GET_LUT_ENABLE_NO_PARMS) && (UINT8(v0) == enable->id) )
            {
                enable->flag = UINT8( v1 );
                return ( 0 );
            }
            else
            {
                return ( -EINVAL );
            }
        }
    }
    else
    {
        res = evaluate_error_response( data, res );
    }

    return ( 0 );
}

/**************************************************************************//**
 * @brief Enables or disables the gamma LUT module
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[in]  flag     flag to set (0 = disable, 1 = enable)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_lut_enable
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;
    
    ctrl_protocol_enable_t * enable;

    // parameter check
    if ( !no || !values || (sizeof(*enable) != no) )
    {
        return ( -EINVAL );
    }
    
    enable = (ctrl_protocol_enable_t *)values;

    return ( set_param_int_X( channel,
                CMD_SET_LUT_ENABLE, INT( enable->id ), INT( enable->flag ) ) );
}

/**************************************************************************//**
 * @brief Gets the operational mode of gamma LUT module
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] mode     current mode
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_lut_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             mode
)
{
    (void) ctx;

    int v;
    int res;

    // parameter check
    if ( !mode )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_LUT_MODE, CMD_SYNC_LUT_MODE, CMD_SET_LUT_MODE, &v );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_LUT_MODE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( v );

    return ( 0 );
}

/**************************************************************************//**
 * @brief Sets the operational mode of the LUT module
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[in]  mode     mode to set (0 = interpolate, 1 = fast gamma,
 *                      2 = fixed gamma curve)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_lut_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode
)
{
    (void) ctx;

    return ( set_param_int_X_with_tmo( channel,
                                       CMD_SET_LUT_MODE, CMD_SET_LUT_MODE_TMO,
                                       INT( mode ) ) );
}

/**************************************************************************//**
 * @brief Gets the fixed gamma curve mode of gamma LUT module
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] mode     current mode
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_lut_fixed_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             mode
)
{
    (void) ctx;

    int v;
    int res;

    // parameter check
    if ( !mode )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_LUT_FIXED_MODE, CMD_SYNC_LUT_FIXED_MODE, CMD_SET_LUT_FIXED_MODE, &v );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_LUT_FIXED_MODE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *mode = UINT8( v );

    return ( 0 );
}

/**************************************************************************//**
 * @brief Sets the fixed gamma curve mode of the LUT module
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[in]  mode     mode to set (0 = Rec.709, 1 = PQ, 2 = HLG)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_lut_fixed_mode
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               mode
)
{
    (void) ctx;

    return ( set_param_int_X_with_tmo( channel,
                                       CMD_SET_LUT_FIXED_MODE, CMD_SET_LUT_FIXED_MODE_TMO,
                                       INT( mode ) ) );
}

/**************************************************************************//**
 * @brief Get current LUT preset storage
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] value    currently selected preset storage
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_lut_preset
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             value 
)
{
    (void) ctx;

    int v;
    int res;

    // parameter check
    if ( !value )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_LUT_PRESET, CMD_SYNC_LUT_PRESET, CMD_SET_LUT_PRESET, &v );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_LUT_PRESET_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *value = UINT8( v );

    return ( 0 );
}

/******************************************************************************
 * @brief Set current LUT preset storage
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[in]  value    preset storage to select
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_lut_preset
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               value
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_LUT_PRESET, INT( value ) ) );
}

/******************************************************************************
 * @brief Sets read/write index of all gamma LUT components
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[in]  index    gamma LUT index to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_lut_write_index
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t const              index
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_LUT_WRITE_ADDR, INT( index ) ) );
}

/**************************************************************************//**
 * @brief Gets read/write index of gamma LUT red component
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] index    current gamma LUT index of red component
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_lut_write_index_red
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t * const            index
)
{
    return( get_lut_write_index_internal( ctx, channel, index,
                CMD_GET_LUT_WRITE_ADDR_RED,
                CMD_SET_LUT_WRITE_ADDR_RED,
                CMD_SYNC_LUT_WRITE_ADDR_RED ) );
}

/**************************************************************************//**
 * @brief Sets read/write index of gamma LUT red component
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[in]  index    gamma LUT index of red component to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_lut_write_index_red
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t const              index
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_LUT_WRITE_ADDR_RED, INT( index ) ) );
}

/**************************************************************************//**
 * @brief Gets read/write index of gamma LUT green component
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] index    current gamma LUT index of green component
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_lut_write_index_green
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t * const            index
)
{
    return( get_lut_write_index_internal( ctx, channel, index,
                CMD_GET_LUT_WRITE_ADDR_GREEN,
                CMD_SET_LUT_WRITE_ADDR_GREEN,
                CMD_SYNC_LUT_WRITE_ADDR_GREEN ) );
}

/**************************************************************************//**
 * @brief Sets read/write index of gamma LUT green component
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[in]  index    gamma LUT index of red component to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_lut_write_index_green
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t const              index
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_LUT_WRITE_ADDR_GREEN, INT( index ) ) );
}

/**************************************************************************//**
 * @brief Gets read/write index of gamma LUT blue component
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] index    current gamma LUT index of green component
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_lut_write_index_blue
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t * const            index
)
{
    return( get_lut_write_index_internal( ctx, channel, index,
                CMD_GET_LUT_WRITE_ADDR_BLUE,
                CMD_SET_LUT_WRITE_ADDR_BLUE,
                CMD_SYNC_LUT_WRITE_ADDR_BLUE ) );
}

/**************************************************************************//**
 * @brief Sets read/write index of gamma LUT blue component
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[in]  index    gamma LUT index of red component to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_lut_write_index_blue
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint16_t const              index
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_LUT_WRITE_ADDR_BLUE, INT( index ) ) );
}

/******************************************************************************
 * get_lut_read_red - Reads a number of values from red LUT component
 *****************************************************************************/
static int get_lut_read_red
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint16_t * const            values
)
{
    (void) ctx;
    
    int no_got;

    int res = ( get_lut_read_internal( ctx, channel, values, no, &no_got,
                CMD_GET_LUT_READ_RED, CMD_SYNC_LUT_READ_RED, CMD_SET_LUT_READ_RED ) );
    if ( res )
    {
        return ( res );
    }

    if ( no_got != no )
    {
        return ( -EFAULT );
    }

    return ( 0 );
}

/******************************************************************************
 * get_lut_read_green - Reads a number of values from green LUT component
 *****************************************************************************/
static int get_lut_read_green
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint16_t * const            values
)
{
    (void) ctx;
    
    int no_got;

    int res = ( get_lut_read_internal( ctx, channel, values, no, &no_got,
                CMD_GET_LUT_READ_GREEN, CMD_SYNC_LUT_READ_GREEN, CMD_SET_LUT_READ_GREEN ) );
    if ( res )
    {
        return ( res );
    }

    if ( no_got != no )
    {
        return ( -EFAULT );
    }

    return ( 0 );
}

/******************************************************************************
 * get_lut_read_blue - Reads a number of values from blue LUT component
 *****************************************************************************/
static int get_lut_read_blue
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint16_t * const            values
)
{
    (void) ctx;

    int no_got;

    int res = ( get_lut_read_internal( ctx, channel, values, no, &no_got,
                CMD_GET_LUT_READ_BLUE, CMD_SYNC_LUT_READ_BLUE, CMD_SET_LUT_READ_BLUE ) );
    if ( res )
    {
        return ( res );
    }

    if ( no_got != no )
    {
        return ( -EFAULT );
    }

    return ( 0 );
}

/******************************************************************************
 * set_lut_reset - Clears configuration and starts interpolation mode 
 *                 configuration on all gamma LUT components.
 *****************************************************************************/
static int set_lut_reset
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0( channel, CMD_SET_LUT_RESET ) );
}

/******************************************************************************
 * set_lut_reset_red - Clears configuration and starts interpolation mode 
 *                     configuration on gamma LUT red component.
 *****************************************************************************/
static int set_lut_reset_red
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0( channel, CMD_SET_LUT_RESET_RED ) );
}

/******************************************************************************
 * set_lut_reset_green - Clears configuration and starts interpolation mode 
 *                       configuration on gamma LUT green component.
 *****************************************************************************/
static int set_lut_reset_green
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0( channel, CMD_SET_LUT_RESET_GREEN ) );
}

/******************************************************************************
 * set_lut_reset_blue - Clears configuration and starts interpolation mode 
 *                      configuration on gamma LUT blue component.
 *****************************************************************************/
static int set_lut_reset_blue
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0( channel, CMD_SET_LUT_RESET_BLUE ) );
}

/******************************************************************************
 * set_lut_reset_master - 
 *****************************************************************************/
static int set_lut_reset_master
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0( channel, CMD_SET_LUT_RESET_MASTER ) );
}

/******************************************************************************
 * set_lut_sample - adds sample points to all LUT components.
 *****************************************************************************/
static int set_lut_sample
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    ctrl_protocol_samples_t * samples;

    if ( !no || !values || (no != sizeof(ctrl_protocol_samples_t)) )
    {
        return ( -EINVAL );
    }

    samples = (ctrl_protocol_samples_t *)values;

    return ( set_lut_sample_internal( ctx, channel,
        samples->x_i, samples->y_i, samples->no,
        CMD_SET_LUT_SAMPLE_8, CMD_SET_LUT_SAMPLE_4, CMD_SET_LUT_SAMPLE_2, CMD_SET_LUT_SAMPLE_1 ) );
}

/******************************************************************************
 * get_lut_sample_red - Gets sample points of red LUT components.
 *****************************************************************************/
static int get_lut_sample_red
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    ctrl_protocol_samples_t * samples;

    if ( !no || !values || (no != sizeof(ctrl_protocol_samples_t)) )
    {
        return ( -EINVAL );
    }

    samples = (ctrl_protocol_samples_t *)values;

	return ( get_lut_sample_internal( ctx, channel,
        samples->x_i, samples->y_i, samples->no, &samples->no,
		CMD_GET_LUT_SAMPLE_RED, CMD_SET_LUT_SAMPLE_RED_8, CMD_SYNC_LUT_SAMPLE_RED ) );
}

/******************************************************************************
 * set_lut_sample_red - Adds sample points to red LUT components.
 *****************************************************************************/
static int set_lut_sample_red
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    ctrl_protocol_samples_t * samples;

    if ( !no || !values || (no != sizeof(ctrl_protocol_samples_t)) )
    {
        return ( -EINVAL );
    }

    samples = (ctrl_protocol_samples_t *)values;

    return ( set_lut_sample_internal( ctx, channel,
        samples->x_i, samples->y_i, samples->no,
        CMD_SET_LUT_SAMPLE_RED_8, CMD_SET_LUT_SAMPLE_RED_4, CMD_SET_LUT_SAMPLE_RED_2, CMD_SET_LUT_SAMPLE_RED_1 ) );
}

/******************************************************************************
 * get_lut_sample_green - Gets sample points of green LUT components.
 *****************************************************************************/
static int get_lut_sample_green
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    ctrl_protocol_samples_t * samples;

    if ( !no || !values || (no != sizeof(ctrl_protocol_samples_t)) )
    {
        return ( -EINVAL );
    }

    samples = (ctrl_protocol_samples_t *)values;

	return ( get_lut_sample_internal( ctx, channel,
        samples->x_i, samples->y_i, samples->no, &samples->no,
		CMD_GET_LUT_SAMPLE_GREEN, CMD_SET_LUT_SAMPLE_GREEN_8, CMD_SYNC_LUT_SAMPLE_GREEN ) );
}

/******************************************************************************
 * set_lut_sample_green - Adds sample points to green LUT components.
 *****************************************************************************/
static int set_lut_sample_green
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    ctrl_protocol_samples_t * samples;

    if ( !no || !values || (no != sizeof(ctrl_protocol_samples_t)) )
    {
        return ( -EINVAL );
    }

    samples = (ctrl_protocol_samples_t *)values;

    return ( set_lut_sample_internal( ctx, channel,
        samples->x_i, samples->y_i, samples->no,
        CMD_SET_LUT_SAMPLE_GREEN_8, CMD_SET_LUT_SAMPLE_GREEN_4, CMD_SET_LUT_SAMPLE_GREEN_2, CMD_SET_LUT_SAMPLE_GREEN_1 ) );
}

/******************************************************************************
 * get_lut_sample_blue - Gets sample points of blue LUT components.
 *****************************************************************************/
static int get_lut_sample_blue
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    ctrl_protocol_samples_t * samples;

    if ( !no || !values || (no != sizeof(ctrl_protocol_samples_t)) )
    {
        return ( -EINVAL );
    }

    samples = (ctrl_protocol_samples_t *)values;

	return ( get_lut_sample_internal( ctx, channel,
        samples->x_i, samples->y_i, samples->no, &samples->no,
		CMD_GET_LUT_SAMPLE_BLUE, CMD_SET_LUT_SAMPLE_BLUE_8, CMD_SYNC_LUT_SAMPLE_BLUE ) );
}

/******************************************************************************
 * set_lut_sample_blue - Adds sample points to blue LUT components.
 *****************************************************************************/
static int set_lut_sample_blue
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    ctrl_protocol_samples_t * samples;

    if ( !no || !values || (no != sizeof(ctrl_protocol_samples_t)) )
    {
        return ( -EINVAL );
    }

    samples = (ctrl_protocol_samples_t *)values;

    return ( set_lut_sample_internal( ctx, channel,
        samples->x_i, samples->y_i, samples->no,
        CMD_SET_LUT_SAMPLE_BLUE_8, CMD_SET_LUT_SAMPLE_BLUE_4, CMD_SET_LUT_SAMPLE_BLUE_2, CMD_SET_LUT_SAMPLE_BLUE_1 ) );
}

/******************************************************************************
 * get_lut_sample_master - Gets sample points of master LUT components.
 *****************************************************************************/
static int get_lut_sample_master
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    ctrl_protocol_samples_t * samples;

    if ( !no || !values || (no != sizeof(ctrl_protocol_samples_t)) )
    {
        return ( -EINVAL );
    }

    samples = (ctrl_protocol_samples_t *)values;

	return ( get_lut_sample_internal( ctx, channel,
        samples->x_i, samples->y_i, samples->no, &samples->no,
		CMD_GET_LUT_SAMPLE_MASTER, CMD_SET_LUT_SAMPLE_MASTER_8, CMD_SYNC_LUT_SAMPLE_MASTER ) );
}

/******************************************************************************
 * set_lut_sample_master - Adds sample points to master LUT components.
 *****************************************************************************/
static int set_lut_sample_master
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    ctrl_protocol_samples_t * samples;

    if ( !no || !values || (no != sizeof(ctrl_protocol_samples_t)) )
    {
        return ( -EINVAL );
    }

    samples = (ctrl_protocol_samples_t *)values;

    return ( set_lut_sample_internal( ctx, channel,
        samples->x_i, samples->y_i, samples->no,
        CMD_SET_LUT_SAMPLE_MASTER_8, CMD_SET_LUT_SAMPLE_MASTER_4, CMD_SET_LUT_SAMPLE_MASTER_2, CMD_SET_LUT_SAMPLE_MASTER_1 ) );
}

/******************************************************************************
 * set_lut_rec709 - Set rec709 parameters for all LUT components
 *****************************************************************************/
static int set_lut_rec709
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    ctrl_protocol_rec709_t * rec709;

    if ( !no || !values || (no != sizeof(ctrl_protocol_rec709_t)) )
    {
        return ( -EINVAL );
    }

    rec709 = (ctrl_protocol_rec709_t *)values;
    
    return ( set_param_int_X( channel,
                CMD_SET_LUT_REC709,
                INT( rec709->threshold ), INT( rec709->lcontrast ),
                INT( rec709->lbrightness), INT( rec709->contrast),
                INT( rec709->gamma), INT( rec709->brightness) ) );
}

/******************************************************************************
 * set_lut_interpolate
 * - Interpolates all LUT values based on the given sample points.
 *****************************************************************************/
static int set_lut_interpolate
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0_with_tmo( channel, CMD_SET_LUT_INTERPOLATE, CMD_SET_LUT_INTERPOLATE_TMO ) );
}

/******************************************************************************
 * set_lut_interpolate
 * - Interpolates red LUT values based on the given sample points.
 *****************************************************************************/
static int set_lut_interpolate_red
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0_with_tmo( channel, CMD_SET_LUT_INTERPOLATE_RED, CMD_SET_LUT_INTERPOLATE_RED_TMO ) );
}

/******************************************************************************
 * set_lut_interpolate
 * - Interpolates green LUT values based on the given sample points.
 *****************************************************************************/
static int set_lut_interpolate_green
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0_with_tmo( channel, CMD_SET_LUT_INTERPOLATE_GREEN, CMD_SET_LUT_INTERPOLATE_GREEN_TMO ) );
}

/******************************************************************************
 * set_lut_interpolate
 * - Interpolates blue LUT values based on the given sample points.
 *****************************************************************************/
static int set_lut_interpolate_blue
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0_with_tmo( channel, CMD_SET_LUT_INTERPOLATE_BLUE, CMD_SET_LUT_INTERPOLATE_BLUE_TMO ) );
}

/**************************************************************************//**
 * @brief Gets the currently set gamma value of the luts fast gamma function
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[out] gamma    current gamma setting
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int get_lut_fast_gamma
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int16_t * const             gamma
)
{
    (void) ctx;

    int v;
    int res;

    // parameter check
    if ( !gamma )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_LUT_FAST_GAMMA, CMD_SYNC_LUT_FAST_GAMMA, CMD_SET_LUT_FAST_GAMMA, &v );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_LUT_FAST_GAMMA_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *gamma = INT16( v );

    return ( 0 );
}

/**************************************************************************//**
 * @brief Sets the gamma value of the fast gamma function and recalculates the
 *        LUT
 *
 * @param[in]  ctx      private protocol context
 * @param[in]  channel  control channel to send the request
 * @param[in]  gamma    gamma value to set (multiplied by 1000)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
static int set_lut_fast_gamma
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int16_t const               gamma
)
{
    (void) ctx;

    return ( set_param_int_X_with_tmo( channel,
                                       CMD_SET_LUT_FAST_GAMMA, CMD_SET_LUT_FAST_GAMMA_TMO,
                                       INT( gamma ) ) );
}

/******************************************************************************
 * ISP protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_lut_drv_t provideo_lut_drv = 
{
    .get_lut_enable             = get_lut_enable,
    .set_lut_enable             = set_lut_enable,
    .get_lut_mode               = get_lut_mode,
    .set_lut_mode               = set_lut_mode,
    .get_lut_fixed_mode         = get_lut_fixed_mode,
    .set_lut_fixed_mode         = set_lut_fixed_mode,
    .get_lut_preset             = get_lut_preset,
    .set_lut_preset             = set_lut_preset,
    .set_lut_write_index        = set_lut_write_index,
    .get_lut_write_index_red    = get_lut_write_index_red,
    .set_lut_write_index_red    = set_lut_write_index_red,
    .get_lut_write_index_green  = get_lut_write_index_green,
    .set_lut_write_index_green  = set_lut_write_index_green,
    .get_lut_write_index_blue   = get_lut_write_index_blue,
    .set_lut_write_index_blue   = set_lut_write_index_blue,
    .get_lut_read_red           = get_lut_read_red,
    .get_lut_read_green         = get_lut_read_green,
    .get_lut_read_blue          = get_lut_read_blue,
    .set_lut_reset              = set_lut_reset,
    .set_lut_reset_red          = set_lut_reset_red,
    .set_lut_reset_green        = set_lut_reset_green,
    .set_lut_reset_blue         = set_lut_reset_blue,
    .set_lut_reset_master       = set_lut_reset_master,
    .set_lut_sample             = set_lut_sample,
    .get_lut_sample_red         = get_lut_sample_red,
    .set_lut_sample_red         = set_lut_sample_red,
    .get_lut_sample_green       = get_lut_sample_green,
    .set_lut_sample_green       = set_lut_sample_green,
    .get_lut_sample_blue        = get_lut_sample_blue,
    .set_lut_sample_blue        = set_lut_sample_blue,
    .get_lut_sample_master      = get_lut_sample_master,
    .set_lut_sample_master      = set_lut_sample_master,
    .set_lut_rec709             = set_lut_rec709,
    .set_lut_interpolate        = set_lut_interpolate,
    .set_lut_interpolate_red    = set_lut_interpolate_red,
    .set_lut_interpolate_green  = set_lut_interpolate_green,
    .set_lut_interpolate_blue   = set_lut_interpolate_blue,
    .get_lut_fast_gamma         = get_lut_fast_gamma,
    .set_lut_fast_gamma         = set_lut_fast_gamma
};

/******************************************************************************
 * provideo_protocol_lut_init
 *****************************************************************************/
int provideo_protocol_lut_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_lut_register( handle, ctx, &provideo_lut_drv ) );
}

