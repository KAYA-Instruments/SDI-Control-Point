
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
 * @file    ctrl_protocol_knee.h
 *
 * @brief   Generic knee control protocol functions
 *
 *****************************************************************************/
#ifndef __CTRL_PROTOCOL_KNEE_H__
#define __CTRL_PROTOCOL_KNEE_H__

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
 * @brief knee function configuration
 *****************************************************************************/
typedef struct ctrl_protocol_knee_s
{
    uint8_t     enable;         /**< enable status */
    uint8_t     knee_point;     /**< knee point */
    uint16_t    knee_slope;     /**< knee slope */
    uint16_t    white_clip;     /**< white clip */
} ctrl_protocol_knee_t;

/**************************************************************************//**
 * @brief Gets enable status of Multi Color Controller.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  status      current enable status of KNEE
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_knee_enable
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t * const                 enable
);

/**************************************************************************//**
 * @brief Sets enable status of Multi Color Controller.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[in]   status      status to set (enable or disable)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_knee_enable
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    uint8_t const                   enable
);

/**************************************************************************//**
 * @brief Gets enable status of Multi Color Controller.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  status      current enable status of KNEE
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_get_knee_config
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    int const                       no,
    uint8_t * const                 values
);

/**************************************************************************//**
 * @brief Gets enable status of Multi Color Controller.
 *
 * @param[in]   channel     control channel instance
 * @param[in]   protocol    control protocol instance
 * @param[out]  status      current enable status of KNEE
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_set_knee_config
(
    ctrl_protocol_handle_t const    protocol,
    ctrl_channel_handle_t const     channel, 
    int const                       no,
    uint8_t * const                 values
);

/**************************************************************************//**
 * @brief KNEE protocol driver implementation
 *****************************************************************************/
typedef struct ctrl_protocol_knee_drv_s
{
    ctrl_protocol_get_uint8_t       get_knee_enable;
    ctrl_protocol_set_uint8_t       set_knee_enable;
    ctrl_protocol_uint8_array_t     get_knee_config;
    ctrl_protocol_uint8_array_t     set_knee_config;
} ctrl_protocol_knee_drv_t;

/******************************************************************************
 * @brief      Register a protocol implementation at control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 * @param      ctx      private context of protocol driver implementation
 * @param      drv      driver functions of protocol implementation
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_knee_register
(
    ctrl_protocol_handle_t const    handle,
    void * const                    ctx,
    ctrl_protocol_knee_drv_t * const drv
);

/******************************************************************************
 * @brief      Remove/unregister a protocol implementation from 
 *             control protocol layer
 *
 * @param      handle   instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_protocol_knee_unregister
(
    ctrl_protocol_handle_t const handle
);

/* @} ctrl_protocol_layer */

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_PROTOCOL_KNEE_H__ */

