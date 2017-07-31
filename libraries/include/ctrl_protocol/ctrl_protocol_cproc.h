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
 * @file    ctrl_protocol_cproc.h
 *
 * @brief   Generic color processing control protocol functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_CPROC_H__
#define __CTRL_PROTOCOL_CPROC_H__

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
 * @brief Get post processing brightness
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  brightness  current brightness value
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_post_bright
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t * const              brightness
);

/**************************************************************************//**
 * @brief Set post processing brightness. Y_out = Y + offset
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   brightness  brightness value to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_post_bright
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t const                brightness
);

/**************************************************************************//**
 * @brief Get post processing contrast
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  contrast    current contrast value
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_post_cont
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             contrast
);

/**************************************************************************//**
 * @brief Set post processing contrast. Y_out = Y * factor / 128
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   contrast    contrast value to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_post_cont
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               contrast
);

/**************************************************************************//**
 * @brief Get post processing color saturation.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   saturation  current saturation value
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_post_sat
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             saturation
);

/**************************************************************************//**
 * @brief Set post processing color saturation.
 *
 *        The following formula is used.
 *         C[b,r]_out = C[b,r] * factor / 128
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   saturation  saturation value to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_post_sat
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               saturation
);

/**************************************************************************//**
 * @brief Get post processing color hue offset angle.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   hue         current hue value
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_post_hue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t * const              hue 
);

/**************************************************************************//**
 * @brief Set post processing color hue offset angle.
 *
 *        The following formula is used.
 *         Cb' =  Cb * cos (offset * 90 / 128) + Cr * sin (offset * 90 / 128)
 *         Cr' = -Cb * sin (offset * 90 / 128) + Cr * cos (offset * 90 / 128)

 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   hue         hue value to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_post_hue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t const                hue
);

/**************************************************************************//**
 * @brief CPROC protocol driver implementation
 *****************************************************************************/
typedef struct ctrl_protocol_cproc_drv_s
{
    ctrl_protocol_get_int16_t           get_post_bright;
    ctrl_protocol_set_int16_t           set_post_bright;
    ctrl_protocol_get_uint16_t          get_post_cont;
    ctrl_protocol_set_uint16_t          set_post_cont;
    ctrl_protocol_get_uint16_t          get_post_sat;
    ctrl_protocol_set_uint16_t          set_post_sat;
    ctrl_protocol_get_int16_t           get_post_hue;
    ctrl_protocol_set_int16_t           set_post_hue;
} ctrl_protocol_cproc_drv_t;

/******************************************************************************
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_cproc_register
(
    ctrl_protocol_handle_t const      handle,
    void * const                      ctx,
    ctrl_protocol_cproc_drv_t * const drv
);

/******************************************************************************
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_cproc_unregister
(
    ctrl_protocol_handle_t const handle
);

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_CPROC_H__ */


