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
#ifndef __CUBIC_H__
#define __CUBIC_H__

#include <stdint.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * @brief maximal samples for cubic spline interpolation
 *****************************************************************************/
#define CSIP_MAX_SAMPLES    ( 4096u )

/******************************************************************************
 * @brief sample type definition for cubic spline interpolation
 *****************************************************************************/
#if    defined(CFG_CSIP_SAMPLE_INT32)
typedef int32_t     spline_interpolation_sample_type_t;
#define CSIP_SAMPLE_FORMAT	"%" PRId32
#elif  defined(CFG_CSIP_SAMPLE_UINT32)
typedef uint32_t    spline_interpolation_sample_type_t;
#define CSIP_SAMPLE_FORMAT	"%" PRIu32
#elif  defined(CFG_CSIP_SAMPLE_INT64)
typedef int64_t    	spline_interpolation_sample_type_t;
#define CSIP_SAMPLE_FORMAT	"%" PRId64
#elif  defined(CFG_CSIP_SAMPLE_UINT64)
typedef uint64_t    spline_interpolation_sample_type_t;
#define CSIP_SAMPLE_FORMAT	"%" PRIu64
#elif  defined(CFG_CSIP_SAMPLE_FLOAT)
typedef float       spline_interpolation_sample_type_t;
#define CSIP_SAMPLE_FORMAT	"%f"
#elif  defined(CFG_CSIP_SAMPLE_DOUBLE)
typedef double      spline_interpolation_sample_type_t;
#define CSIP_SAMPLE_FORMAT	"%lf"
#else 
#error CFG_CSIP_SAMPLE_XXX not defined
#endif /* defined(CFG_SIGNAL_UINT32) */

/******************************************************************************
 * @brief type-cast macro
 *****************************************************************************/
#define CSIP_SAMPLE_TYPE(x)   ((spline_interpolation_sample_type_t)(x))

/******************************************************************************
 * @brief cubic spline interpolation states
 *****************************************************************************/
typedef enum spline_interpolation_state_s
{
    CSIP_STATE_UNITIALIZED     = 0,        /**< Indicates uninitialized spline interpolation */
    CSIP_STATE_INITIALIZED     = 1,        /**< Indicates initialized spline interpolation */
    CSIP_STATE_GOT_SAMPLES     = 2,        /**< Indicates that context has sample interpolation */
    CSIP_STATE_RUNNING         = 3,        /**< Indicates running spline interpolation */
    CSIP_STATE_MAX
} spline_interpolation_state_t;

/******************************************************************************
 * @brief cubic spline interpolation context
 *****************************************************************************/
typedef struct spline_interpolation_ctx_s
{
	double                              z[CSIP_MAX_SAMPLES];
    int32_t                             h[CSIP_MAX_SAMPLES];
#if defined(CFG_HOLD_SAMPLES)
    spline_interpolation_sample_type_t  x[CSIP_MAX_SAMPLES]; /*< samples internally hold */
    spline_interpolation_sample_type_t  y[CSIP_MAX_SAMPLES]; /*< samples internally hold */
#else   /* defined(CFG_HOLD_SAMPLES) */
    spline_interpolation_sample_type_t  *x; /*< samples hold outside */
    spline_interpolation_sample_type_t  *y; /*< samples hold outside */
#endif  /* defined(CFG_HOLD_SAMPLES) */ 
    uint32_t                     n;
    spline_interpolation_state_t state; /*< stae of interpolation */
} spline_interpolation_ctx_t;

/**************************************************************************//**
 * @brief      Initialize interpolation context
 *
 * @param[out] ctx  pointer to interpolation context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int sm_cubic_spline_interpolation_init
(
    spline_interpolation_ctx_t ** ctx
);

/**************************************************************************//**
 * @brief      Reset interpolation context
 *
 * @param[x]   ctx  interpolation handle
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int sm_cubic_spline_interpolation_reset
(
    spline_interpolation_ctx_t * const ctx
);

#if defined(CFG_HOLD_SAMPLES)
/**************************************************************************//**
 * @brief      Add a number of samples to interpolation context
 *
 * @note       Only available of samples hold inside the context
 *
 * @param[in]  ctx  interpolation handle
 * @param[in]  x    x coordinates of samples 
 * @param[in]  y    y coordinates of samples 
 * @param[in]  no   number of sample to add
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int sm_cubic_spline_interpolation_add_samples
(
    spline_interpolation_ctx_t * const          ctx,
    spline_interpolation_sample_type_t * const  x,
    spline_interpolation_sample_type_t * const  y,
    unsigned const                              no
);
#endif  /* defined(CFG_HOLD_SAMPLES) */ 

/**************************************************************************//**
 * @brief      Set samples to interpolation context
 *
 * @param[in]  ctx  interpolation handle
 * @param[in]  x    x coordinates of samples 
 * @param[in]  y    y coordinates of samples 
 * @param[in]  no   number of sample to set
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int sm_cubic_spline_interpolation_set_samples
(
    spline_interpolation_ctx_t * const          ctx,
    spline_interpolation_sample_type_t * const  x,
    spline_interpolation_sample_type_t * const  y,
    unsigned const                              no
);

/**************************************************************************//**
 * @brief      Initialize interpolation algorithm, computes the interim values
 *             of cubic spline interpolation algorithm
 *
 * @note       needs to be (re)called after adding new sample points
 *
 * @param[in]  ctx  pointer to interpolation context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int sm_cubic_spline_interpolation_calc_init
(
    spline_interpolation_ctx_t * const ctx
);

/**************************************************************************//**
 * @brief      Run cubic spline interpolation for x
 *
 * @param[in]  ctx  pointer to interpolation context
 * @param[in]  x    x coordinate of sample point to interpolate
 * @param[out] y    resulting y coordinate of sample point
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int sm_cubic_spline_interpolation_calc
(
    spline_interpolation_ctx_t * const          ctx,
    spline_interpolation_sample_type_t const    x,
    spline_interpolation_sample_type_t * const  y
);

#ifdef __cplusplus
}
#endif

#endif /* __CUBIC_H__ */

