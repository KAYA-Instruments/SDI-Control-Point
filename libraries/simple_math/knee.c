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
#include <string.h>
#include <errno.h>
#include <math.h>

#include <simple_math/knee.h>

// tone map function (knee function)
// y = s * x          for x <= x0
// y = a*(x+c)^p - b  for x > x0  ,  p = 1/2;

/**< Macro to validate the passed handle */
#define CHECK_KIP_HANDLE(handle)                    \
{                                                   \
    if (NULL == (handle) )                          \
    {                                               \
        return ( -EFAULT );                         \
    }                                               \
}

/**< Macro to validate the current state */
#define CHECK_KIP_STATE(ctx, value)                 \
{                                                   \
    if ((ctx == NULL) || (ctx->state != (value)) )  \
    {                                               \
        return ( -EFAULT );                         \
    }                                               \
}

/******************************************************************************
 * local variables
 *****************************************************************************/
static knee_interpolation_ctx_t sKnee_interpol_ctx;

/******************************************************************************
 * sm_knee_interpolation_init
 *****************************************************************************/
int sm_knee_interpolation_init
(
    knee_interpolation_ctx_t ** ctx
)
{
    memset( &sKnee_interpol_ctx, 0 ,sizeof(sKnee_interpol_ctx) );
    sKnee_interpol_ctx.state = KIP_STATE_INITIALIZED;
    *ctx = &sKnee_interpol_ctx;
    return ( 0 );
}

/******************************************************************************
 * sm_knee_interpolation_reset
 *****************************************************************************/
int sm_knee_interpolation_reset
(
    knee_interpolation_ctx_t * const ctx
)
{
    CHECK_KIP_HANDLE( ctx );
    memset( ctx, 0 ,sizeof(sKnee_interpol_ctx) );
    ctx->state = KIP_STATE_INITIALIZED;
    return ( 0 );
}

/******************************************************************************
 * sm_knee_interpolation_calc_init
 *****************************************************************************/
int sm_knee_interpolation_calc_init
(
    knee_interpolation_ctx_t * const   ctx,
    uint8_t const                      knee,
    uint16_t const                     slope,
    uint8_t const                      clip
)
{
    float k  = 0.0f;
    float x0 = 0.0f;
    float s  = 0.0f;
    float cl = 0.0f;

    CHECK_KIP_HANDLE( ctx );
    CHECK_KIP_STATE( ctx, KIP_STATE_INITIALIZED );

    // range check slope
    if ( (slope <= 100) || (slope > 1600) )
    {
        return ( -EINVAL );
    }

    // range check white-clip
    if ( (clip < 100) || (clip > 109) )
    {
        return ( -EINVAL );
    }

    // range check knee position
    if ( (knee < 1) || (knee > (clip-1)) )
    {
        return ( -EINVAL );
    }

    k  = powf( (float)knee / 100.0f, 1.0f / 0.45f ) * 100.0f;
    cl = (float)clip / 100.0f;
    s  = (float)slope / 100.0f;
    x0 = k / 100.0f / powf( (float)cl, 1.0f/0.45f ) / s;

    ctx->a = -s * powf( 1.0f/(s * (s - 1.0f)), 0.5f ) * (s*x0 - 1);
    ctx->c = ((s*x0)*(s*x0) - 4*(s*x0)*s + 2*(s*x0) + 1) / (4*s*(s - 1));
    ctx->b = ctx->a * powf(ctx->c+1.0f, 0.5f) - 1.0f;

    ctx->x0 = x0;
    ctx->s = s;
    
    ctx->state = KIP_STATE_RUNNING;

    return ( 0 );
}

/******************************************************************************
 * sm_knee_interpolation_calc
 *****************************************************************************/
int sm_knee_interpolation_calc
(
    knee_interpolation_ctx_t * const   ctx,
    uint32_t const                     x,
    uint32_t * const                   gain,
    uint8_t const                      bit_width_input,
    uint8_t const                      bit_width_output
)
{
    float Vin       = (float)x;
    float Vout      = 0.0f;
    
    CHECK_KIP_HANDLE( ctx );
    CHECK_KIP_STATE( ctx, KIP_STATE_RUNNING );
    
    uint32_t size_i = (1ul << bit_width_input);
    uint32_t size_o = (1ul << bit_width_output);

    if ( !ctx || !gain )
    {
        return ( -EINVAL );
    }

    Vin  /= (float)size_i;  // normalize to range 0..1
    if ( Vin < ctx->x0 )
    {
        Vout  = ctx->s;
    }
    else
    {
        Vout  = Vin;
        Vout += ctx->c;
        Vout  = powf( Vout, 0.5f );
        Vout *= ctx->a;
        Vout -= ctx->b;
        Vout /= Vin;
    }
    
    Vout *= (float)size_o;  // normalize to output bit-width
    Vout += 0.5f;           // for rounding
    *gain = (uint32_t)Vout;

    return ( 0 );
}


