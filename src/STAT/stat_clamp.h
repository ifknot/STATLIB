#ifndef STAT_CLAMP_H
#define STAT_CLAMP_H

#include "stat_types.h"
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>

// ========================
// Scalar Functions
// ========================

/**
 * @brief Clamps a float value to [min, max] range
 * @param value Input value
 * @param min Minimum bound (inclusive)
 * @param max Maximum bound (inclusive)
 * @return Clamped value
 * @note Sets errno=EDOM if value is NaN or min > max
 */
static inline stat_float_t stat_clamp_float(stat_float_t value, stat_float_t min, stat_float_t max) {
    if (isnan(value) || min > max) errno = EDOM;
    return (value < min) ? min : ((value > max) ? max : value);
}

/**
 * @brief Clamps an integer value to [min, max] range
 * @param value Input value
 * @param min Minimum bound (inclusive)
 * @param max Maximum bound (inclusive)
 * @return Clamped value
 * @note Sets errno=EINVAL if min > max
 */
static inline stat_int_t stat_clamp_int(stat_int_t value, stat_int_t min, stat_int_t max) {
    if (min > max) errno = EINVAL;
    return (value < min) ? min : ((value > max) ? max : value);
}

// ========================
// Array Functions
// ========================

/**
 * @brief Clamps float array values to [min, max] range
 * @param destination Output array (must be pre-allocated)
 * @param source Input array
 * @param count Number of elements
 * @param min Minimum bound (inclusive)
 * @param max Maximum bound (inclusive)
 * @return destination pointer for chaining
 * @throws EINVAL if count=0 or min > max, EDOM if NaN encountered
 * @assert Fails if source or destination is NULL
 */
stat_float_t* stat_clamp_float_array(
    stat_float_t* destination,
    const stat_float_t* source,
    stat_size_t count,
    stat_float_t min,
    stat_float_t max
);

/**
 * @brief Clamps integer array values to [min, max] range
 * @param destination Output array (must be pre-allocated)
 * @param source Input array
 * @param count Number of elements
 * @param min Minimum bound (inclusive)
 * @param max Maximum bound (inclusive)
 * @return destination pointer for chaining
 * @throws EINVAL if count=0 or min > max
 * @assert Fails if source or destination is NULL
 */
stat_int_t* stat_clamp_int_array(
    stat_int_t* destination,
    const stat_int_t* source,
    stat_size_t count,
    stat_int_t min,
    stat_int_t max
);

#endif // STAT_CLAMP_H
