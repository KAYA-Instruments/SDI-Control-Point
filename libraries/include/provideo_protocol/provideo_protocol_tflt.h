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
 * @file    provideo_protocol_tflt.h
 *
 * @brief   ProVideo TFLT protocol implementation
 *
 *****************************************************************************/
#ifndef __PROVIDEO_PROTOCOL_TFLT_H__
#define __PROVIDEO_PROTOCOL_TFLT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <ctrl_protocol/ctrl_protocol.h>

/**************************************************************************//**
 * @brief      Register the TFLT protocol implementation at control protocol layer
 *
 * @param[in]  handle    Instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_tflt_init
(
    ctrl_protocol_handle_t const handle
);

#ifdef __cplusplus
}
#endif

#endif /* __PROVIDEO_PROTOCOL_TFLT_H__ */

