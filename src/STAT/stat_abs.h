#ifndef STAT_ABS_H
#define STAT_ABS_H

#include "stat_types.h"


/**
 * @brief Compute absolute values of float array (in-place)
 * @param values Array of values (can be length 1)
 * @param count Number of elements
 */
void stat_abs_f(stat_float_t* values, stat_size_t count);

/**
 * @brief Compute absolute values of int32 array (in-place)
 * @param values Array to modify (can be length 1)
 * @param count Number of elements
 * @return Number of successful conversions (count-INT32_MIN occurrences)
 */
stat_size_t stat_abs_i32(stat_int_t* values, stat_size_t count)

/**
 * @brief Safe absolute values (out-of-place)
 * @param input Source array
 * @param output Destination array (pre-allocated)
 * @param count Number of elements
 * @return Number of successful conversions
 */
stat_size_t stat_safe_abs_i32(
    const stat_int_t* input, 
    stat_int_t* output, 
    stat_size_t count
)

/**
 * @brief Scalar wrapper for stat_abs_f (float)
 * @param x Single value to process
 * @return Absolute value
 */
static inline stat_float_t stat_abs_scalar_f(stat_float_t x) {
    stat_abs_f(&x, 1);
    return x;
}

/**
 * @brief Scalar wrapper for stat_abs_i32 (int32)
 * @param x Single value to process
 * @return Absolute value (undefined if x=INT32_MIN)
 */
static inline stat_int_t stat_abs_scalar_i32(stat_int_t x) {
    stat_abs_i32(&x, 1);
    return x;
}

/**
 * @brief Safe scalar wrapper for stat_safe_abs_i32
 * @param x Single value to process
 * @param result Output pointer (unchanged if x=INT32_MIN)
 * @return True if successful, False if x=INT32_MIN
 */
static inline bool stat_safe_abs_scalar_i32(stat_int_t x, stat_int_t* result) {
    stat_int_t tmp = x;
    if (stat_abs_i32(&tmp, 1) == 0) {
        return false;
    }
    *result = tmp;
    return true;
}


#endif // STAT_ABS_H
