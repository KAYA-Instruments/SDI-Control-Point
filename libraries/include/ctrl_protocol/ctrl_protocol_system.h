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
 * @file    ctrl_protocol_system.h
 *
 * @brief   Generic system control protocol functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_SYSTEM_H__
#define __CTRL_PROTOCOL_SYSTEM_H__

#include <stdint.h>

#include <ctrl_channel/ctrl_channel.h>
#include <ctrl_protocol/ctrl_protocol.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * @defgroup ctrl_protocol_layer Generic Control Protocol Layer implementation
 * @{
 *****************************************************************************/

/**************************************************************************//**
 * @brief version feature bit-mask (supported features of connected camera)
 *****************************************************************************/
enum SYSTEM_FEATURE
{
    SYSTEM_FEATURE_UNKNOWN        = 0x00000000u,    /**< reset value */
    SYSTEM_FEATURE_CAMERA_LINK    = 0x00000001u,    /**< camera link device */
    SYSTEM_FEATURE_XPRESS         = 0x00000002u,    /**< coax-press device */
    SYSTEM_FEATURE_CHANNEL_0      = 0x00000004u,    /**< video pipeline/chain 0 available */
    SYSTEM_FEATURE_CHANNEL_1      = 0x00000008u,    /**< video pipeline/chain 1 available */
    SYSTEM_FEATURE_FPNC           = 0x00000020u,    /**< fix pattern noise correction available */
    SYSTEM_FEATURE_TFLT           = 0x00000040u,    /**< temporal filter available */
    SYSTEM_FEATURE_BFC            = 0x00000100u,    /**< black frame correction available */
    SYSTEM_FEATURE_ZEBRA          = 0x00000800u,    /**< OSD for zebra available */
    SYSTEM_FEATURE_LOGO           = 0x00001000u,    /**< OSD for logo available */
    SYSTEM_FEATURE_TEST_PATTERN   = 0x00001000u,    /**< OSD for test-pattern available */
    SYSTEM_FEATURE_CENTER_MARKER  = 0x00001000u,    /**< OSD for center marker available */
    SYSTEM_FEATURE_MCC            = 0x00002000u,    /**< MCC is available */
    SYSTEM_FEATURE_SEN_LUT        = 0x00004000u,    /**< sensor LUT available */
};

#define IS_SYSTEM_FEATURE_CAMERA_LINK(x)   ( x & SYSTEM_FEATURE_CAMERA_LINK )
#define IS_SYSTEM_FEATURE_XPRESS(x)        ( x & SYSTEM_FEATURE_XPRESS )
#define IS_SYSTEM_FEATURE_CHANNEL_0(x)     ( x & SYSTEM_FEATURE_CHANNEL_0 )
#define IS_SYSTEM_FEATURE_CHANNEL_1(x)     ( x & SYSTEM_FEATURE_CHANNEL_1 )
#define IS_SYSTEM_FEATURE_FPNC(x)          ( x & SYSTEM_FEATURE_FPNC )
#define IS_SYSTEM_FEATURE_TFLT(x)          ( x & SYSTEM_FEATURE_TFLT )
#define IS_SYSTEM_FEATURE_BFC(x)           ( x & SYSTEM_FEATURE_BFC )
#define IS_SYSTEM_FEATURE_ZEBRA(x)         ( x & SYSTEM_FEATURE_ZEBRA )
#define IS_SYSTEM_FEATURE_LOGO(x)          ( x & SYSTEM_FEATURE_LOGO )
#define IS_SYSTEM_FEATURE_TEST_PATTERN(x)  ( x & SYSTEM_FEATURE_TEST_PATTERN )
#define IS_SYSTEM_FEATURE_CENTER_MARKER(x) ( x & SYSTEM_FEATURE_CENTER_MARKER )
#define IS_SYSTEM_FEATURE_MCC(x)           ( x & SYSTEM_FEATURE_MCC )
#define IS_SYSTEM_FEATURE_SEN_LUT(x)       ( x & SYSTEM_FEATURE_SEN_LUT )

/**************************************************************************//**
 * @brief application version type
 *****************************************************************************/
typedef char ctrl_protocol_system_desc_t[50];

/**************************************************************************//**
 * @brief Version object
 *****************************************************************************/
typedef struct ctrl_protocol_version_s
{
    ctrl_protocol_system_desc_t
                system_platform;            /**< system platform, used to identify this device */
    ctrl_protocol_system_desc_t
                device_name;                /**< device name, specified by user */
    uint32_t    system_id[4];               /**< 96 bit unique system identifier */
    uint32_t    hw_revision;                /**< bitstream revision */
    ctrl_protocol_system_desc_t
                system_validity;            /**< system validity (set by device firmware) */
    uint32_t    feature_mask_HW;            /**< feature mask hardware */
    uint32_t    feature_mask_SW;            /**< feature mask software */
    uint32_t    resolution_mask[3];         /**< 96 bit resolution mask */
    uint32_t    loader_version[2];          /**< boot loader version */
    ctrl_protocol_system_desc_t
                sw_release_id;              /**< software release desciption */
    ctrl_protocol_system_desc_t
                sw_release_date;            /**< software release date */
    ctrl_protocol_system_desc_t
                sw_build_date;              /**< software build date */
} ctrl_protocol_version_t;

/**************************************************************************//**
 * @brief system temperature struct
 *****************************************************************************/
typedef struct ctrl_protocol_temp_s
{
    uint8_t     id;         /**< temperature sensor identifier */
    float       temp;       /**< sensor value in °C */
    char        name[16];   /**< name of the sensor, e.g. "CPU" or "FPGA" */
} ctrl_protocol_temp_t;

/**************************************************************************//**
 * @brief Gets system information
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of values (sizeof(ctrl_protocol_version_t))
 * @param[in]  values   points to buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_system_info
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Get the system platform name which is used to identify the device.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of values (sizeof(ctrl_protocol_device_id_t))
 * @param[in]  values   points to buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_system_platform
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Get the device name which can be set by the user.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of values (sizeof(ctrl_protocol_device_id_t))
 * @param[in]  values   points to buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_device_name
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Set the device name.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of values (sizeof(ctrl_protocol_device_id_t))
 * @param[in]  values   points to buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_device_name
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Device identifier object
 *****************************************************************************/
typedef struct ctrl_protocol_device_id_s
{
    uint32_t id0;
    uint32_t id1;
    uint32_t id2;
    uint32_t id3;
} ctrl_protocol_device_id_t;

/**************************************************************************//**
 * @brief Get the 128 bit unique device identifier.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of values (sizeof(ctrl_protocol_device_id_t))
 * @param[in]  values   points to buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_system_id
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Get the system validity string.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of values (sizeof(ctrl_protocol_system_desc_t))
 * @param[in]  values   points to buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_system_validity
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              validity
);

/**************************************************************************//**
 * @brief Get the 32 bit bitstream version.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] version  bitstream version
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_bitstream_version
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             version 
);

/**************************************************************************//**
 * @brief Get the 32 bit bootloader version.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] version  bootloader version
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_bootloader_version
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             version 
);

/**************************************************************************//**
 * @brief Get the application version string.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of values (sizeof(ctrl_protocol_system_desc_t))
 * @param[out] version  application version string
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_application_version
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              version 
);

/**************************************************************************//**
 * @brief Get the application release date string.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of values (sizeof(ctrl_protocol_system_desc_t))
 * @param[out] date     application release date string
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_application_release_date
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              date
);

/**************************************************************************//**
 * @brief Get the application build date string.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of values (sizeof(ctrl_protocol_system_desc_t))
 * @param[out] date     application build date string
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_application_build_date
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              date
);

/**************************************************************************//**
 * @brief Get the 32 bit hardware feature mask.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] mask     feature mask
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_feature_mask_hw
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             mask
);

/**************************************************************************//**
 * @brief Get the 32 bit software/application feature mask.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] mask     feature mask
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_feature_mask_sw
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             mask
);

/**************************************************************************//**
 * @brief Resolution mask identifier object
 *****************************************************************************/
typedef struct ctrl_protocol_resolution_id_s
{
    uint32_t id0;
    uint32_t id1;
    uint32_t id2;
} ctrl_protocol_resolution_id_t;

/**************************************************************************//**
 * @brief Get the 96 bit resolution mask (resolutions supported by device).
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of values (sizeof(ctrl_protocol_device_id_t))
 * @param[in]  values   points to buffer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_resolution_mask
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Get the current baud-rate of the rs232 terminal interface
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] baudrate current baudrate
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_rs232_baud
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             baudrate
);

/**************************************************************************//**
 * @brief Set the baud-rate of the rs232 terminal interface
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  baudrate baudrate to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_rs232_baud
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               baudrate
);

/**************************************************************************//**
 * @brief Get the current baud-rate of the rs485 terminal interface
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] baudrate current baudrate
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_rs485_baud
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             baudrate
);

/**************************************************************************//**
 * @brief Set the baud-rate of the rs485 terminal interface
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  baudrate baudrate to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_rs485_baud
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               baudrate
);

/**************************************************************************//**
 * @brief Get the current slave address of the connected rs485 device
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] address  current slave address
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_rs485_addr
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             address
);

/**************************************************************************//**
 * @brief Set the slave address of the connected rs485 device
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  address  slave address to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_rs485_addr
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    uint32_t const               address
);

/**************************************************************************//**
 * @brief Get the current broadcast address of the connected rs485 device
 *
 * @param[in]  channel              control channel instance
 * @param[in]  protocol             control protocol instance
 * @param[out] broadcast_address    current broadcast address
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_rs485_bc_addr
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel,
    uint32_t * const             broadcast_address
);

/**************************************************************************//**
 * @brief Set the broadcast address of the connected rs485 device
 *
 * @param[in]  channel              control channel instance
 * @param[in]  protocol             control protocol instance
 * @param[in]  broadcast_address    broadcast address to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_rs485_bc_addr
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel,
    uint32_t const               broadcast_address
);

/**************************************************************************//**
 * @brief Get the current broadcast master state of the connected rs485 device.
 *        If the device is the master it will answer to broadcast messages.
 *
 * @param[in]  channel      control channel instance
 * @param[in]  protocol     control protocol instance
 * @param[out] is_master    current broadcast master state
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_rs485_bc_master
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              is_master
);

/**************************************************************************//**
 * @brief Enable broadcast master mode on the connected rs485 device that has
 *        the given master_address.
 *
 * @param[in]  channel          control channel instance
 * @param[in]  protocol         control protocol instance
 * @param[in]  master_address   address of device that shall be broadcast master
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_rs485_bc_master
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel,
    int32_t const                master_address
);

/**************************************************************************//**
 * @brief Connection information about a device
 *****************************************************************************/
typedef struct ctrl_protocol_device_s
{
    ctrl_protocol_system_desc_t device_platform;
    ctrl_protocol_system_desc_t device_name;
    unsigned int rs485_address;
    unsigned int rs485_bc_address;
    unsigned int rs485_bc_master;
} ctrl_protocol_device_t;

/**************************************************************************//**
 * @brief Get the device name which can be set by the user.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       size of the buffer, as we can detect a maximum of 99
 *                      devices it has to be sizeof(ctrl_protocol_device_t) * 99
 * @param[in]  buffer   points to buffer. You can write an unsigned integer to
 *                      this buffer which contains the timeout in ms. If no value is
 *                      written (0) the default timeout is used.
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_device_list
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              buffer
);

/**************************************************************************//**
 * @brief Get the current enable status of console prompt
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] flag     enable status (0: disabled, 1:enabled)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_prompt
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              flag
);

/**************************************************************************//**
 * @brief Set the current enable status of console prompt
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  flag     enable status to set (0: disable, 1:enable)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_prompt
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                flag
);

/**************************************************************************//**
 * @brief Get the current debug level
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] level    debug level
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_debug
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              level 
);

/**************************************************************************//**
 * @brief Set the debug level
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] level    debug level to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_debug
(
    ctrl_protocol_handle_t const handle,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                level
);

/**************************************************************************//**
 * @brief Get the current runtime counter value
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] cnt      runtime counter value in seconds
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_runtime
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint32_t * const             cnt
);

/**************************************************************************//**
 * @brief Gets the value of the gevin temperature sensor
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to read, has to be
 *                       sizeof(ctrl_protocol_temp_t)
 * @param[out]  values   current temp struct (@see ctrl_protocol_temp_t)
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_temp
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Get the maximum logged temperature in °C
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  no       number of values to read, has to be 2
 * @param[out] values   array of two values, first value is logged maximum
 *                      temperature in °C, second value is allowed maximum
 *                      temperature in °C
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_max_temp
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int32_t * const              values
);

/**************************************************************************//**
 * @brief Reset the maximum logged temperature to the current temp value
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_max_temp_reset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Get the current cooling fan speed in %
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] speed    current fan speed in %
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_fan_speed
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              speed
);

/**************************************************************************//**
 * @brief Get the current target temperature of the fan control
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] target   current target temperature
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_fan_target
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              target
);

/**************************************************************************//**
 * @brief Set the target system temperature of the fan control
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[in]  target   target temperature to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_fan_target
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                target
);

/**************************************************************************//**
 * @brief Get the amount of occured over temperature events
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 * @param[out] count    number of over temp events that were logged
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_over_temp_count
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint32_t * const             count
);

/**************************************************************************//**
 * @brief Flush buffers of the device by sending multiple new lines
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_flush_buffers
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);


/**************************************************************************//**
 * @brief Run the reboot command on device 
 *
 * @note       The control channel needs to be reconnected.
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_reboot
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Switch the device into update state
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_update
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Save settings
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_save_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Load saved settings
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_load_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Reset to factory settings
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_reset_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Copy chain settings 
 *
 * @param[in]  channel  control channel instance
 * @param[in]  protocol control protocol instance
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_copy_settings
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief System protocol driver implementation
 *****************************************************************************/
typedef struct ctrl_protocol_sys_drv_s
{
    ctrl_protocol_uint8_array_t     get_system_info;
    ctrl_protocol_uint8_array_t     get_system_platform;
    ctrl_protocol_uint8_array_t     get_device_name;
    ctrl_protocol_uint8_array_t     set_device_name;
    ctrl_protocol_uint8_array_t     get_system_id;
    ctrl_protocol_uint8_array_t     get_system_validity;
    ctrl_protocol_get_uint32_t      get_bitstream_version;
    ctrl_protocol_get_uint32_t      get_bootloader_version;
    ctrl_protocol_uint8_array_t     get_application_version;
    ctrl_protocol_uint8_array_t     get_application_release_date;
    ctrl_protocol_uint8_array_t     get_application_build_date;
    ctrl_protocol_get_uint32_t      get_feature_mask_hw;
    ctrl_protocol_get_uint32_t      get_feature_mask_sw;
    ctrl_protocol_uint8_array_t     get_resolution_mask;
    ctrl_protocol_get_uint32_t      get_rs232_baud;
    ctrl_protocol_set_uint32_t      set_rs232_baud;
    ctrl_protocol_get_uint32_t      get_rs485_baud;
    ctrl_protocol_set_uint32_t      set_rs485_baud;
    ctrl_protocol_get_uint32_t      get_rs485_addr;
    ctrl_protocol_set_uint32_t      set_rs485_addr;
    ctrl_protocol_get_uint32_t      get_rs485_bc_addr;
    ctrl_protocol_set_uint32_t      set_rs485_bc_addr;
    ctrl_protocol_get_uint8_t       get_rs485_bc_master;
    ctrl_protocol_set_int32_t       set_rs485_bc_master;
    ctrl_protocol_uint8_array_t     get_device_list;
    ctrl_protocol_get_uint8_t       get_prompt;
    ctrl_protocol_set_uint8_t       set_prompt;
    ctrl_protocol_get_uint8_t       get_debug;
    ctrl_protocol_set_uint8_t       set_debug;
    ctrl_protocol_get_uint32_t      get_runtime;
    ctrl_protocol_uint8_array_t     get_temp;
    ctrl_protocol_int32_array_t     get_max_temp;
    ctrl_protocol_run_t             max_temp_reset;
    ctrl_protocol_get_uint8_t       get_fan_speed;
    ctrl_protocol_get_uint8_t       get_fan_target;
    ctrl_protocol_set_uint8_t       set_fan_target;
    ctrl_protocol_get_uint32_t      get_over_temp_count;
    ctrl_protocol_run_t             flush_buffers;
    ctrl_protocol_run_t             reboot;
    ctrl_protocol_run_t             update;
    ctrl_protocol_run_t             save_settings;
    ctrl_protocol_run_t             load_settings;
    ctrl_protocol_run_t             reset_settings;
    ctrl_protocol_uint8_array_t     copy_settings;
} ctrl_protocol_sys_drv_t;

/******************************************************************************
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_sys_register
(
    ctrl_protocol_handle_t const    handle,
    void * const                    ctx,
    ctrl_protocol_sys_drv_t * const drv
);

/******************************************************************************
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_sys_unregister
(
    ctrl_protocol_handle_t const handle
);

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_SYSTEM_H__ */

