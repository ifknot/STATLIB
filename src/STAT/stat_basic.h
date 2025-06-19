#ifndef STAT_BASIC_H
#define STAT_BASIC_H

#include "stat_types.h"

// ========================
// Core Functions
// ========================

/**
 * @brief Finds minimum value in a float array
 * @param[in]  data  Input array
 * @param[in]  count Number of elements
 * @return Minimum value (NAN if empty or invalid)
 * @throws EINVAL if count=0
 * @assert Fails if data=NULL
 */
stat_float_t stat_min_f(const stat_float_t* data, stat_size_t count);

/**
 * @brief Finds maximum value in a float array
 * @param[in]  data  Input array
 * @param[in]  count Number of elements
 * @return Maximum value (NAN if empty or invalid)
 * @throws EINVAL if count=0
 * @assert Fails if data=NULL
 */
stat_float_t stat_max_f(const stat_float_t* data, stat_size_t count);

/**
 * @brief Computes range (max - min) of a float array
 * @param[in]  data  Input array
 * @param[in]  count Number of elements
 * @return Range value (NAN if empty or invalid)
 * @throws EINVAL if count=0
 * @assert Fails if data=NULL
 */
stat_float_t stat_range_f(const stat_float_t* data, stat_size_t count);

// ========================
// Scalar Wrappers
// ========================

/**
 * @brief Scalar minimum of two floats
 * @param[in] a First value
 * @param[in] b Second value
 * @return Smaller value
 */
static inline stat_float_t stat_min_scalar_f(stat_float_t a, stat_float_t b) {
    return a < b ? a : b;
}

/**
 * @brief Scalar maximum of two floats
 * @param[in] a First value
 * @param[in] b Second value
 * @return Larger value
 */
static inline stat_float_t stat_max_scalar_f(stat_float_t a, stat_float_t b) {
    return a > b ? a : b;
}

#endif // STAT_BASIC_H
