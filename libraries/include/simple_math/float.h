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
#ifndef __FLOAT_H__
#define __FLOAT_H__

#include <stdint.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @brief Fast sine approximation taken from
 *        http://lab.polygonal.de/2007/07/18/fast-and-accurate-sinecosine-approximation/
 *
 * @param[in]   angle Input angle in radian
 *
 * @return      sine of angle
 *****************************************************************************/
float sin_f( float angle );

/**************************************************************************//**
 * @brief Fast cosine approximation taken from
 *        http://lab.polygonal.de/2007/07/18/fast-and-accurate-sinecosine-approximation/
 *
 * @param[in]   angle Input angle in radian
 *
 * @return      cosine of angle
 *****************************************************************************/
float cos_f( float angle );

/**************************************************************************//**
 * @brief Get the minimum value from a and b
 *
 * @param[in]   a first value to compare
 * @param[in]   b second value to compare
 *
 * @return      minimum value of a and b
 *****************************************************************************/
float min_f( float a, float b );

/**************************************************************************//**
 * @brief Get the maximum value from a and b
 *
 * @param[in]   a first value to compare
 * @param[in]   b second value to compare
 *
 * @return      maximum value of a and b
 *****************************************************************************/
float max_f( float a, float b );

/* The following pow function is taken from the "fastapprox" library
 * which can be downloaded on GitHub: https://github.com/romeric/fastapprox
 * It is published under the following terms and conditions: */

/*=====================================================================*
 *                   Copyright (C) 2011 Paul Mineiro                   *
 * All rights reserved.                                                *
 *                                                                     *
 * Redistribution and use in source and binary forms, with             *
 * or without modification, are permitted provided that the            *
 * following conditions are met:                                       *
 *                                                                     *
 *     * Redistributions of source code must retain the                *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer.                                       *
 *                                                                     *
 *     * Redistributions in binary form must reproduce the             *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer in the documentation and/or            *
 *     other materials provided with the distribution.                 *
 *                                                                     *
 *     * Neither the name of Paul Mineiro nor the names                *
 *     of other contributors may be used to endorse or promote         *
 *     products derived from this software without specific            *
 *     prior written permission.                                       *
 *                                                                     *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND              *
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,         *
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES               *
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE             *
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER               *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,                 *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES            *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE           *
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR                *
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF          *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY              *
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE             *
 * POSSIBILITY OF SUCH DAMAGE.                                         *
 *                                                                     *
 * Contact: Paul Mineiro <paul@mineiro.com>                            *
 *=====================================================================*/

/**************************************************************************//**
 * @brief Get the approximate value of x to the power of p
 *
 * @param[in]   x base
 * @param[in]   p exponent
 *
 * @return      x^p
 *****************************************************************************/
float fastpow (float x, float p);

#ifdef __cplusplus
}
#endif

#endif /* __SIMPLE_MATH_H__ */

