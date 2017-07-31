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
 * @file    ctrl_protocol_lut.h
 *
 * @brief   Generic lut control protocol functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_LUT_H__
#define __CTRL_PROTOCOL_LUT_H__

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
 * @brief max numbers of LUT values
 *****************************************************************************/
#define MAX_VALUES_LUT          ( 4096u )
#define MAX_NO_SAMPLE_POINTS    ( 24u )

/**************************************************************************//**
 * @brief sample point settings
 *****************************************************************************/
typedef struct ctrl_protocol_enable_s
{
    uint8_t     id;      /**< lut identifier */
    uint8_t     flag;    /**< lut enable status */ 
} ctrl_protocol_enable_t;

/**************************************************************************//**
 * @brief rec709 settings
 *****************************************************************************/
typedef struct ctrl_protocol_rec709_s
{
    uint16_t    threshold;      /**< threshold between linear and non-linear range */
    uint16_t    lcontrast;      /**< contrast value in linear range */
    int16_t     lbrightness;    /**< brightness value in linear range */
    uint16_t    contrast;       /**< contrast value in non-linear range */
    uint16_t    gamma;          /**< gamma value in non-linear range */
    int16_t     brightness;     /**< brightness value in non-linear range */
} ctrl_protocol_rec709_t;

/**************************************************************************//**
 * @brief Gets the enable status of LUT module 
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to read
 * @param[out]  values   current enable struct (@see ctrl_protocol_enable_t)
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_lut_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Enables or disables the LUT module
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to write
 * @param[out]  values   current enable struct (@see ctrl_protocol_enable_t)
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Gets the currently selected preset storage
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  value    currently selected preset storage
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_lut_preset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              value
);

/**************************************************************************//**
 * @brief Set current preset storage
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   value    preset storage to select
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_preset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                value
);

/******************************************************************************
 * @brief Sets read/write index of all LUT components
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   index    LUT index to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_write_index
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               index 
);

/**************************************************************************//**
 * @brief Gets read/write index of LUT red component
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  index    current LUT index of red component
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_lut_write_index_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             index
);

/**************************************************************************//**
 * @brief Sets read/write index of gamma LUT red component
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   index    gamma LUT index of red component to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_write_index_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               index 
);

/**************************************************************************//**
 * @brief Gets read/write index of gamma LUT green component
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  index    current gamma LUT index of red component
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_lut_write_index_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             index
);

/**************************************************************************//**
 * @brief Sets read/write index of gamma LUT green component
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   index    gamma LUT index of red component to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_write_index_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               index 
);

/**************************************************************************//**
 * @brief Gets read/write index of gamma LUT blue component
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  index    current gamma LUT index of red component
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_lut_write_index_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             index
);

/**************************************************************************//**
 * @brief Sets read/write index of gamma LUT blue component
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   index    gamma LUT index of red component to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_write_index_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               index 
);

/**************************************************************************//**
 * @brief Reads a number of values from gamma LUT red component
 *
 * @note Start index is set by @see ctrl_protocol_set_lut_write_index_red
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to read
 * @param[out]  values   current LUT values
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_lut_read_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint16_t * const             values
);

/**************************************************************************//**
 * @brief Reads a number of values from gamma LUT green component
 *
 * @note Start index is set by @see ctrl_protocol_set_lut_write_index_green
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to read
 * @param[out]  values   current LUT values
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_lut_read_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint16_t * const             values
);

/**************************************************************************//**
 * @brief Reads a number of values from gamma LUT blue component
 *
 * @note Start index is set by @see ctrl_protocol_set_lut_write_index_blue
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to read
 * @param[out]  values   current LUT values
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_lut_read_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint16_t * const             values
);

/**************************************************************************//**
 * @brief Clears configuration and starts interpolation mode configuration 
 *        on all gamma LUT components.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_reset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Clears configuration and starts interpolation mode configuration 
 *        on gamma LUT red component.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_reset_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Clears configuration and starts interpolation mode configuration 
 *        on gamma LUT green component.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_reset_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Clears configuration and starts interpolation mode configuration 
 *        on gamma LUT blue component.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_reset_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Clears configuration and starts interpolation mode configuration 
 *        on gamma LUT master component.
 *
 * @note  Master component is a virtual component.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_reset_master
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief sample point settings
 *****************************************************************************/
typedef struct ctrl_protocol_samples_s
{
    uint32_t    no;
    uint16_t    x_i[MAX_NO_SAMPLE_POINTS];
    uint16_t    y_i[MAX_NO_SAMPLE_POINTS];
} ctrl_protocol_samples_t;

/**************************************************************************//**
 * @brief Set interpolation samples of all LUT table.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to read
 * @param[out]  values   current values (@see ctrl_protocol_samples_t)
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_sample
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Get interpolation samples of red LUT table.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to read
 * @param[out]  values   current values (@see ctrl_protocol_samples_t)
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_lut_sample_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Set interpolation samples of red LUT table.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to read
 * @param[int]  values   values to set (@see ctrl_protocol_samples_t)
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_sample_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Get interpolation samples of green LUT table.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to read
 * @param[out]  values   current values (@see ctrl_protocol_samples_t)
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_lut_sample_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Set interpolation samples of green LUT table.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to read
 * @param[int]  values   values to set (@see ctrl_protocol_samples_t)
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_sample_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Get interpolation samples of blue LUT table.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to read
 * @param[out]  values   current values (@see ctrl_protocol_samples_t)
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_lut_sample_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Set interpolation samples of blue LUT table.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to read
 * @param[int]  values   values to set (@see ctrl_protocol_samples_t)
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_sample_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Get interpolation samples of master LUT table.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to read
 * @param[out]  values   current values (@see ctrl_protocol_samples_t)
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_lut_sample_master
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Set interpolation samples of master LUT table.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to read
 * @param[int]  values   values to set (@see ctrl_protocol_samples_t)
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_sample_master
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Set a gamma curve specified by the values to all gamma LUT components
 *
 * @note  REC.709 gamma correction curve is split into a linear part
 *        (black region) and a non-linear part (light region)
 *        - linear curve    : V_out = lcontrast * V_in + lbrightness
 *        - non-linear curve: V_out = contrast * V_in ^ gamma + brightness
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values, shall be equal to sizeof(ctrl_protocol_gamma_t)
 * @param[in]   values   gamma values of red component
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_rec709
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Run interpolation on all gamma LUT components.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_interpolate
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Run interpolation on red gamma LUT components.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_interpolate_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Run interpolation on green gamma LUT components.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_interpolate_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief Run interpolation on blue gamma LUT components.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lut_interpolate_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief GAMMA protocol driver implementation
 *****************************************************************************/
typedef struct ctrl_protocol_lut_drv_s
{
    ctrl_protocol_uint8_array_t     get_lut_enable;
    ctrl_protocol_uint8_array_t     set_lut_enable;
    ctrl_protocol_get_uint8_t       get_lut_preset;
    ctrl_protocol_set_uint8_t       set_lut_preset;
    ctrl_protocol_set_uint16_t      set_lut_write_index;
    ctrl_protocol_get_uint16_t      get_lut_write_index_red;
    ctrl_protocol_set_uint16_t      set_lut_write_index_red;
    ctrl_protocol_get_uint16_t      get_lut_write_index_green;
    ctrl_protocol_set_uint16_t      set_lut_write_index_green;
    ctrl_protocol_get_uint16_t      get_lut_write_index_blue;
    ctrl_protocol_set_uint16_t      set_lut_write_index_blue;
    ctrl_protocol_uint16_array_t    get_lut_read_red;
    ctrl_protocol_uint16_array_t    get_lut_read_green;
    ctrl_protocol_uint16_array_t    get_lut_read_blue;
    ctrl_protocol_run_t             set_lut_reset;
    ctrl_protocol_run_t             set_lut_reset_red;
    ctrl_protocol_run_t             set_lut_reset_green;
    ctrl_protocol_run_t             set_lut_reset_blue;
    ctrl_protocol_run_t             set_lut_reset_master;
    ctrl_protocol_uint8_array_t     set_lut_sample;
    ctrl_protocol_uint8_array_t     get_lut_sample_red;
    ctrl_protocol_uint8_array_t     set_lut_sample_red;
    ctrl_protocol_uint8_array_t     get_lut_sample_green;
    ctrl_protocol_uint8_array_t     set_lut_sample_green;
    ctrl_protocol_uint8_array_t     get_lut_sample_blue;
    ctrl_protocol_uint8_array_t     set_lut_sample_blue;
    ctrl_protocol_uint8_array_t     get_lut_sample_master;
    ctrl_protocol_uint8_array_t     set_lut_sample_master;
    ctrl_protocol_uint8_array_t     set_lut_rec709;
    ctrl_protocol_run_t             set_lut_interpolate;
    ctrl_protocol_run_t             set_lut_interpolate_red;
    ctrl_protocol_run_t             set_lut_interpolate_green;
    ctrl_protocol_run_t             set_lut_interpolate_blue;
} ctrl_protocol_lut_drv_t;

/******************************************************************************
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_lut_register
(
    ctrl_protocol_handle_t const        handle,
    void * const                        ctx,
    ctrl_protocol_lut_drv_t * const   drv
);

/******************************************************************************
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_lut_unregister
(
    ctrl_protocol_handle_t const handle
);

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_LUT_H__ */

