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

#include <simple_math/cubic.h>

/******************************************************************************
 * local macros
 *****************************************************************************/

/**< Macro to validate the passed handle */
#define CHECK_CSIP_HANDLE(handle)                   \
{                                                   \
    if (NULL == (handle) )                          \
    {                                               \
        return ( -EFAULT );                         \
    }                                               \
}

/**< Macro to validate the current state */
#define CHECK_CSIP_STATE(ctx, value)                \
{                                                   \
    if ((ctx == NULL) || (ctx->state != (value)) )  \
    {                                               \
        return ( -EFAULT );                         \
    }                                               \
}

/**< Macro to validate the current state */
#define CHECK_CSIP_STATE_RANGE(ctx, min, max)       \
{                                                   \
    if ((ctx == NULL)                          ||   \
        ((ctx->state<min) && (ctx->state>max)) )    \
    {                                               \
        return ( -EFAULT );                         \
    }                                               \
}

/******************************************************************************
 * local variables
 *****************************************************************************/
static spline_interpolation_ctx_t sSpline_interpol_ctx;

/******************************************************************************
 * sm_cubic_spline_interpolation_init
 *****************************************************************************/
int sm_cubic_spline_interpolation_init
(
    spline_interpolation_ctx_t ** ctx
)
{
    memset( &sSpline_interpol_ctx, 0 ,sizeof(sSpline_interpol_ctx) );
    sSpline_interpol_ctx.state = CSIP_STATE_INITIALIZED;
    *ctx = &sSpline_interpol_ctx;
    return ( 0 );
}

/******************************************************************************
 * sm_cubic_spline_interpolation_reset
 *****************************************************************************/
int sm_cubic_spline_interpolation_reset
(
    spline_interpolation_ctx_t * const ctx
)
{
    CHECK_CSIP_HANDLE( ctx );
    memset( ctx, 0 ,sizeof(sSpline_interpol_ctx) );
    ctx->state = CSIP_STATE_INITIALIZED;
    return ( 0 );
}

#if defined(CFG_HOLD_SAMPLES)
/******************************************************************************
 * sm_cubic_spline_interpolation_add_samples
 *
 * TODO: check the x-order: x0 < x_i < x_(i+1) < ... < x_n
 *****************************************************************************/
int sm_cubic_spline_interpolation_add_samples
(
    spline_interpolation_ctx_t * const          ctx,
    spline_interpolation_sample_type_t * const  x,
    spline_interpolation_sample_type_t * const  y,
    unsigned const                              no
)
{
    CHECK_CSIP_HANDLE( ctx );
    CHECK_CSIP_STATE_RANGE( ctx, CSIP_STATE_INITIALIZED, CSIP_STATE_GOT_SAMPLES );

    // check available size
    uint32_t n;
    if ( (CSIP_MAX_SAMPLES - ctx->n) < no )
    {
        // fill up remaining space (start with first sample)
        n = CSIP_MAX_SAMPLES - ctx->n;
    }
    else
    {
        n = no;
    }

    // copy nodes to internal memory
    uint32_t i, j;
    int found;

    for ( j = 0u; j<n; j++ ) 
    {
        found = 0;

        for (i = 0u; i<ctx->n; i++ )
        {
            if ( ctx->x[i] == x[j] )
            {
                ctx->y[i] = y[j];
                found = 1;
                break;
            }
        }

        if ( !found ) 
        {
            ctx->x[ctx->n] = x[j];
            ctx->y[ctx->n] = y[j];
            ctx->n++;
        }
    }

    // reorder sample points in ascending order
    uint32_t c, d;
    int32_t t_x, t_y;
    for (c = 1 ; c <= (n - 1); c++)
    {
        d = c;
        while ( d > 0 && ctx->x[d] < ctx->x[d-1] )
        {
            t_x        = ctx->x[d];
            t_y        = ctx->y[d];
            ctx->x[d]  = ctx->x[d-1];
            ctx->y[d]  = ctx->y[d-1];
            ctx->x[d-1] = t_x;
            ctx->y[d-1] = t_y;
            d--;
        }
    }
    
    ctx->state = CSIP_STATE_GOT_SAMPLES;

    return ( 0 );
}
#endif  /* defined(CFG_HOLD_SAMPLES) */

/******************************************************************************
 * sm_cubic_spline_interpolation_set_samples
 *
 * TODO: check the x-order: x0 < x_i < x_(i+1) < ... < x_n
 *****************************************************************************/
int sm_cubic_spline_interpolation_set_samples
(
    spline_interpolation_ctx_t * const          ctx,
    spline_interpolation_sample_type_t * const  x,
    spline_interpolation_sample_type_t * const  y,
    unsigned const                              no
)
{
    CHECK_CSIP_HANDLE( ctx );
    CHECK_CSIP_STATE_RANGE( ctx, CSIP_STATE_INITIALIZED, CSIP_STATE_GOT_SAMPLES );

    // check available size
    uint32_t n;
    if ( CSIP_MAX_SAMPLES < no )
    {
        n = CSIP_MAX_SAMPLES;
    }
    else
    {
        n = no;
    }

#if defined(CFG_HOLD_SAMPLES)
    // copy nodes to internal memory
    uint32_t i;
    
    for ( i = 0u; i<ctx->n; i++ )
    {
        ctx->x[i] = 0;
        ctx->y[i] = 0;
    }
    
    // reset number
    ctx->n = 0;

    for ( i = 0u; i<n; i++ )
    {
        ctx->x[i] = x[i];
        ctx->y[i] = y[i];
    }
#else   /* defined(CFG_HOLD_SAMPLES) */
    // set internal pointer to external memory
    ctx->x = x;
    ctx->y = y;
#endif  /* defined(CFG_HOLD_SAMPLES) */
    ctx->n = no;

    // reorder sample points in ascending order
    uint32_t c, d;
    int32_t t_x, t_y;
    for (c = 1 ; c <= (n - 1); c++)
    {
        d = c;
        while ( d > 0 && ctx->x[d] < ctx->x[d-1] )
        {
            t_x        = ctx->x[d];
            t_y        = ctx->y[d];
            ctx->x[d]  = ctx->x[d-1];
            ctx->y[d]  = ctx->y[d-1];
            ctx->x[d-1] = t_x;
            ctx->y[d-1] = t_y;
            d--;
        }
    }

    ctx->n     = n;
    ctx->state = CSIP_STATE_GOT_SAMPLES;

    return ( 0 );
}

/******************************************************************************
 * sm_cubic_spline_interpolation_calc_init
 *****************************************************************************/
int sm_cubic_spline_interpolation_calc_init
(
    spline_interpolation_ctx_t * const ctx
)
{
	double F[CSIP_MAX_SAMPLES];
	double u[CSIP_MAX_SAMPLES];
	double v[CSIP_MAX_SAMPLES];

    unsigned i, n;
    
    CHECK_CSIP_HANDLE( ctx );
    CHECK_CSIP_STATE( ctx, CSIP_STATE_GOT_SAMPLES );

    memset( F, 0, sizeof(F) );
    memset( u, 0, sizeof(u) );
    memset( v, 0, sizeof(v) );
    
    n = ctx->n - 1;

    // compute the h_i and b_i
    for( i = 0u; i<=(n-1); i++ )
    {
        ctx->h[i] = ctx->x[i+1] - ctx->x[i];
        F[i]      = (double)(ctx->y[i+1] - ctx->y[i]) / (double)ctx->h[i];
    }

    // Gaussian elimination
    u[1] = 2.0f * (double)(ctx->h[0] + ctx->h[1]);
    v[1] = 6.0f * (F[1] - F[0]);
    for( i = 2u; i<=(n-1); i++ )
    {
        u[i] = 2.0f * (ctx->h[i-1] + ctx->h[i]) - ((ctx->h[i-1] * ctx->h[i-1]) / u[i-1]);
        v[i] = 6.0f * (F[i] - F[i-1]) - ((ctx->h[i-1] * v[i-1]) / u[i-1]);
    }

    // Back-substitution
    ctx->z[n] = 0.0f;
    ctx->z[0] = 0.0f;
    for ( i=(n-1); i>0; i-- )
    {
        ctx->z[i] = (v[i] - ctx->h[i]*ctx->z[i+1]) / u[i];
    }
    
    ctx->state = CSIP_STATE_RUNNING;

    return ( 0 );
}

/******************************************************************************
 * sm_cubic_spline_interpolation_calc
 *****************************************************************************/
int sm_cubic_spline_interpolation_calc
(
    spline_interpolation_ctx_t * const          ctx,
    spline_interpolation_sample_type_t const    x,
    spline_interpolation_sample_type_t * const  y
)
{
    double a, b, c, d;   // Coefficients
    double S;
    int32_t dx;
    
    CHECK_CSIP_HANDLE( ctx );
    CHECK_CSIP_STATE( ctx, CSIP_STATE_RUNNING );

    if ( !y )
    {
        return ( -EINVAL );
    }
    
    int i, n;
    
    n = ctx->n;

    // clip y to a horizontal line
    if ( ctx->x[0] >= x )
    {
        *y = ctx->y[0];
        //*y = 0;
        return ( 0 );
    }

    // interpolate
    for( i=0; i<(n-1); i++ )
    {
        if( (ctx->x[i] <= x) && (x <= ctx->x[i+1]) )
        {
            goto found;            
        }
    }

    // clip y to a horizontal line
    *y = ctx->y[i];
    //*y = 0;

    return ( 0 );

found:
    dx = (x-ctx->x[i]);
    if ( dx != 0 )
    {
        a = (double)ctx->y[i];
        b = ((double)ctx->h[i] * ctx->z[i+1])/(-6.0f)
                - ((double)ctx->h[i] * ctx->z[i])/3.0f
                + ((double)(ctx->y[i+1]-ctx->y[i]))/(double)ctx->h[i];
        c = ctx->z[i] / 2;
        d = (ctx->z[i+1] - ctx->z[i]) / (6.0f * ctx->h[i]);

        S = a + (dx) * (b + (dx)*(c + (dx)*d));
    
        *y = (int32_t)(S + 0.5f);  // for rounding
    }
    else
    {
        *y = ctx->y[i];
    }
    
    return ( 0 );
}

