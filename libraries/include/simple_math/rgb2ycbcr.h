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
#ifndef __RGB2YCbCr_H__
#define __RGB2YCbCr_H__

#include <stdint.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * coefficient calculation definitions
 *****************************************************************************/
#define U_MAX           ( 0.4360f )
#define V_MAX           ( 0.6150f )

#define Cb_MAX          ( 0.5000f )
#define Cr_MAX          ( 0.5000f )

#define Kr_BT601        ( 0.2990f )
#define Kb_BT601        ( 0.1140f )
#define Kg_BT601        ( 1.0000f - Kr_BT601 - Kb_BT601 )

#define Kr_BT709        ( 0.2126f )
#define Kb_BT709        ( 0.0722f )
#define Kg_BT709        ( 1.0000f - Kr_BT709 - Kb_BT709 )

#define Kr_SMPTE240M    ( 0.2120f )
#define Kb_SMPTE240M    ( 0.0870f )
#define Kg_SMPTE240M    ( 1.0000f - Kr_SMPTE240M - Kb_SMPTE240M )

#define Kr_BT2020       ( 0.2627f )
#define Kb_BT2020       ( 0.0593f )
#define Kg_BT2020       ( 1.0000f - Kr_BT2020 - Kb_BT2020 )

/**************************************************************************//**
 * @brief Computes RGB to YCbCr coefficients (BT.601 Standard).
 *
 * @param[inout] c0     coefficient c0 
 * @param[inout] c1     coefficient c1 
 * @param[inout] c2     coefficient c2 
 * @param[inout] c3     coefficient c3 
 * @param[inout] c4     coefficient c4 
 * @param[inout] c5     coefficient c5 
 * @param[inout] c6     coefficient c6 
 * @param[inout] c7     coefficient c7 
 * @param[inout] c8     coefficient c8 
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int cal_YCbCr_coefficients_bt601
(
    float * c0, float * c1, float * c2,
    float * c3, float * c4, float * c5,
    float * c6, float * c7, float * c8
);

/**************************************************************************//**
 * @brief Computes RGB to YCbCr coefficients (BT.709 Standard).
 *
 * @param[inout] c0     coefficient c0 
 * @param[inout] c1     coefficient c1 
 * @param[inout] c2     coefficient c2 
 * @param[inout] c3     coefficient c3 
 * @param[inout] c4     coefficient c4 
 * @param[inout] c5     coefficient c5 
 * @param[inout] c6     coefficient c6 
 * @param[inout] c7     coefficient c7 
 * @param[inout] c8     coefficient c8 
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int cal_YCbCr_coefficients_bt709
(
    float * c0, float * c1, float * c2,
    float * c3, float * c4, float * c5,
    float * c6, float * c7, float * c8
);

/**************************************************************************//**
 * @brief Computes RGB to YCbCr coefficients (SMPTE 240M Standard).
 *
 * @param[inout] c0     coefficient c0 
 * @param[inout] c1     coefficient c1 
 * @param[inout] c2     coefficient c2 
 * @param[inout] c3     coefficient c3 
 * @param[inout] c4     coefficient c4 
 * @param[inout] c5     coefficient c5 
 * @param[inout] c6     coefficient c6 
 * @param[inout] c7     coefficient c7 
 * @param[inout] c8     coefficient c8 
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int cal_YCbCr_coefficients_smpte240M
(
    float * c0, float * c1, float * c2,
    float * c3, float * c4, float * c5,
    float * c6, float * c7, float * c8
);

/**************************************************************************//**
 * @brief Computes RGB to YCbCr coefficients (BT.2020 Standard).
 *
 * @param[inout] c0     coefficient c0
 * @param[inout] c1     coefficient c1
 * @param[inout] c2     coefficient c2
 * @param[inout] c3     coefficient c3
 * @param[inout] c4     coefficient c4
 * @param[inout] c5     coefficient c5
 * @param[inout] c6     coefficient c6
 * @param[inout] c7     coefficient c7
 * @param[inout] c8     coefficient c8
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int cal_YCbCr_coefficients_bt2020
(
    float * c0, float * c1, float * c2,
    float * c3, float * c4, float * c5,
    float * c6, float * c7, float * c8
);

/**************************************************************************//**
 * @brief Computes RGB to YCbCr coefficients regarding Kr and Kb.
 *
 * @param[in]    Kr     colour-difference signal coefficient red
 * @param[in]    Kb     colour-difference signal coefficient blue
 * @param[inout] c0     coefficient c0 
 * @param[inout] c1     coefficient c1 
 * @param[inout] c2     coefficient c2 
 * @param[inout] c3     coefficient c3 
 * @param[inout] c4     coefficient c4 
 * @param[inout] c5     coefficient c5 
 * @param[inout] c6     coefficient c6 
 * @param[inout] c7     coefficient c7 
 * @param[inout] c8     coefficient c8 
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int cal_YCbCr_coefficients
(
    float Kr, float Kb,
    float * c0, float * c1, float * c2,
    float * c3, float * c4, float * c5,
    float * c6, float * c7, float * c8
);

#ifdef __cplusplus
}
#endif

#endif /* __RGB2YCbCr_H__ */

