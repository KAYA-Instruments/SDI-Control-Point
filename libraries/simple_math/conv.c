/******************************************************************************
 *
 * Copyright 2015, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
#include <simple_math/conv.h>

/******************************************************************************
 * init - set all component of a sample vector to sample
 *****************************************************************************/
void init
(
    conv_sample_type_t * const  samples,
    unsigned const              len,
    conv_sample_type_t const    sample
)
{
    if ( samples && (len > 0u) )
    {
        unsigned i;
        for ( i=0u; i<len; i++ )
        {
            samples[i] = sample;
        }
    }
}

/******************************************************************************
 * ones - set all component of a sample vector to 1
 *****************************************************************************/
void ones
(
    conv_sample_type_t * const   samples,
    unsigned const               len
)
{
    init( samples, len, CONV_SAMPLE_TYPE(1) );
}

/******************************************************************************
 * sum - calculates the component sum of a sample vector
 *****************************************************************************/
conv_sample_type_t sum
(
    conv_sample_type_t * const  samples,
    unsigned const              len
)
{
    if ( samples && (len > 0u) )
    {
        unsigned i;
        conv_sample_type_t sum = CONV_SAMPLE_TYPE(0);
        for ( i=0u; i<len; i++ )
        {
            sum += samples[i];
        }
        return ( sum );
    }

    return ( CONV_SAMPLE_TYPE(0) );
}

/******************************************************************************
 * upscale - multiplies a sample vector by a scalar
 *****************************************************************************/
void upscale
( 
    conv_sample_type_t * const  samples,
    unsigned const              len,
    conv_sample_type_t const    fac
)
{
    if ( samples && (len > 0u) )
    {
        unsigned i;
        for( i=0u; i<len; i++ )
        {
            samples[i] *= fac;
        }
    }
}

/******************************************************************************
 * downscale - divides a sample vector by a scalar
 *****************************************************************************/
void downscale
(
    conv_sample_type_t * const  samples,
    unsigned const              len,
    conv_sample_type_t const    div
)
{
    if ( samples && (len > 0) && (div != CONV_SAMPLE_TYPE(0)) )
    {
        unsigned i;
        for( i=0u; i<len; i++ )
        {
            samples[i] /= div;
        }
    }
}

/******************************************************************************
 * divide -  divides corresponding vector components
 *****************************************************************************/
void divide
(
    conv_sample_type_t * const  samples,
    conv_sample_type_t * const  div,
    conv_sample_type_t * const  result,
    unsigned const              len
)
{
    if ( samples && div && result && (len > 0) )
    {
        unsigned i;
        for ( i=0 ; i<len ; i++ )
        {
            result[i] = samples[i] / div[i];
        }
    }
}

/******************************************************************************
 * subtract - subtracts corresponding vector components
 *****************************************************************************/
void subtract
(
    conv_sample_type_t * const  samples_a,
    conv_sample_type_t * const  samples_b,
    conv_sample_type_t * const  result,
    unsigned const              len
)
{
    if ( samples_a && samples_b && result && (len>0) ) 
    {
        unsigned i;
        for ( i=0 ; i<len; i++ )
        {
            result[i] = samples_a[i] - samples_b[i];
        }
    }
}

/******************************************************************************
 * conv - runs the convolution (result = samples * filter)
 *****************************************************************************/
void conv
(
    conv_sample_type_t * const  samples,
    unsigned const              samples_len,
    conv_sample_type_t * const  filter,
    unsigned const              filter_len,
    conv_sample_type_t * const  result,
    unsigned const              result_len
)
{
    if ( samples && (samples_len > 0) &&
         filter  && (filter_len  > 0) && 
         result  && (result_len >= (samples_len + filter_len - 1u)) )
    {
        unsigned i;
        for ( i = 0u; i < (samples_len + filter_len - 1u); i++ )
        {
            unsigned kmin, kmax, k;

            result[i] = CONV_SAMPLE_TYPE(0);

            kmin = (i >= filter_len  - 1u) ? i - (filter_len  - 1u) : 0;
            kmax = (i <  samples_len - 1u) ? i : (samples_len - 1u);

            for ( k = kmin; k <= kmax; k++ )
            {
                result[i] += samples[k] * filter[i - k];
            }
        }
    }
}

/******************************************************************************
 * filter2 - calculates the filter2-function
 * (filter2 is similar to conv, but the output signal has the same size like
 *  the input signal)
 *****************************************************************************/
void filter2
(
    conv_sample_type_t * const  filter,
    unsigned const              filter_len,
    conv_sample_type_t * const  samples,
    unsigned const              samples_len,
    conv_sample_type_t * const  result
)
{
    if ( samples && (samples_len > 0) &&
         filter  && (filter_len  > 0) &&
         result                       )
    {
        conv_sample_type_t r[samples_len + filter_len - 1u];
        
        unsigned i;

        conv( samples, samples_len, filter, filter_len, r, (samples_len + filter_len - 1u) );

        // cut the result, so it's length is equal to the
        // signal length and rotate by 180 degrees
        for ( i=0u; i<samples_len; i++ )
        {
            result[i] = r[i+(filter_len/2)];
        }
    }
}

