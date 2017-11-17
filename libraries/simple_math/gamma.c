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
#include <stdint.h>

#include <math.h>

#include <simple_math/float.h>
#include <simple_math/gamma.h>

/******************************************************************************
 * sm_degamma
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
)
{
    float Vin       = (float)value;
    float Vout      = 0.0f;
    float g         = (float)unit/(float)gamma;
    int32_t res     = 0;
    uint32_t size_i = (1ul << bit_width_input);
    uint32_t size_o = (1ul << bit_width_output);
    float k         = (float)kink/(float)(unit);

    // formula = contrast * Vin^1/g + brightness 
    Vin  /= (float)size_i;                                  // normalize to to 0..1
    Vout  = Vin;
    Vout *= (float)unit;                                    // unit scaling
    if ( Vin < k )
    {
        Vout += (float)linear_brightness;                   // add brightness offset
        Vout /= (float)linear_contrast;
    }
    else
    {
        Vout += (float)brightness;                          // add brightness offset
        Vout /= (float)contrast;                            // contrast
        Vout  = (gamma != unit) ? powf( Vout, g ) : Vout;   // compute Vin^g when g!=1
    } 
    Vout *= (float)size_o;                                  // scale to output bit-width
    Vout += 0.5f;                                           // for rounding
    res   = (int32_t)Vout;

    // clip to range 0..(size_o-1)
    clip( res, 0, (int32_t)(size_o-1) );

    return ( (uint32_t)(res) );
}

/******************************************************************************
 * sm_gamma
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
)
{
    float Vin       = (float)value;
    float Vout      = 0.0f;
    float g         = (float)gamma / (float)unit;
    int32_t res     = 0;
    uint32_t size_i = (1ul << bit_width_input);
    uint32_t size_o = (1ul << bit_width_output);
    float k         = (float)kink/(float)(unit);

    // see ITU-R BT.709-5 (pg. 18, "opto-electronic conversion" )
    // V = 1.099 L^0.45 – 0.099    for 1 >= L >= 0.018
    // V = 4.500 L                 for 0.018 > L >= 0
    Vin  /= (float)size_i;                                  // normalize to 0..1
    if ( Vin < k )
    {
        Vout  = Vin;                                        // linear range
        Vout *= (float)linear_contrast;
        Vout += (float)linear_brightness;                   // add brightness offset
    }
    else
    {
        // formula = contrast * Vin^g + brightness 
        Vout  = (gamma != unit) ? powf( Vin, g ) : Vin;     // compute Vin^g when g!=1
        Vout *= (float)contrast;                            // contrast
        Vout += (float)brightness;                          // add brightness offset
    }
    Vout /= (float)unit;                                    // unit scaling
    Vout *= (float)size_o;                                  // normalize to output bit-width
    Vout += 0.5f;                                           // for rounding
    res   = (int32_t)Vout;

    // clip to range 0..(size_o-1)
    clip( res, 0, (int32_t)(size_o-1) );

    return ( (uint32_t)(res) );
}

/******************************************************************************
 * sm_gamma_float - identical to sm_gamma, but uses float input values for
 *                  the constants. It also uses fastpow() instead of powf()
 *                  which is a lot faster but less precise.
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
)
{
    float Vin       = value;
    float Vout      = 0.0f;
    int32_t res     = 0;
    uint32_t size_i = (1ul << bit_width_input);
    uint32_t size_o = (1ul << bit_width_output);
    float k         = kink;

    // see ITU-R BT.709-5 (pg. 18, "opto-electronic conversion" )
    // V = 1.099 L^0.45 – 0.099    for 1 >= L >= 0.018
    // V = 4.500 L                 for 0.018 > L >= 0
    Vin  /= (float)size_i;                                      // normalize to 0..1
    if ( Vin < k )
    {
        Vout  = Vin;                                            // linear range
        Vout *= linear_contrast;
        Vout += linear_brightness;                              // add brightness offset
    }
    else
    {
        // formula = contrast * Vin^g + brightness
        // use fastpow which is less precise than powf, but a lot faster
        Vout  = (gamma != 1.0f) ? fastpow( Vin, gamma ) : Vin;  // compute Vin^g when g!=1
        Vout *= contrast;                                       // contrast
        Vout += brightness;                                     // add brightness offset
    }
    Vout *= (float)size_o;                                      // normalize to output bit-width
    Vout += 0.5f;                                               // for rounding
    res   = (int32_t)Vout;

    // clip to range 0..(size_o-1)
    clip( res, 0, (int32_t)(size_o-1) );

    return ( (uint32_t)(res) );
}

/******************************************************************************
 * sm_pq - Calculates the PQ gamma value for a given input value.
 *****************************************************************************/
uint32_t sm_pq
(
        uint32_t const value,
        uint8_t const  bit_width_input,
        uint8_t const  bit_width_output
)
{
    float Vin       = value;
    float E_non_lin = 0.0f;
    float Y         = 0.0f;
    float Vout      = 0.0f;
    int32_t res     = 0;
    uint32_t size_i = (1ul << bit_width_input);
    uint32_t size_o = (1ul << bit_width_output);

    /* See ITU-R BT.2100:
     * https://www.itu.int/dms_pubrec/itu-r/rec/bt/R-REC-BT.2100-1-201706-I!!PDF-E.pdf
     *
     * E_non_lin = 1.099 * (59.5208 * L)^0.45 - 0.099   for 1         >= L >  0.0003024
     * E_non_lin = 267.84 * L                           for 0.0003024 >= L >= 0
     *
     * Y = E_non_lin^2.4 / 100 */

    Vin  /= (float)size_i;                      // normalize to 0..1

    // Compute non linear representation of the input values
    if ( Vin <= 0.0003024f )
    {
        E_non_lin = 267.84 * Vin;
    }
    else
    {
        E_non_lin = 1.099f * fastpow((59.5208f * Vin), 0.45f) - 0.099f;
    }

    // Calculate Y value
    Y = fastpow(E_non_lin, 2.4f) / 100.0f;

    // Calculate resulting code value
    float const c1 = 0.8359375f;
    float const c2 = 18.8515625f;
    float const c3 = 18.6875f;
    float const m1 = 0.1593017578125f;
    float const m2 = 78.84375f;

    Vout = fastpow(((c1 + c2 * fastpow(Y, m1)) / (1 + c3 * fastpow(Y, m1))), m2);

    Vout *= (float)size_o;                      // normalize to output bit-width
    Vout += 0.5f;                               // for rounding
    res   = (int32_t)Vout;

    // clip to range 0..(size_o-1)
    clip( res, 0, (int32_t)(size_o-1) );

    return ( (int32_t)(res) );
}

/******************************************************************************
 * sm_hlg - Calculates the hybrid log-gamma value for a given input value.
 *****************************************************************************/
uint32_t sm_hlg
(
        uint32_t const value,
        uint8_t const  bit_width_input,
        uint8_t const  bit_width_output
)
{
    float Vin       = value;
    float Vout      = 0.0f;
    int32_t res     = 0;
    uint32_t size_i = (1ul << bit_width_input);
    uint32_t size_o = (1ul << bit_width_output);

    /* See ITU-R BT.2100:
     * https://www.itu.int/dms_pubrec/itu-r/rec/bt/R-REC-BT.2100-1-201706-I!!PDF-E.pdf
     *
     * V = a * ln(12 * L - b) + c   for 1    >= L >  1/12
     * V = sqrt(3 * L)              for 1/12 >= L >= 0
     *
     * where a = 0.17883277,
     *       b = 1-4a = 0.28466892,
     *       c = 0.5-a*ln(4a) = 0.55991073 */

    Vin  /= (float)size_i;                      // normalize to 0..1

    float const a = 0.17883277;
    float const b = 0.28466892;
    float const c = 0.55991073;
    float const threshold = 1.0f / 12.0f;

    if ( Vin <= threshold )
    {
        Vout = sqrtf(3.0f * Vin);
    }
    else
    {
        Vout = (a * logf((12.0f * Vin) - b)) + c;
    }

    Vout *= (float)size_o;                      // normalize to output bit-width
    Vout += 0.5f;                               // for rounding
    res   = (int32_t)Vout;

    // clip to range 0..(size_o-1)
    clip( res, 0, (int32_t)(size_o-1) );

    return ( (int32_t)(res) );
}
