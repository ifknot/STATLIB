#ifndef STAT_UTIL_H
#define STAT_UTIL_H

#include "stat_types.h"

/**
 * @file stat_util.h
 * @brief Utility functions for statistical operations
 * 
 * Contains general-purpose utility functions that support statistical calculations,
 * including sorting routines and floating-point classification.
 */

/**
 * @brief Sorts an array of floating-point values in ascending order
 * @param[in,out] data Array to be sorted (modified in-place)
 * @param[in] size Number of elements in the array
 * @note Uses efficient in-place sorting algorithm
 * @warning Invalidates any existing sort order in the array
 * @assert data != NULL
 */
void stat_sort_f(stat_float_t* data, stat_size_t size);

/**
 * @brief Sorts an array of integer values in ascending order
 * @param[in,out] data Array to be sorted (modified in-place)
 * @param[in] size Number of elements in the array
 * @note Optimized for integer sorting
 * @warning Invalidates any existing sort order in the array
 * @assert data != NULL
 */
void stat_sort_i(stat_int_t* data, stat_size_t size);

/**
 * @brief Checks if a floating-point value is finite
 * @param[in] value Value to check
 * @return true if value is finite (not NaN or infinite), false otherwise
 * @note Equivalent to !isnan(value) && !isinf(value)
 */
bool stat_is_finite(stat_float_t value);

/**
 * @brief Checks if a floating-point value is normal
 * @param[in] value Value to check
 * @return true if value is normal (not zero, subnormal, infinite, or NaN)
 * @note Useful for validating inputs before statistical operations
 */
bool stat_is_normal(stat_float_t value);

#endif // STAT_UTIL_H
