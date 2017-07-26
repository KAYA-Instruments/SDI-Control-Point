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

#include <math.h>

#include <simple_math/rgb2ycbcr.h>

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
 * test_bt601_coefficients - tests bt601 coefficients
 *****************************************************************************/
static void test_bt601_coefficients( void )
{
    float c0, c1, c2;
    float c3, c4, c5;
    float c6, c7, c8;

    int res = cal_YCbCr_coefficients_bt601
                    ( &c0, &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8 );
    TEST_ASSERT_EQUAL_INT( 0, res );

    printf( "\nBT.601\n" );
    printf( "Y  = %+6.5f %+6.5f %+6.5f\n", c0, c1, c2 );
    printf( "Cb = %+6.5f %+6.5f %+6.5f\n", c3, c4, c5 );
    printf( "Cr = %+6.5f %+6.5f %+6.5f\n", c6, c7, c8 );
    printf( "\n" );
}

/******************************************************************************
 * test_bt709_coefficients - tests bt709 coefficients
 *****************************************************************************/
static void test_bt709_coefficients( void )
{
    float c0, c1, c2;
    float c3, c4, c5;
    float c6, c7, c8;

    int res = cal_YCbCr_coefficients_bt709
                    ( &c0, &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    printf( "\nBT.709\n" );
    printf( "Y  = %+6.5f %+6.5f %+6.5f\n", c0, c1, c2 );
    printf( "Cb = %+6.5f %+6.5f %+6.5f\n", c3, c4, c5 );
    printf( "Cr = %+6.5f %+6.5f %+6.5f\n", c6, c7, c8 );
    printf( "\n" );
}

/******************************************************************************
 * test_coefficients - tests bt709 coefficients
 *****************************************************************************/
static void test_coefficients( void )
{
    float c0_601, c1_601, c2_601;
    float c3_601, c4_601, c5_601;
    float c6_601, c7_601, c8_601;
    
    float c0_709, c1_709, c2_709;
    float c3_709, c4_709, c5_709;
    float c6_709, c7_709, c8_709;
    
    float c0, c1, c2;
    float c3, c4, c5;
    float c6, c7, c8;

    int res = cal_YCbCr_coefficients_bt601
                    ( &c0_601, &c1_601, &c2_601, &c3_601, &c4_601, &c5_601, &c6_601, &c7_601, &c8_601 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    res = cal_YCbCr_coefficients_bt709
                    ( &c0_709, &c1_709, &c2_709, &c3_709, &c4_709, &c5_709, &c6_709, &c7_709, &c8_709 );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = cal_YCbCr_coefficients( Kr_BT601, Kb_BT601, &c0, &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8 );
    TEST_ASSERT_EQUAL_INT( 0, res );

    TEST_ASSERT( fabs(c0_601 - c0) < 0.00001f );
    TEST_ASSERT( fabs(c1_601 - c1) < 0.00001f );
    TEST_ASSERT( fabs(c2_601 - c2) < 0.00001f );
    TEST_ASSERT( fabs(c3_601 - c3) < 0.00001f );
    TEST_ASSERT( fabs(c4_601 - c4) < 0.00001f );
    TEST_ASSERT( fabs(c5_601 - c5) < 0.00001f );
    TEST_ASSERT( fabs(c6_601 - c6) < 0.00001f );
    TEST_ASSERT( fabs(c7_601 - c7) < 0.00001f );
    TEST_ASSERT( fabs(c8_601 - c8) < 0.00001f );

    res = cal_YCbCr_coefficients( Kr_BT709, Kb_BT709, &c0, &c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8 );
    TEST_ASSERT_EQUAL_INT( 0, res );

    TEST_ASSERT( fabs(c0_709 - c0) < 0.00001f );
    TEST_ASSERT( fabs(c1_709 - c1) < 0.00001f );
    TEST_ASSERT( fabs(c2_709 - c2) < 0.00001f );
    TEST_ASSERT( fabs(c3_709 - c3) < 0.00001f );
    TEST_ASSERT( fabs(c4_709 - c4) < 0.00001f );
    TEST_ASSERT( fabs(c5_709 - c5) < 0.00001f );
    TEST_ASSERT( fabs(c6_709 - c6) < 0.00001f );
    TEST_ASSERT( fabs(c7_709 - c7) < 0.00001f );
    TEST_ASSERT( fabs(c8_709 - c8) < 0.00001f );
}
 
/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef rgb2ycbcr_tests( void )
{
    EMB_UNIT_TESTFIXTURES( fixtures )
    {
        new_TestFixture( "bt601 coefficients"   , test_bt601_coefficients ),
        new_TestFixture( "bt709 coefficients"   , test_bt709_coefficients ),
        new_TestFixture( "coefficients"         , test_coefficients ),
    };
    EMB_UNIT_TESTCALLER( rgb2ycbcr_tests, "RGB2YCbCr", setup, teardown, fixtures );

    return ( (TestRef)&rgb2ycbcr_tests );
}


