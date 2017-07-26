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
/**
 * @file    cubic_tests.c
 *
 * @brief   Implementation of unit tests for cubic spline interpolation
 *
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <simple_math/cubic.h>

#include <embUnit/embUnit.h>

/******************************************************************************
 * local definitions
 *****************************************************************************/
#ifdef _WIN32
#define sleep(n)                Sleep(1000 * n)
#endif

#define ARRAY_SIZE(x)           ( sizeof(x)/sizeof(x[0]) )

/******************************************************************************
 * called by test-framework before test-procedure, opens the com-port
 *****************************************************************************/
static void setup( void )
{
}

/******************************************************************************
 * called by test-framework after test-procedure, close the com-port
 *****************************************************************************/
static void teardown( void )
{
}

/******************************************************************************
 * test_cubic_init - checks if context initialization works
 *****************************************************************************/
static void test_cubic_init( void )
{
    int res;

    // declare interpolation context
    spline_interpolation_ctx_t * ctx = NULL;

    // init interpolation context
    res = sm_cubic_spline_interpolation_init( &ctx );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT( ctx != NULL );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_INITIALIZED, ctx->state );
    TEST_ASSERT_EQUAL_INT( 0, ctx->n );
}

#if defined(CFG_HOLD_SAMPLES)
/******************************************************************************
 * test_cubic_init - checks if adding samples works
 *****************************************************************************/
static void test_cubic_add_samples( void )
{
    int res;

    spline_interpolation_sample_type_t x_0[3] = { CSIP_SAMPLE_TYPE(1), CSIP_SAMPLE_TYPE(5), CSIP_SAMPLE_TYPE(10) };
    spline_interpolation_sample_type_t y_0[3] = { CSIP_SAMPLE_TYPE(1), CSIP_SAMPLE_TYPE(5), CSIP_SAMPLE_TYPE(10) };
    
    spline_interpolation_sample_type_t x_1[5] = { CSIP_SAMPLE_TYPE(4), CSIP_SAMPLE_TYPE(8), CSIP_SAMPLE_TYPE(12), CSIP_SAMPLE_TYPE(14), CSIP_SAMPLE_TYPE(16) };
    spline_interpolation_sample_type_t y_1[5] = { CSIP_SAMPLE_TYPE(4), CSIP_SAMPLE_TYPE(8), CSIP_SAMPLE_TYPE(12), CSIP_SAMPLE_TYPE(14), CSIP_SAMPLE_TYPE(16) };
    
    spline_interpolation_sample_type_t x_2[3] = { CSIP_SAMPLE_TYPE(5), CSIP_SAMPLE_TYPE(10), CSIP_SAMPLE_TYPE(11) };
    spline_interpolation_sample_type_t y_2[3] = { CSIP_SAMPLE_TYPE(5), CSIP_SAMPLE_TYPE(10), CSIP_SAMPLE_TYPE(11) };

    // declare interpolation context
    spline_interpolation_ctx_t * ctx = NULL;

    // init interpolation context
    res = sm_cubic_spline_interpolation_init( &ctx );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT( ctx != NULL );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_INITIALIZED, ctx->state );

    /////////////////////////////////////////////////////////////////
    // TEST-CASE 1: Add 2 different series
    /////////////////////////////////////////////////////////////////

    // add samples x_0, y_0 to interpolation context
    TEST_ASSERT( ARRAY_SIZE(x_0) == ARRAY_SIZE(y_0) );
    res = sm_cubic_spline_interpolation_add_samples( ctx, x_0, y_0, ARRAY_SIZE(x_0) );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_GOT_SAMPLES, ctx->state );
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(x_0), ctx->n );

    // add samples x_1, y_1 to interpolation context
    TEST_ASSERT( ARRAY_SIZE(x_1) == ARRAY_SIZE(y_1) );
    res = sm_cubic_spline_interpolation_add_samples( ctx, x_1, y_1, ARRAY_SIZE(x_1) );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_GOT_SAMPLES, ctx->state );
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(x_1) + ARRAY_SIZE(x_0), ctx->n );
   
    // check the values
    TEST_ASSERT( x_0[0] == ctx->x[0] );     // 1, 1
    TEST_ASSERT( x_1[0] == ctx->x[1] );     // 4, 4
    TEST_ASSERT( x_0[1] ==  ctx->x[2] );     // 5, 5
    TEST_ASSERT( x_1[1] ==  ctx->x[3] );     // 8, 8
    TEST_ASSERT( x_0[2] ==  ctx->x[4] );     // 10, 10
    TEST_ASSERT( x_1[2] ==  ctx->x[5] );     // 12, 12
    TEST_ASSERT( x_1[3] ==  ctx->x[6] );     // 14, 14
    TEST_ASSERT( x_1[4] ==  ctx->x[7] );     // 16, 16

    TEST_ASSERT( y_0[0] == ctx->y[0] );     // 1, 1
    TEST_ASSERT( y_1[0] == ctx->y[1] );     // 4, 4
    TEST_ASSERT( y_0[1] == ctx->y[2] );     // 5, 5
    TEST_ASSERT( y_1[1] == ctx->y[3] );     // 8, 8
    TEST_ASSERT( y_0[2] == ctx->y[4] );     // 10, 10
    TEST_ASSERT( y_1[2] == ctx->y[5] );     // 12, 12
    TEST_ASSERT( y_1[3] == ctx->y[6] );     // 14, 14
    TEST_ASSERT( y_1[4] == ctx->y[7] );     // 16, 16

    /////////////////////////////////////////////////////////////////
    // TEST-CASE 2: Add a serie twice
    /////////////////////////////////////////////////////////////////

    // reset interpolation context
    res = sm_cubic_spline_interpolation_reset( ctx );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT( ctx != NULL );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_INITIALIZED, ctx->state );
    TEST_ASSERT_EQUAL_INT( 0, ctx->n );

    // add samples x_0, y_0 to interpolation context
    TEST_ASSERT( ARRAY_SIZE(x_0) == ARRAY_SIZE(y_0) );
    res = sm_cubic_spline_interpolation_add_samples( ctx, x_0, y_0, ARRAY_SIZE(x_0) );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_GOT_SAMPLES, ctx->state );
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(x_0), ctx->n );

    // add samples x_0, y_0 to interpolation context
    TEST_ASSERT( ARRAY_SIZE(x_0) == ARRAY_SIZE(y_0) );
    res = sm_cubic_spline_interpolation_add_samples( ctx, x_0, y_0, ARRAY_SIZE(x_0) );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_GOT_SAMPLES, ctx->state );
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(x_0), ctx->n );

    // check the values
    TEST_ASSERT( x_0[0] == ctx->x[0] );     // 1, 1
    TEST_ASSERT( x_0[1] == ctx->x[1] );     // 5, 5
    TEST_ASSERT( x_0[2] == ctx->x[2] );     // 10, 10

    TEST_ASSERT( y_0[0] == ctx->y[0] );     // 1, 1
    TEST_ASSERT( y_0[1] == ctx->y[1] );     // 5, 5
    TEST_ASSERT( y_0[2] == ctx->y[2] );     // 10, 10

    /////////////////////////////////////////////////////////////////
    // TEST-CASE 3: Add 2 series with 2 equal x values
    /////////////////////////////////////////////////////////////////

    // reset interpolation context
    res = sm_cubic_spline_interpolation_reset( ctx );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT( ctx != NULL );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_INITIALIZED, ctx->state );
    TEST_ASSERT_EQUAL_INT( 0, ctx->n );

    // add samples x_0, y_0 to interpolation context
    TEST_ASSERT( ARRAY_SIZE(x_0) == ARRAY_SIZE(y_0) );
    res = sm_cubic_spline_interpolation_add_samples( ctx, x_0, y_0, ARRAY_SIZE(x_0) );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_GOT_SAMPLES, ctx->state );
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(x_0), ctx->n );

    // add samples x_2, y_2 to interpolation context
    TEST_ASSERT( ARRAY_SIZE(x_2) == ARRAY_SIZE(y_2) );
    res = sm_cubic_spline_interpolation_add_samples( ctx, x_2, y_2, ARRAY_SIZE(x_2) );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_GOT_SAMPLES, ctx->state );
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(x_0) + ARRAY_SIZE(x_2) - 2, ctx->n );

    // check the values
    TEST_ASSERT( x_0[0] == ctx->x[0] );     // 1, 1
    TEST_ASSERT( x_0[1] == ctx->x[1] );     // 5, 5
    TEST_ASSERT( x_0[2] == ctx->x[2] );     // 10, 10
    TEST_ASSERT( x_2[2] == ctx->x[3] );     // 11, 11

    TEST_ASSERT( y_0[0] == ctx->y[0] );     // 1, 1
    TEST_ASSERT( y_0[1] == ctx->y[1] );     // 5, 5
    TEST_ASSERT( y_0[2] == ctx->y[2] );     // 10, 10
    TEST_ASSERT( y_2[2] == ctx->y[3] );     // 11, 11
}
#endif // CFG_HOLD_SAMPLES

/******************************************************************************
 * test_cubic_interpolation - checks if cubic spline interpolation works
 *****************************************************************************/
static void test_cubic_set_samples( void )
{
    spline_interpolation_sample_type_t x_0[3] = { CSIP_SAMPLE_TYPE(1), CSIP_SAMPLE_TYPE(5), CSIP_SAMPLE_TYPE(10) };
    spline_interpolation_sample_type_t y_0[3] = { CSIP_SAMPLE_TYPE(1), CSIP_SAMPLE_TYPE(5), CSIP_SAMPLE_TYPE(10) };
    
    spline_interpolation_sample_type_t x_1[5] = { CSIP_SAMPLE_TYPE(4), CSIP_SAMPLE_TYPE(8), CSIP_SAMPLE_TYPE(12), CSIP_SAMPLE_TYPE(14), CSIP_SAMPLE_TYPE(16) };
    spline_interpolation_sample_type_t y_1[5] = { CSIP_SAMPLE_TYPE(4), CSIP_SAMPLE_TYPE(8), CSIP_SAMPLE_TYPE(12), CSIP_SAMPLE_TYPE(14), CSIP_SAMPLE_TYPE(16) };
    
    int res;

    // declare interpolation context
    spline_interpolation_ctx_t * ctx = NULL;

    // init interpolation context
    res = sm_cubic_spline_interpolation_init( &ctx );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT( ctx != NULL );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_INITIALIZED, ctx->state );
    TEST_ASSERT_EQUAL_INT( 0, ctx->n );

    /////////////////////////////////////////////////////////////////
    // TEST-CASE 1: Set a series of sample values
    /////////////////////////////////////////////////////////////////

    // set samples x_0, y_0 to interpolation context
    TEST_ASSERT( ARRAY_SIZE(x_0) == ARRAY_SIZE(y_0) );
    res = sm_cubic_spline_interpolation_set_samples( ctx, x_0, y_0, ARRAY_SIZE(x_0) );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_GOT_SAMPLES, ctx->state );
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(x_0), ctx->n );

    // check the values
    TEST_ASSERT( x_0[0] == ctx->x[0] );     // 1, 1
    TEST_ASSERT( x_0[1] == ctx->x[1] );     // 5, 5
    TEST_ASSERT( x_0[2] == ctx->x[2] );     // 10, 10

    TEST_ASSERT( y_0[0] == ctx->y[0] );     // 1, 1
    TEST_ASSERT( y_0[1] == ctx->y[1] );     // 5, 5
    TEST_ASSERT( y_0[2] == ctx->y[2] );     // 10, 10

    /////////////////////////////////////////////////////////////////
    // TEST-CASE 2: Overwrite samples with a second series
    /////////////////////////////////////////////////////////////////

    // reset interpolation context
    res = sm_cubic_spline_interpolation_reset( ctx );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT( ctx != NULL );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_INITIALIZED, ctx->state );
    TEST_ASSERT_EQUAL_INT( 0, ctx->n );

    // set samples x_1, y_1 to interpolation context
    TEST_ASSERT( ARRAY_SIZE(x_1) == ARRAY_SIZE(y_1) );
    res = sm_cubic_spline_interpolation_set_samples( ctx, x_1, y_1, ARRAY_SIZE(x_1) );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_GOT_SAMPLES, ctx->state );
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(x_1), ctx->n );

    // set samples x_0, y_0 to interpolation context
    TEST_ASSERT( ARRAY_SIZE(x_0) == ARRAY_SIZE(y_0) );
    res = sm_cubic_spline_interpolation_set_samples( ctx, x_0, y_0, ARRAY_SIZE(x_0) );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_GOT_SAMPLES, ctx->state );
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(x_0), ctx->n );

    // check the values
    TEST_ASSERT( x_0[0] == ctx->x[0] );     // 1, 1
    TEST_ASSERT( x_0[1] == ctx->x[1] );     // 5, 5
    TEST_ASSERT( x_0[2] == ctx->x[2] );     // 10, 10

    TEST_ASSERT( y_0[0] == ctx->y[0] );     // 1, 1
    TEST_ASSERT( y_0[1] == ctx->y[1] );     // 5, 5
    TEST_ASSERT( y_0[2] == ctx->y[2] );     // 10, 10
}

/******************************************************************************
 * test_cubic_linear_interpolation 
 * - checks if spline interpolation works for linear curves (algorithm check)
 *****************************************************************************/
static void test_cubic_linear_interpolation( void )
{
    // linear curve
    spline_interpolation_sample_type_t x_0[5] = { CSIP_SAMPLE_TYPE(0), CSIP_SAMPLE_TYPE(5), CSIP_SAMPLE_TYPE(10), CSIP_SAMPLE_TYPE(15), CSIP_SAMPLE_TYPE(20) };
    spline_interpolation_sample_type_t y_0[5] = { CSIP_SAMPLE_TYPE(0), CSIP_SAMPLE_TYPE(5), CSIP_SAMPLE_TYPE(10), CSIP_SAMPLE_TYPE(15), CSIP_SAMPLE_TYPE(20) };

    spline_interpolation_sample_type_t y;

    int res;

    int32_t i;

    // declare interpolation context
    spline_interpolation_ctx_t * ctx = NULL;

    // init interpolation context
    res = sm_cubic_spline_interpolation_init( &ctx );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT( ctx != NULL );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_INITIALIZED, ctx->state );
    TEST_ASSERT_EQUAL_INT( 0, ctx->n );

    // set samples x_0, y_0 to interpolation context
    TEST_ASSERT( ARRAY_SIZE(x_0) == ARRAY_SIZE(y_0) );
    res = sm_cubic_spline_interpolation_set_samples( ctx, x_0, y_0, ARRAY_SIZE(x_0) );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_GOT_SAMPLES, ctx->state );
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(x_0), ctx->n );

    // calculation int
    res = sm_cubic_spline_interpolation_calc_init( ctx );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_RUNNING, ctx->state );
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(x_0), ctx->n );

    // check the values
    TEST_ASSERT_EQUAL_INT( x_0[0], ctx->x[0] );     // 0, 0
    TEST_ASSERT_EQUAL_INT( x_0[1], ctx->x[1] );     // 5, 5
    TEST_ASSERT_EQUAL_INT( x_0[2], ctx->x[2] );     // 10, 10
    TEST_ASSERT_EQUAL_INT( x_0[3], ctx->x[3] );     // 15, 15
    TEST_ASSERT_EQUAL_INT( x_0[4], ctx->x[4] );     // 20, 20

    TEST_ASSERT_EQUAL_INT( y_0[0], ctx->y[0] );     // 0, 0
    TEST_ASSERT_EQUAL_INT( y_0[1], ctx->y[1] );     // 5, 5
    TEST_ASSERT_EQUAL_INT( y_0[2], ctx->y[2] );     // 10, 10
    TEST_ASSERT_EQUAL_INT( y_0[3], ctx->y[3] );     // 15, 15
    TEST_ASSERT_EQUAL_INT( y_0[4], ctx->y[4] );     // 20, 20

    // run over sample points
    TEST_ASSERT( ARRAY_SIZE(x_0) == ARRAY_SIZE(y_0) );
    for ( i = 0u; i<(int32_t)ARRAY_SIZE(x_0); i++ )
    {
        res = sm_cubic_spline_interpolation_calc( ctx, x_0[i], &y );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT( ctx->y[i] == y );
    }

    // run from x_0 to x_n by incrementing
    TEST_ASSERT( ARRAY_SIZE(x_0) == ARRAY_SIZE(y_0) );
    for ( i = x_0[0u]; i<=x_0[ARRAY_SIZE(x_0)-1]; i++ )
    {
        res = sm_cubic_spline_interpolation_calc( ctx, CSIP_SAMPLE_TYPE(i), &y );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT( CSIP_SAMPLE_TYPE(i) == y );
    }
}

/******************************************************************************
 * test_square_linear_interpolation 
 * - checks if spline interpolation works for square curves (algorithm check)
 *****************************************************************************/
static void test_cubic_square_interpolation( void )
{
    // linear curve
    spline_interpolation_sample_type_t x_0[5]
        = { CSIP_SAMPLE_TYPE(5), CSIP_SAMPLE_TYPE(10), CSIP_SAMPLE_TYPE(15), CSIP_SAMPLE_TYPE(20), CSIP_SAMPLE_TYPE(25) };
    spline_interpolation_sample_type_t y_0[5]
        = { x_0[0]*x_0[0], x_0[1]*x_0[1], x_0[2]*x_0[2], x_0[3]*x_0[3], x_0[4]*x_0[4] };

    spline_interpolation_sample_type_t y;

    int res;

    int32_t i;

    // declare interpolation context
    spline_interpolation_ctx_t * ctx = NULL;

    // init interpolation context
    res = sm_cubic_spline_interpolation_init( &ctx );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT( ctx != NULL );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_INITIALIZED, ctx->state );
    TEST_ASSERT_EQUAL_INT( 0, ctx->n );

    // set samples x_0, y_0 to interpolation context
    TEST_ASSERT( ARRAY_SIZE(x_0) == ARRAY_SIZE(y_0) );
    res = sm_cubic_spline_interpolation_set_samples( ctx, x_0, y_0, ARRAY_SIZE(x_0) );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_GOT_SAMPLES, ctx->state );
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(x_0), ctx->n );

    // calculation int
    res = sm_cubic_spline_interpolation_calc_init( ctx );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( CSIP_STATE_RUNNING, ctx->state );
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(x_0), ctx->n );

    // check the values
    TEST_ASSERT_EQUAL_INT( x_0[0], ctx->x[0] );     // 0, 0
    TEST_ASSERT_EQUAL_INT( x_0[1], ctx->x[1] );     // 5, 5
    TEST_ASSERT_EQUAL_INT( x_0[2], ctx->x[2] );     // 10, 10
    TEST_ASSERT_EQUAL_INT( x_0[3], ctx->x[3] );     // 15, 15
    TEST_ASSERT_EQUAL_INT( x_0[4], ctx->x[4] );     // 20, 20

    TEST_ASSERT_EQUAL_INT( y_0[0], ctx->y[0] );     // 0, 0
    TEST_ASSERT_EQUAL_INT( y_0[1], ctx->y[1] );     // 5, 5
    TEST_ASSERT_EQUAL_INT( y_0[2], ctx->y[2] );     // 10, 10
    TEST_ASSERT_EQUAL_INT( y_0[3], ctx->y[3] );     // 15, 15
    TEST_ASSERT_EQUAL_INT( y_0[4], ctx->y[4] );     // 20, 20

    // run over sample points
    TEST_ASSERT( ARRAY_SIZE(x_0) == ARRAY_SIZE(y_0) );
    for ( i = 0u; i<(int32_t)ARRAY_SIZE(x_0); i++ )
    {
        res = sm_cubic_spline_interpolation_calc( ctx, x_0[i], &y );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT( ctx->y[i] == y );
    }

    // run from x_0 to x_n by incrementing
    double mse = 0.0f;  // mean square error
    TEST_ASSERT( ARRAY_SIZE(x_0) == ARRAY_SIZE(y_0) );
    for ( i = x_0[0u]; i<=x_0[ARRAY_SIZE(x_0)-1]; i++ )
    {
        res = sm_cubic_spline_interpolation_calc( ctx, CSIP_SAMPLE_TYPE(i), &y );
        TEST_ASSERT_EQUAL_INT( 0, res );
        mse += (i*i-y) * (i*i-y);
    }
    mse /= (double)i;
    TEST_ASSERT( mse < 1.5f );
}

/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef cubic_tests( void )
{
    EMB_UNIT_TESTFIXTURES( fixtures )
    {
        new_TestFixture( "cubic_init"                , test_cubic_init ),
#if defined(CFG_HOLD_SAMPLES)
        new_TestFixture( "cubic_add_samples"         , test_cubic_add_samples ),
#endif
        new_TestFixture( "cubic_set_samples"         , test_cubic_set_samples ),
        new_TestFixture( "cubic_linear_interpolation", test_cubic_linear_interpolation ),
        new_TestFixture( "cubic_square_interpolation", test_cubic_square_interpolation ),
    };
    EMB_UNIT_TESTCALLER( cubic_tests, "CUBIC SPLINE INTERPOLATION", setup, teardown, fixtures );

    return ( (TestRef)&cubic_tests );
}


