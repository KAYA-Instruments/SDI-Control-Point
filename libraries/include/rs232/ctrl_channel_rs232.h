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
 * @file    ctrl_channel_rs232.h
 *
 * @brief   ProVideo RS232/USB serial command interface
 *
 *****************************************************************************/
#ifndef __CTRL_CHANNEL_RS232_H__
#define __CTRL_CHANNEL_RS232_H__

#include <ctrl_channel/ctrl_channel.h>
#include <ctrl_channel/ctrl_channel_serial.h>

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @brief RS232 internal context
 * @note  Memory for this context needs to be provided by the upper layer. 
 *****************************************************************************/
typedef struct ctrl_channel_rs232_context_s
{
    uint8_t     idx;                /**< system port index */
} ctrl_channel_rs232_context_t;

/**************************************************************************//**
 * @brief      Initialize a RS232 control channel instance 
 *
 * @note       memory allocation is not part of this module, so this 
 *             function provides memory to the control channel layer
 *
 * @param[in]  ch   pointer to an control channel instance object
 * @param[in]  ctx  pointer to an internal control channel context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_channel_rs232_init
(
    ctrl_channel_handle_t const             ch,
    ctrl_channel_rs232_context_t * const    ctx
);

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_CHANNEL_RS232_H__ */

