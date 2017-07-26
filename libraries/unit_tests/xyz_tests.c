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
 * @file    xyz_tests.c
 *
 * @brief   Implementation of unit tests for cubic spline interpolation
 *
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <math.h>

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

#define SCALING_FACTOR          ( 1.0f )

static double sRGB[9]           = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
static double sXYZ_bradford[9]  = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
static double sC_bradford[9]    = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
static double sXYZ_kries[9]     = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
static double sC_kries[9]       = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

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

extern int XYZtoCorColorTemp_Robertson( double *xyz, double * ct );
extern int XYZtoCorColorTemp_McCamy( double *xyz, double * ct );
extern int CorColorTemptoXYZ( double ct, double * xyz );

#if 0
static int RGBtoXYZ_D50( double *rgb, double * xyz )
{
    // transform RGB -> XYZ
    // use Matrix for D50 illuminations
    double m[9] =
    { 
        0.4360747f, 0.3850649f, 0.1430804f, 
        0.2225045f, 0.7168786f, 0.0606169f,
        0.0139322f, 0.0971045f, 0.7141733f
    };
 
    xyz[0] = m[0]*rgb[0] + m[1]*rgb[1] + m[2]*rgb[2];
    xyz[1] = m[3]*rgb[0] + m[4]*rgb[1] + m[5]*rgb[2];
    xyz[2] = m[6]*rgb[0] + m[7]*rgb[1] + m[8]*rgb[2];

    return ( 0 );
}
#endif

static int RGBtoXYZ_D65( double *rgb, double * xyz )
{
    // transform RGB -> XYZ
    // use Matrix for D65 illuminations
    double m[9] =
    { 
        0.4124564f, 0.3575761f, 0.1804375f, 
        0.2126729f, 0.7151522f, 0.0721750f,
        0.0193339f, 0.1191920f, 0.9503041f
    };
 
    xyz[0] = m[0]*rgb[0] + m[1]*rgb[1] + m[2]*rgb[2];
    xyz[1] = m[3]*rgb[0] + m[4]*rgb[1] + m[5]*rgb[2];
    xyz[2] = m[6]*rgb[0] + m[7]*rgb[1] + m[8]*rgb[2];

    return ( 0 );
}

static int XYZtoRGB( double *xyz, double * rgb )
{
    // transform XYZ -> RGB 
    // use Matrix for D65 illuminations
    double m[9] =
    { 
        3.2404542f, -1.5371385f, -0.4985314f,
       -0.9692660f,  1.8760108f,  0.0415560f,
        0.0556434f, -0.2040259f,  1.0572252f
    };
 
    rgb[0] = m[0]*xyz[0] + m[1]*xyz[1] + m[2]*xyz[2];
    rgb[1] = m[3]*xyz[0] + m[4]*xyz[1] + m[5]*xyz[2];
    rgb[2] = m[6]*xyz[0] + m[7]*xyz[1] + m[8]*xyz[2];

    return ( 0 );
}

static int InverseMatrix( double * M, double * INV )
{
    double det = (   (M[0]*((M[8u]*M[4u] - M[7u]*M[5u])))
                   - (M[1]*((M[8u]*M[3u] - M[6u]*M[5u])))
                   + (M[2]*((M[7u]*M[3u] - M[6u]*M[4u]))) );
    
    if ( det >  1.0e-20 )
    {
        INV[0] = (M[8]*M[4] - M[7]*M[5]) / det;
        INV[1] = (M[7]*M[2] - M[8]*M[1]) / det;
        INV[2] = (M[5]*M[1] - M[4]*M[2]) / det;
        INV[3] = (M[6]*M[5] - M[8]*M[3]) / det;
        INV[4] = (M[8]*M[0] - M[6]*M[2]) / det;
        INV[5] = (M[3]*M[2] - M[5]*M[0]) / det;
        INV[6] = (M[7]*M[3] - M[6]*M[4]) / det;
        INV[7] = (M[6]*M[1] - M[7]*M[0]) / det;
        INV[8] = (M[4]*M[0] - M[3]*M[1]) / det;
    }
    else
    {
        return ( -EINVAL );
    }

    return ( 0 );
}

typedef struct rgb_vector_s
{
    int red;
    int green;
    int blue;
} rgb_vector_t;


/******************************************************************************
 * test_xyz_illu_A - checks if conversion to XYZ color space works
 *****************************************************************************/
static void test_xyz_illu_A( void )
{
    rgb_vector_t rgb_meas[10] =
    {
        { 800, 800, 768 },
        { 784, 784, 752 },
        { 784, 768, 736 },
        { 784, 784, 752 },
        { 784, 768, 736 },
        { 768, 768, 720 },
        { 784, 784, 752 },
        { 784, 768, 736 },
        { 784, 784, 752 },
        { 784, 768, 736 }
    };

    rgb_vector_t rgb_meas_wp_revert[10] =
    {
        { 793, 747, 365 },
        { 777, 732, 358 },
        { 773, 720, 351 },
        { 777, 732, 358 },
        { 773, 720, 351 },
        { 757, 715, 346 },
        { 777, 732, 358 },
        { 773, 720, 351 },
        { 777, 732, 358 },
        { 773, 720, 351 }
    };

    // correlation transform Bradford D65 -> A in XYZ space
    double m1[9] = {
        1.2164557f, 0.1109905f, -0.1549325f, 
        0.1533326f, 0.9152313f, -0.0559953f,
       -0.0239469f, 0.0358984f,  0.3147529f
    };

    // correlation transform von Kries D65 -> A in XYZ space
    double m2[9] = {
        1.0710732f, 0.2441000f, -0.1502741f,
        0.0268127f, 0.9804060f, -0.0054100f,
        0.0000000f, 0.0000000f,  0.3268187f
    };

    double RGB[3];
    double XYZ[3];
    double XYZ_bradford[3];
    double XYZ_kries[3];

    double red   = 0.0f;
    double green = 0.0f;
    double blue  = 0.0f;
    
    double red1   = 0.0f;
    double green1 = 0.0f;
    double blue1  = 0.0f;

    double x_wp;
    double y_wp;
    
    double ct_bradford = 0.0f;
    double ct_kries    = 0.0f;

    int res;
    
    for ( unsigned i=0; i<ARRAY_SIZE(rgb_meas); i++ )
    {
        red   += (double)(rgb_meas[i].red);
        green += (double)(rgb_meas[i].green);
        blue  += (double)(rgb_meas[i].blue);
        
        red1   += (double)(rgb_meas_wp_revert[i].red);
        green1 += (double)(rgb_meas_wp_revert[i].green);
        blue1  += (double)(rgb_meas_wp_revert[i].blue);
    }

    red   /= ARRAY_SIZE(rgb_meas);
    green /= ARRAY_SIZE(rgb_meas);
    blue  /= ARRAY_SIZE(rgb_meas);
    
    red1   /= ARRAY_SIZE(rgb_meas);
    green1 /= ARRAY_SIZE(rgb_meas);
    blue1  /= ARRAY_SIZE(rgb_meas);

    RGB[0] = red   / SCALING_FACTOR;
    RGB[1] = green / SCALING_FACTOR;
    RGB[2] = blue  / SCALING_FACTOR;

    res = RGBtoXYZ_D65( RGB, XYZ );
    TEST_ASSERT_EQUAL_INT( 0, res );

    x_wp = XYZ[0] / (XYZ[0] + XYZ[1] + XYZ[2]);
    y_wp = XYZ[1] / (XYZ[0] + XYZ[1] + XYZ[2]);

    XYZ_bradford[0] = m1[0]*XYZ[0] + m1[1]*XYZ[1] + m1[2]*XYZ[2];
    XYZ_bradford[1] = m1[3]*XYZ[0] + m1[4]*XYZ[1] + m1[5]*XYZ[2];
    XYZ_bradford[2] = m1[6]*XYZ[0] + m1[7]*XYZ[1] + m1[8]*XYZ[2];
    
    XYZ_kries[0] = m2[0]*XYZ[0] + m2[1]*XYZ[1] + m2[2]*XYZ[2];
    XYZ_kries[1] = m2[3]*XYZ[0] + m2[4]*XYZ[1] + m2[5]*XYZ[2];
    XYZ_kries[2] = m2[6]*XYZ[0] + m2[7]*XYZ[1] + m2[8]*XYZ[2];

    res = XYZtoCorColorTemp_Robertson( XYZ_bradford, &ct_bradford );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    res = XYZtoCorColorTemp_Robertson( XYZ_kries, &ct_kries );
    TEST_ASSERT_EQUAL_INT( 0, res );

    printf("\nR=%8.5f; G=%8.5f; B=%8.5f\n", RGB[0], RGB[1], RGB[2] );
    printf("X=%8.5f; Y=%8.5f; Z=%8.5f\n", XYZ[0], XYZ[1], XYZ[2] );
    printf("x_wp=%0.5f; y_wp = %0.5f\n", x_wp, y_wp );
    
    printf("bradford: ct = %8.2fK, X=%8.5f; Y=%8.5f; Z=%8.5f\n",
            ct_bradford, XYZ_bradford[0], XYZ_bradford[1], XYZ_bradford[2] );
    printf("kries   : ct = %8.2fK, X=%8.5f; Y=%8.5f; Z=%8.5f\n",
            ct_kries, XYZ_kries[0], XYZ_kries[1], XYZ_kries[2] );

    sRGB[0] = red1   / SCALING_FACTOR;
    sRGB[3] = green1 / SCALING_FACTOR;
    sRGB[6] = blue1  / SCALING_FACTOR;
    
    sXYZ_bradford[0] = XYZ_bradford[0];
    sXYZ_bradford[3] = XYZ_bradford[1];
    sXYZ_bradford[6] = XYZ_bradford[2];
    
    sXYZ_kries[0] = XYZ_kries[0];
    sXYZ_kries[3] = XYZ_kries[1];
    sXYZ_kries[6] = XYZ_kries[2];
}

/******************************************************************************
 * test_xyz_illu_CWF - checks if conversion to XYZ color space works
 *****************************************************************************/
static void test_xyz_illu_CWF( void )
{
    rgb_vector_t rgb_meas[7] =
    {
        { 416, 432, 400 },
        { 416, 432, 400 },
        { 400, 432, 400 },
        { 400, 432, 400 },
        { 400, 432, 400 },
        { 400, 432, 400 },
        { 400, 432, 400 },
    };
  
    rgb_vector_t rgb_meas_wp_revert[7] =
    {
        { 278, 426, 227 },
        { 278, 426, 227 },
        { 272, 423, 226 },
        { 272, 423, 226 },
        { 272, 423, 226 },
        { 272, 423, 226 },
        { 272, 423, 226 },
    };

    // correlation transform Bradford D65 -> CWF in XYZ space
    double m1[9] = {
        1.0902706f, 0.0445381f, -0.0816910f, 
        0.0593007f, 0.9745354f, -0.0283781f,
       -0.0142228f, 0.0226100f,  0.6105988f
    };

    // correlation transform von Kries D65 -> CWF in XYZ space
    double m2[9] = {
        1.0300500f, 0.1032084f, -0.0830066f,
        0.0113367f, 0.9917148f, -0.0022869f,
        0.0000000f, 0.0000000f,  0.6189488f,
    };

    double RGB[3];
    double XYZ[3];
    double XYZ_bradford[3];
    double XYZ_kries[3];

    double red   = 0.0f;
    double green = 0.0f;
    double blue  = 0.0f;

    double red1   = 0.0f;
    double green1 = 0.0f;
    double blue1  = 0.0f;

    double x_wp;
    double y_wp;
    
    double ct_bradford = 0.0f;
    double ct_kries    = 0.0f;

    int res;
    
    for ( unsigned i=0; i<ARRAY_SIZE(rgb_meas); i++ )
    {
        red   += (double)(rgb_meas[i].red);
        green += (double)(rgb_meas[i].green);
        blue  += (double)(rgb_meas[i].blue);
        
        red1   += (double)(rgb_meas_wp_revert[i].red);
        green1 += (double)(rgb_meas_wp_revert[i].green);
        blue1  += (double)(rgb_meas_wp_revert[i].blue);
    }

    red   /= ARRAY_SIZE(rgb_meas);
    green /= ARRAY_SIZE(rgb_meas);
    blue  /= ARRAY_SIZE(rgb_meas);

    red1   /= ARRAY_SIZE(rgb_meas);
    green1 /= ARRAY_SIZE(rgb_meas);
    blue1  /= ARRAY_SIZE(rgb_meas);

    RGB[0] = red   / SCALING_FACTOR;
    RGB[1] = green / SCALING_FACTOR;
    RGB[2] = blue  / SCALING_FACTOR;

    res = RGBtoXYZ_D65( RGB, XYZ );
    TEST_ASSERT_EQUAL_INT( 0, res );

    x_wp = XYZ[0] / (XYZ[0] + XYZ[1] + XYZ[2]);
    y_wp = XYZ[1] / (XYZ[0] + XYZ[1] + XYZ[2]);

    XYZ_bradford[0] = m1[0]*XYZ[0] + m1[1]*XYZ[1] + m1[2]*XYZ[2];
    XYZ_bradford[1] = m1[3]*XYZ[0] + m1[4]*XYZ[1] + m1[5]*XYZ[2];
    XYZ_bradford[2] = m1[6]*XYZ[0] + m1[7]*XYZ[1] + m1[8]*XYZ[2];
    
    XYZ_kries[0] = m2[0]*XYZ[0] + m2[1]*XYZ[1] + m2[2]*XYZ[2];
    XYZ_kries[1] = m2[3]*XYZ[0] + m2[4]*XYZ[1] + m2[5]*XYZ[2];
    XYZ_kries[2] = m2[6]*XYZ[0] + m2[7]*XYZ[1] + m2[8]*XYZ[2];

    res = XYZtoCorColorTemp_Robertson( XYZ_bradford, &ct_bradford );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    res = XYZtoCorColorTemp_Robertson( XYZ_kries, &ct_kries );
    TEST_ASSERT_EQUAL_INT( 0, res );

    printf("\nR=%8.5f; G=%8.5f; B=%8.5f\n", RGB[0], RGB[1], RGB[2] );
    printf("X=%8.5f; Y=%8.5f; Z=%8.5f\n", XYZ[0], XYZ[1], XYZ[2] );
    printf("x_wp=%0.5f; y_wp = %0.5f\n", x_wp, y_wp );
    
    printf("bradford: ct = %8.2fK, X=%8.5f; Y=%8.5f; Z=%8.5f\n",
            ct_bradford, XYZ_bradford[0], XYZ_bradford[1], XYZ_bradford[2] );
    printf("kries   : ct = %8.2fK, X=%8.5f; Y=%8.5f; Z=%8.5f\n",
            ct_kries, XYZ_kries[0], XYZ_kries[1], XYZ_kries[2] );
    
    sRGB[1] = red1   / SCALING_FACTOR;
    sRGB[4] = green1 / SCALING_FACTOR;
    sRGB[7] = blue1  / SCALING_FACTOR;
 
    sXYZ_bradford[1] = XYZ_bradford[0];
    sXYZ_bradford[4] = XYZ_bradford[1];
    sXYZ_bradford[7] = XYZ_bradford[2];
    
    sXYZ_kries[1] = XYZ_kries[0];
    sXYZ_kries[4] = XYZ_kries[1];
    sXYZ_kries[7] = XYZ_kries[2];
}

/******************************************************************************
 * test_xyz_illu_D65 - checks if conversion to XYZ color space works
 *****************************************************************************/
static void test_xyz_illu_D65( void )
{
    rgb_vector_t rgb_meas[8] =
    {
        { 448, 464, 464 }, 
        { 448, 464, 464 },
        { 448, 464, 464 },
        { 448, 464, 464 },
        { 448, 464, 464 },
        { 448, 464, 464 },
        { 464, 464, 464 },
        { 448, 480, 464 }
    };

    rgb_vector_t rgb_meas_wp_revert[8] =
    {
        { 271, 461, 405 }, 
        { 271, 461, 405 },
        { 271, 461, 405 },
        { 271, 461, 405 },
        { 271, 461, 405 },
        { 271, 461, 405 },
        { 277, 463, 406 },
        { 273, 472, 409 }
    };

    double RGB[3];
    double XYZ[3];

    double red   = 0.0f;
    double green = 0.0f;
    double blue  = 0.0f;

    double red1   = 0.0f;
    double green1 = 0.0f;
    double blue1  = 0.0f;

    double x_wp;
    double y_wp;
    
    double ct = 0.0f;

    int res;
    
    for ( unsigned i=0; i<ARRAY_SIZE(rgb_meas); i++ )
    {
        red   += (double)(rgb_meas[i].red);
        green += (double)(rgb_meas[i].green);
        blue  += (double)(rgb_meas[i].blue);

        red1   += (double)(rgb_meas_wp_revert[i].red);
        green1 += (double)(rgb_meas_wp_revert[i].green);
        blue1  += (double)(rgb_meas_wp_revert[i].blue);
    }

    red   /= ARRAY_SIZE(rgb_meas);
    green /= ARRAY_SIZE(rgb_meas);
    blue  /= ARRAY_SIZE(rgb_meas);
    
    red1   /= ARRAY_SIZE(rgb_meas);
    green1 /= ARRAY_SIZE(rgb_meas);
    blue1  /= ARRAY_SIZE(rgb_meas);

    RGB[0] = red   / SCALING_FACTOR;
    RGB[1] = green / SCALING_FACTOR;
    RGB[2] = blue  / SCALING_FACTOR;

    res = RGBtoXYZ_D65( RGB, XYZ );
    TEST_ASSERT_EQUAL_INT( 0, res );

    x_wp = XYZ[0] / (XYZ[0] + XYZ[1] + XYZ[2]);
    y_wp = XYZ[1] / (XYZ[0] + XYZ[1] + XYZ[2]);

    res = XYZtoCorColorTemp_Robertson( XYZ, &ct );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    printf("\nR=%8.5f; G=%8.5f; B=%8.5f\n", RGB[0], RGB[1], RGB[2] );
    printf("X=%8.5f; Y=%8.5f; Z=%8.5f\n", XYZ[0], XYZ[1], XYZ[2] );
    printf("x_wp=%0.5f; y_wp = %0.5f\n", x_wp, y_wp );
    
    printf("ct = %8.2fK, X=%8.5f; Y=%8.5f; Z=%8.5f\n",
            ct, XYZ[0], XYZ[1], XYZ[2] );
    
    sRGB[2] = red1   / SCALING_FACTOR;
    sRGB[5] = green1 / SCALING_FACTOR;
    sRGB[8] = blue1  / SCALING_FACTOR;

    sXYZ_bradford[2] = XYZ[0];
    sXYZ_bradford[5] = XYZ[1];
    sXYZ_bradford[8] = XYZ[2];
    
    sXYZ_kries[2] = XYZ[0];
    sXYZ_kries[5] = XYZ[1];
    sXYZ_kries[8] = XYZ[2];
}
 
/******************************************************************************
 * test_xyz_transformation_matrices - checks if conversion to XYZ color space works
 *****************************************************************************/
static void test_xyz_transformation_matrices( void )
{
    double invRGB[9];
    int res;
 
    printf("\n");
    printf("             ( R_A=%8.5f; R_CWF=%8.5f; R_D65=%8.5f )\n", sRGB[0], sRGB[1], sRGB[2] );
    printf("         M = ( G_A=%8.5f; G_CWF=%8.5f; G_D65=%8.5f )\n", sRGB[3], sRGB[4], sRGB[5] );
    printf("             ( B_A=%8.5f; B_CWF=%8.5f; B_D65=%8.5f )\n", sRGB[6], sRGB[7], sRGB[8] );

    printf("\n");
    printf("             ( X_A=%8.5f; X_CWF=%8.5f; X_D65=%8.5f )\n", sXYZ_bradford[0], sXYZ_bradford[1], sXYZ_bradford[2] );
    printf("M_bradford = ( Y_A=%8.5f; Y_CWF=%8.5f; Y_D65=%8.5f )\n", sXYZ_bradford[3], sXYZ_bradford[4], sXYZ_bradford[5] );
    printf("             ( Z_A=%8.5f; Z_CWF=%8.5f; Z_D65=%8.5f )\n", sXYZ_bradford[6], sXYZ_bradford[7], sXYZ_bradford[8] );

    printf("\n");
    printf("             ( X_A=%8.5f; X_CWF=%8.5f; X_D65=%8.5f )\n", sXYZ_kries[0], sXYZ_kries[1], sXYZ_kries[2] );
    printf("   M_kries = ( Y_A=%8.5f; Y_CWF=%8.5f; Y_D65=%8.5f )\n", sXYZ_kries[3], sXYZ_kries[4], sXYZ_kries[5] );
    printf("             ( Z_A=%8.5f; Z_CWF=%8.5f; Z_D65=%8.5f )\n", sXYZ_kries[6], sXYZ_kries[7], sXYZ_kries[8] );

    res = InverseMatrix( sRGB, invRGB );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    printf("\n");
    printf("             ( R_A=%8.5f; R_CWF=%8.5f; R_D65=%8.5f )\n", invRGB[0], invRGB[1], invRGB[2] );
    printf("     inv M = ( G_A=%8.5f; G_CWF=%8.5f; G_D65=%8.5f )\n", invRGB[3], invRGB[4], invRGB[5] );
    printf("             ( B_A=%8.5f; B_CWF=%8.5f; B_D65=%8.5f )\n", invRGB[6], invRGB[7], invRGB[8] );

    // C = T * S^(-1)
    sC_bradford[0] = sXYZ_bradford[0] * invRGB[0] + sXYZ_bradford[1] * invRGB[3] + sXYZ_bradford[2] * invRGB[6];
    sC_bradford[1] = sXYZ_bradford[0] * invRGB[1] + sXYZ_bradford[1] * invRGB[4] + sXYZ_bradford[2] * invRGB[7];
    sC_bradford[2] = sXYZ_bradford[0] * invRGB[2] + sXYZ_bradford[1] * invRGB[5] + sXYZ_bradford[2] * invRGB[8];
            
    sC_bradford[3] = sXYZ_bradford[3] * invRGB[0] + sXYZ_bradford[4] * invRGB[3] + sXYZ_bradford[5] * invRGB[6];
    sC_bradford[4] = sXYZ_bradford[3] * invRGB[1] + sXYZ_bradford[4] * invRGB[4] + sXYZ_bradford[5] * invRGB[7];
    sC_bradford[5] = sXYZ_bradford[3] * invRGB[2] + sXYZ_bradford[4] * invRGB[5] + sXYZ_bradford[5] * invRGB[8];
            
    sC_bradford[6] = sXYZ_bradford[6] * invRGB[0] + sXYZ_bradford[7] * invRGB[3] + sXYZ_bradford[8] * invRGB[6];
    sC_bradford[7] = sXYZ_bradford[6] * invRGB[1] + sXYZ_bradford[7] * invRGB[4] + sXYZ_bradford[8] * invRGB[7];
    sC_bradford[8] = sXYZ_bradford[6] * invRGB[2] + sXYZ_bradford[7] * invRGB[5] + sXYZ_bradford[8] * invRGB[8];

    printf("\n");
    printf("             ( R_A=%8.5f; R_CWF=%8.5f; R_D65=%8.5f )\n", sC_bradford[0], sC_bradford[1], sC_bradford[2] );
    printf("C_bradford = ( G_A=%8.5f; G_CWF=%8.5f; G_D65=%8.5f )\n", sC_bradford[3], sC_bradford[4], sC_bradford[5] );
    printf("             ( B_A=%8.5f; B_CWF=%8.5f; B_D65=%8.5f )\n", sC_bradford[6], sC_bradford[7], sC_bradford[8] );

    // C = T * S^(-1)
    sC_kries[0] = sXYZ_kries[0] * invRGB[0] + sXYZ_kries[1] * invRGB[3] + sXYZ_kries[2] * invRGB[6];
    sC_kries[1] = sXYZ_kries[0] * invRGB[1] + sXYZ_kries[1] * invRGB[4] + sXYZ_kries[2] * invRGB[7];
    sC_kries[2] = sXYZ_kries[0] * invRGB[2] + sXYZ_kries[1] * invRGB[5] + sXYZ_kries[2] * invRGB[8];
            
    sC_kries[3] = sXYZ_kries[3] * invRGB[0] + sXYZ_kries[4] * invRGB[3] + sXYZ_kries[5] * invRGB[6];
    sC_kries[4] = sXYZ_kries[3] * invRGB[1] + sXYZ_kries[4] * invRGB[4] + sXYZ_kries[5] * invRGB[7];
    sC_kries[5] = sXYZ_kries[3] * invRGB[2] + sXYZ_kries[4] * invRGB[5] + sXYZ_kries[5] * invRGB[8];
            
    sC_kries[6] = sXYZ_kries[6] * invRGB[0] + sXYZ_kries[7] * invRGB[3] + sXYZ_kries[8] * invRGB[6];
    sC_kries[7] = sXYZ_kries[6] * invRGB[1] + sXYZ_kries[7] * invRGB[4] + sXYZ_kries[8] * invRGB[7];
    sC_kries[8] = sXYZ_kries[6] * invRGB[2] + sXYZ_kries[7] * invRGB[5] + sXYZ_kries[8] * invRGB[8];

    printf("\n");
    printf("             ( R_A=%8.5f; R_CWF=%8.5f; R_D65=%8.5f )\n", sC_kries[0], sC_kries[1], sC_kries[2] );
    printf("   C_kries = ( G_A=%8.5f; G_CWF=%8.5f; G_D65=%8.5f )\n", sC_kries[3], sC_kries[4], sC_kries[5] );
    printf("             ( B_A=%8.5f; B_CWF=%8.5f; B_D65=%8.5f )\n", sC_kries[6], sC_kries[7], sC_kries[8] );
}


/******************************************************************************
 * test_xyz_correlated_ct - checks if conversion to XYZ color space works
 *****************************************************************************/
static void test_illumination( double * RGB )
{
    double XYZ[3];
    double XYZ_ct[3];

    double ct;
    double ct_XYZ;

    int res;

    // bradford
    XYZ[0] = sC_bradford[0] * RGB[0] + sC_bradford[1] * RGB[1] + sC_bradford[2] * RGB[2];
    XYZ[1] = sC_bradford[3] * RGB[0] + sC_bradford[4] * RGB[1] + sC_bradford[5] * RGB[2];
    XYZ[2] = sC_bradford[6] * RGB[0] + sC_bradford[7] * RGB[1] + sC_bradford[8] * RGB[2];

    // McCamy
    res = XYZtoCorColorTemp_McCamy( XYZ, &ct );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = CorColorTemptoXYZ( ct, XYZ_ct );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = XYZtoCorColorTemp_McCamy( XYZ_ct, &ct_XYZ );
    TEST_ASSERT_EQUAL_INT( 0, res );

    printf( "McCamy + Bradford\n" );
    printf( "X = %6.2f, Y = %6.2f, Z = %6.2f\n", XYZ[0], XYZ[1], XYZ[2] );
    printf( "X = %6.2f, Y = %6.2f, Z = %6.2f\n", XYZ_ct[0], XYZ_ct[1], XYZ_ct[2] );
    printf( "ct=%8.5fK, ct'=%8.5fK d_ct=%8.5fK\n", ct, ct_XYZ, fabs(ct-ct_XYZ) );
    
    // delta < 5%
    TEST_ASSERT( fabs(ct-ct_XYZ) < ct*0.05f );
    
    // Robertson 
    res = XYZtoCorColorTemp_Robertson( XYZ, &ct );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = CorColorTemptoXYZ( ct, XYZ_ct );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    res = XYZtoCorColorTemp_McCamy( XYZ_ct, &ct_XYZ );
    TEST_ASSERT_EQUAL_INT( 0, res );

    printf( "Robertson + Bradford\n");
    printf( "X = %6.2f, Y = %6.2f, Z = %6.2f\n", XYZ[0], XYZ[1], XYZ[2] );
    printf( "X = %6.2f, Y = %6.2f, Z = %6.2f\n", XYZ_ct[0], XYZ_ct[1], XYZ_ct[2] );
    printf( "ct=%8.5fK, ct'=%8.5fK d_ct=%8.5fK\n", ct, ct_XYZ, fabs(ct-ct_XYZ) );
   
    // delta < 5%
    TEST_ASSERT( fabs(ct-ct_XYZ) < ct*0.05f );

    // von kries
    XYZ[0] = sC_kries[0] * RGB[0] + sC_kries[1] * RGB[1] + sC_kries[2] * RGB[2];
    XYZ[1] = sC_kries[3] * RGB[0] + sC_kries[4] * RGB[1] + sC_kries[5] * RGB[2];
    XYZ[2] = sC_kries[6] * RGB[0] + sC_kries[7] * RGB[1] + sC_kries[8] * RGB[2];

    // McCamy
    res = XYZtoCorColorTemp_McCamy( XYZ, &ct );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    res = CorColorTemptoXYZ( ct, XYZ_ct );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    res = XYZtoCorColorTemp_McCamy( XYZ_ct, &ct_XYZ );
    TEST_ASSERT_EQUAL_INT( 0, res );

    printf( "McCamy + Kries\n");
    printf( "X = %6.2f, Y = %6.2f, Z = %6.2f\n", XYZ[0], XYZ[1], XYZ[2] );
    printf( "X = %6.2f, Y = %6.2f, Z = %6.2f\n", XYZ_ct[0], XYZ_ct[1], XYZ_ct[2] );
    printf( "ct=%8.5fK, ct'=%8.5fK d_ct=%8.5fK\n", ct, ct_XYZ, fabs(ct-ct_XYZ) );
    
    // delta < 5%
    TEST_ASSERT( fabs(ct-ct_XYZ) < ct*0.05f );

    // Robertson 
    res = XYZtoCorColorTemp_Robertson( XYZ, &ct );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = CorColorTemptoXYZ( ct, XYZ_ct );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    res = XYZtoCorColorTemp_McCamy( XYZ_ct, &ct_XYZ );
    TEST_ASSERT_EQUAL_INT( 0, res );

    printf( "Robertson + Kries\n" );
    printf( "X = %6.2f, Y = %6.2f, Z = %6.2f\n", XYZ[0], XYZ[1], XYZ[2] );
    printf( "X = %6.2f, Y = %6.2f, Z = %6.2f\n", XYZ_ct[0], XYZ_ct[1], XYZ_ct[2] );
    printf( "ct=%8.5fK, ct'=%8.5fK d_ct=%8.5fK\n", ct, ct_XYZ, fabs(ct-ct_XYZ) );
    
    // delta < 5%
    TEST_ASSERT( fabs(ct-ct_XYZ) < ct*0.05f );
}

static void test_xyz_correlated_ct( void )
{
    double RGB_A[3]    = { 793.f, 747.f, 365.f };
    double RGB_CWF[3]  = { 278.f, 426.f, 227.f };
    double RGB_D65[3]  = { 271.f, 461.f, 405.f };
    double RGB_TL84[3] = { 568.f, 715.f, 404.f };
    double RGB_CWF1[3] = { 389.f, 599.f, 329.f };
    double RGB_CWF2[3] = { 354.f, 534.f, 268.f };

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // A
    ///////////////////////////////////////////////////////////////////////////////////////////////
    printf("\n--- Illu A ---\n");
    test_illumination( RGB_A );

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // CWF
    ///////////////////////////////////////////////////////////////////////////////////////////////
    printf("\n--- Illu CWF ---\n");
    test_illumination( RGB_CWF );

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // D65
    ///////////////////////////////////////////////////////////////////////////////////////////////
    printf("\n--- Illu D65 ---\n");
    test_illumination( RGB_D65 );

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // TL84
    ///////////////////////////////////////////////////////////////////////////////////////////////
    printf("\n--- Illu TL84 ---\n");
    test_illumination( RGB_TL84 );

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // CWF1
    ///////////////////////////////////////////////////////////////////////////////////////////////
    printf("\n--- Illu CWF1 ---\n");
    test_illumination( RGB_CWF1 );

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // CWF2
    ///////////////////////////////////////////////////////////////////////////////////////////////
    printf("\n--- Illu CWF2 ---\n");
    test_illumination( RGB_CWF2 );
}

/******************************************************************************
 * test_xyz_colortemp - checks if color-temperature calculation works
 *****************************************************************************/
static void test_xyz_colortemp( void )
{
    double XYZ[3];
    double RGB[3];
    
    double ct[25] =
    {
        3000, 3100, 3200, 3300, 3400,
        3450, 3500, 3600, 3700, 3800,
        4000, 4500, 5000, 5500, 6000,
        6500, 7000, 7500, 8000, 8500,
        9000, 9200, 9400, 9500, 9900,
    };
    
    double ct_res;

    int res;

    printf("\n");
    for ( unsigned i=0; i<ARRAY_SIZE(ct); i++ )
    {
        res = CorColorTemptoXYZ( ct[i], XYZ );
        TEST_ASSERT_EQUAL_INT( 0, res );

        res = XYZtoCorColorTemp_Robertson( XYZ, &ct_res );
        TEST_ASSERT_EQUAL_INT( 0, res );

        if ( !res )
        {
            printf("%2d) ct=%5.1fK, delta_ct=%2.8f\n", i, ct[i], fabs(ct[i] - ct_res) );
        }
        
        // +/- 1% 
        TEST_ASSERT( (fabs(ct[i] - ct_res) < (ct[i] * 0.01)) );

        res = XYZtoRGB( XYZ, RGB );
        TEST_ASSERT_EQUAL_INT( 0, res );
    
        res = RGBtoXYZ_D65( RGB, XYZ );
        TEST_ASSERT_EQUAL_INT( 0, res );
        
        res = XYZtoCorColorTemp_Robertson( XYZ, &ct_res );
        TEST_ASSERT_EQUAL_INT( 0, res );

        if ( !res )
        {
            printf("%2d) ct=%5.1fK, delta_ct=%2.8f\n", i, ct[i], fabs(ct[i] - ct_res) );
        }
        
        // +/- 1% 
        TEST_ASSERT( (fabs(ct[i] - ct_res) < (ct[i] * 0.01)) );
    }
}

/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef xyz_tests( void )
{
    EMB_UNIT_TESTFIXTURES( fixtures )
    {
        new_TestFixture( "xyz_illu_A"                   , test_xyz_illu_A ),
        new_TestFixture( "xyz_illu_CWF"                 , test_xyz_illu_CWF ),
        new_TestFixture( "xyz_illu_D65"                 , test_xyz_illu_D65 ),
        new_TestFixture( "xyz_transformation_matrices"  , test_xyz_transformation_matrices ),
        new_TestFixture( "xyz_correlated_ct"            , test_xyz_correlated_ct ),
        new_TestFixture( "xyz_colortemp", test_xyz_colortemp ),
    };
    EMB_UNIT_TESTCALLER( xyz_tests, "XYZ tests", setup, teardown, fixtures );

    return ( (TestRef)&xyz_tests );
}


