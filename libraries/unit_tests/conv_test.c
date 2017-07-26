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

#include <simple_math/conv.h>

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
 * test_conv_init - checks if vector initialization works
 *****************************************************************************/
static void test_conv_init( void )
{
    conv_sample_type_t vector_0[10];
    conv_sample_type_t vector_1[10];

    conv_sample_type_t sum_0;
    conv_sample_type_t sum_1;

    unsigned i;

    // reset vector_0 and vector_1
    memset( vector_0, 0, sizeof(vector_0) );
    memset( vector_1, 0, sizeof(vector_1) );

    // fill vector_0 with '1'
    ones( vector_0, ARRAY_SIZE(vector_0) );
    for ( i=0u; i<ARRAY_SIZE(vector_0); i++ )
    {
        TEST_ASSERT( vector_0[i] == CONV_SAMPLE_TYPE(1) );
    }
    // sum-up vector_0
    sum_0 = sum( vector_0, ARRAY_SIZE(vector_0) );
    TEST_ASSERT( sum_0 == ARRAY_SIZE(vector_0) * CONV_SAMPLE_TYPE(1) )
    
    // fill vector_1 with '200'
    init( vector_1, ARRAY_SIZE(vector_1), CONV_SAMPLE_TYPE(200) );
    for ( i=0u; i<ARRAY_SIZE(vector_1); i++ )
    {
        TEST_ASSERT( vector_1[i] == CONV_SAMPLE_TYPE(200) );
    }
    // sum-up vector_1
    sum_1 = sum( vector_1, ARRAY_SIZE(vector_1) );
    TEST_ASSERT( sum_1 == ARRAY_SIZE(vector_1) * CONV_SAMPLE_TYPE(200) )
}


/******************************************************************************
 * test_conv_upscale - checks if vector upscaling works
 *****************************************************************************/
static void test_conv_upscale( void )
{
    conv_sample_type_t vector_0[10];
    conv_sample_type_t vector_1[10];

    unsigned i;

    // reset vector_0 and vector_1
    memset( vector_0, 0, sizeof(vector_0) );
    memset( vector_1, 0, sizeof(vector_1) );
    
    // expect same vector size 
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(vector_0), ARRAY_SIZE(vector_1) );

    // fill vector_0 with '1'
    ones( vector_0, ARRAY_SIZE(vector_0) );
    for ( i=0u; i<ARRAY_SIZE(vector_0); i++ )
    {
        TEST_ASSERT( vector_0[i] == CONV_SAMPLE_TYPE(1) );
    }

    // fill vector_1 with '200'
    init( vector_1, ARRAY_SIZE(vector_1), CONV_SAMPLE_TYPE(200) );
    for ( i=0u; i<ARRAY_SIZE(vector_1); i++ )
    {
        TEST_ASSERT( vector_1[i] == CONV_SAMPLE_TYPE(200) );
    }

    // upscale vector_0  by '200'
    upscale( vector_0, ARRAY_SIZE(vector_0), CONV_SAMPLE_TYPE(200) );
    for ( i=0u; i<ARRAY_SIZE(vector_0); i++ )
    {
        TEST_ASSERT( vector_0[i] == vector_1[i] );
        TEST_ASSERT( vector_0[i] == CONV_SAMPLE_TYPE(200) );
    }

    // downscale vector_0  by '200'
    downscale( vector_0, ARRAY_SIZE(vector_0), CONV_SAMPLE_TYPE(200) );
    for ( i=0u; i<ARRAY_SIZE(vector_0); i++ )
    {
        TEST_ASSERT( vector_0[i] != vector_1[i] );
        TEST_ASSERT( vector_0[i] == CONV_SAMPLE_TYPE(1) );
        TEST_ASSERT( vector_1[i] == CONV_SAMPLE_TYPE(200) );
    }

    // fill vector_0 with '400'
    upscale( vector_0, ARRAY_SIZE(vector_0), CONV_SAMPLE_TYPE(400) );
    upscale( vector_1, ARRAY_SIZE(vector_1), CONV_SAMPLE_TYPE(2) );
    for ( i=0u; i<ARRAY_SIZE(vector_1); i++ )
    {
        TEST_ASSERT( vector_0[i] == vector_1[i] );
        TEST_ASSERT( vector_0[i] == CONV_SAMPLE_TYPE(400) );
        TEST_ASSERT( vector_1[i] == CONV_SAMPLE_TYPE(400) );
    }
}

/******************************************************************************
 * test_conv_divide - checks if vector substracting and dividing works
 *****************************************************************************/
static void test_conv_divide( void )
{
    conv_sample_type_t vector_0[10];
    conv_sample_type_t vector_1[10];
    conv_sample_type_t vector_2[10];
    conv_sample_type_t vector_3[10];

    unsigned i;

    // reset vector_0 and vector_1
    memset( vector_0, 0, sizeof(vector_0) );
    memset( vector_1, 0, sizeof(vector_1) );
    memset( vector_2, 0, sizeof(vector_2) );
    memset( vector_3, 0, sizeof(vector_3) );
    
    // expect same vector size 
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(vector_0), ARRAY_SIZE(vector_1) );

    // fill vector_0 with '100'
    init( vector_0, ARRAY_SIZE(vector_0), CONV_SAMPLE_TYPE(100) );
    for ( i=0u; i<ARRAY_SIZE(vector_0); i++ )
    {
        TEST_ASSERT( vector_0[i] == CONV_SAMPLE_TYPE(100) );
    }

    // fill vector_1 with '200'
    init( vector_1, ARRAY_SIZE(vector_1), CONV_SAMPLE_TYPE(200) );
    for ( i=0u; i<ARRAY_SIZE(vector_1); i++ )
    {
        TEST_ASSERT( vector_1[i] == CONV_SAMPLE_TYPE(200) );
    }

    // divide vector_1 by vector_0
    divide( vector_1, vector_0, vector_2, ARRAY_SIZE(vector_1) );
    for ( i=0u; i<ARRAY_SIZE(vector_1); i++ )
    {
        TEST_ASSERT( vector_2[i] == CONV_SAMPLE_TYPE(2) );
    }

    // substract vector_1 by vector_0
    subtract( vector_1, vector_0, vector_3, ARRAY_SIZE(vector_1) );
    for ( i=0u; i<ARRAY_SIZE(vector_1); i++ )
    {
        TEST_ASSERT( vector_3[i] == CONV_SAMPLE_TYPE(100) );
    }

}

/******************************************************************************
 * test_conv_conv - checks if convolution works
 *****************************************************************************/
static void test_conv_conv( void )
{
    conv_sample_type_t vector_0[10];
    conv_sample_type_t vector_1[20];
    conv_sample_type_t vector_2[ARRAY_SIZE(vector_0) + ARRAY_SIZE(vector_1) - 1u];

    unsigned i;

    conv_sample_type_t sample;

    // reset vector_0 and vector_1
    memset( vector_0, 0, sizeof(vector_0) );
    memset( vector_1, 0, sizeof(vector_1) );
    memset( vector_2, 0, sizeof(vector_2) );
 
    // expect same vector size 
    TEST_ASSERT_EQUAL_INT( ARRAY_SIZE(vector_0) + ARRAY_SIZE(vector_1) - 1u, ARRAY_SIZE(vector_2) );

    // fill vector_0 with '1'
    init( vector_0, ARRAY_SIZE(vector_0), CONV_SAMPLE_TYPE(1) );
    for ( i=0u; i<ARRAY_SIZE(vector_0); i++ )
    {
        TEST_ASSERT( vector_0[i] == CONV_SAMPLE_TYPE(1) );
    }

    // vector_2 = vector_0 * vector_0 (=> triangle)
    conv( vector_0, ARRAY_SIZE(vector_0), vector_0, ARRAY_SIZE(vector_0), vector_2, ARRAY_SIZE(vector_2) );
    sample = CONV_SAMPLE_TYPE(0);
    for ( i=0u; i<ARRAY_SIZE(vector_0) + ARRAY_SIZE(vector_0) - 1u; i++ )
    {
        if ( i <= (ARRAY_SIZE(vector_0) + ARRAY_SIZE(vector_0) - 1u)/2 )
        {
            sample += CONV_SAMPLE_TYPE(1);
        }
        else
        {
            sample -= CONV_SAMPLE_TYPE(1);

        }
        TEST_ASSERT( sample == vector_2[i] );
    }

    // fill vector_1 with '1'
    init( vector_1, ARRAY_SIZE(vector_1), CONV_SAMPLE_TYPE(1) );
    for ( i=0u; i<ARRAY_SIZE(vector_1); i++ )
    {
        TEST_ASSERT( vector_1[i] == CONV_SAMPLE_TYPE(1) );
    }
    
    // vector_2 = vector_0 * vector_1 (=> trapezoid)
    TEST_ASSERT(  ARRAY_SIZE(vector_0) <  ARRAY_SIZE(vector_1) );
    conv( vector_0, ARRAY_SIZE(vector_0), vector_1, ARRAY_SIZE(vector_1), vector_2, ARRAY_SIZE(vector_2) );
    sample = CONV_SAMPLE_TYPE(0);
    for ( i=0u; i<ARRAY_SIZE(vector_0) + ARRAY_SIZE(vector_1) - 1u; i++ )
    {
        if ( i < ARRAY_SIZE(vector_0) )
        {
            sample += CONV_SAMPLE_TYPE(1);
        }
        else if ( i < ARRAY_SIZE(vector_1) )
        {
            sample -= CONV_SAMPLE_TYPE(0);
        }
        else
        {
            sample -= CONV_SAMPLE_TYPE(1);

        }
        TEST_ASSERT( sample == vector_2[i] );
    }
}

/******************************************************************************
 * test_conv_filter2 - checks if filter2 function works
 *****************************************************************************/
static void test_conv_filter2( void )
{
    conv_sample_type_t vector_0[10];
    conv_sample_type_t vector_1[20];
    conv_sample_type_t result[20];

    unsigned i;

    // reset vector_0 and vector_1
    memset( vector_0, 0, sizeof(vector_0) );
    memset( vector_1, 0, sizeof(vector_1) );

    // fill vector_0 and vector_1 with '1'
    ones( vector_0, ARRAY_SIZE(vector_0) );
    ones( vector_1, ARRAY_SIZE(vector_1) );

    for ( i=0u; i<ARRAY_SIZE(vector_0); i++ )
    {
        TEST_ASSERT( vector_0[i] == CONV_SAMPLE_TYPE(1) );
    }

    for ( i=0u; i<ARRAY_SIZE(vector_1); i++ )
    {
        TEST_ASSERT( vector_1[i] == CONV_SAMPLE_TYPE(1) );
    }

    filter2( vector_0, ARRAY_SIZE(vector_0), vector_1, ARRAY_SIZE(vector_1), result );

#if 0
    for ( i=0u; i<ARRAY_SIZE(result); i++ )
    {
        printf( "f_%d = "CONV_SAMPLE_FORMAT"\n", i, result[i] );
    }
#endif    
}


/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef conv_tests( void )
{
    EMB_UNIT_TESTFIXTURES( fixtures )
    {
        new_TestFixture( "conv_init"   , test_conv_init ),
        new_TestFixture( "conv_upscale", test_conv_upscale ),
        new_TestFixture( "conv_divide" , test_conv_divide ),
        new_TestFixture( "conv_conv"   , test_conv_conv ),
        new_TestFixture( "conv_filter2", test_conv_filter2 ),
    };
    EMB_UNIT_TESTCALLER( conv_tests, "CONVOLUTION", setup, teardown, fixtures );

    return ( (TestRef)&conv_tests );
}


