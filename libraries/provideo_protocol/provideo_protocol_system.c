/******************************************************************************
 *
 * Copyright 2015, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
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
#define CMD_VERSION_SYS_PLATFORM                ( "platform : %s\n" )

#define CMD_SYNC_VERSION_DEV_NAME               ( "device name" )
#define CMD_VERSION_DEV_NAME                    ( "device name : %[^\r\t\n]\n" )
                                               
#define CMD_SYNC_VERSION_SYS_ID                 ( "system-id" )
#define CMD_VERSION_SYS_ID                      ( "system-id : %x-%x-%x-%x\n" )
                                               
#define CMD_SYNC_VERSION_HW_REVISION            ( "hw revision" )
#define CMD_VERSION_HW_REVISION                 ( "hw revision : %x\n" )
                                               
#define CMD_SYNC_VERSION_SYS_VALIDITY           ( "system validity" )
#define CMD_VERSION_SYS_VALIDITY                ( "system validity: %[^\t\r\n]\n" )
                                               
#define CMD_SYNC_VERSION_FEAT_MASK_HW           ( "feature mask HW" )
#define CMD_VERSION_FEAT_MASK_HW                ( "feature mask HW: %x\n" )
                                               
#define CMD_SYNC_VERSION_FEAT_MASK_SW           ( "feature mask SW" )
#define CMD_VERSION_FEAT_MASK_SW                ( "feature mask SW: %x\n" )

#define CMD_SYNC_VERSION_RESOLUTION_MASK        ( "resolution mask" )
#define CMD_VERSION_RESOLUTION_MASK             ( "resolution mask: %x-%x-%x\n" )
                                               
#define CMD_SYNC_VERSION_LOADER_VERS            ( "loader version" )
#define CMD_VERSION_LOADER_VERS                 ( "loader version : %i (%i)\n" )
                                               
#define CMD_SYNC_VERSION_SW_RELEASE_ID          ( "sw-release-id" )
#define CMD_VERSION_SW_RELEASE_ID               ( "sw-release-id  : %[^\t\r\n]\n" )

#define CMD_SYNC_VERSION_SW_RELEASE_DATE        ( "sw-release-date" )
#define CMD_VERSION_SW_RELEASE_DATE             ( "sw-release-date: %[^\t\r\n]\n" )

#define CMD_SYNC_VERSION_SW_BUILD_DATE          ( "sw-build-date" )
#define CMD_VERSION_SW_BUILD_DATE               ( "sw-build-date : %[^\t\r\n]\n" )
                                               
#define CMD_VERSION_SW_RELEASE_ID_NUMBER_0      ( "v%i_%i_%i" )
#define CMD_VERSION_SW_RELEASE_ID_NUMBER_1      ( "v%i_%i_%i" )

#define CMD_GET_SYSTEM_PLATFORM_NO_PARMS            ( 1 )
#define CMD_GET_DEVICE_NAME_NO_PARMS                ( 1 )
#define CMD_GET_SYSTEM_ID_NO_PARMS                  ( 4 )
#define CMD_GET_BITSTREAM_VERSION_NO_PARMS          ( 1 )
#define CMD_GET_BOOTLOADER_VERSION_NO_PARMS         ( 2 )
#define CMD_GET_APPLICATION_VERSION_NO_PARMS        ( 1 )
#define CMD_GET_APPLICATION_RELEASE_DATE_NO_PARMS   ( 1 )
#define CMD_GET_APPLICATION_BUILD_DATE_NO_PARMS     ( 1 )
#define CMD_GET_FEAT_MASK_HW_NO_PARMS               ( 1 )
#define CMD_GET_FEAT_MASK_SW_NO_PARMS               ( 1 )
#define CMD_GET_RESOLUTION_MASK_NO_PARMS            ( 3 )

#define CMD_GET_VERSION_NUM_RESPONSE_LINES          ( 13 )

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
 * @brief command "prompt" 
 *****************************************************************************/
#define CMD_GET_PROMPT                          ( "prompt\n" )
#define CMD_SET_PROMPT                          ( "prompt %i\n" )
#define CMD_SYNC_PROMPT                         ( "prompt " )
#define CMD_GET_PROMPT_NO_PARMS                 ( 1 )
#define CMD_GET_PROMPT_TMO                      ( 20 )
#define CMD_SET_PROMPT_TMO                      ( 20 )

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
#define CMD_SET_RUNTIME                         ( "runtime %i\n" )
#define CMD_SYNC_RUNTIME                        ( "runtime " )
#define CMD_GET_RUNTIME_NO_PARMS                ( 1 )

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
#define CMD_SAVE_SETTINGS                       ( "save_settings\n" )
#define CMD_SAVE_SETTINGS_TMO                   ( 500 )

/******************************************************************************
 * @brief command "load_settings" 
 *****************************************************************************/
#define CMD_LOAD_SETTINGS                       ( "load_settings\n" )
#define CMD_LOAD_SETTINGS_TMO                   ( 30000 )

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
            if ( res != CMD_GET_SYSTEM_PLATFORM_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }

        // set pointer to device name
        s = strstr( data, CMD_SYNC_VERSION_DEV_NAME );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_DEV_NAME, version->device_name );
            if ( res != CMD_GET_DEVICE_NAME_NO_PARMS )
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
            if ( res != CMD_GET_SYSTEM_ID_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }

        // set pointer to bitstream revision
        s = strstr( data, CMD_SYNC_VERSION_HW_REVISION );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_HW_REVISION, &version->hw_revision );
            if ( res != CMD_GET_BITSTREAM_VERSION_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }

        // set pointer to system validity 
        s = strstr( data, CMD_SYNC_VERSION_SYS_VALIDITY );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_SYS_VALIDITY, version->system_validity );
            if ( res != 1 )
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
        s = strstr( data, CMD_SYNC_VERSION_LOADER_VERS );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_LOADER_VERS,
                    &version->loader_version[0u], &version->loader_version[1u] );
            if ( res != CMD_GET_BOOTLOADER_VERSION_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }

        // set pointer to software release identifier 
        s = strstr( data, CMD_SYNC_VERSION_SW_RELEASE_ID );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_SW_RELEASE_ID, version->sw_release_id_NAME );
            if ( res != CMD_GET_APPLICATION_VERSION_NO_PARMS )
            {
                return ( -EINVAL );
            }
        }

        // set pointer to software release date
        s = strstr( data, CMD_SYNC_VERSION_SW_RELEASE_DATE );
        if ( s )
        {
            res = sscanf( s, CMD_VERSION_SW_RELEASE_DATE, version->sw_release_date );
            if ( res != CMD_GET_APPLICATION_RELEASE_DATE_NO_PARMS )
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

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_SYS_PLATFORM, CMD_VERSION_SYS_PLATFORM, (char *)name );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_SYSTEM_PLATFORM_NO_PARMS )
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

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_DEV_NAME, CMD_VERSION_DEV_NAME, (char *)name );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_SYSTEM_PLATFORM_NO_PARMS )
    {
        return ( -EFAULT );
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
    else if ( res != CMD_GET_SYSTEM_ID_NO_PARMS )
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

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_SYS_VALIDITY, CMD_VERSION_SYS_VALIDITY, (char *)validity );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_APPLICATION_VERSION_NO_PARMS )
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
    else if ( res != CMD_GET_BITSTREAM_VERSION_NO_PARMS )
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
            CMD_GET_VERSION, CMD_SYNC_VERSION_LOADER_VERS, CMD_VERSION_LOADER_VERS, &value0, &value1 );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_BOOTLOADER_VERSION_NO_PARMS )
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

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_SW_RELEASE_ID, CMD_VERSION_SW_RELEASE_ID, (char *)version );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_APPLICATION_VERSION_NO_PARMS )
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

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_SW_RELEASE_DATE, CMD_VERSION_SW_RELEASE_DATE, (char *)date );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_APPLICATION_RELEASE_DATE_NO_PARMS )
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

    // command call to get 1 parameter from provideo system
    res = get_param_int_X( channel, CMD_GET_VERSION_NUM_RESPONSE_LINES,
            CMD_GET_VERSION, CMD_SYNC_VERSION_SW_BUILD_DATE, CMD_VERSION_SW_BUILD_DATE, (char *)date );

    // return error code
    if ( res < 0 )
    {
        return ( res );
    }

    // return -EFAULT if number of parameter not matching
    else if ( res != CMD_GET_APPLICATION_BUILD_DATE_NO_PARMS )
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
 * get_runtime - to get the tuntime counter
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
            CMD_GET_RUNTIME, CMD_SYNC_RUNTIME, CMD_SET_RUNTIME, &value );

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
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0_with_tmo( channel, CMD_SAVE_SETTINGS, CMD_SAVE_SETTINGS_TMO ) );
}

/******************************************************************************
 * load_settings - load saved settings on device
 *****************************************************************************/
static int load_settings
(
    void * const                ctx,
    ctrl_channel_handle_t const channel
)
{
    (void) ctx;

    return ( set_param_0_with_tmo( channel, CMD_LOAD_SETTINGS, CMD_LOAD_SETTINGS_TMO ) );
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
 * System protocol driver declaration
 *****************************************************************************/
static ctrl_protocol_sys_drv_t provideo_sys_drv = 
{
    .get_system_info              = get_system_info,
    .get_system_platform          = get_system_platform,
    .get_device_name              = get_device_name,
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
    .get_prompt                   = get_prompt,
    .set_prompt                   = set_prompt,
    .get_debug                    = get_debug,
    .set_debug                    = set_debug,
    .get_runtime                  = get_runtime,
    .reboot                       = reboot,
    .update                       = update,
    .save_settings                = save_settings,
    .load_settings                = load_settings,
    .reset_settings               = reset_settings,
    .copy_settings                = copy_settings,
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

