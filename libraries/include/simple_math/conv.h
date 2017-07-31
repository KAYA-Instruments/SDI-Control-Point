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
#ifndef __CONV_H__
#define __CONV_H__

#include <stdint.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * @brief sample type definition for convolution and filter functions
 *****************************************************************************/
#if    defined(CFG_CONV_SAMPLE_INT32)
typedef int32_t     conv_sample_type_t;
#define CONV_SAMPLE_FORMAT	"%"PRId32
#elif  defined(CFG_CONV_SAMPLE_UINT32)
typedef uint32_t    conv_sample_type_t;
#define CONV_SAMPLE_FORMAT	"%"PRIu32
#elif  defined(CFG_CONV_SAMPLE_INT64)
typedef int64_t    	conv_sample_type_t;
#define CONV_SAMPLE_FORMAT	"%"PRId64
#elif  defined(CFG_CONV_SAMPLE_UINT64)
typedef uint64_t    conv_sample_type_t;
#define CONV_SAMPLE_FORMAT	"%"PRIu64
#elif  defined(CFG_CONV_SAMPLE_FLOAT)
typedef float       conv_sample_type_t;
#define CONV_SAMPLE_FORMAT	"%f"
#elif  defined(CFG_CONV_SAMPLE_DOUBLE)
typedef double      conv_sample_type_t;
#define CONV_SAMPLE_FORMAT	"%lf"
#else 
#error CFG_CONV_SAMPLE_XXX not defined
#endif /* defined(CFG_SIGNAL_UINT32) */

/******************************************************************************
 * @brief type-cast macro
 *****************************************************************************/
#define CONV_SAMPLE_TYPE(x)     ((conv_sample_type_t)(x))

/**************************************************************************//**
 * @brief      Fill all vector components with a given constant sample value
 *
 * @param[in]  samples  vector components
 * @param[in]  len      number of vector components
 * @param[in]  sample   constant sample value
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
void init
(
    conv_sample_type_t * const  samples,
    unsigned const              len,
    conv_sample_type_t const    sample
);


/**************************************************************************//**
 * @brief      Fill all vector components with ones
 *
 * @param[in]  samples  vector components
 * @param[in]  len      number of vector components
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
void ones
(
    conv_sample_type_t * const  samples,
    unsigned const              len
);

/**************************************************************************//**
 * @brief      Sum up all vector components
 *
 * @param[in]  samples  vector components
 * @param[in]  len      number of vector components
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
conv_sample_type_t sum
(
    conv_sample_type_t * const  samples,
    unsigned const              len
);

/**************************************************************************//**
 * @brief      Upscale all vector components by a factor
 *
 * @param[in]  samples  vector items
 * @param[in]  len      number of vector items
 * @param[in]  fac      upscale factor
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
void upscale
(
    conv_sample_type_t * const  samples,
    unsigned const              len,
    conv_sample_type_t const    fac
);

/**************************************************************************//**
 * @brief      Downscale all vector items by a factor
 *
 * @param[in]  samples  vector items
 * @param[in]  len      number of vector items
 * @param[in]  div      downscale divider
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
void downscale
(
    conv_sample_type_t * const  samples,
    unsigned                    len,
    conv_sample_type_t const    div
);

/**************************************************************************//**
 * @brief      Divide corresponding vector components
 *
 * @note       condition: all vectors have same size (len)
 *
 * @param[in]  samples  vector items
 * @param[in]  div      divider vector
 * @param[out] result   result vector
 * @param[in]  len      number of vector items
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
void divide
(
    conv_sample_type_t * const  samples,
    conv_sample_type_t * const  div,
    conv_sample_type_t * const  result,
    unsigned const              len
);

/**************************************************************************//**
 * @brief      Subtract corresponding vector components
 *
 * @note       condition: all vectors have same size (len) 
 *
 * @param[in]  samples  vector items
 * @param[in]  div      divider vector
 * @param[out] result   result vector
 * @param[in]  len      number of vector items
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
void subtract
(
    conv_sample_type_t * const  samples_a,
    conv_sample_type_t * const  samples_b,
    conv_sample_type_t * const  result,
    unsigned const              len
);

/**************************************************************************//**
 * @brief      Convolute vector by a filter
 *
 * @param[in]  samples      vector items
 * @param[in]  samples_len  number of vector items
 * @param[in]  filter       filter items
 * @param[in]  filter_len   number of filter items
 * @param[out] result       convolution result (vector)
 * @param[in]  result_len   number of preallocated items in result vector 
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
void conv
(
    conv_sample_type_t * const  samples,
    unsigned const              samples_len,
    conv_sample_type_t * const  filter,
    unsigned const              filter_len,
    conv_sample_type_t * const  result,
    unsigned const              result_len
);


/**************************************************************************//**
 * @brief      Convolute vector by a filter
 *
 * @note       This function is similar to conv, but the output signal has
 *             the same size like the input signal (@sample_len)
 *
 * @param[in]  filter       filter items
 * @param[in]  filter_len   number of filter items
 * @param[in]  samples      vector items
 * @param[in]  samples_len  number of vector items
 * @param[out] result       convolution result (vector)
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
void filter2
(
    conv_sample_type_t * const  filter,
    unsigned const              filter_len,
    conv_sample_type_t * const  samples,
    unsigned const              samples_len,
    conv_sample_type_t * const  result
);

#ifdef __cplusplus
}
#endif

#endif /* __CONV_H__ */

