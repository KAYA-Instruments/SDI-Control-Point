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

