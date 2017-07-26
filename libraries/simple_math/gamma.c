/**************************************************************************//**
 *
 * Copyright 2015, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
#include <stdint.h>

#include <simple_math/simple_math.h>

/******************************************************************************
 * sm_degamma
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
    clip( res, 0, ((int32_t)(size_o-1)) );

    return ( ((uint32_t)(res)) );
}

/******************************************************************************
 * sm_gamma_correction
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
    clip( res, 0, ((int32_t)(size_o-1)) );

    return ( ((uint32_t)(res)) );
}


