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
 * @file    ctrl_channel_generic.h
 *
 * @brief   ProVideo Generic interface definitions
 *
 *****************************************************************************/
#ifndef __CTRL_CHANNEL_GENERIC_H__
#define __CTRL_CHANNEL_GENERIC_H__

#include <ctrl_channel/ctrl_channel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @brief generic open configuration for channels, it's more a transfer object
 *****************************************************************************/
typedef struct ctrl_channel_generic_open_config_s
{
    void * config;      /**< pointer to a specific open configuration */
    int    size;        /**< size of specific open configuration */
} ctrl_channel_generic_config_t;

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_CHANNEL_GENERIC_H__ */

