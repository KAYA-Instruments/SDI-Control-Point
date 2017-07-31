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
 * @file    ctrl_protocol_auto.h
 *
 * @brief   Generic auto control protocol functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_AUTO_H__
#define __CTRL_PROTOCOL_AUTO_H__

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
 * @brief Number of rgb statistic values
 *****************************************************************************/
#define NO_VALUES_RGB_STATISTIC         ( 3 ) 

/**************************************************************************//**
 * @brief Number of histogram statistic values
 *****************************************************************************/
#define NO_VALUES_HISTOGRAM_STATISTIC   ( 16 ) 

/**************************************************************************//**
 * @brief Number of exposure statistic values
 *****************************************************************************/
#define NO_VALUES_EXPOSURE_STATISTIC    ( 25 ) 

/**************************************************************************//**
 * @brief Number of xyz statistic values
 *****************************************************************************/
#define NO_VALUES_XYZ_STATISTIC         ( 3 )

/**************************************************************************//**
 * @brief Number of xyz conversion values
 *****************************************************************************/
#define NO_VALUES_COLOR_XYZ             ( 9 )

/**************************************************************************//**
 * @brief Number of aec setup values
 *****************************************************************************/
#define NO_VALUES_AEC_SETUP             ( 8 )

/**************************************************************************//**
 * @brief Gets the enable state of auto-exposure
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  enable   current enable state
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_aec_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              enable 
);

/**************************************************************************//**
 * @brief Sets the enable state of auto-exposure
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   enable   enable state to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_aec_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                enable
);

/**************************************************************************//**
 * @brief Gets the current setup of the auto exposure module
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to get
 * @param[in]   values   pointer to array where received values are stored
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_aec_setup
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no, 
    int32_t * const              values
);

/**************************************************************************//**
 * @brief Configures the auto exposure module
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to set
 * @param[in]   values   pointer to array of values to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_aec_setup
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no, 
    int32_t * const              values
);

/**************************************************************************//**
 * @brief Gets the enable state of auto-white balance
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  enable   current enable state
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_awb_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              enable 
);

/**************************************************************************//**
 * @brief Sets the enable state of auto-white balance
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   enable   enable state to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_awb_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                enable
);

/**************************************************************************//**
 * @brief Gets the auto-white balance speed (damping)
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  enable   current enable state
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_awb_speed
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              speed 
);

/**************************************************************************//**
 * @brief Sets the auto-white balance speed (damping)
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   enable   enable state to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_awb_speed
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                speed
);

/**************************************************************************//**
 * @brief  Trigger auto white-balance to run once
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_run_wb
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel
);

/**************************************************************************//**
 * @brief White-balance description name.
 *****************************************************************************/
typedef char wb_preset_name_t[20];

/**************************************************************************//**
 * @brief White-balance preset object.
 *****************************************************************************/
typedef struct ctrl_protocol_wb_preset_s
{
    uint8_t             id;     /**< identifier */
    wb_preset_name_t    name;   /**< descriptor name */
    uint16_t            ct;     /**< color temperature */
} ctrl_protocol_wb_preset_t;

/**************************************************************************//**
 * @brief Gets number of white blance presets.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  no       number of presets 
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_no_wbpresets
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              no
);

/**************************************************************************//**
 * @brief Gets white blance presets.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  no       number of bytes in array 
 * @param[out]  values   array pointer
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_wbpresets
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint8_t * const              values
);

/**************************************************************************//**
 * @brief Sets a white blance preset.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  id       preset to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_wbpreset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                id
);

/**************************************************************************//**
 * @brief Gets RGB mean statistic of image
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  no       number of unsigned 16-bit values in array 
 * @param[out]  values   array pointer
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_stat_rgb
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint16_t * const             values
);

/**************************************************************************//**
 * @brief Gets enable status of histogram statistic measurment
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  flag     current enable status (0: disable, 1:enabled)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_stat_histogram_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              flag
);

/**************************************************************************//**
 * @brief Enables/disables the histogram statistic measurment
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   flag     flag to set (0: disable / 1: enable )
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_stat_histogram_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                flag
);

/**************************************************************************//**
 * @brief Gets histogram statistics
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  flag     current enable status (0: disable, 1:enabled)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_stat_histogram
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint32_t * const             values
);

/**************************************************************************//**
 * @brief Gets enable status of exposure statistic measurment
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  flag     current enable status (0: disable, 1:enabled)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_stat_exposure_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              flag
);

/**************************************************************************//**
 * @brief Enables/disables the exposure statistic measurment
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   flag     flag to set (0: disable / 1: enable )
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_stat_exposure_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                flag
);

/**************************************************************************//**
 * @brief Gets the exposure statistic
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of array/list items
 * @param[out]  values   array (memory pointer) to fill
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_stat_exposure
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint16_t * const             values
);

/**************************************************************************//**
 * @brief Gets XYZ mean statistic of image
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of array/list items
 * @param[out]  values   array (memory pointer) to fill
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_stat_xyz
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int32_t * const              values
);

/**************************************************************************//**
 * @brief Gets the sensor-specific XYZ conversion matrix
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of array/list items
 * @param[out]  values   array (memory pointer) to fill
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_color_xyz
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    int32_t * const              values
);

/**************************************************************************//**
 * @brief AUTO protocol driver implementation
 *****************************************************************************/
typedef struct ctrl_protocol_auto_drv_s
{
    // auto exposure commands
    ctrl_protocol_get_uint8_t       get_aec_enable;
    ctrl_protocol_set_uint8_t       set_aec_enable;
    ctrl_protocol_int32_array_t     get_aec_setup;
    ctrl_protocol_int32_array_t     set_aec_setup;
    // auto white-balance commands
    ctrl_protocol_get_uint8_t       get_awb_enable;
    ctrl_protocol_set_uint8_t       set_awb_enable;
    ctrl_protocol_get_uint8_t       get_awb_speed;
    ctrl_protocol_set_uint8_t       set_awb_speed;
    ctrl_protocol_run_t             run_wb;
    ctrl_protocol_get_uint8_t       get_no_wbpresets;
    ctrl_protocol_uint8_array_t     get_wbpresets;
    ctrl_protocol_set_uint8_t       set_wbpreset;
    // statistic commands
    ctrl_protocol_uint16_array_t    get_stat_rgb;
    ctrl_protocol_get_uint8_t       get_stat_histogram_enable;
    ctrl_protocol_set_uint8_t       set_stat_histogram_enable;
    ctrl_protocol_uint32_array_t    get_stat_histogram;
    ctrl_protocol_get_uint8_t       get_stat_exposure_enable;
    ctrl_protocol_set_uint8_t       set_stat_exposure_enable;
    ctrl_protocol_uint16_array_t    get_stat_exposure;
    ctrl_protocol_int32_array_t     get_stat_xyz;
    ctrl_protocol_int32_array_t     get_color_xyz;
} ctrl_protocol_auto_drv_t;

/******************************************************************************
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_auto_register
(
    ctrl_protocol_handle_t const     handle,
    void * const                     ctx,
    ctrl_protocol_auto_drv_t * const drv
);

/******************************************************************************
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_auto_unregister
(
    ctrl_protocol_handle_t const handle
);

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_AUTO_H__ */

