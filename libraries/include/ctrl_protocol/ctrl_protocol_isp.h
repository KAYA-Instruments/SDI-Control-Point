/******************************************************************************
 *
 * Copyright 2016, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
/**
 * @file    ctrl_protocol_isp.h
 *
 * @brief   Generic isp control protocol functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_ISP_H__
#define __CTRL_PROTOCOL_ISP_H__

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
 * @brief Number of black master values
 *****************************************************************************/
#define NO_VALUES_BLACK_MASTER          ( 3 ) 

/**************************************************************************//**
 * @brief Number of color conversion values (3x3 matrix)
 *****************************************************************************/
#define NO_VALUES_COLOR_CONVERSION      ( 9 ) 

/**************************************************************************//**
 * @brief Number of cross-talk values
 *****************************************************************************/
#define NO_VALUES_CROSS                 ( 9 ) 

/**************************************************************************//**
 * @brief Number of cross-talk offset values
 *****************************************************************************/
#define NO_VALUES_CROSS_OFFSET          ( 3 ) 

/**************************************************************************//**
 * @brief Number of lsc values
 *****************************************************************************/
#define NO_VALUES_LSC                   ( 5 )


/**************************************************************************//**
 * @brief Gets the current setup of the lens shading correction module
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to get
 * @param[in]   values   pointer to array where received values are stored
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_lsc
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint32_t * const             values
);

/**************************************************************************//**
 * @brief Configures the lens shading correction module
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values to set
 * @param[in]   values   pointer to array of values to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_lsc
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint32_t * const             values
);

/**************************************************************************//**
 * @brief Gets currently configured bayer-pattern.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  pattern  currently configured bayer-pattern
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_bayer
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              pattern
);

/**************************************************************************//**
 * @brief Sets bayer-pattern in image processing.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   pattern  pattern to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_bayer
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                pattern
);

/**************************************************************************//**
 * @brief Gets currently configured red gain.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  gain     current red gain
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_gain_red
( 
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             gain
);

/**************************************************************************//**
 * @brief Sets red gain.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   gain     red gain to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_gain_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               gain
);

/**************************************************************************//**
 * @brief Gets currently configured green gain.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  gain     current green gain
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_gain_green
( 
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             gain
);

/**************************************************************************//**
 * @brief Sets green gain.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   gain     green gain to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_gain_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               gain
);

/**************************************************************************//**
 * @brief Gets currently configured blue gain.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  gain     current blue gain
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_gain_blue
( 
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             gain
);

/**************************************************************************//**
 * @brief Sets blue gain.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   gain     blue gain to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_gain_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               gain
);

/**************************************************************************//**
 * @brief Gets currently configured black level offset of red component.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  offset   current red offset
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_black_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t * const              offset
);

/**************************************************************************//**
 * @brief Sets black level offset of red component.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   offset   current red offset
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_black_red
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t const                offset
);

/**************************************************************************//**
 * @brief Gets currently configured black level offset of green component.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  offset   current green offset
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_black_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t * const              offset
);

/**************************************************************************//**
 * @brief Sets black level offset of green component.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   offset   green offset value to set.
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_black_green
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t const                offset
);

/**************************************************************************//**
 * @brief Gets currently configured black level offset of blue component.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  offset   current blue offset
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_black_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t * const              offset
);

/**************************************************************************//**
 * @brief Sets black level offset of blue component.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   offset   blue offset value to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_black_blue
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int16_t const                offset
);

/**************************************************************************//**
 * @brief Number of flare level values
 *****************************************************************************/
#define NO_VALUES_ISP_FLARE     ( 3 ) 

/**************************************************************************//**
 * @brief Gets currently configured flare compensation level of red, green and
 *        blue-component.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  no       number of values
 * @param[out]  values   array pointer
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_flare
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    int const                    no,
    uint16_t * const             values
);

/**************************************************************************//**
 * @brief Sets flare compensation level of red, green and blue-component.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values
 * @param[in]   values   array pointer
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_flare
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    uint16_t * const             values
);

/**************************************************************************//**
 * @brief Gets black-level master offsets of red, green and blue-component.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  no       number of values
 * @param[out]  values   array pointer
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_black_master
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int16_t * const              values
);

/**************************************************************************//**
 * @brief Sets black-level master offset for red, green and blue-component.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values
 * @param[in]   values   array pointer
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_black_master
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int16_t * const              values
);

/**************************************************************************//**
 * @brief Gets enable status of denoise and sharpen filter stage.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  flag     current enable status of filter stage
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_filter_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              flag
);

/**************************************************************************//**
 * @brief Enables/disables the denoise and sharpen filter stage.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   flag     flag to set (0: disable / 1: enable )
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_filter_enable
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                flag
);

/**************************************************************************//**
 * @brief Gets detail enhance level of sharpen filter stage.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  level    current detail level of filter stage
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_filter_detail
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              level
);

/**************************************************************************//**
 * @brief Sets detail enhance level of sharpen filter stage.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   level    detail level to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_filter_detail
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                level
);

/**************************************************************************//**
 * @brief Gets denoise level of denoise filter stage.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  level    current denoise level of filter stage
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_filter_denoise
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t * const              level
);

/**************************************************************************//**
 * @brief Sets denoise level of denoise filter stage.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   level    denoise level to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_filter_denoise
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    uint8_t const                level
);

/**************************************************************************//**
 * @brief Get the 3-by-3 color conversion (RGB->YUV) matrix.
 *
 *        The following formula is used for the conversion.
 *         Y  = (c0 *r + c1*g + c2 *b) / 4096 + 64
 *         Cb = (c3 *r + c4*g + c5 *b) / 4096 + 512
 *         Cr = (c6 *r + c7*g + c8 *b) / 4096 + 512
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  no       number of values
 * @param[out]  values   array pointer
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_color_conv
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int16_t * const              values
);

/**************************************************************************//**
 * @brief Sets the 3-by-3 color conversion (RGB->YUV) matrix.
 *
 *        The following formula is used for the conversion.
 *         Y  = (c0 *r + c1*g + c2 *b) / 4096 + 64
 *         Cb = (c3 *r + c4*g + c5 *b) / 4096 + 512
 *         Cr = (c6 *r + c7*g + c8 *b) / 4096 + 512
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  no       number of values
 * @param[out]  values   array pointer
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_color_conv
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int16_t * const              values
);

/**************************************************************************//**
 * @brief Get color cross-talk/correction (RGB->RGB) matrix.
 *
 *        The following formula is used for the conversion.
 *         R' = (R x c0 + G x c1 + B x c2) / 4096
 *         G' = (R x c3 + G x c4 + B x c5) / 4096
 *         B' = (R x c6 + G x c7 + B x c8) / 4096
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  no       number of values
 * @param[out]  values   array pointer
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_color_cross
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int32_t * const              values
);

/**************************************************************************//**
 * @brief Sets color cross-talk/correction (RGB->RGB) matrix.
 *
 *        The following formula is used for the conversion.
 *         R' = (R x c0 + G x c1 + B x c2) / 4096
 *         G' = (R x c3 + G x c4 + B x c5) / 4096
 *         B' = (R x c6 + G x c7 + B x c8) / 4096
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  no       number of values
 * @param[out]  values   array pointer
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_color_cross
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int32_t * const              values
);

/**************************************************************************//**
 * @brief Get color cross-talk/correction offsets.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  no       number of values
 * @param[out]  values   array pointer
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_color_cross_offset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int16_t * const              values
);

/**************************************************************************//**
 * @brief Sets the color cross-talk/correction offsets.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   no       number of values
 * @param[in]   values   array pointer
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_color_cross_offset
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel, 
    int const                    no,
    int16_t * const              values
);

/**************************************************************************//**
 * @brief Gets currently configured split-screen mode.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[out]  enable   current enable status to get
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_split_screen
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t * const              enable
);

/**************************************************************************//**
 * @brief Sets split-screen mode in image processing.
 *
 * @param[in]   channel  control channel instance
 * @param[in]   protocol control protocol instance
 * @param[in]   enable   enable to set
 *
 * @return      0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_split_screen
(
    ctrl_protocol_handle_t const protocol,
    ctrl_channel_handle_t const  channel,
    uint8_t const                enable
);


/**************************************************************************//**
 * @brief ISP protocol driver implementation
 *****************************************************************************/
typedef struct ctrl_protocol_isp_drv_s
{
    ctrl_protocol_uint32_array_t        get_lsc;
    ctrl_protocol_uint32_array_t        set_lsc;
    ctrl_protocol_get_uint8_t           get_bayer;
    ctrl_protocol_set_uint8_t           set_bayer;
    ctrl_protocol_get_uint16_t          get_gain_red;
    ctrl_protocol_set_uint16_t          set_gain_red;
    ctrl_protocol_get_uint16_t          get_gain_green;
    ctrl_protocol_set_uint16_t          set_gain_green;
    ctrl_protocol_get_uint16_t          get_gain_blue;
    ctrl_protocol_set_uint16_t          set_gain_blue;
    ctrl_protocol_get_int16_t           get_black_red;
    ctrl_protocol_set_int16_t           set_black_red;
    ctrl_protocol_get_int16_t           get_black_green;
    ctrl_protocol_set_int16_t           set_black_green;
    ctrl_protocol_get_int16_t           get_black_blue;
    ctrl_protocol_set_int16_t           set_black_blue;
    ctrl_protocol_uint16_array_t        get_flare;
    ctrl_protocol_uint16_array_t        set_flare; 
    ctrl_protocol_int16_array_t         get_black_master;
    ctrl_protocol_int16_array_t         set_black_master; 
    ctrl_protocol_get_uint8_t           get_filter_enable;
    ctrl_protocol_set_uint8_t           set_filter_enable;
    ctrl_protocol_get_uint8_t           get_filter_detail;
    ctrl_protocol_set_uint8_t           set_filter_detail;
    ctrl_protocol_get_uint8_t           get_filter_denoise;
    ctrl_protocol_set_uint8_t           set_filter_denoise;
    ctrl_protocol_int16_array_t         get_color_conv;
    ctrl_protocol_int16_array_t         set_color_conv;
    ctrl_protocol_int32_array_t         get_color_cross;
    ctrl_protocol_int32_array_t         set_color_cross;
    ctrl_protocol_int16_array_t         get_color_cross_offset;
    ctrl_protocol_int16_array_t         set_color_cross_offset;
    ctrl_protocol_get_uint8_t           get_split_screen;
    ctrl_protocol_set_uint8_t           set_split_screen;
} ctrl_protocol_isp_drv_t;

/******************************************************************************
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_isp_register
(
    ctrl_protocol_handle_t const    handle,
    void * const                    ctx,
    ctrl_protocol_isp_drv_t * const drv
);

/******************************************************************************
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_isp_unregister
(
    ctrl_protocol_handle_t const handle
);

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_ISP_H__ */

