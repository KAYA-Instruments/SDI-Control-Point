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
 * @file    provideo_protocol_common.c
 *
 * @brief   Implementation of provideo protocol helper functions
 *
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <ctrl_channel/ctrl_channel.h>

#include <provideo_protocol/provideo_protocol_common.h>

#define USE_CUSTOM_GET_TIME
#ifdef USE_CUSTOM_GET_TIME
/* Mahr: Note on "clock_gettime":
 * Using clock_gettime() on non POSIX systems (windows) is tricky. Acutally MinGW should support it,
 * but while cross-compiling using MXE it still was not able to link it no matter what I tried.
 * So we now use a custom get time function for windows, which can be found here:
 * https://stackoverflow.com/questions/5404277/porting-clock-gettime-to-windows */
#ifdef _WIN32
#include <windows.h>

static LARGE_INTEGER getFILETIMEoffset()
{
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;

    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return (t);
}

static int clock_gettime_windows(struct timeval *tv)
{
    LARGE_INTEGER           t;
    FILETIME                f;
    double                  microseconds;
    static LARGE_INTEGER    offset;
    static double           frequencyToMicroseconds;
    static int              initialized = 0;
    static BOOL             usePerformanceCounter = 0;

    if (!initialized) {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
        } else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter) QueryPerformanceCounter(&t);
    else {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }

    t.QuadPart -= offset.QuadPart;
    microseconds = (double)t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = microseconds;
    tv->tv_sec = t.QuadPart / 1000000;
    tv->tv_usec = t.QuadPart % 1000000;
    return (0);
}
#endif // _WIN32
#endif // USE_CUSTOM_GET_TIME

/******************************************************************************
 * get_time_monotonic - Platform dependend clock_gettime function
 *****************************************************************************/
static int get_time_monotonic(struct timespec * time)
{
    int ret = 0;

#if defined(_WIN32) && defined(USE_CUSTOM_GET_TIME)
    struct timeval tv;
    ret = clock_gettime_windows( &tv );
    time->tv_sec = tv.tv_sec;
    time->tv_nsec = tv.tv_usec * 1000;
#else
    ret = clock_gettime(CLOCK_MONOTONIC, time);
#endif

    return ret;
}

/******************************************************************************
 * evaluate_error_response - evaluate error message from provideo device
 *****************************************************************************/
int evaluate_error_response
(
    char *  data,
    int     res
)
{
    if ( strstr( data, CMD_ERROR_OUT_OF_RANGE ) )
    {
        return ( -EINVAL );
    }

    if ( strstr( data, CMD_ERROR_INVALID_NUMBER_PARAMS ) )
    {
        return ( -ERANGE );
    }

    if ( strstr( data, CMD_ERROR_RESSOURCE_BUSY ) )
    {
        return ( -EBUSY );
    }

    if ( strstr( data, CMD_ERROR_OPERATION_NOT_SUPPORTED ) )
    {
        return ( -ENOSYS );
    }

    if ( strstr( data, CMD_ERROR_LICENSE_INVALID ) )
    {
        return ( -EACCES );
    }

    if ( strstr( data, CMD_ERROR_INVALID_CHAIN ) )
    {
        return ( -ENODEV );
    }

    if ( strstr( data, CMD_ERROR_GENLOCK ) )
    {
        return ( -ENOSYS );
    }

    return ( res );
}

/******************************************************************************
 * evaluate_set_response - evaluate response of a provideo-device
 *                         to a set command with the default timeout
 *****************************************************************************/
int evaluate_set_response
(
    ctrl_channel_handle_t const channel
)
{
    return evaluate_set_response_with_tmo( channel, DEFAULT_CMD_TIMEOUT );
}

/******************************************************************************
 * evaluate_set_response_with_tmo - evaluate response of a provideo-device 
 * to a set command with a specific timeout
 *****************************************************************************/
int evaluate_set_response_with_tmo
(
    ctrl_channel_handle_t const channel,
    int const                   tmo_ms
)
{
    char buf[CMD_SINGLE_LINE_RESPONSE_SIZE];
    char data[CMD_SINGLE_LINE_RESPONSE_SIZE*2];

    struct timespec start, now;
    int loop = 1;

    // start timer
    get_time_monotonic( &start );

    // set received_string to zero
    memset( data, 0u, sizeof(data) );

    // wait for answer from COM-Port
    while ( loop )
    {
        int n;
        
        // clear poll buffer
        memset( buf, 0u, sizeof(buf) );

        // poll for data data (NOTE: reserve last byte for '\0')
        n = ctrl_channel_receive_response( channel, (uint8_t *)buf, (sizeof(buf) - 1u) );

        // evaluate number of received data
        if ( n > 0 )
        {
            // always put a "null" at the end of a string
            buf[n] = '\0';

            // check for buffer overflow
            if ( strlen(data) + strlen(buf) >= sizeof(data) )
            {
                // device is sending more data than expected -> throw away old data
                memset( data, 0u, sizeof(data) );
            }

            // append poll buffer to receive buffer
            strcat( data, buf );

            // check if string is complete and valid
            if ( strstr( data, CMD_OK ) )
            {
                return ( 0 );
            }

            // check if string is complete and error-message
            else if ( strstr( data, CMD_FAIL ) )
            {
                return ( evaluate_error_response( data, -EINVAL ) );
            }

            // else stay in loop
            else
            {
                // do nothing
            }
        }
        else
        {
            // timeout handling
            get_time_monotonic( &now );
            int diff_ms = (now.tv_sec - start.tv_sec) * 1000 + (now.tv_nsec - start.tv_nsec) / 1000000;
            loop = (diff_ms > tmo_ms) ? 0 : 1;
        }
    }

    return ( -EILSEQ );
}

/******************************************************************************
 * evaluate_get_response - evaluate response of a provideo-device to a get command
 *****************************************************************************/
int evaluate_get_response
(
    ctrl_channel_handle_t const channel,
    char *                      data,
    int                         len
)
{
    return evaluate_get_response_with_tmo( channel, data, len, DEFAULT_CMD_TIMEOUT );
}

/******************************************************************************
 * evaluate_get_response - evaluate response of a provideo-device
 * to a get command with a specific timeout in ms
 *****************************************************************************/
int evaluate_get_response_with_tmo
(
    ctrl_channel_handle_t const channel,
    char *                      data,
    int                         len,
    int const                   tmo_ms
)
{
    struct timespec start, now;
    int loop = 1;
    int i = 0;

    // start timer
    get_time_monotonic( &start );

    // set received_string and buffer to zero
    memset( data, 0u, len );

    // wait for answer from COM-Port
    while ( loop )
    {
        int n;

        // poll for data data (NOTE: reserve last byte for '\0')
        n = ctrl_channel_receive_response( channel, (uint8_t *)&data[i], (len - i - 1)  );

        // evaluate number of received data
        if ( n > 0 )
        {
            i += n;

            // check data buffer size
            if ( i >= len )
            {
                return ( -EINVAL );
            }

            // check if string is complete and valid
            if ( strstr( data, CMD_OK ) )
            {
                return ( 0 );
            }

            // check if string is complete and error-message
            else if ( strstr( data, CMD_FAIL ) )
            {
                return ( -EINVAL );
            }

            // else stay in loop
            else
            {
                // do nothing
            }
        }
        else
        {
            // timeout handling
            get_time_monotonic( &now );
            int diff_ms = (now.tv_sec - start.tv_sec) * 1000 + (now.tv_nsec - start.tv_nsec) / 1000000;
            loop = (diff_ms > tmo_ms) ? 0 : 1;
        }
    }

    return ( -EILSEQ );
}

/******************************************************************************
 * set_param_0 - Send a parameterless command
 *****************************************************************************/
int set_param_0
(
    ctrl_channel_handle_t const  channel, 
    char * const                 data
)
{
    // send data buffer to control channel
    ctrl_channel_send_request( channel, (uint8_t *)data, strlen( data ) );

    // wait for response and evaluate it
    return ( evaluate_set_response( channel ) );
}

/******************************************************************************
 * set_param_0_with_tmo  - Send a parameterless command and use timeout
 *****************************************************************************/
int set_param_0_with_tmo
(
    ctrl_channel_handle_t const  channel, 
    char * const                 data,
    int const                    tmo_ms
)
{
    // send data buffer to control channel
    ctrl_channel_send_request( channel, (uint8_t *)data, strlen( data ) );

    // wait for response and evaluate it
    return ( evaluate_set_response_with_tmo( channel, tmo_ms ) );
}

/******************************************************************************
 * get_param_int_X - Sends a given get-command to provideo device and parses
 *                   device response for a variable number of integer values
 *****************************************************************************/
int get_param_int_X
(
    ctrl_channel_handle_t const  channel,
    int const                    lines,
    char * const                 cmd_get,
    char * const                 cmd_sync,
    char * const                 cmd_set,
    ...
)
{
    char data[(lines*CMD_SINGLE_LINE_RESPONSE_SIZE)];

    va_list args;

    int res;

    // send get-command to control channel
    ctrl_channel_send_request( channel, (uint8_t *)cmd_get, strlen(cmd_get) );

    // read response from provideo device
    res = evaluate_get_response( channel, data, sizeof(data) );
    if ( !res )
    {
        // get start position of command
        char * s = strstr( data, cmd_sync );
        if ( s )
        {
            // parse command
            va_start( args, cmd_set );
            res = vsscanf( s, cmd_set, args );
            va_end( args );
           
            return ( res );
        }
        else
        {
            // command not found in received data
            return ( -EFAULT );
        }
    }
    else
    {
        res = evaluate_error_response( data, res );
    }

    return ( res );
}

/******************************************************************************
 * get_param_int_X - Sends a given get-command to provideo device and parses
 *                   device response for a variable number of integer values
 *                   with a specified timeout in ms
 *****************************************************************************/
int get_param_int_X_with_tmo
(
    ctrl_channel_handle_t const  channel,
    int const                    lines,
    char * const                 cmd_get,
    char * const                 cmd_sync,
    char * const                 cmd_set,
    int const                    cmd_timeout_ms,
    ...
)
{
    char data[(lines*CMD_SINGLE_LINE_RESPONSE_SIZE)];

    va_list args;

    int res;

    // send get-command to control channel
    ctrl_channel_send_request( channel, (uint8_t *)cmd_get, strlen(cmd_get) );

    // read response from provideo device
    res = evaluate_get_response_with_tmo( channel, data, sizeof(data), cmd_timeout_ms );
    if ( !res )
    {
        // get start position of command
        char * s = strstr( data, cmd_sync );
        if ( s )
        {
            // parse command
            va_start( args, cmd_timeout_ms );
            res = vsscanf( s, cmd_set, args );
            va_end( args );

            return ( res );
        }
        else
        {
            // command not found in received data
            return ( -EFAULT );
        }
    }
    else
    {
        res = evaluate_error_response( data, res );
    }

    return ( res );
}

/******************************************************************************
 * set_param_int_X - Send a set command with X interger parameters
 *****************************************************************************/
int set_param_int_X
(
    ctrl_channel_handle_t const channel, 
    char * const                cmd_set,
    ...
)
{
    char command[CMD_SINGLE_LINE_COMMAND_SIZE];

    va_list args;

    int res;
    
    // clear command buffer
    memset( command, 0, sizeof(command) );

    // create command to send
    va_start( args, cmd_set );
    res = vsnprintf( command, sizeof(command), cmd_set, args );
    va_end( args );

    // prevent buffer overrun
    if ( res >= INT(sizeof(command)) )
    {
        return ( -EFAULT );
    }
    
    // send command to COM port
    ctrl_channel_send_request( channel, (uint8_t *)command, strlen(command) );

    // wait for response and evaluate
    return ( evaluate_set_response( channel ) );
}

/******************************************************************************
 * set_param_int_X - Send a set command with X interger parameters
 *                   with a specified timeout in ms
 *****************************************************************************/
int set_param_int_X_with_tmo
(
    ctrl_channel_handle_t const channel,
    char * const                cmd_set,
    int const                   cmd_timeout_ms,
    ...
)
{
    char command[CMD_SINGLE_LINE_COMMAND_SIZE];

    va_list args;

    int res;

    // clear command buffer
    memset( command, 0, sizeof(command) );

    // create command to send
    va_start( args, cmd_timeout_ms );
    res = vsnprintf( command, sizeof(command), cmd_set, args );
    va_end( args );

    // prevent buffer overrun
    if ( res >= INT(sizeof(command)) )
    {
        return ( -EFAULT );
    }

    // send command to COM port
    ctrl_channel_send_request( channel, (uint8_t *)command, strlen(command) );

    // wait for response and evaluate
    return ( evaluate_set_response_with_tmo( channel, cmd_timeout_ms ) );
}

