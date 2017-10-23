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
#include <stdint.h>

#include <simple_math/float.h>

/******************************************************************************
 * sin_f
 * See: http://lab.polygonal.de/2007/07/18/fast-and-accurate-sinecosine-approximation/
 *****************************************************************************/
float sin_f( float angle )
{
    //always wrap input angle to -PI..PI
    if ( angle < -3.14159265f )
    {
        angle += 6.28318531f;
    }
    else
    if ( angle >  3.14159265f )
    {
        angle -= 6.28318531f;
    }

    float sin = 0.0f;

    //compute sine
    if ( angle < 0.0f )
    {
        sin = 1.27323954f * angle + .405284735f * angle * angle;

        if ( sin < 0.0f )
        {
            sin = .225f * (sin *-sin - sin) + sin;
        }
        else
        {
            sin = .225f * (sin * sin - sin) + sin;
        }
    }
    else
    {
        sin = 1.27323954f * angle - 0.405284735f * angle * angle;

        if ( sin < 0.0f )
        {
            sin = .225f * (sin *-sin - sin) + sin;
        }
        else
        {
            sin = .225f * (sin * sin - sin) + sin;
        }
    }

    return sin;
}

/******************************************************************************
 * cos_f
 * See: http://lab.polygonal.de/2007/07/18/fast-and-accurate-sinecosine-approximation/
 *****************************************************************************/
float cos_f( float angle )
{
    //always wrap input angle to -PI..PI
    if ( angle < -3.14159265f )
    {
        angle += 6.28318531f;
    }
    else
    if ( angle >  3.14159265f )
    {
        angle -= 6.28318531f;
    }

    //compute cosine: sin(x + PI/2) = cos(x)
    angle += 1.57079632f;
    if ( angle >  3.14159265f )
    {
        angle -= 6.28318531f;
    }

    float cos = 0.0f;

    if ( angle < 0.0f )
    {
        cos = 1.27323954f * angle + 0.405284735f * angle * angle;

        if ( cos < 0.0f )
        {
            cos = .225f * (cos *-cos - cos) + cos;
        }
        else
        {
            cos = .225f * (cos * cos - cos) + cos;
        }
    }
    else
    {
        cos = 1.27323954f * angle - 0.405284735f * angle * angle;

        if ( cos < 0.0f )
        {
            cos = .225f * (cos *-cos - cos) + cos;
        }
        else
        {
            cos = .225f * (cos * cos - cos) + cos;
        }
    }

    return cos;
}

/******************************************************************************
 * min_f
 *****************************************************************************/
float min_f( float a, float b )
{
    if ( a < b )
    {
        return a;
    }
    else
    {
        return b;
    }
}

/******************************************************************************
 * max_f
 *****************************************************************************/
float max_f( float a, float b )
{
    if ( a > b )
    {
        return a;
    }
    else
    {
        return b;
    }
}

/* The following functions are taken from the "fastapprox" library which can be
 * downloaded over GitHub: https://github.com/romeric/fastapprox
 * It is published under the following terms */

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

/******************************************************************************
 * fastlog2 - computes log2(x)
 *****************************************************************************/
static inline float fastlog2 (float x)
{
  union { float f; uint32_t i; } vx = { x };
  union { uint32_t i; float f; } mx = { (vx.i & 0x007FFFFF) | 0x3f000000 };
  float y = vx.i;
  y *= 1.1920928955078125e-7f;

  return y - 124.22551499f
           - 1.498030302f * mx.f
           - 1.72587999f / (0.3520887068f + mx.f);
}

/******************************************************************************
 * fastpow2 - computes 2^p
 *****************************************************************************/
static inline float fastpow2 (float p)
{
  float offset = (p < 0) ? 1.0f : 0.0f;
  float clipp = (p < -126) ? -126.0f : p;
  int w = clipp;
  float z = clipp - w + offset;
  union { uint32_t i; float f; } v = { (uint32_t)( (1 << 23) * (clipp + 121.2740575f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z) ) };

  return v.f;
}

/******************************************************************************
 * fastpow - computes x^p
 *****************************************************************************/
float fastpow (float x, float p)
{
  return fastpow2 (p * fastlog2 (x));
}
