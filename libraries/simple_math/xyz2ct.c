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
#include <errno.h>
#include <float.h>
#include <math.h>

#include <simple_math/xyz2ct.h>

// http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html

/******************************************************************************
 *
 *      Author: Bruce Justin Lindbloom
 *
 *      Copyright (c) 2003 Bruce Justin Lindbloom. All rights reserved.
 *
 *      Input:  xyz = pointer to the input array of X, Y and Z color components (in that order).
 *              temp = pointer to where the computed correlated color temperature should be placed.
 *
 *      Output: *temp = correlated color temperature, if successful.
 *                    = unchanged if unsuccessful.
 *
 *      Return: 0 if successful, else -1.
 *
 *      Description:
 *              This is an implementation of Robertson's method of computing the correlated color
 *              temperature of an XYZ color. It can compute correlated color temperatures in the
 *              range [1666.7K, infinity].
 *
 *      Reference:
 *              "Color Science: Concepts and Methods, Quantitative Data and Formulae", Second Edition,
 *              Gunter Wyszecki and W. S. Stiles, John Wiley & Sons, 1982, pp. 227, 228.
 *
 *****************************************************************************/

/* LERP(a,b,c) = linear interpolation macro, is 'a' when c == 0.0 and 'b' when c == 1.0 */
#define LERP(a,b,c)     (((b) - (a)) * (c) + (a))

typedef struct UVT
{
    double  u;
    double  v;
    double  t;
} UVT;

static double rt[31] =
{   /* reciprocal temperature (K) */
     DBL_MIN,  10.0e-6,  20.0e-6,  30.0e-6,  40.0e-6,  50.0e-6,
     60.0e-6,  70.0e-6,  80.0e-6,  90.0e-6, 100.0e-6, 125.0e-6,
    150.0e-6, 175.0e-6, 200.0e-6, 225.0e-6, 250.0e-6, 275.0e-6,
    300.0e-6, 325.0e-6, 350.0e-6, 375.0e-6, 400.0e-6, 425.0e-6,
    450.0e-6, 475.0e-6, 500.0e-6, 525.0e-6, 550.0e-6, 575.0e-6,
    600.0e-6
};

UVT uvt[31] =
{
    { 0.18006, 0.26352, -0.24341 },
    { 0.18066, 0.26589, -0.25479 },
    { 0.18133, 0.26846, -0.26876 },
    { 0.18208, 0.27119, -0.28539 },
    { 0.18293, 0.27407, -0.30470 },
    { 0.18388, 0.27709, -0.32675 },
    { 0.18494, 0.28021, -0.35156 },
    { 0.18611, 0.28342, -0.37915 },
    { 0.18740, 0.28668, -0.40955 },
    { 0.18880, 0.28997, -0.44278 },
    { 0.19032, 0.29326, -0.47888 },
    { 0.19462, 0.30141, -0.58204 },
    { 0.19962, 0.30921, -0.70471 },
    { 0.20525, 0.31647, -0.84901 },
    { 0.21142, 0.32312, -1.01820 },
    { 0.21807, 0.32909, -1.21680 },
    { 0.22511, 0.33439, -1.45120 },
    { 0.23247, 0.33904, -1.72980 },
    { 0.24010, 0.34308, -2.06370 },
    { 0.24792, 0.34655, -2.46810 }, /* Note: 0.24792 is a corrected value for the error found in W&S as 0.24702 */
    { 0.25591, 0.34951, -2.96410 },
    { 0.26400, 0.35200, -3.58140 },
    { 0.27218, 0.35407, -4.36330 },
    { 0.28039, 0.35577, -5.37620 },
    { 0.28863, 0.35714, -6.72620 },
    { 0.29685, 0.35823, -8.59550 },
    { 0.30505, 0.35907, -11.3240 },
    { 0.31320, 0.35968, -15.6280 },
    { 0.32129, 0.36011, -23.3250 },
    { 0.32931, 0.36038, -40.7700 },
    { 0.33724, 0.36051, -116.450 }
};


int XYZtoCorColorTemp_Robertson( double * xyz, double * temp )
{
    double us, vs, p, di, dm;
    int i;


    if ( (xyz[0] < 1.0e-20) && (xyz[1] < 1.0e-20) && (xyz[2] < 1.0e-20) )
    {
        /* protect against possible divide-by-zero failure */
        return ( -EINVAL );
    }

    us = (4.0 * xyz[0]) / (xyz[0] + 15.0 * xyz[1] + 3.0 * xyz[2]);
    vs = (6.0 * xyz[1]) / (xyz[0] + 15.0 * xyz[1] + 3.0 * xyz[2]);
    dm = 0.0;

    for (i = 0; i < 31; i++)
    {
        di = (vs - uvt[i].v) - uvt[i].t * (us - uvt[i].u);
        
        if ( (i > 0) && (((di < 0.0) && (dm >= 0.0)) || ((di >= 0.0) && (dm < 0.0))) )
        {
            /* found lines bounding (us, vs) : i-1 and i */
            break;
        }

        dm = di;
    }

    if ( i == 31 )
    {
        /* bad XYZ input, color temp would be less than minimum of 1666.7 degrees, or too far towards blue */
        return ( -EFAULT );
    }

    di = di / sqrt(1.0 + uvt[i    ].t * uvt[i    ].t);
    dm = dm / sqrt(1.0 + uvt[i - 1].t * uvt[i - 1].t);
    /* p = interpolation parameter, 0.0 : i-1, 1.0 : i */
    p = dm / (dm - di);
    p = 1.0 / (LERP(rt[i - 1], rt[i], p));
    *temp = p;

    /* success */
    return ( 0 );
}

// McCamy’s formula
// https://en.wikipedia.org/wiki/Color_temperature#Approximation
int XYZtoCorColorTemp_McCamy( double * xyz, double * temp )
{
    if ( temp )
    {
        double x = xyz[0] / ( xyz[0] + xyz[1] + xyz[2] );
        double y = xyz[1] / ( xyz[0] + xyz[1] + xyz[2] );

        double n  = ( x - 0.3320f ) / ( 0.1858f - y );

        *temp = 449.0f*n*n*n + 3525.0f*n*n + 6823.3f*n + 5520.33f;
    
        return ( 0 );
    }

    return ( -EINVAL );
}

// http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
//
// Here are the equations to convert from the correlated color 
// temperature of a CIE D-illuminant to the chromaticity of that D-illuminant.
int CorColorTemptoXYZ( double ct, double * xyz )
{
    double xd;
    double yd;
    double zd;
    
    if ( ct < 7000 )
    {
        xd  = -4.607000 * 1000 / ct * 1000 / ct * 1000 / ct;
        xd +=  2.967800 * 1000 / ct * 1000 / ct;
        xd +=  0.099110 * 1000 / ct;
        xd += 0.244063;
    }
    else
    {
        xd  = -2.006400 * 1000 / ct * 1000 / ct * 1000 / ct;
        xd +=  1.901800 * 1000 / ct * 1000 / ct;
        xd +=  0.247480 * 1000 / ct;
        xd += 0.237040;        
    }
    
    yd = -3.000 * xd * xd + 2.870 * xd - 0.275;

    zd = 1 - xd - yd;

    // Y= 0.5
    xyz[1] = 0.5;

    xyz[0] = (xd * xyz[1]) / yd;
    xyz[2] = (zd * xyz[1]) / yd;

    return ( 0 );
}


