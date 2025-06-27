#ifndef STAT_ROUND_H
#define STAT_ROUND_H

#include "stat_types.h"

/**
 * @file stat_round.h
 * @brief Rounding and conversion functions for statistical calculations
 * 
 * This module provides various rounding functions and type conversion utilities
 * for statistical operations. It includes both scalar and array-based operations.
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

/**
 * @brief Converts an array of integers to an array of floats
 * @param destination Pointer to destination float array
 * @param source Pointer to source integer array
 * @return Pointer to the destination array
 */
stat_float_t* stat_cast_int_to_float_array(stat_float_t* destination, stat_int_t* source);

/* Suggested array conversion functions */

/**
 * @brief Rounds all elements in a float array to integers (in-place)
 * @param array The array to round
 * @param count Number of elements in the array
 */
void stat_round_array_to_i(stat_float_t* array, size_t count);

/**
 * @brief Rounds all elements in a float array to integers (to a new array)
 * @param dest Destination integer array
 * @param src Source float array
 * @param count Number of elements to process
 */
void stat_round_array_to_i_dest(stat_int_t* dest, const stat_float_t* src, size_t count);

/**
 * @brief Floors all elements in a float array to integers (in-place)
 * @param array The array to process
 * @param count Number of elements in the array
 */
void stat_floor_array_to_i(stat_float_t* array, size_t count);

/**
 * @brief Ceils all elements in a float array to integers (in-place)
 * @param array The array to process
 * @param count Number of elements in the array
 */
void stat_ceil_array_to_i(stat_float_t* array, size_t count);

/**
 * @brief Truncates all elements in a float array to integers (in-place)
 * @param array The array to process
 * @param count Number of elements in the array
 */
void stat_trunc_array_to_i(stat_float_t* array, size_t count);

/**
 * @brief Converts a float array to an integer array with rounding
 * @param dest Destination integer array
 * @param src Source float array
 * @param count Number of elements to process
 */
void stat_float_array_to_int(stat_int_t* dest, const stat_float_t* src, size_t count);

/**
 * @brief Rounds all elements in an array to specified decimal places (in-place)
 * @param array The array to process
 * @param count Number of elements in the array
 * @param decimals Number of decimal places to round to
 */
void stat_round_array_decimal(stat_float_t* array, size_t count, int decimals);

/**
 * @brief Rounds all elements in an array to specified multiples (in-place)
 * @param array The array to process
 * @param count Number of elements in the array
 * @param multiple The multiple to round to
 */
void stat_round_array_to_multiple(stat_float_t* array, size_t count, stat_float_t multiple);

#endif // STAT_ROUND_H
