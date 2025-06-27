#ifndef STAT_COMPARE_H
#define STAT_COMPARE_H

#include "stat_types.h"

/**
 * @file stat_compare.h
 * @brief Floating-point comparison utilities with tolerance support
 */

/**
 * @brief Compares two floating-point numbers with epsilon tolerance
 * @param a First value to compare
 * @param b Second value to compare
 * @param epsilon Absolute tolerance for equality
 * @return -1 if a < b (beyond epsilon), 
 *          1 if a > b (beyond epsilon), 
 *          0 if approximately equal (within epsilon)
 * @note Uses absolute comparison only - for relative tolerance see stat_almost_equal()
 */
int stat_compare_floats(stat_float_t a, stat_float_t b, stat_float_t epsilon);

/**
 * @brief Tests approximate equality with both relative and absolute tolerances
 * @param a First value to compare
 * @param b Second value to compare
 * @param rel_epsilon Relative tolerance (percentage of larger value)
 * @param abs_epsilon Absolute tolerance (fixed value)
 * @return true if values are approximately equal, false otherwise
 * @note For values near zero, absolute tolerance is used
 */
bool stat_almost_equal(stat_float_t a, stat_float_t b, 
                      stat_float_t rel_epsilon, stat_float_t abs_epsilon);

/**
 * @brief Checks if a value is approximately zero
 * @param value Value to test
 * @param threshold Absolute tolerance threshold
 * @return true if |value| < threshold, false otherwise
 */
bool stat_is_near_zero(stat_float_t value, stat_float_t threshold);

/**
 * @brief qsort-compatible float comparison wrapper
 * @param a Pointer to first float value
 * @param b Pointer to second float value
 * @return Comparison result suitable for qsort()
 * @note Uses default epsilon of 1e-9 internally
 */
int stat_compare_floats_qsort(const void* a, const void* b);

/**
 * @brief qsort-compatible integer comparison wrapper
 * @param a Pointer to first integer value
 * @param b Pointer to second integer value
 * @return Comparison result suitable for qsort()
 */
int stat_compare_ints_qsort(const void* a, const void* b);

#endif // STAT_COMPARE_H
