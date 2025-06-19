#ifndef STAT_CLAMP_H
#define STAT_CLAMP_H

#include "stat_types.h"
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>

/**
 * @brief Clamps float values to [min, max] range
 * @param[in]  src    Input array (must not be NULL)
 * @param[out] dst    Output array (must be pre-allocated)
 * @param[in]  count  Number of elements
 * @param[in]  min    Minimum bound (inclusive)
 * @param[in]  max    Maximum bound (inclusive)
 * @return dst pointer for chaining
 * @throws EINVAL if count=0 or min > max, EDOM if NaN encountered
 * @assert Fails if src or dst is NULL
 */
stat_float_t* stat_clamp_f(
    const stat_float_t* src,
    stat_float_t* dst,
    stat_size_t count,
    stat_float_t min,
    stat_float_t max
);

/**
 * @brief Clamps int32 values to [min, max] range
 * @param[in]  src    Input array (must not be NULL)
 * @param[out] dst    Output array (must be pre-allocated)
 * @param[in]  count  Number of elements
 * @param[in]  min    Minimum bound (inclusive)
 * @param[in]  max    Maximum bound (inclusive)
 * @return dst pointer for chaining
 * @throws EINVAL if count=0 or min > max
 * @assert Fails if src or dst is NULL
 */
stat_int_t* stat_clamp_i32(
    const stat_int_t* src,
    stat_int_t* dst,
    stat_size_t count,
    stat_int_t min,
    stat_int_t max
);

/**
 * @brief Scalar float clamp
 * @param[in] x   Input value
 * @param[in] min Minimum bound
 * @param[in] max Maximum bound
 * @return Clamped value
 * @note Sets errno=EDOM if x is NaN or min > max
 */
static inline stat_float_t stat_clamp_scalar_f(
    stat_float_t x,
    stat_float_t min,
    stat_float_t max
) {
    if (isnan(x) || min > max) errno = EDOM;
    return (x < min) ? min : ((x > max) ? max : x);
}

/**
 * @brief Scalar int32 clamp
 * @param[in] x   Input value
 * @param[in] min Minimum bound
 * @param[in] max Maximum bound
 * @return Clamped value
 * @note Sets errno=EINVAL if min > max
 */
static inline stat_int_t stat_clamp_scalar_i32(
    stat_int_t x,
    stat_int_t min,
    stat_int_t max
) {
    if (min > max) errno = EINVAL;
    return (x < min) ? min : ((x > max) ? max : x);
}

#endif // STAT_CLAMP_H
