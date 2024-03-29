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
 * @file    provideo_protocol_system.c
 *
 * @brief   ProVideo SYSTEM protocol implementations
 *
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_system.h>

#include <provideo_protocol/provideo_protocol_common.h>

/******************************************************************************
 * @brief command "version" 
 *****************************************************************************/
#define CMD_GET_VERSION                         ( "version\n" )

#define CMD_SYNC_VERSION_SYS_PLATFORM           ( "platform" )
#define CMD_VERSION_SYS_PLATFORM                ( "platform : %49[^\t\r\n]\n" )
#define CMD_GET_SYS_PLATFORM_NO_PARMS           ( 1 )

#define CMD_SYNC_VERSION_DEV_NAME               ( "device name" )
#define CMD_VERSION_DEV_NAME                    ( "device name : %49[^\t\r\n]\n" )
#define CMD_GET_DEV_NAME_NO_PARMS               ( 1 )
                                               
#define CMD_SYNC_VERSION_SYS_ID                 ( "system-id" )
#define CMD_VERSION_SYS_ID                      ( "system-id : %x-%x-%x-%x\n" )
#define CMD_GET_SYS_ID_NO_PARMS                 ( 4 )
                                               
#define CMD_SYNC_VERSION_HW_REVISION            ( "hw revision" )
#define CMD_VERSION_HW_REVISION                 ( "hw revision : %x\n" )
#define CMD_GET_HW_REVISION_NO_PARMS            ( 1 )
                                               
#define CMD_SYNC_VERSION_SYS_VALIDITY           ( "system validity" )
#define CMD_VERSION_SYS_VALIDITY                ( "system validity: %49[^\t\r\n]\n" )
#define CMD_GET_SYS_VALIDITY_NO_PARAMS          ( 1 )
                                               
#define CMD_SYNC_VERSION_FEAT_MASK_HW           ( "feature mask HW" )
#define CMD_VERSION_FEAT_MASK_HW                ( "feature mask HW: %x\n" )
#define CMD_GET_FEAT_MASK_HW_NO_PARMS           ( 1 )
                                               
#define CMD_SYNC_VERSION_FEAT_MASK_SW           ( "feature mask SW" )
#define CMD_VERSION_FEAT_MASK_SW                ( "feature mask SW: %x\n" )
#define CMD_GET_FEAT_MASK_SW_NO_PARMS           ( 1 )

#define CMD_SYNC_VERSION_RESOLUTION_MASK        ( "resolution mask" )
#define CMD_VERSION_RESOLUTION_MASK             ( "resolution mask: %x-%x-%x\n" )
#define CMD_GET_RESOLUTION_MASK_NO_PARMS        ( 3 )
                                               
#define CMD_SYNC_VERSION_LOADER_VERSION         ( "loader version" )
#define CMD_VERSION_LOADER_VERSION              ( "loader version : %i (%i)\n" )
#define CMD_GET_LOADER_VERSION_NO_PARMS         ( 2 )
                                               
#define CMD_SYNC_VERSION_SW_RELEASE_ID          ( "sw-release-id" )
#define CMD_VERSION_SW_RELEASE_ID               ( "sw-release-id  : %49[^\t\r\n]\n" )
#define CMD_GET_SW_RELEASE_ID_NO_PARMS          ( 1 )

#define CMD_SYNC_VERSION_SW_RELEASE_DATE        ( "sw-release-date" )
#define CMD_VERSION_SW_RELEASE_DATE             ( "sw-release-date: %49[^\t\r\n]\n" )
#define CMD_GET_SW_RELEASE_DATE_NO_PARMS        ( 1 )

#define CMD_SYNC_VERSION_SW_BUILD_DATE          ( "sw-build-date" )
#define CMD_VERSION_SW_BUILD_DATE               ( "sw-build-date : %49[^\t\r\n]\n" )
#define CMD_GET_SW_BUILD_DATE_NO_PARMS          ( 1 )

#define CMD_GET_VERSION_NUM_RESPONSE_LINES      ( 13 )

/******************************************************************************
 * @brief command "rs232_baud" 
 *****************************************************************************/
#define CMD_GET_RS232_BAUD                      ( "rs232_baud\n" )
#define CMD_SET_RS232_BAUD                      ( "rs232_baud %i\n" )
#define CMD_SYNC_RS232_BAUD                     ( "rs232_baud " )
#define CMD_GET_RS232_BAUD_NO_PARMS             ( 1 )

/******************************************************************************
 * @brief command "rs485_baud" 
 *****************************************************************************/
#define CMD_GET_RS485_BAUD                      ( "rs485_baud\n" )
#define CMD_SET_RS485_BAUD                      ( "rs485_baud %i\n" )
#define CMD_SYNC_RS485_BAUD                     ( "rs485_baud " )
#define CMD_GET_RS485_BAUD_NO_PARMS             ( 1 )

/******************************************************************************
 * @brief command "rs485_addr"
 *****************************************************************************/
#define CMD_GET_RS485_ADDR                      ( "rs485_addr\n" )
#define CMD_SET_RS485_ADDR                      ( "rs485_addr %i\n" )
#define CMD_SYNC_RS485_ADDR                     ( "rs485_addr " )
#define CMD_GET_RS485_ADDR_NO_PARMS             ( 1 )

/******************************************************************************
 * @brief command "rs485_bc_addr"
 *****************************************************************************/
#define CMD_GET_RS485_BC_ADDR                   ( "rs485_bc_addr\n" )
#define CMD_SET_RS485_BC_ADDR                   ( "rs485_bc_addr %i\n" )
#define CMD_SYNC_RS485_BC_ADDR                  ( "rs485_bc_addr " )
#define CMD_GET_RS485_BC_ADDR_NO_PARMS          ( 1 )

/******************************************************************************
 * @brief command "rs485_bc_master"
 *****************************************************************************/
#define CMD_GET_RS485_BC_MASTER                 ( "rs485_bc_master\n" )
#define CMD_SET_RS485_BC_MASTER                 ( "rs485_bc_master %i\n" )
#define CMD_SYNC_RS485_BC_MASTER                ( "rs485_bc_master " )
#define CMD_GET_RS485_BC_MASTER_NO_PARMS        ( 1 )

/******************************************************************************
 * @brief command "rs485_term"
 *****************************************************************************/
#define CMD_GET_RS485_TERMINATION               ( "rs485_term\n" )
#define CMD_SET_RS485_TERMINATION               ( "rs485_term %i\n" )
#define CMD_SYNC_RS485_TERMINATION              ( "rs485_term " )
#define CMD_GET_RS485_TERMINATION_NO_PARMS      ( 1 )

/******************************************************************************
 * @brief command "prompt" 
 *****************************************************************************/
#define CMD_GET_PROMPT                          ( "prompt\n" )
#define CMD_SET_PROMPT                          ( "prompt %i\n" )
#define CMD_SYNC_PROMPT                         ( "prompt " )
#define CMD_GET_PROMPT_NO_PARMS                 ( 1 )
#define CMD_GET_PROMPT_TMO                      ( 200 )
#define CMD_SET_PROMPT_TMO                      ( 200 )

/******************************************************************************
 * @brief command "debug" 
 *****************************************************************************/
#define CMD_GET_DEBUG                           ( "debug\n" )
#define CMD_SET_DEBUG                           ( "debug %i\n" )
#define CMD_SYNC_DEBUG                          ( "debug " )
#define CMD_GET_DEBUG_NO_PARMS                  ( 1 )

/******************************************************************************
 * @brief command "runtime" 
 *****************************************************************************/
#define CMD_GET_RUNTIME                         ( "runtime\n" )
#define CMD_SYNC_RUNTIME                        ( "runtime " )
#define CMD_GET_RUNTIME_RESPONSE                ( "runtime %i\n" )
#define CMD_GET_RUNTIME_NO_PARMS                ( 1 )

/******************************************************************************
 * @brief command "temp"
 *****************************************************************************/
#define CMD_GET_TEMP                            ( "temp %i\n" )
#define CMD_SYNC_TEMP                           ( "temp " )
#define CMD_GET_TEMP_RESPONSE                   ( "temp %i %f %15s\n" )
#define CMD_GET_TEMP_NO_PARMS                   ( 3 )

/******************************************************************************
 * @brief command "max_temp"
 *****************************************************************************/
#define CMD_GET_MAX_TEMP                        ( "max_temp\n" )
#define CMD_SYNC_MAX_TEMP                       ( "max_temp " )
#define CMD_GET_MAX_TEMP_RESPONSE               ( "max_temp %i %i %i\n")
#define CMD_GET_MAX_TEMP_NO_PARAMS              ( 3 )

/******************************************************************************
 * @brief command "max_temp_reset"
 *****************************************************************************/
#define CMD_MAX_TEMP_RESET                      ( "max_temp_reset\n" )

/******************************************************************************
 * @brief command "fan_speed"
 *****************************************************************************/
#define CMD_GET_FAN_SPEED                       ( "fan_speed\n" )
#define CMD_SYNC_FAN_SPEED                      ( "fan_speed " )
#define CMD_GET_FAN_SPEED_RESPONSE              ( "fan_speed %i\n")
#define CMD_GET_FAN_SPEED_NO_PARAMS             ( 1 )

/******************************************************************************
 * @brief command "fan_target"
 *****************************************************************************/
#define CMD_GET_FAN_TARGET                      ( "fan_target\n" )
#define CMD_SET_FAN_TARGET                      ( "fan_target %i\n" )
#define CMD_SYNC_FAN_TARGET                     ( "fan_target " )
#define CMD_GET_FAN_TARGET_NO_PARMS             ( 1 )

/******************************************************************************
 * @brief command "over_temp_count"
 *****************************************************************************/
#define CMD_GET_OVER_TEMP_COUNT                 ( "over_temp_count\n" )
#define CMD_SYNC_OVER_TEMP_COUNT                ( "over_temp_count " )
#define CMD_GET_OVER_TEMP_COUNT_RESPONSE        ( "over_temp_count %i\n")
#define CMD_GET_OVER_TEMP_COUNT_NO_PARAMS       ( 1 )

/******************************************************************************
 * @brief command "name"
 *****************************************************************************/
#define CMD_GET_NAME                            ( "name\n" )
#define CMD_GET_NAME_REPLY                      ( "name %49[^\t\r\n]\n" )
#define CMD_SET_NAME                            ( "name %s\n" )
#define CMD_SYNC_NAME                           ( "name " )
#define CMD_GET_NAME_NO_PARMS                   ( 1 )

/******************************************************************************
 * @brief command "identify"
 *****************************************************************************/
#define CMD_GET_DEVICE_LIST                     ( "identify\n" )
#define CMD_GET_DEVICE_LIST_REPLY               ( "id: %49s %u %u %u %49[^\t\r\n]\n%n" )
#define CMD_SYNC_DEVICE_LIST                    ( "id: ")
#define CMD_GET_DEVICE_LIST_NO_PARAMS           ( 5 )
#define CMD_DEVICE_LIST_MAX_DEVICES             ( 100 )     // Has to be equal to (MAX_DEVICE_ID + 1) which is defined in defines.h
#define CMD_GET_DEVICE_LIST_MAX_TMO             ( 3000 )

/******************************************************************************
 * @brief command "flush_buffers"
 *****************************************************************************/
#define CMD_FLUSH_BUFFERS                       ( "\n\n\n\n\n" )

/******************************************************************************
 * @brief command "reboot" 
 *****************************************************************************/
#define CMD_REBOOT                              ( "reboot\n" )

/******************************************************************************
 * @brief command "fw_update" 
 *****************************************************************************/
#define CMD_UPDATE                              ( "fw_update\n" )

/******************************************************************************
 * @brief command "save_settings" 
 *****************************************************************************/
#define CMD_SAVE_SETTINGS                       ( "save_settings %i\n" )
#define CMD_SAVE_SETTINGS_TMO                   ( 30000 )

/******************************************************************************
 * @brief command "load_settings" 
 *****************************************************************************/
#define CMD_LOAD_SETTINGS                       ( "load_settings %i\n" )
#define CMD_LOAD_SETTINGS_TMO                   ( 50000 )

/******************************************************************************
 * @brief command "default_settings"
 *****************************************************************************/
#define CMD_GET_DEFAULT_SETTINGS                ( "default_settings\n" )
#define CMD_SET_DEFAULT_SETTINGS                ( "default_settings %i\n" )
#define CMD_SYNC_DEFAULT_SETTINGS               ( "default_settings " )
#define CMD_DEFAULT_SETTINGS_TMO                ( 30000 )
#define CMD_GET_DEFAULT_SETTINGS_NO_PARMS       ( 1 )

/******************************************************************************
 * @brief command "reset_settings" 
 *****************************************************************************/
#define CMD_RESET_SETTINGS                      ( "reset_settings\n" )
#define CMD_RESET_SETTINGS_TMO                  ( 30000 )

/******************************************************************************
 * @brief command "copy_settings" 
 *****************************************************************************/
#define CMD_COPY_SETTINGS                       ( "copy_settings %i %i\n" )
#define CMD_COPY_SETTINGS_NO_PARMS              ( 2 )

/******************************************************************************
 * @brief command "dump_settings"
 *****************************************************************************/
#define CMD_GET_DEVICE_SETTINGS                 ( "dump_settings\n" )
#define CMD_SYNC_GET_DEVICE_SETTINGS            ( "dump_settings" )
#define CMD_SET_DEVICE_SETTINGS                 ( "dump_settings %2000[^\t\r\n]\n" )
#define CMD_GET_DEVICE_SETTINGS_NO_PARAMS       ( 1 )

#define CMD_GET_DEVICE_SETTINGS_RESPONSE_LINES  ( 2000 )

#define CMD_SET_SETTINGS                        ( "%s\n" )

#define CMD_GET_DEVICE_SETTINGS_TMO             ( 10000 )

/******************************************************************************
 * get_system_info
 *****************************************************************************/
static int get_system_info
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    char data[(CMD_GET_VERSION_NUM_RESPONSE_LINES*CMD_SINGLE_LINE_RESPONSE_SIZE)];

    int res;

    // parameter check
    if ( !no || !values )
    {
        return ( -EINVAL );
    }

    // send get-command to control channel
    ctrl_channel_send_request( channel, (uint8_t *)CMD_GET_VERSION, strlen(CMD_GET_VERSION) );

    // read response from provideo device
    res = evaluate_get_response( channel, data, sizeof(data) );
    if ( !res )
    {
        ctrl_protocol_version_t * version = (ctrl_protocol_version_t *)values;

        // clear version object
        memset( version, 0, sizeof(ctrl_protocol_version_t) );

        // set pointer to platform name
        char * s = strstr( data, CMD_SYNC_VERSION_SYS_PLATFORM );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_SYS_PLATFORM, version->system_platform );
            if ( res != CMD_GET_SYS_PLATFORM_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }

        // set pointer to device name
        s = strstr( data, CMD_SYNC_VERSION_DEV_NAME );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_DEV_NAME, version->device_name );
            if ( res != CMD_GET_DEV_NAME_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }

        // set pointer to system identifier 
        s = strstr( data, CMD_SYNC_VERSION_SYS_ID );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_SYS_ID,
                            &version->system_id[0],
                            &version->system_id[1],
                            &version->system_id[2],
                            &version->system_id[3] );
            if ( res != CMD_GET_SYS_ID_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }

        // set pointer to bitstream revision
        s = strstr( data, CMD_SYNC_VERSION_HW_REVISION );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_HW_REVISION, &version->hw_revision );
            if ( res != CMD_GET_HW_REVISION_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }

        // set pointer to system validity 
        s = strstr( data, CMD_SYNC_VERSION_SYS_VALIDITY );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_SYS_VALIDITY, version->system_validity );
            if ( res != CMD_GET_SYS_VALIDITY_NO_PARAMS )
            {
                return ( -EINVAL );
            }
        }

        // set pointer to feature mask hardware 
        s = strstr( data, CMD_SYNC_VERSION_FEAT_MASK_HW );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_FEAT_MASK_HW, &version->feature_mask_HW );
            if ( res != CMD_GET_FEAT_MASK_HW_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }
 
        // set pointer to feature mask software
        s = strstr( data, CMD_SYNC_VERSION_FEAT_MASK_SW );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_FEAT_MASK_SW, &version->feature_mask_SW );
            if ( res != CMD_GET_FEAT_MASK_SW_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }

        // set pointer to system identifier 
        s = strstr( data, CMD_SYNC_VERSION_RESOLUTION_MASK );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_RESOLUTION_MASK,
                            &version->resolution_mask[0],
                            &version->resolution_mask[1],
                            &version->resolution_mask[2] );
            if ( res != CMD_GET_RESOLUTION_MASK_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }

        // set pointer to feature mask software
        s = strstr( data, CMD_SYNC_VERSION_LOADER_VERSION );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_LOADER_VERSION,
                    &version->loader_version[0u], &version->loader_version[1u] );
            if ( res != CMD_GET_LOADER_VERSION_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }

        // set pointer to software release identifier 
        s = strstr( data, CMD_SYNC_VERSION_SW_RELEASE_ID );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_SW_RELEASE_ID, version->sw_release_id );
            if ( res != CMD_GET_SW_RELEASE_ID_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }

        // set pointer to software release date
        s = strstr( data, CMD_SYNC_VERSION_SW_RELEASE_DATE );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_SW_RELEASE_DATE, version->sw_release_date );
            if ( res != CMD_GET_SW_RELEASE_DATE_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }

        // set pointer to software build date
        s = strstr( data, CMD_SYNC_VERSION_SW_BUILD_DATE );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_SW_BUILD_DATE, version->sw_build_date );
            if ( res != CMD_GET_SW_BUILD_DATE_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }

        return ( 0 );
    }
    else
    {
        res = evaluate_error_response( data, res );
    }

    return ( res );
}

/******************************************************************************
 * get_system_platform
 *****************************************************************************/
static int get_system_platform
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             name
)
{
    (void) ctx;

    int res;

    // parameter check
    if ( !no || !name || (no != sizeof(ctrl_protocol_system_desc_t)) )
    {
        return ( -EINVAL );
    }

    // command call to get a string from provideo system
    res = get_param_string( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_SYS_PLATFORM, CMD_VERSION_SYS_PLATFORM, (char *)name );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_SYS_PLATFORM_NO_PARMS )
    {
        return ( -EFAULT );
    }
    
    return ( 0 );
}

/******************************************************************************
 * get_device_name
 *****************************************************************************/
static int get_device_name
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             name
)
{
    (void) ctx;

    int res;

    // parameter check
    if ( !no || !name || (no != sizeof(ctrl_protocol_system_desc_t)) )
    {
        return ( -EINVAL );
    }

    // command call to get a string from provideo system
    res = get_param_string( channel, 1, CMD_GET_NAME, CMD_SYNC_NAME, CMD_GET_NAME_REPLY, (char *)name );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_NAME_NO_PARMS )
    {
        return ( -EFAULT );
    }

    return ( 0 );
}

/******************************************************************************
 * get_device_name
 *****************************************************************************/
static int set_device_name
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             name
)
{
    (void) ctx;

    int res;

    // parameter check
    if ( !no || !name || (no > (int)sizeof(ctrl_protocol_system_desc_t)) )
    {
        return ( -EINVAL );
    }

    // command call to get a string from provideo system
    res = set_param_string( channel, CMD_SET_NAME, (char *)name );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    return ( 0 );
}

/******************************************************************************
 * get_system_id
 *****************************************************************************/
static int get_system_id
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    int value0;
    int value1;
    int value2;
    int value3;

    int res;

    ctrl_protocol_device_id_t * device_id;

    // parameter check
    if ( !no || !values || (no != sizeof(ctrl_protocol_device_id_t)) )
    {
        return ( -EINVAL );
    }
    
    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_SYS_ID, CMD_VERSION_SYS_ID, &value0, &value1, &value2, &value3 );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_SYS_ID_NO_PARMS )
    {
        return ( -EFAULT );
    }
    
    // type-cast to range
    device_id = (ctrl_protocol_device_id_t *)values;
    device_id->id0 = UINT32( value0 );
    device_id->id1 = UINT32( value1 );
    device_id->id2 = UINT32( value2 );
    device_id->id3 = UINT32( value3 );
    
    return ( 0 );
}

/******************************************************************************
 * get_system_validity
 *****************************************************************************/
static int get_system_validity
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             validity
)
{
    (void) ctx;

    int res;

    // parameter check
    if ( !no || !validity || (no != sizeof(ctrl_protocol_system_desc_t)) )
    {
        return ( -EINVAL );
    }

    // command call to get a string from provideo system
    res = get_param_string( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_SYS_VALIDITY, CMD_VERSION_SYS_VALIDITY, (char *)validity );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_SYS_VALIDITY_NO_PARAMS )
    {
        return ( -EFAULT );
    }
    
    return ( 0 );
}

/******************************************************************************
 * get_bitstream_version
 *****************************************************************************/
static int get_bitstream_version
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            version
)
{
    (void) ctx;

    int value;

    int res;

    // parameter check
    if ( !version )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_HW_REVISION, CMD_VERSION_HW_REVISION, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_HW_REVISION_NO_PARMS )
    {
        return ( -EFAULT );
    }
    
    // type-cast to range
    *version = UINT32( value );

    return ( 0 );
}

/******************************************************************************
 * get_bootloader_version
 *****************************************************************************/
static int get_bootloader_version
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            version
)
{
    (void) ctx;

    int value0;
    int value1;

    int res;

    // parameter check
    if ( !version )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_LOADER_VERSION, CMD_VERSION_LOADER_VERSION, &value0, &value1 );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_LOADER_VERSION_NO_PARMS )
    {
        return ( -EFAULT );
    }
    
    // type-cast to range
    *version = UINT32( value0 );

    return ( 0 );
}

/******************************************************************************
 * get_application_version
 *****************************************************************************/
static int get_application_version
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             version
)
{
    (void) ctx;

    int res;

    // parameter check
    if ( !no || !version || (no != sizeof(ctrl_protocol_system_desc_t)) )
    {
        return ( -EINVAL );
    }

    // command call to get a string from provideo system
    res = get_param_string( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_SW_RELEASE_ID, CMD_VERSION_SW_RELEASE_ID, (char *)version );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_SW_RELEASE_ID_NO_PARMS )
    {
        return ( -EFAULT );
    }
    
    return ( 0 );
}

/******************************************************************************
 * get_application_release_date
 *****************************************************************************/
static int get_application_release_date
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             date
)
{
    (void) ctx;

    int res;

    // parameter check
    if ( !no || !date || (no != sizeof(ctrl_protocol_system_desc_t)) )
    {
        return ( -EINVAL );
    }

    // command call to get a string from provideo system
    res = get_param_string( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_SW_RELEASE_DATE, CMD_VERSION_SW_RELEASE_DATE, (char *)date );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_SW_RELEASE_DATE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    return ( 0 );
}

/******************************************************************************
 * get_application_build_date
 *****************************************************************************/
static int get_application_build_date
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             date
)
{
    (void) ctx;

    int res;

    // parameter check
    if ( !no || !date || (no != sizeof(ctrl_protocol_system_desc_t)) )
    {
        return ( -EINVAL );
    }

    // command call to get a string from provideo system
    res = get_param_string( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_SW_BUILD_DATE, CMD_VERSION_SW_BUILD_DATE, (char *)date );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_SW_BUILD_DATE_NO_PARMS )
    {
        return ( -EFAULT );
    }

    return ( 0 );
}

/******************************************************************************
 * get_feature_mask_hw
 *****************************************************************************/
static int get_feature_mask_hw
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            mask
)
{
    (void) ctx;

    int value;

    int res;

    // parameter check
    if ( !mask )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_FEAT_MASK_HW, CMD_VERSION_FEAT_MASK_HW, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_FEAT_MASK_HW_NO_PARMS )
    {
        return ( -EFAULT );
    }
    
    // type-cast to range
    *mask = UINT32( value );

    return ( 0 );
}

/******************************************************************************
 * get_feature_mask_sw
 *****************************************************************************/
static int get_feature_mask_sw
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            mask
)
{
    (void) ctx;

    int value;

    int res;

    // parameter check
    if ( !mask )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_FEAT_MASK_SW, CMD_VERSION_FEAT_MASK_SW, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_FEAT_MASK_SW_NO_PARMS )
    {
        return ( -EFAULT );
    }
    
    // type-cast to range
    *mask = UINT32( value );

    return ( 0 );
}

/******************************************************************************
 * get_resolution_mask
 *****************************************************************************/
static int get_resolution_mask
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    int value0;
    int value1;
    int value2;

    int res;

    ctrl_protocol_resolution_id_t * res_id;

    // parameter check
    if ( !no || !values || (no != sizeof(ctrl_protocol_resolution_id_t)) )
    {
        return ( -EINVAL );
    }
    
    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_RESOLUTION_MASK, CMD_VERSION_RESOLUTION_MASK, &value0, &value1, &value2 );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_RESOLUTION_MASK_NO_PARMS )
    {
        return ( -EFAULT );
    }
    
    // type-cast to range
    res_id = (ctrl_protocol_resolution_id_t *)values;
    res_id->id0 = UINT32( value0 );
    res_id->id1 = UINT32( value1 );
    res_id->id2 = UINT32( value2 );
    
    return ( 0 );
}

/******************************************************************************
 * get_rs232_baud - to get baud-rate of RS232 terminal interface
 *****************************************************************************/
static int get_rs232_baud
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            baudrate
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !baudrate )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_RS232_BAUD, CMD_SYNC_RS232_BAUD, CMD_SET_RS232_BAUD, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_RS232_BAUD_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *baudrate = UINT32( value );

    return ( 0 );
}

/******************************************************************************
 * set_rs232_baud - function sets baud-rate of RS232 terminal interface 
 *****************************************************************************/
static int set_rs232_baud
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t const              baudrate
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_RS232_BAUD, INT( baudrate ) ) );
}

/******************************************************************************
 * get_rs485_baud - to get baud-rate of RS485 terminal interface
 *****************************************************************************/
static int get_rs485_baud
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            baudrate
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !baudrate )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_RS485_BAUD, CMD_SYNC_RS485_BAUD, CMD_SET_RS485_BAUD, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_RS485_BAUD_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *baudrate = UINT32( value );

    return ( 0 );
}

/******************************************************************************
 * set_rs485_baud - function sets baud-rate of RS485 terminal interface 
 *****************************************************************************/
static int set_rs485_baud
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t const              baudrate
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_RS485_BAUD, INT( baudrate ) ) );
}

/******************************************************************************
 * get_rs485_addr - to get address of RS485 device
 *****************************************************************************/
static int get_rs485_addr
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            address 
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !address )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_RS485_ADDR, CMD_SYNC_RS485_ADDR, CMD_SET_RS485_ADDR, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_RS485_ADDR_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *address = UINT32( value );

    return ( 0 );
}

/******************************************************************************
 * set_rs485_addr - function sets RS485 slave address
 *****************************************************************************/
static int set_rs485_addr
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t const              address
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_RS485_ADDR, INT( address ) ) );
}

/******************************************************************************
 * get_rs485_bc_addr - to get broadcast address of RS485 device
 *****************************************************************************/
static int get_rs485_bc_addr
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            broadcast_address
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !broadcast_address )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_RS485_BC_ADDR, CMD_SYNC_RS485_BC_ADDR, CMD_SET_RS485_BC_ADDR, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_RS485_ADDR_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *broadcast_address = UINT32( value );

    return ( 0 );
}

/******************************************************************************
 * set_rs485_bc_addr - function sets RS485 broadcast address
 *****************************************************************************/
static int set_rs485_bc_addr
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t const              broadcast_address
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_RS485_BC_ADDR, INT( broadcast_address ) ) );
}

/******************************************************************************
 * get_rs485_bc_master - to get broadcast master mode of RS485 device
 *****************************************************************************/
static int get_rs485_bc_master
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             is_enabled
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !is_enabled )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_RS485_BC_MASTER, CMD_SYNC_RS485_BC_MASTER, CMD_SET_RS485_BC_MASTER, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_RS485_BC_MASTER_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *is_enabled = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_rs485_bc_master - function enables broadcast master mode on the RS485
 *                       slave device that has the given address
 *****************************************************************************/
static int set_rs485_bc_master
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int32_t const               master_address
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_RS485_BC_MASTER, INT( master_address ) ) );
}

/******************************************************************************
 * get_rs485_bc_master - function to get the current RS485 termination state
 *****************************************************************************/
static int get_rs485_termination
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             enabled
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !enabled )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_RS485_TERMINATION, CMD_SYNC_RS485_TERMINATION, CMD_SET_RS485_TERMINATION, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_RS485_TERMINATION_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *enabled = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_rs485_bc_master - function that enables or disabled the termination
 *                       of the RS485 port
 *****************************************************************************/
static int set_rs485_termination
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               enable
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_RS485_TERMINATION, INT( enable ) ) );
}

/******************************************************************************
 * get_device_list - gets the list of devices which are connected to this com
 *                   port
 *****************************************************************************/
static int get_device_list
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             buffer
)
{
    (void) ctx;

    char command[CMD_SINGLE_LINE_COMMAND_SIZE];
    char buf[CMD_SINGLE_LINE_RESPONSE_SIZE];
    char data[CMD_SINGLE_LINE_RESPONSE_SIZE*2];

    struct timespec start, now;
    int loop = 1;
    int cnt = 0;

    if ( (no != sizeof(ctrl_protocol_device_t) * CMD_DEVICE_LIST_MAX_DEVICES) || !buffer )
    {
        return ( -EINVAL );
    }

    // Read timeout from buffer
    uint32_t timeout = *((uint32_t*)buffer);
    if ( timeout == 0 || timeout > CMD_GET_DEVICE_LIST_MAX_TMO)
    {
        timeout = CMD_GET_DEVICE_LIST_MAX_TMO;
    }

    ctrl_protocol_device_t * device_list = (ctrl_protocol_device_t *)buffer;

    // clear command buffer
    memset( command, 0, sizeof(command) );

    // create command to send
    sprintf( command, CMD_GET_DEVICE_LIST );

    // send get-command to control channel
    ctrl_channel_send_request( channel, (uint8_t *)command, (int)strlen(command) );

    // clear data buffer
    memset( data, 0, sizeof(data) );
    data[0] = '\0';

    // counter to check for buffer overflow
    unsigned int data_count = 0;

    // start timer
    get_time_monotonic( &start );

    // wait for answer from COM-Port
    while ( loop )
    {
        int n;

        // poll for data (NOTE: reserve last byte for '\0')
        memset( buf, 0, sizeof(buf) );
        n = ctrl_channel_receive_response( channel, (uint8_t *)buf, (sizeof(buf) - 1u) );

        // evaluate number of received data
        if ( n > 0 )
        {
            // always put a "null" at the end of a string
            buf[n] = '\0';

            // increment data counter and check for overflow
            data_count += (unsigned int)n;
            if ( data_count >= sizeof(data) )
            {
                return ( -ENOMEM );
            }

            // append poll buffer to receive buffer
            strcat( data, buf );

            // consume all commands from buffer
            // (therefore get start position of first command)
            char * s = strstr( data, CMD_SYNC_DEVICE_LIST );
            while ( s )
            {
                // parse command
                int offset = 0;
                ctrl_protocol_device_t device;
                int res = sscanf( s, CMD_GET_DEVICE_LIST_REPLY, device.device_platform,
                                                                &device.rs485_address,
                                                                &device.rs485_bc_address,
                                                                &device.rs485_bc_master,
                                                                device.device_name,
                                                                &offset);

                /* Note: the check for offset != 0 is needed, because on Windows it
                 * seems to be possible that sscanf() has a result != 0 (meaning it has
                 * found parameters and parsed them) but still report an offset of 0. */
                if ( (res == CMD_GET_DEVICE_LIST_NO_PARAMS) )// && (offset != 0) && (s[offset-1] == '\n') ) // TODO:check why condition "(offset != 0) && (s[offset-1] == '\n')" not working
                {
                    if ( cnt >= CMD_DEVICE_LIST_MAX_DEVICES )
                    {
                        return ( -ENOMEM );
                    }

                    memcpy(  &device_list[cnt], &device, sizeof(device) );
                    cnt++;

                    // decrement data counter by bytes taken out
                    unsigned int num_bytes_processed = (unsigned int)(s + offset - data);
                    data_count -= num_bytes_processed;

                    // move out the processed command
                    // Note: Use memmove instead of strncpy, because dst and src overlap!
                    memmove( data, (s+offset), data_count );

                    // clear moved out data to make sure buffer is free of garbage
                    memset((data + data_count), 0, num_bytes_processed );

                    // search next command in buffer
                    s = strstr( data, CMD_SYNC_DEVICE_LIST );
                }
                else
                {
                    s = NULL;
                }
            }
        }
        else
        {
            // timeout handling
            get_time_monotonic( &now );
            int diff_ms = (int)((now.tv_sec - start.tv_sec) * 1000 + (now.tv_nsec - start.tv_nsec) / 1000000);
            loop = (diff_ms > (int)timeout) ? 0 : 1;
        }
    }

    // check if last id answer was complete and valid
    if ( strstr( data, CMD_OK ) )
    {
        return ( 0 );
    }

    // check if last id answer was complete and error-message
    else if ( strstr( data, CMD_FAIL ) )
    {
        return ( evaluate_error_response( data, -EINVAL ) );
    }

    return ( -EILSEQ );
}

/******************************************************************************
 * get_prompt - to get the enable state of console prompt
 *****************************************************************************/
static int get_prompt
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
    res = get_param_int_X_with_tmo( channel, 2,
            CMD_GET_PROMPT, CMD_SYNC_PROMPT, CMD_SET_PROMPT, CMD_SET_PROMPT_TMO, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_PROMPT_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *flag = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_prompt - function sets console prompt enable state
 *****************************************************************************/
static int set_prompt
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               flag
)
{
    (void) ctx;

    return ( set_param_int_X_with_tmo( channel, CMD_SET_PROMPT, CMD_SET_PROMPT_TMO, INT( flag ) ) );
}

/******************************************************************************
 * get_debug - to get the current debug level of a connected device 
 *****************************************************************************/
static int get_debug
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             level
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !level )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_DEBUG, CMD_SYNC_DEBUG, CMD_SET_DEBUG, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_DEBUG_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *level = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_debug - function sets debug level on a connected device
 *****************************************************************************/
static int set_debug
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               level
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_DEBUG, INT( level ) ) );
}

/******************************************************************************
 * get_runtime - function to get the runtime counter
 *****************************************************************************/
static int get_runtime
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            cnt
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !cnt )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_RUNTIME, CMD_SYNC_RUNTIME, CMD_GET_RUNTIME_RESPONSE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_RUNTIME_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *cnt = UINT32( value );

    return ( 0 );
}

/******************************************************************************
 * get_temp - function to get the value of a temperature sensor
 *****************************************************************************/
static int get_temp
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

    ctrl_protocol_temp_t * temp;

    int res;

    // parameter check
    if ( !no || !values || (sizeof(*temp) != no) )
    {
        return ( -EINVAL );
    }

    temp = (ctrl_protocol_temp_t *)values;

    // clear command buffer
    memset( command, 0, sizeof(command) );

    sprintf( command, CMD_GET_TEMP, temp->id );

    // send command to COM port
    ctrl_channel_send_request( channel, (uint8_t *)command, (int)strlen(command) );

    // read response from provideo device
    res = evaluate_get_response( channel, data, sizeof(data) );
    if ( !res )
    {
        char * s = strstr( data, CMD_SYNC_TEMP );
        if ( s )
        {
            int id;
            float value;
            char name[16];
            res = sscanf( s, CMD_GET_TEMP_RESPONSE, &id, &value, name );
            if ( (res == CMD_GET_TEMP_NO_PARMS) && (UINT32(id) == temp->id) )
            {
                temp->temp = value;
                strcpy( temp->name, name );
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

    if ( res < 0 )
    {
        return ( res );
    }

    return ( 0 );
}

/******************************************************************************
 * get_max_temp - function to get the maximum logged temperature
 *****************************************************************************/
static int get_max_temp
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    int32_t * const             values
)
{
    (void) ctx;

    int current_value_user, current_value_persistent;
    int max_value;
    int res;

    // parameter check
    if ( no != CMD_GET_MAX_TEMP_NO_PARAMS || !values  )
    {
        return ( -EINVAL );
    }

    // command call to get 2 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_MAX_TEMP, CMD_SYNC_MAX_TEMP, CMD_GET_MAX_TEMP_RESPONSE, &current_value_user,
                                                                            &current_value_persistent,
                                                                            &max_value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_MAX_TEMP_NO_PARAMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    values[0] = INT32( current_value_user );
    values[1] = INT32( current_value_persistent );
    values[2] = INT32( max_value );

    return ( 0 );
}

/******************************************************************************
 * max_temp_reset - reset the maximum logged temperature to the current temp
 *****************************************************************************/
static int max_temp_reset
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;
    
    return ( set_param_0( channel, CMD_MAX_TEMP_RESET ) );
}

/******************************************************************************
 * fan_speed - red the current cooling fan speed in %
 *****************************************************************************/
static int get_fan_speed
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
            CMD_GET_FAN_SPEED, CMD_SYNC_FAN_SPEED, CMD_GET_FAN_SPEED_RESPONSE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_RUNTIME_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *speed = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * get_fan_target - get the target system temperature of the fan control
 *****************************************************************************/
static int get_fan_target
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t * const             target
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !target )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_FAN_TARGET, CMD_SYNC_FAN_TARGET, CMD_SET_FAN_TARGET, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_FAN_TARGET_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *target = UINT8( value );

    return ( 0 );
}

/******************************************************************************
 * set_fan_target - set the target system temperature of the fan control
 *****************************************************************************/
static int set_fan_target
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t const               target
)
{
    (void) ctx;

    return ( set_param_int_X( channel, CMD_SET_FAN_TARGET, INT( target ) ) );
}


/******************************************************************************
 * get_max_temp - function to get the maximum logged temperature
 *****************************************************************************/
static int get_over_temp_count
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint32_t * const            count
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !count )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_OVER_TEMP_COUNT, CMD_SYNC_OVER_TEMP_COUNT, CMD_GET_OVER_TEMP_COUNT_RESPONSE, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_RUNTIME_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *count = UINT32( value );

    return ( 0 );
}

/******************************************************************************
 * flush_buffers - flush the device buffers of the connected device
 *****************************************************************************/
static int flush_buffers
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    // send data buffer to control channel
    ctrl_channel_send_request( channel, (uint8_t *)CMD_FLUSH_BUFFERS, strlen( CMD_FLUSH_BUFFERS ) );

    return 0;
}

/******************************************************************************
 * reboot - reboot the connected device
 *****************************************************************************/
static int reboot
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0( channel, CMD_REBOOT ) );
}

/******************************************************************************
 * update - switch device into firmware update mode
 *****************************************************************************/
static int update
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0( channel, CMD_UPDATE ) );
}

/******************************************************************************
 * save_settings - save current settings on device
 *****************************************************************************/
static int save_settings
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t userSetting
)
{
    (void) ctx;

    return ( set_param_int_X_with_tmo( channel, CMD_SAVE_SETTINGS, CMD_SAVE_SETTINGS_TMO, INT( userSetting ) ) );
}

/******************************************************************************
 * load_settings - load saved settings on device
 *****************************************************************************/
static int load_settings
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t userSetting
)
{
    (void) ctx;

    return ( set_param_int_X_with_tmo( channel, CMD_LOAD_SETTINGS, CMD_LOAD_SETTINGS_TMO, INT( userSetting ) ) );
}

/******************************************************************************
 * set_default_settings - set default settings on device
 *****************************************************************************/
static int set_default_settings
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    uint8_t userSetting
)
{
    (void) ctx;

    return ( set_param_int_X_with_tmo( channel, CMD_SET_DEFAULT_SETTINGS, CMD_DEFAULT_SETTINGS_TMO, INT( userSetting ) ) );
}

/******************************************************************************
 * get_default_settings - get default settings from device
 *****************************************************************************/
static int get_default_settings
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int8_t * const             userSetting
)
{
    (void) ctx;

    int value;
    int res;

    // parameter check
    if ( !userSetting )
    {
        return ( -EINVAL );
    }

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, 2,
            CMD_GET_DEFAULT_SETTINGS, CMD_SYNC_DEFAULT_SETTINGS, CMD_SET_DEFAULT_SETTINGS, &value );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_DEFAULT_SETTINGS_NO_PARMS )
    {
        return ( -EFAULT );
    }

    // type-cast to range
    *userSetting = INT8( value );

    return ( 0 );
}

/******************************************************************************
 * reset_settings - do a factory reset 
 *****************************************************************************/
static int reset_settings
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0_with_tmo( channel, CMD_RESET_SETTINGS, CMD_RESET_SETTINGS_TMO ) );
}

/******************************************************************************
 * copy_settings - copy settings from one chain to another
 *****************************************************************************/
static int copy_settings
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;

    // parameter check
    if ( !no || !values || (no != CMD_COPY_SETTINGS_NO_PARMS) )
    {
        return ( -EINVAL );
    }

    return ( set_param_int_X( channel, CMD_COPY_SETTINGS, INT( values[0] ), INT( values[1] ) ) );
}

/******************************************************************************
 * get_device_settings
 *****************************************************************************/
static int get_device_settings
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             settings
)
{
    (void) ctx;

    int res;

    // parameter check
    if ( !no || !settings || (no != sizeof(ctrl_protocol_system_settings_desc_t)) )
    {
        return ( -EINVAL );
    }

    // command call to get a string from provideo system
    res = get_param_settings_string( channel, CMD_GET_DEVICE_SETTINGS_RESPONSE_LINES,
            CMD_GET_DEVICE_SETTINGS, CMD_SYNC_GET_DEVICE_SETTINGS, CMD_SET_DEVICE_SETTINGS, (char *)settings );

    return ( 0 );
}

/******************************************************************************
 * set_device_settings
 *****************************************************************************/
static int set_device_settings
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             settings
)
{
    (void) ctx;

    int res;

    // parameter check
    if ( !no || !settings || (no > (int)sizeof(ctrl_protocol_system_sett_t)) )
    {
        return ( -EINVAL );
    }

    // command call to send a string to provideo system
    res = set_param_string_with_tmo( channel, CMD_SET_SETTINGS, (char *)settings , CMD_GET_DEVICE_SETTINGS_TMO);

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    return ( 0 );

}

/******************************************************************************
 * System protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_sys_drv_t provideo_sys_drv = 
{
    .get_system_info              = get_system_info,
    .get_system_platform          = get_system_platform,
    .get_device_name              = get_device_name,
    .set_device_name              = set_device_name,
    .get_system_id                = get_system_id,
    .get_system_validity          = get_system_validity,
    .get_bitstream_version        = get_bitstream_version,
    .get_bootloader_version       = get_bootloader_version,
    .get_application_version      = get_application_version,
    .get_application_release_date = get_application_release_date,
    .get_application_build_date   = get_application_build_date,
    .get_feature_mask_hw          = get_feature_mask_hw,
    .get_feature_mask_sw          = get_feature_mask_sw,
    .get_resolution_mask          = get_resolution_mask,
    .get_rs232_baud               = get_rs232_baud,
    .set_rs232_baud               = set_rs232_baud,
    .get_rs485_baud               = get_rs485_baud,
    .set_rs485_baud               = set_rs485_baud,
    .get_rs485_addr               = get_rs485_addr,
    .set_rs485_addr               = set_rs485_addr,
    .get_rs485_bc_addr            = get_rs485_bc_addr,
    .set_rs485_bc_addr            = set_rs485_bc_addr,
    .get_rs485_bc_master          = get_rs485_bc_master,
    .set_rs485_bc_master          = set_rs485_bc_master,
    .get_rs485_termination        = get_rs485_termination,
    .set_rs485_termination        = set_rs485_termination,
    .get_device_list              = get_device_list,
    .get_prompt                   = get_prompt,
    .set_prompt                   = set_prompt,
    .get_debug                    = get_debug,
    .set_debug                    = set_debug,
    .get_runtime                  = get_runtime,
    .get_temp                     = get_temp,
    .get_max_temp                 = get_max_temp,
    .max_temp_reset               = max_temp_reset,
    .get_fan_speed                = get_fan_speed,
    .get_fan_target               = get_fan_target,
    .set_fan_target               = set_fan_target,
    .get_over_temp_count          = get_over_temp_count,
    .flush_buffers                = flush_buffers,
    .reboot                       = reboot,
    .update                       = update,
    .save_settings                = save_settings,
    .load_settings                = load_settings,
    .set_default_settings         = set_default_settings,
    .get_default_settings         = get_default_settings,
    .reset_settings               = reset_settings,
    .copy_settings                = copy_settings,
    .get_device_settings          = get_device_settings,
    .set_device_settings          = set_device_settings,
};

/******************************************************************************
 * provideo_protocol_sys_init
 *****************************************************************************/
int provideo_protocol_sys_init
(
    ctrl_protocol_handle_t const handle,
    void * const                 ctx
)
{
    return ( ctrl_protocol_sys_register( handle, ctx, &provideo_sys_drv ) );
}
