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
 * @file    ctrl_channel_serial.h
 *
 * @brief   ProVideo Serial interface definitions
 *
 *****************************************************************************/
#ifndef __CTRL_CHANNEL_SERIAL_H__
#define __CTRL_CHANNEL_SERIAL_H__

#include <ctrl_channel/ctrl_channel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @brief number of data bits to use
 * @note  slow baudrates below 57600 baud are not supported by the GUI because
 *        the delays / wait times get to long for a fluid user experience
 *****************************************************************************/
#define CTRL_CHANNEL_BAUDRATE_9600    (   9600u ) /**<   9600 bits/s */
#define CTRL_CHANNEL_BAUDRATE_19200   (  19200u ) /**<  19200 bits/s */
#define CTRL_CHANNEL_BAUDRATE_38400   (  38400u ) /**<  38400 bits/s */
#define CTRL_CHANNEL_BAUDRATE_57600   (  57600u ) /**<  57600 bits/s */
#define CTRL_CHANNEL_BAUDRATE_115200  ( 115200u ) /**< 115200 bits/s */

#define CTRL_CHANNEL_BAUDRATE_DEFAULT ( CTRL_CHANNEL_BAUDRATE_115200 )  /**< default configuration if 0 */

/**************************************************************************//**
 * @brief number of data bits to use 
 *****************************************************************************/
#define CTRL_CHANNEL_DATA_BITS_5          ( 5u )  /**< 5 data bits */
#define CTRL_CHANNEL_DATA_BITS_6          ( 6u )  /**< 6 data bits */
#define CTRL_CHANNEL_DATA_BITS_7          ( 7u )  /**< 7 data bits */
#define CTRL_CHANNEL_DATA_BITS_8          ( 8u )  /**< 8 data bits */

#define CTRL_CHANNEL_DATA_BITS_DEFAULT    ( CTRL_CHANNEL_DATA_BITS_8 )  /**< default configuration if 0 */

/**************************************************************************//**
 * @brief parity mode
 *****************************************************************************/
#define CTRL_CHANNEL_PARITY_NONE          ( 0u )  /**< no parity */
#define CTRL_CHANNEL_PARITY_ODD           ( 1u )  /**< odd parity */ 
#define CTRL_CHANNEL_PARITY_EVEN          ( 2u )  /**< even parity */

#define CTRL_CHANNEL_PARITY_DEFAULT       ( CTRL_CHANNEL_PARITY_NONE )  /**< default configuration if 0 */

/**************************************************************************//**
 * @brief number of stop bits
 *****************************************************************************/
#define CTRL_CHANNEL_STOP_BITS_1          ( 1u )  /**< 1 stop bit */
#define CTRL_CHANNEL_STOP_BITS_2          ( 2u )  /**< 2 stop bits */

#define CTRL_CHANNEL_STOP_BITS_DEFAULT    ( CTRL_CHANNEL_STOP_BITS_1 )  /**< default configuration if 0 */

/**************************************************************************//**
 * @brief open configuration for serial channels, like RS232 or RSUSB
 *****************************************************************************/
typedef struct ctrl_channel_rs232_open_config_s
{
    uint8_t     idx;                /**< system port index */
    uint8_t     data;               /**< number of data bits */
    uint8_t     parity;             /**< type of parity */
    uint8_t     stop;               /**< number of stop bits */
    uint32_t    baudrate;           /**< baudrate */
} ctrl_channel_rs232_open_config_t;

/**************************************************************************//**
 * @brief open configuration for serial channels, like RS422 or RS485
 *****************************************************************************/
typedef struct ctrl_channel_rs4xx_open_config_s
{
    uint8_t     idx;                /**< system port index */
    uint8_t     data;               /**< number of data bits */
    uint8_t     parity;             /**< type of parity */
    uint8_t     stop;               /**< number of stop bits */
    uint32_t    baudrate;           /**< baudrate */
    uint8_t     dev_addr;           /**< device/slave address */
} ctrl_channel_rs4xx_open_config_t;

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_CHANNEL_SERIAL_H__ */

