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
#ifndef __KNEE_H__
#define __KNEE_H__

#include <stdint.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * @brief knee interpolation states
 *****************************************************************************/
typedef enum knee_interpolation_state_s
{
    KIP_STATE_UNITIALIZED     = 0,        /**< Indicates uninitialized knee interpolation */
    KIP_STATE_INITIALIZED     = 1,        /**< Indicates initialized knee interpolation */
    KIP_STATE_RUNNING         = 2,        /**< Indicates running knee interpolation (computed interim values) */
    KIP_STATE_MAX
} knee_interpolation_state_t;

/******************************************************************************
 * @brief knee function interpolation context
 *****************************************************************************/
typedef struct knee_interpolation_ctx_s
{
    knee_interpolation_state_t  state;
    float                       a;
    float                       b;
    float                       c;
    float                       x0;
    float                       s;
} knee_interpolation_ctx_t;

/**************************************************************************//**
 * @brief      Initialize interpolation context
 *
 * @param[out] ctx  pointer to interpolation context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int sm_knee_interpolation_init
(
    knee_interpolation_ctx_t ** ctx
);

/**************************************************************************//**
 * @brief      Reset interpolation context
 *
 * @param[x]   ctx  interpolation handle
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int sm_knee_interpolation_reset
(
    knee_interpolation_ctx_t * const ctx
);

/**************************************************************************//**
 * @brief      Initialize knee interpolation algorithm, computes the 
 *             interim values interpolation algorithm
 *
 * @param[in]  ctx      pointer to interpolation context
 * @param[in]  knee     new knee-point to use
 * @param[in]  slope    new knee-slope to use
 * @param[in]  clip     new white-clip to use
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int sm_knee_interpolation_calc_init
(
    knee_interpolation_ctx_t * const    ctx,
    uint8_t const                       knee,
    uint16_t const                      slope,
    uint8_t const                       clip
);

/**************************************************************************//**
 * @brief      Run knee interpolation for x
 *
 * @param[in]  ctx              pointer to interpolation context
 * @param[in]  x                x coordinate of sample point to interpolate
 * @param[out] gain             resulting gain value
 * @param[in]  bit_width_input  
 * @param[in]  bit_width_output
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int sm_knee_interpolation_calc
(
    knee_interpolation_ctx_t * const    ctx,
    uint32_t const                      x,
    uint32_t * const                    gain,
    uint8_t const                       bit_width_input,
    uint8_t const                       bit_width_output
);

#ifdef __cplusplus
}
#endif

#endif /* __KNEE_H__ */

