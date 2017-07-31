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
 * @file  simple_math.h
 *
 * @brief simple math function implementation
 *
 *****************************************************************************/
#ifndef __SIMPLE_MATH_H__
#define __SIMPLE_MATH_H__

#include <stdint.h>

#define clip(x, min, max)   \
{                           \
    if ( x < min )          \
    {                       \
        x = min;            \
    }                       \
    else if ( x > max )     \
    {                       \
        x = max;            \
    }                       \
    else                    \
    {                       \
        x = x;              \
    }                       \
}

#define sqrtf( x )    __builtin_sqrtf(x)
#define sqrt( x )     __builtin_sqrt(x)
#define powf( x, y )  __builtin_powf( x, y )
#define roundf( x )   __builtin_roundf( x )

/**************************************************************************//**
 * @brief Compute a square root of an unsigned 32 Bit value
 *
 * @param[in]   square  input value
 *
 * @return      square root
 *****************************************************************************/
uint32_t sqrt_u32( uint32_t const square );

/**************************************************************************//**
 * @brief Compute a square root of an unsigned 16 Bit value
 *
 * @param[in]   square  input value
 *
 * @return      square root
 *****************************************************************************/
uint16_t sqrt_u16( uint16_t const square );

/**************************************************************************//**
 * @brief Compute a linear sample for a given input-value
 *
 * Formula:
 * Vout = (contrast * Vin + brightness) / unit
 *
 * @param[in]   value               input value (X-axis)
 * @param[in]   contrast            slope 
 * @param[in]   brightness          offset
 * @param[in]   unit                unit of contrast and brightness
 * @param[in]   bit_width_input     bit-width of input
 * @param[in]   bit_width_output    bit-width of output
 *
 * @return      computed linear sample
 *****************************************************************************/
uint32_t sm_linear 
(
    uint32_t const  value,
    int32_t const   contrast,
    int32_t const   brightness,
    int32_t const   unit,
    uint8_t const   bit_width_input,
    uint8_t const   bit_width_output
);

/**************************************************************************//**
 * @brief Compute a gamma sample for a given input-value
 *
 * Formula:
 * Vout = (contrast * Vin ^ (gamma/unit) + brightness) / unit
 *
 * @param[in]   value               input value (X-axis)
 * @param[in]   kink                kink 
 * @param[in]   linear_contrast     contrast (slope) in linear range
 * @param[in]   linear_brightness   brightness (offset) in linear range
 * @param[in]   contrast            slope 
 * @param[in]   gamma               gamma value
 * @param[in]   brightness          offset
 * @param[in]   unit                unit of contrast, gamma, and brightness
 * @param[in]   bit_width_input     bit-width of input
 * @param[in]   bit_width_output    bit-width of output
 *
 * @return      computed gamma-correction sample
 *****************************************************************************/
uint32_t sm_degamma_correction
(
    uint32_t const  value,
    int32_t const   kink,
    int32_t const   linear_contrast,
    int32_t const   linear_brightness,
    int32_t const   contrast,
    int32_t const   gamma,
    int32_t const   brightness,
    int32_t const   unit,
    uint8_t const   bit_width_input,
    uint8_t const   bit_width_output
);

/**************************************************************************//**
 * @brief Compute a gamma-correction sample for a given input-value
 *
 * Formula:
 * Vout = (contrast * Vin ^ (unit/gamma) + brightness) / unit
 *
 * @param[in]   value               input value (X-axis)
 * @param[in]   kink                kink 
 * @param[in]   linear_contrast     contrast (slope) in linear range
 * @param[in]   linear_brightness   brightness (offset) in linear range
 * @param[in]   contrast            slope 
 * @param[in]   gamma               gamma value
 * @param[in]   brightness          offset
 * @param[in]   unit                unit of contrast, gamma, and brightness
 * @param[in]   bit_width_input     bit-width of input
 * @param[in]   bit_width_output    bit-width of output
 *
 * @return      computed gamma-correction sample
 *****************************************************************************/
uint32_t sm_gamma_correction
(
    uint32_t const  value,
    int32_t const   kink,
    int32_t const   linear_contrast,
    int32_t const   linear_brightness,
    int32_t const   contrast,
    int32_t const   gamma,
    int32_t const   brightness,
    int32_t const   unit,
    uint8_t const   bit_width_input,
    uint8_t const   bit_width_output
);

/******************************************************************************
 * sm_cubic_spline_interpolation
 *****************************************************************************/
#define SM_MAX_SIS_SAMPLES       ( 64u )

typedef enum spline_interpolation_state_s
{
    SIS_UNITIALIZED     = 0,        /**< Indicates uninitialized spline interpolation */
    SIS_INITIALIZED     = 1,        /**< Indicates initialized spline interpolation */
    SIS_GOT_SAMPLES     = 2,        /**< Indicates that context has sample interpolation */
    SIS_RUNNING         = 3         /**< Indicates running spline interpolation */
} spline_interpolation_state_t;

typedef struct spline_interpolation_ctx_s
{
    float                        z[SM_MAX_SIS_SAMPLES];
    int16_t                      h[SM_MAX_SIS_SAMPLES];
#if defined(CFG_HOLD_SAMPLES)
    uint16_t                     x[SM_MAX_SIS_SAMPLES]; /*< samples internally hold */
    uint16_t                     y[SM_MAX_SIS_SAMPLES]; /*< samples internally hold */
#else   /* defined(CFG_HOLD_SAMPLES) */
    uint16_t *                   x;     /*< samples hold outside */
    uint16_t *                   y;     /*< samples hold outside */
#endif  /* defined(CFG_HOLD_SAMPLES) */ 
    uint16_t                     n;
    spline_interpolation_state_t state; /*< stae of interpolation */
} spline_interpolation_ctx_t;

int sm_cubic_spline_interpolation_init
(
    spline_interpolation_ctx_t ** ctx
);

int sm_cubic_spline_interpolation_reset
(
    spline_interpolation_ctx_t * const ctx
);

#if defined(CFG_HOLD_SAMPLES)
int sm_cubic_spline_interpolation_add_samples
(
    spline_interpolation_ctx_t * const ctx,
    uint16_t * const                   x,
    uint16_t * const                   y,
    uint16_t const                     no
);
#endif  /* defined(CFG_HOLD_SAMPLES) */ 

int sm_cubic_spline_interpolation_set_samples
(
    spline_interpolation_ctx_t * const ctx,
    uint16_t * const                   x,
    uint16_t * const                   y,
    uint16_t const                     no
);

int sm_cubic_spline_interpolation_calc_init
(
    spline_interpolation_ctx_t * const ctx
);

int sm_cubic_spline_interpolation_calc
(
    spline_interpolation_ctx_t * const ctx,
    uint16_t const                     x,
    uint16_t * const                   y
);

#endif /* __SIMPLE_MATH_H__ */

