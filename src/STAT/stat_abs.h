#ifndef STAT_ABS_H
#define STAT_ABS_H

#include <errno.h>
#include <math.h>

#include "stat_types.h"

/**
 * @brief Computes absolute values of a stat_float_t array
 * @param[in]  src   Source array
 * @param[out] dst   Destination array
 * @param[in]  count Number of elements
 * @return dst pointer for chaining
 * @note dst may alias src (safe for in-place operation)
 * @throws EINVAL if count=0, EDOM if NaN encountered
 * @assert Fails if src or dst is NULL
 */
stat_float_t* stat_abs_f(const stat_float_t* src, stat_float_t* dst, stat_size_t size);

/**
 * @brief Computes absolute values of an int32 array
 * @param[in]  src   Source array
 * @param[out] dst   Destination array
 * @param[in]  count Number of elements
 * @return dst pointer for chaining
 * @throws EINVAL if size=0, ERANGE if INT32_MIN encountered
 * @assert Fails if src or dst is NULL
 */
int32_t* stat_abs_i(const int32_t* src, int32_t* dst, stat_size_t size);

/**
 * @brief Scalar absolute value (stat_float_t)
 * @param[in] x Input value
 * @return Absolute value
 * @note Sets errno=EDOM if x=NaN
 */
static inline stat_float_t stat_abs_scalar_f(stat_float_t x) {
    if (isnan(x)) errno = EDOM;
    return x < 0 ? -x : x;
}

/**
 * @brief Scalar absolute value (int32_t)
 * @param[in] x Input value
 * @return Absolute value (undefined if x=INT32_MIN)
 */
static inline int32_t stat_abs_scalar_i32(int32_t x) {
    if (x == INT32_MIN) errno = ERANGE;
    return x < 0 ? -x : x;
}

#endif
