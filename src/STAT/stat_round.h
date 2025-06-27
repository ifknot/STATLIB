
/**
 * @file stat_round.h
 * @brief Rounding and conversion functions for statistical calculations
 *
 * This module provides various rounding functions and type conversion utilities
 * for statistical operations. It includes both scalar and array-based operations.
 */
#ifndef STAT_ROUND_H
#define STAT_ROUND_H

#include "stat_types.h"

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

/**
 * @brief Rounds a floating-point value to the nearest integer
 * @param value The value to round
 * @return The rounded integer value
 */
stat_int_t stat_round_to_i(stat_float_t value);

/**
 * @brief Rounds a floating-point value down to the nearest integer (floor)
 * @param value The value to round
 * @return The floored integer value
 */
stat_int_t stat_floor_to_i(stat_float_t value);

/**
 * @brief Rounds a floating-point value up to the nearest integer (ceiling)
 * @param value The value to round
 * @return The ceiling integer value
 */
stat_int_t stat_ceil_to_i(stat_float_t value);

/**
 * @brief Truncates a floating-point value to an integer (removes fractional part)
 * @param value The value to truncate
 * @return The truncated integer value
 */
stat_int_t stat_trunc_to_i(stat_float_t value);

/**
 * @brief Rounds a value to the nearest multiple of a specified number
 * @param value The value to round
 * @param multiple The multiple to round to
 * @return The rounded value
 */
stat_float_t stat_round_to_multiple(stat_float_t value, stat_float_t multiple);

/**
 * @brief Rounds a value to a specified number of decimal places
 * @param value The value to round
 * @param decimals Number of decimal places to round to
 * @return The rounded value
 */
stat_float_t stat_round_decimal(stat_float_t value, int decimals);

/**
 * @brief Safely rounds a floating-point value to integer with overflow checking
 * @param value The value to round
 * @param result Pointer to store the result
 * @return true if rounding was successful, false if overflow occurred
 */
bool stat_safe_round_to_i(stat_float_t value, stat_int_t* result);
/* Array conversion functions (non-inplace, destination-first pattern) */

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
stat_int_t* stat_safe_float_to_int_array(stat_int_t* destination, const stat_float_t* source, stat_size_t* success_count);

#endif // STAT_ROUND_H
