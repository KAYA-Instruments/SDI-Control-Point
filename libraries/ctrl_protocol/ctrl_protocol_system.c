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
 * @file    ctrl_protocol_system.c
 *
 * @brief   Implementation of generic system functions
 *
 *****************************************************************************/
#include <stdio.h>
#include <errno.h>

#include <ctrl_channel/ctrl_channel.h>
#include <ctrl_protocol/ctrl_protocol.h>

#include "ctrl_protocol_priv.h"

/******************************************************************************
 * @brief Macro for type-casting to function driver 
 *****************************************************************************/
#define SYS_DRV( drv )      ((ctrl_protocol_sys_drv_t *)drv)

/******************************************************************************
 * ctrl_protocol_get_system_info
 *****************************************************************************/
int ctrl_protocol_get_system_info
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_system_info );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( SYS_DRV(protocol->drv)->get_system_info( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_system_platform
 *****************************************************************************/
int ctrl_protocol_get_system_platform
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_system_platform );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( SYS_DRV(protocol->drv)->get_system_platform( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_device_name
 *****************************************************************************/
int ctrl_protocol_get_device_name
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_device_name );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( SYS_DRV(protocol->drv)->get_device_name( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_set_device_name
 *****************************************************************************/
int ctrl_protocol_set_device_name
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), set_device_name );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( SYS_DRV(protocol->drv)->set_device_name( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_system_id
 *****************************************************************************/
int ctrl_protocol_get_system_id
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_system_id );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( SYS_DRV(protocol->drv)->get_system_id( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_system_validity
 *****************************************************************************/
int ctrl_protocol_get_system_validity
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              validity
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_system_validity );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( validity );
    return ( SYS_DRV(protocol->drv)->get_system_validity( protocol->ctx, channel, no, validity ) );
}

/******************************************************************************
 * ctrl_protocol_get_bitstream_version
 *****************************************************************************/
int ctrl_protocol_get_bitstream_version
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             version 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_bitstream_version );
    CHECK_NOT_NULL( version );
    return ( SYS_DRV(protocol->drv)->get_bitstream_version( protocol->ctx, channel, version ) );
}

/******************************************************************************
 * ctrl_protocol_get_bootloader_version
 *****************************************************************************/
int ctrl_protocol_get_bootloader_version
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             version 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_bootloader_version );
    CHECK_NOT_NULL( version );
    return ( SYS_DRV(protocol->drv)->get_bootloader_version( protocol->ctx, channel, version ) );
}

/******************************************************************************
 * ctrl_protocol_get_application_version
 *****************************************************************************/
int ctrl_protocol_get_application_version
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              version
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_application_version );
    CHECK_NOT_NULL( version );
    return ( SYS_DRV(protocol->drv)->get_application_version( protocol->ctx, channel, no, version ) );
}

/******************************************************************************
 * ctrl_protocol_get_application_release_date
 *****************************************************************************/
int ctrl_protocol_get_application_release_date
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              date
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_application_release_date );
    CHECK_NOT_NULL( date );
    return ( SYS_DRV(protocol->drv)->get_application_release_date( protocol->ctx, channel, no, date ) );
}

/******************************************************************************
 * ctrl_protocol_get_application_build_date
 *****************************************************************************/
int ctrl_protocol_get_application_build_date
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              date
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_application_build_date );
    CHECK_NOT_NULL( date );
    return ( SYS_DRV(protocol->drv)->get_application_build_date( protocol->ctx, channel, no, date ) );
}

/******************************************************************************
 * ctrl_protocol_get_feature_mask_hw
 *****************************************************************************/
int ctrl_protocol_get_feature_mask_hw
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             mask
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_feature_mask_hw );
    CHECK_NOT_NULL( mask );
    return ( SYS_DRV(protocol->drv)->get_feature_mask_hw( protocol->ctx, channel, mask ) );
}

/******************************************************************************
 * ctrl_protocol_get_feature_mask_sw
 *****************************************************************************/
int ctrl_protocol_get_feature_mask_sw
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             mask
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_feature_mask_sw );
    CHECK_NOT_NULL( mask );
    return ( SYS_DRV(protocol->drv)->get_feature_mask_sw( protocol->ctx, channel, mask ) );
}

/******************************************************************************
 * ctrl_protocol_get_resolution_mask
 *****************************************************************************/
int ctrl_protocol_get_resolution_mask
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_resolution_mask );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( SYS_DRV(protocol->drv)->get_resolution_mask( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_rs232_baud
 *****************************************************************************/
int ctrl_protocol_get_rs232_baud
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             baudrate
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_rs232_baud );
    CHECK_NOT_NULL( baudrate );
    return ( SYS_DRV(protocol->drv)->get_rs232_baud( protocol->ctx, channel, baudrate ) );
}

/******************************************************************************
 * ctrl_protocol_set_rs232_baud
 *****************************************************************************/
int ctrl_protocol_set_rs232_baud
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               baudrate
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), set_rs232_baud );
    return ( SYS_DRV(protocol->drv)->set_rs232_baud( protocol->ctx, channel, baudrate ) );
}

/******************************************************************************
 * ctrl_protocol_get_rs485_baud
 *****************************************************************************/
int ctrl_protocol_get_rs485_baud
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             baudrate
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_rs485_baud );
    CHECK_NOT_NULL( baudrate );
    return ( SYS_DRV(protocol->drv)->get_rs485_baud( protocol->ctx, channel, baudrate ) );
}

/******************************************************************************
 * ctrl_protocol_set_rs485_baud
 *****************************************************************************/
int ctrl_protocol_set_rs485_baud
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               baudrate
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), set_rs485_baud );
    return ( SYS_DRV(protocol->drv)->set_rs485_baud( protocol->ctx, channel, baudrate ) );
}

/******************************************************************************
 * ctrl_protocol_get_rs485_addr
 *****************************************************************************/
int ctrl_protocol_get_rs485_addr
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             address 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_rs485_addr );
    CHECK_NOT_NULL( address );
    return ( SYS_DRV(protocol->drv)->get_rs485_addr( protocol->ctx, channel, address ) );
}

/******************************************************************************
 * ctrl_protocol_set_rs485_addr
 *****************************************************************************/
int ctrl_protocol_set_rs485_addr
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint32_t const               address
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), set_rs485_addr );
    return ( SYS_DRV(protocol->drv)->set_rs485_addr( protocol->ctx, channel, address ) );
}

/******************************************************************************
 * ctrl_protocol_get_rs485_bc_addr
 *****************************************************************************/
int ctrl_protocol_get_rs485_bc_addr
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint32_t * const             broadcast_address
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_rs485_bc_addr );
    CHECK_NOT_NULL( broadcast_address );
    return ( SYS_DRV(protocol->drv)->get_rs485_bc_addr( protocol->ctx, channel, broadcast_address ) );
}

/******************************************************************************
 * ctrl_protocol_set_rs485_bc_addr
 *****************************************************************************/
int ctrl_protocol_set_rs485_bc_addr
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint32_t const               broadcast_address
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), set_rs485_bc_addr );
    return ( SYS_DRV(protocol->drv)->set_rs485_bc_addr( protocol->ctx, channel, broadcast_address ) );
}

/******************************************************************************
 * ctrl_protocol_get_rs485_bc_master
 *****************************************************************************/
int ctrl_protocol_get_rs485_bc_master
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              is_master
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_rs485_bc_master );
    CHECK_NOT_NULL( is_master );
    return ( SYS_DRV(protocol->drv)->get_rs485_bc_master( protocol->ctx, channel, is_master ) );
}

/******************************************************************************
 * ctrl_protocol_set_rs485_addr
 *****************************************************************************/
int ctrl_protocol_set_rs485_bc_master
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int32_t const                master_address
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), set_rs485_bc_master );
    return ( SYS_DRV(protocol->drv)->set_rs485_bc_master( protocol->ctx, channel, master_address ) );
}

/******************************************************************************
 * ctrl_protocol_get_rs485_termination
 *****************************************************************************/
int ctrl_protocol_get_rs485_termination
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              enabled
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_rs485_termination );
    CHECK_NOT_NULL( enabled );
    return ( SYS_DRV(protocol->drv)->get_rs485_termination( protocol->ctx, channel, enabled ) );
}

/******************************************************************************
 * ctrl_protocol_set_rs485_termination
 *****************************************************************************/
int ctrl_protocol_set_rs485_termination
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                enable
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), set_rs485_termination );
    return ( SYS_DRV(protocol->drv)->set_rs485_termination( protocol->ctx, channel, enable ) );
}

/******************************************************************************
 * ctrl_protocol_set_rs485_addr
 *****************************************************************************/
int ctrl_protocol_get_device_list
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              buffer
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_device_list );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( buffer );
    return ( SYS_DRV(protocol->drv)->get_device_list( protocol->ctx, channel, no, buffer ) );
}

/******************************************************************************
 * ctrl_protocol_get_prompt
 *****************************************************************************/
int ctrl_protocol_get_prompt
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              flag
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_prompt );
    CHECK_NOT_NULL( flag );
    return ( SYS_DRV(protocol->drv)->get_prompt( protocol->ctx, channel, flag ) );
}

/******************************************************************************
 * ctrl_protocol_set_prompt
 *****************************************************************************/
int ctrl_protocol_set_prompt
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                flag
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), set_prompt );
    return ( SYS_DRV(protocol->drv)->set_prompt( protocol->ctx, channel, flag ) );
}

/******************************************************************************
 * ctrl_protocol_get_debug
 *****************************************************************************/
int ctrl_protocol_get_debug
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              level 
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_debug );
    CHECK_NOT_NULL( level );
    return ( SYS_DRV(protocol->drv)->get_debug( protocol->ctx, channel, level ) );
}

/******************************************************************************
 * ctrl_protocol_set_debug
 *****************************************************************************/
int ctrl_protocol_set_debug
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                level
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), set_debug );
    return ( SYS_DRV(protocol->drv)->set_debug( protocol->ctx, channel, level ) );
}

/******************************************************************************
 * ctrl_protocol_get_runtime
 *****************************************************************************/
int ctrl_protocol_get_runtime
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             cnt
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_runtime );
    CHECK_NOT_NULL( cnt );
    return ( SYS_DRV(protocol->drv)->get_runtime( protocol->ctx, channel, cnt ) );
}

/******************************************************************************
 * ctrl_protocol_get_temp
 *****************************************************************************/
int ctrl_protocol_get_temp
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_temp );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( SYS_DRV(protocol->drv)->get_temp( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_get_max_temp
 *****************************************************************************/
int ctrl_protocol_get_max_temp
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int32_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_max_temp );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( SYS_DRV(protocol->drv)->get_max_temp( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_max_temp_reset
 *****************************************************************************/
int ctrl_protocol_max_temp_reset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), max_temp_reset );
    return ( SYS_DRV(protocol->drv)->max_temp_reset( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_get_fan_speed
 *****************************************************************************/
int ctrl_protocol_get_fan_speed
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              speed
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_fan_speed );
    CHECK_NOT_NULL( speed );
    return ( SYS_DRV(protocol->drv)->get_fan_speed( protocol->ctx, channel, speed ) );
}

/******************************************************************************
 * ctrl_protocol_get_fan_target
 *****************************************************************************/
int ctrl_protocol_get_fan_target
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              target
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_fan_target );
    CHECK_NOT_NULL( target );
    return ( SYS_DRV(protocol->drv)->get_fan_target( protocol->ctx, channel, target ) );
}

/******************************************************************************
 * ctrl_protocol_set_fan_target
 *****************************************************************************/
int ctrl_protocol_set_fan_target
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                target
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), set_fan_target );
    return ( SYS_DRV(protocol->drv)->set_fan_target( protocol->ctx, channel, target ) );
}

/******************************************************************************
 * ctrl_protocol_get_over_temp_count
 *****************************************************************************/
int ctrl_protocol_get_over_temp_count
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint32_t * const             count
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), get_over_temp_count );
    CHECK_NOT_NULL( count );
    return ( SYS_DRV(protocol->drv)->get_over_temp_count( protocol->ctx, channel, count ) );
}

/******************************************************************************
 * ctrl_protocol_flush_buffers
 *****************************************************************************/
int ctrl_protocol_flush_buffers
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), flush_buffers );
    return ( SYS_DRV(protocol->drv)->flush_buffers( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_reboot
 *****************************************************************************/
int ctrl_protocol_reboot
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), reboot );
    return ( SYS_DRV(protocol->drv)->reboot( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_update
 *****************************************************************************/
int ctrl_protocol_update
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), update );
    return ( SYS_DRV(protocol->drv)->update( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_save_settings
 *****************************************************************************/
int ctrl_protocol_save_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), save_settings );
    return ( SYS_DRV(protocol->drv)->save_settings( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_load_settings
 *****************************************************************************/
int ctrl_protocol_load_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), load_settings );
    return ( SYS_DRV(protocol->drv)->load_settings( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_reset_settings
 *****************************************************************************/
int ctrl_protocol_reset_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), reset_settings );
    return ( SYS_DRV(protocol->drv)->reset_settings( protocol->ctx, channel ) );
}

/******************************************************************************
 * ctrl_protocol_copy_settings
 *****************************************************************************/
int ctrl_protocol_copy_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
)
{
    CHECK_HANDLE( protocol );
    CHECK_DRV_FUNC( SYS_DRV(protocol->drv), copy_settings );
    CHECK_NOT_NULL( no );
    CHECK_NOT_NULL( values );
    return ( SYS_DRV(protocol->drv)->copy_settings( protocol->ctx, channel, no, values ) );
}

/******************************************************************************
 * ctrl_protocol_sys_register
 *****************************************************************************/
int ctrl_protocol_sys_register
(
    ctrl_protocol_handle_t const    handle,
    void * const                    ctx,
    ctrl_protocol_sys_drv_t * const drv
)
{
    CHECK_HANDLE( handle );

    handle->drv = (void *)drv;
    handle->ctx = ctx;

    return ( 0 );
}

/******************************************************************************
 * ctrl_protocol_sys_unregister
 *****************************************************************************/
int ctrl_protocol_sys_unregister
(
    ctrl_protocol_handle_t const handle
)
{
    CHECK_HANDLE( handle );

    handle->drv = NULL;
    handle->ctx = NULL;

    return ( 0 );
}
