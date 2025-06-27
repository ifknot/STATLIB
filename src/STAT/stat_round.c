#ifndef STAT_ROUND_H
#define STAT_ROUND_H

#include "stat_types.h"

/**
 * @file stat_round.h
 * @brief Rounding and conversion functions for statistical calculations
 * 
 * This module provides various rounding functions and type conversion utilities
 * for statistical operations. All array operations store results in a separate
 * destination array (non-destructive to source).
 */

/**
 * @def stat_lround(x)
 * @brief Rounds a floating-point value to the nearest integer (as int32_t)
 * @param x The value to round
 * @note This is a compatibility macro that uses lround() when available,
 *       or a fallback implementation for Watcom C.
 */
#ifdef __WATCOMC__
#define stat_lround(x) ((int32_t)((x) >= 0 ? (x) + 0.5 : (x) - 0.5))
#else
#define stat_lround(x) lround(x)
#endif

/* Scalar rounding functions */
stat_int_t stat_round_to_i(stat_float_t value);
stat_int_t stat_floor_to_i(stat_float_t value);
stat_int_t stat_ceil_to_i(stat_float_t value);
stat_int_t stat_trunc_to_i(stat_float_t value);
stat_float_t stat_round_to_multiple(stat_float_t value, stat_float_t multiple);
stat_float_t stat_round_decimal(stat_float_t value, int decimals);
bool stat_safe_round_to_i(stat_float_t value, stat_int_t* result);

/* Array conversion functions (non-inplace, destination-first pattern) */

/**
 * @brief Converts an array of integers to an array of floats
 * @param destination Pointer to pre-allocated float array for results
 * @param source Pointer to source integer array
 * @return Pointer to the destination array
 * @note The destination array must have at least the same capacity as source
 */
stat_float_t* stat_cast_int_to_float_array(stat_float_t* destination, const stat_int_t* source);

/**
 * @brief Converts an array of floats to integers using rounding
 * @param destination Pointer to pre-allocated integer array for results
 * @param source Pointer to source float array
 * @return Pointer to the destination array
 */
stat_int_t* stat_round_float_to_int_array(stat_int_t* destination, const stat_float_t* source);

/**
 * @brief Converts an array of floats to integers using floor operation
 * @param destination Pointer to pre-allocated integer array for results
 * @param source Pointer to source float array
 * @return Pointer to the destination array
 */
stat_int_t* stat_floor_float_to_int_array(stat_int_t* destination, const stat_float_t* source);

/**
 * @brief Converts an array of floats to integers using ceiling operation
 * @param destination Pointer to pre-allocated integer array for results
 * @param source Pointer to source float array
 * @return Pointer to the destination array
 */
stat_int_t* stat_ceil_float_to_int_array(stat_int_t* destination, const stat_float_t* source);

/**
 * @brief Converts an array of floats to integers using truncation
 * @param destination Pointer to pre-allocated integer array for results
 * @param source Pointer to source float array
 * @return Pointer to the destination array
 */
stat_int_t* stat_trunc_float_to_int_array(stat_int_t* destination, const stat_float_t* source);

/**
 * @brief Rounds float array elements to specified decimal places
 * @param destination Pointer to pre-allocated float array for results
 * @param source Pointer to source float array
 * @param decimals Number of decimal places to round to
 * @return Pointer to the destination array
 */
stat_float_t* stat_round_float_decimal_array(stat_float_t* destination, const stat_float_t* source, int decimals);

/**
 * @brief Rounds float array elements to nearest multiples
 * @param destination Pointer to pre-allocated float array for results
 * @param source Pointer to source float array
 * @param multiple The multiple to round to
 * @return Pointer to the destination array
 */
stat_float_t* stat_round_float_to_multiple_array(stat_float_t* destination, const stat_float_t* source, stat_float_t multiple);

/**
 * @brief Safely converts float array to integers with overflow checking
 * @param destination Pointer to pre-allocated integer array for results
 * @param source Pointer to source float array
 * @param success_count Output parameter for number of successful conversions
 * @return Pointer to the destination array
 * @note Elements that would overflow are set to 0 and don't increment success_count
 */
stat_int_t* stat_safe_float_to_int_array(stat_int_t* destination, const stat_float_t* source, size_t* success_count);

#endif // STAT_ROUND_H
