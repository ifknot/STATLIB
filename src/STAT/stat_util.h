#ifndef STAT_UTIL_H
#define STAT_UTIL_H

#include "stat_types.h"

/**
 * @file stat_util.h
 * @brief Utility functions for statistical operations
 * 
 * Contains general-purpose utility functions including sorting routines,
 * floating-point classification, and array validation/operations.
 */

// ========================
// Sorting Functions
// ========================

/**
 * @brief Sorts an array of floating-point values in ascending order
 * @param[in,out] data Array to be sorted (modified in-place)
 * @param[in] size Number of elements in the array
 * @note Uses quicksort algorithm (O(n log n) average case)
 * @warning Invalidates any existing sort order in the array
 * @assert data != NULL
 */
void stat_sort_f(stat_float_t* data, stat_size_t size);

/**
 * @brief Sorts an array of integer values in ascending order
 * @param[in,out] data Array to be sorted (modified in-place)
 * @param[in] size Number of elements in the array
 * @note Uses insertion sort for small arrays, quicksort otherwise
 * @warning Invalidates any existing sort order in the array
 * @assert data != NULL
 */
void stat_sort_i(stat_int_t* data, stat_size_t size);

// ========================
// Value Validation
// ========================

/**
 * @brief Checks if a floating-point value is finite
 * @param[in] value Value to check
 * @return true if value is finite (not NaN or infinite), false otherwise
 */
bool stat_is_finite(stat_float_t value);

/**
 * @brief Checks if a floating-point value is normal
 * @param[in] value Value to check
 * @return true if value is normal (not zero, subnormal, infinite, or NaN)
 */
bool stat_is_normal(stat_float_t value);

/**
 * @brief Comprehensive validity check for statistical calculations
 * @param[in] value Value to check
 * @return true if value is finite, normal, and within reasonable bounds
 */
bool stat_is_valid(stat_float_t value);

// ========================
// Array Validation
// ========================

/**
 * @brief Checks if all elements in a float array are valid
 * @param[in] data Array to check
 * @param[in] size Number of elements
 * @return true if all elements are valid, false otherwise
 * @assert data != NULL
 */
bool stat_array_is_valid_f(const stat_float_t* data, stat_size_t size);

/**
 * @brief Checks if an array is sorted in ascending order
 * @param[in] data Array to check
 * @param[in] size Number of elements
 * @return true if array is sorted, false otherwise
 * @assert data != NULL
 */
bool stat_array_is_sorted_f(const stat_float_t* data, stat_size_t size);

/**
 * @brief Checks if an array contains only finite values
 * @param[in] data Array to check
 * @param[in] size Number of elements
 * @return true if all elements are finite, false otherwise
 * @assert data != NULL
 */
bool stat_array_is_finite_f(const stat_float_t* data, stat_size_t size);

// ========================
// Array Operations
// ========================

/**
 * @brief Replaces NaN values in an array with a specified substitute
 * @param[in,out] data Array to process
 * @param[in] size Number of elements
 * @param[in] substitute Value to use instead of NaN
 * @return Number of replacements made
 * @assert data != NULL
 */
stat_size_t stat_array_replace_nan_f(stat_float_t* data, stat_size_t size, stat_float_t substitute);

#endif // STAT_UTIL_H
