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
 * @file    provideo_protocol_common.h
 *
 * @brief   ProVideo internal definitions
 *
 *****************************************************************************/
#ifndef __PROVIDEO_PROTOCOL_COMMON_H__
#define __PROVIDEO_PROTOCOL_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <ctrl_protocol/ctrl_protocol.h>

#include <time.h>

/******************************************************************************
 * @defgroup command_common common protocol definitions
 * @{
 *****************************************************************************/

/******************************************************************************
 * @brief compute number of array-items
 *****************************************************************************/
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)       ( sizeof(x)/sizeof(x[0]) )
#endif

/******************************************************************************
 * @brief type-cast to a signed int
 *****************************************************************************/
#ifndef INT
#define INT(x)              ((int)x)
#endif

/******************************************************************************
 * @brief type-cast to unsigned 8 bit integer value
 *****************************************************************************/
#ifndef UINT8
#define UINT8(x)            ((uint8_t)x)
#endif

/******************************************************************************
 * @brief type-cast to signed 8 bit integer value
 *****************************************************************************/
#ifndef INT8
#define INT8(x)             ((int8_t)x)
#endif

/******************************************************************************
 * @brief type-cast to unsigned 16 bit integer value
 *****************************************************************************/
#ifndef UINT16
#define UINT16(x)           ((uint16_t)x)
#endif

/******************************************************************************
 * @brief type-cast to signed 16 bit integer value
 *****************************************************************************/
#ifndef INT16
#define INT16(x)            ((int16_t)x)
#endif

/******************************************************************************
 * @brief type-cast to unsigned 32 bit integer value
 *****************************************************************************/
#ifndef UINT32
#define UINT32(x)           ((uint32_t)x)
#endif

/******************************************************************************
 * @brief type-cast to signed 32 bit integer value
 *****************************************************************************/
#ifndef INT32
#define INT32(x)            ((int32_t)x)
#endif

/******************************************************************************
 * @brief Default command time-out 100 milliseconds
 *****************************************************************************/
#define DEFAULT_CMD_TIMEOUT                 ( 400 )

/******************************************************************************
 * @brief max buffer size of a single line command 
 *****************************************************************************/
#define CMD_SINGLE_LINE_COMMAND_SIZE        ( 128u )

/******************************************************************************
 * @brief max buffer size of a single line response 
 *****************************************************************************/
#define CMD_SINGLE_LINE_RESPONSE_SIZE       ( 200u )

/******************************************************************************
 * @brief max buffer size of a multi line response 
 *****************************************************************************/
#define CMD_MULTI_LINE_RESPONSE_SIZE        ( 80u )

/******************************************************************************
 * @brief command execution successful
 *****************************************************************************/
#define CMD_OK                              ( "OK" )

/******************************************************************************
 * @brief command execution failed 
 *****************************************************************************/
#define CMD_FAIL                            ( "FAIL" )

/******************************************************************************
 * @brief new line
 *****************************************************************************/
#define CMD_NEW_LINE                        ( "\n\r" )

/******************************************************************************
 * @brief command error message "parameter out of range" sent by provideo device
 *****************************************************************************/
#define CMD_ERROR_OUT_OF_RANGE              ( "ERROR: param out of range" )

/******************************************************************************
 * @brief command error message "invalid number of parameters" sent by provideo device
 *****************************************************************************/
#define CMD_ERROR_INVALID_NUMBER_PARAMS     ( "ERROR: invalid number of params" )

/******************************************************************************
 * @brief command error message "resource busy" sent by provideo device
 *****************************************************************************/
#define CMD_ERROR_RESSOURCE_BUSY            ( "ERROR: resource busy" )

/******************************************************************************
 * @brief command error message "operation not supported" sent by provideo device
 *****************************************************************************/
#define CMD_ERROR_OPERATION_NOT_SUPPORTED   ( "ERROR: operation not supported" )

/******************************************************************************
 * @brief command error message "Hardware missmatch" sent by provideo device
 *****************************************************************************/
#define CMD_ERROR_LICENSE_INVALID           ( "ERROR: Hardware missmatch. Please contact support." )

/******************************************************************************
 * @brief command error message "invalid chain" sent by provideo device
 *****************************************************************************/
#define CMD_ERROR_INVALID_CHAIN             ( "ERROR: invalid chain" )

/******************************************************************************
 * @brief command error message "gen-lock error" sent by provideo device
 *****************************************************************************/
#define CMD_ERROR_GENLOCK                   ( "ERROR: gen-lock error" )

/******************************************************************************
 * @brief Get a monotinic timestamp
 *
 * @param[out]   time    returned timestamp
 *
 * @return 0 on success, error code otherwise
 *****************************************************************************/
int get_time_monotonic
(
    struct timespec * time
);

/******************************************************************************
 * @brief Evaluate error messages from provideo device with the default timeout
 *
 * @param[in]   data    received response data from provideo device
 * @param[in]   res     default error value to return
 *
 * @return specific error code on match, default error value otherwise
 *****************************************************************************/
int evaluate_error_response
(
    char *  data,
    int     res
);

/******************************************************************************
 * @brief Evaluate a set-command response of a provideo-device with the
 *        default timeout
 *
 * @param[in]   channel control channel to use
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int evaluate_set_response
(
    ctrl_channel_handle_t const channel
);

/******************************************************************************
 * @brief Evaluate response of a provideo-device to a set command
 *        with a specific timeout
 *
 * @param[in]   channel control channel to use
 * @param[in]   tmo_ms  time in ms that is waited until timeout (command failed)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int evaluate_set_response_with_tmo
(
    ctrl_channel_handle_t const channel,
    int const                   tmo_ms
);

/******************************************************************************
 * @brief Evaluate a get-command response of a provideo-device
 *
 * @param[in]   channel control channel to use
 * @param[in]   data    data buffer to fill
 * @param[in]   len     amount of bytes to read
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int evaluate_get_response
(
    ctrl_channel_handle_t const channel,
    char *                      data,
    int                         len
);

/******************************************************************************
 * @brief Evaluate a get-command response of a provideo-device
 *        with a specific timeout
 *
 * @param[in]   channel control channel to use
 * @param[in]   data    data buffer to fill
 * @param[in]   len     amount of bytes to read
 * @param[in]   tmo     time-out to use in ms
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int evaluate_get_response_with_tmo
(
    ctrl_channel_handle_t const channel,
    char *                      data,
    int                         len,
    int const                   tmo_ms
);

/******************************************************************************
 * @brief  Send a data buffer via control channel (null terminated)
 *
 * @param[in]   channel control channel to use
 * @param[in]   data    data buffer to send
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int set_param_0
(
    ctrl_channel_handle_t const channel,
    char * const                data
);

/******************************************************************************
 * @brief  Send a data buffer via control channel (null terminated)
 *
 * @param[in]   channel control channel to use
 * @param[in]   data    data buffer to send
 * @param[in]   tmo     time-out to use in ms
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int set_param_0_with_tmo
(
    ctrl_channel_handle_t const channel,
    char * const                data,
    int const                   tmo_ms
);

/******************************************************************************
 * @brief Sends a given get-command to provideo device and parses device
 *        response for a string
 *
 * @param[in]   channel         control channel to use
 * @param[in]   lines           expected number of response lines
 * @param[in]   cmd_get         command string to readout parameter fron device
 * @param[in]   cmd_sync        command string to sync response buffer
 * @param[in]   cmd_set         formatted command string to parse response buffer
 * @param[in]   param           pointer to returned string
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int get_param_string
(
    ctrl_channel_handle_t const  channel,
    int const                    lines,
    char * const                 cmd_get,
    char * const                 cmd_sync,
    char * const                 cmd_set,
    char *                       param
);

/******************************************************************************
 * @brief Sends a given set-command to provideo device and transmitts a string
 *
 * @param[in]   channel         control channel to use
 * @param[in]   cmd_set         formatted command string
 * @param[in]   string          string that shall be send after cmd
 *****************************************************************************/
int set_param_string
(
    ctrl_channel_handle_t const channel,
    char * const                cmd_set,
    char * const                string
);

/******************************************************************************
 * @brief Sends a given get-command to provideo device and parses device 
 *        response for a variable number of integer values
 *
 * @param[in]   channel         control channel to use
 * @param[in]   lines           expected number of response lines
 * @param[in]   cmd_get         command string to readout parameter fron device
 * @param[in]   cmd_sync        command string to sync response buffer
 * @param[in]   cmd_set         formatted command string to parse response buffer
 * @param[in]   ...             list of integer parameter
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int get_param_int_X
(
    ctrl_channel_handle_t const     channel,
    int const                       lines,
    char * const                    cmd_get,
    char * const                    cmd_sync,
    char * const                    cmd_set,
    ...
);

/******************************************************************************
 * @brief Sends a given get-command to provideo device and parses device
 *        response for a variable number of integer values with a specified
 *        timeout in ms
 *
 * @param[in]   channel         control channel to use
 * @param[in]   lines           expected number of response lines
 * @param[in]   cmd_get         command string to readout parameter fron device
 * @param[in]   cmd_sync        command string to sync response buffer
 * @param[in]   cmd_set         formatted command string to parse response buffer
 * @param[in]   cmd_timeout_ms  time in ms that is waited until timeout (command failed)
 * @param[in]   ...             list of integer parameter
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int get_param_int_X_with_tmo
(
    ctrl_channel_handle_t const     channel,
    int const                       lines,
    char * const                    cmd_get,
    char * const                    cmd_sync,
    char * const                    cmd_set,
    int const                       cmd_timeout_ms,
    ...
);

/******************************************************************************
 * @brief Send a set command with a variable bumber of interger parameters
 *
 * @param[in]   channel control channel to use
 * @param[in]   cmd_set         formatted command string
 * @param[in]   ...             list of integer parameter
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int set_param_int_X
(
    ctrl_channel_handle_t const     channel,
    char * const                    cmd_set,
    ...
);

/******************************************************************************
 * @brief Send a set command with X interger parameters
 *        with a specified timeout in ms
 *
 * @param[in]   channel control channel to use
 * @param[in]   cmd_set         formatted command string
 * @param[in]   cmd_timeout_ms  time in ms that is waited until timeout (command failed)
 * @param[in]   ...             list of integer parameter
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int set_param_int_X_with_tmo
(
    ctrl_channel_handle_t const channel,
    char * const                cmd_set,
    int const                   cmd_timeout_ms,
    ...
);

/* @} command_common */

#ifdef __cplusplus
}
#endif

#endif /* __PROVIDEO_CMDS_COMMON_H__ */

