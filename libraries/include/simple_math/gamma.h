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
#ifndef __GAMMA_H__
#define __GAMMA_H__

#include <stdint.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

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
uint32_t sm_degamma
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
uint32_t sm_gamma
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
 * @brief Compute a gamma-correction sample for a given input-value.
 *        Identical to sm_gamma, but uses float input values for the constants.
 *        It also uses fastpow() instead of powf() which is a lot faster but
 *        less precise.
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
 * @param[in]   bit_width_input     bit-width of input
 * @param[in]   bit_width_output    bit-width of output
 *
 * @return      computed gamma-correction sample
 *****************************************************************************/
uint32_t sm_gamma_float
(
    uint32_t const  value,
    float   const   kink,
    float   const   linear_contrast,
    float   const   linear_brightness,
    float   const   contrast,
    float   const   gamma,
    float   const   brightness,
    uint8_t const   bit_width_input,
    uint8_t const   bit_width_output
);

/**************************************************************************//**
 * @brief Calculates the PQ gamma value for a given input value.
 *
 * @param[in]   value               input value (X-axis)
 * @param[in]   bit_width_input     bit-width of input
 * @param[in]   bit_width_output    bit-width of output
 *
 * @return      computed gamma-correction sample
 *****************************************************************************/
uint32_t sm_pq
(
        uint32_t const value,
        uint8_t const  bit_width_input,
        uint8_t const  bit_width_output
);

/**************************************************************************//**
 * @brief Calculates the hybrid log-gamma value for a given input value.
 *
 * @param[in]   value               input value (X-axis)
 * @param[in]   bit_width_input     bit-width of input
 * @param[in]   bit_width_output    bit-width of output
 *
 * @return      computed gamma-correction sample
 *****************************************************************************/
uint32_t sm_hlg
(
        uint32_t const value,
        uint8_t const  bit_width_input,
        uint8_t const  bit_width_output
);

#ifdef __cplusplus
}
#endif

#endif /* __SIMPLE_MATH_H__ */

