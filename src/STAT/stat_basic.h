#ifndef STAT_BASIC_H
#define STAT_BASIC_H

#include "stat_types.h"

// ========================
// Scalar Functions
// ========================

/**
 * @brief Scalar minimum of two floats
 * @param[in] a First value
 * @param[in] b Second value
 * @return Smaller value
 */
static inline stat_float_t stat_min_float(stat_float_t a, stat_float_t b) {
    return a < b ? a : b;
}

/**
 * @brief Scalar maximum of two floats
 * @param[in] a First value
 * @param[in] b Second value
 * @return Larger value
 */
static inline stat_float_t stat_max_float(stat_float_t a, stat_float_t b) {
    return a > b ? a : b;
}

/**
 * @brief Scalar minimum of two integers
 * @param[in] a First value
 * @param[in] b Second value
 * @return Smaller value
 */
static inline stat_int_t stat_min_int(stat_int_t a, stat_int_t b) {
    return a < b ? a : b;
}

/**
 * @brief Scalar maximum of two integers
 * @param[in] a First value
 * @param[in] b Second value
 * @return Larger value
 */
static inline stat_int_t stat_max_int(stat_int_t a, stat_int_t b) {
    return a > b ? a : b;
}

// ========================
// Array Functions
// ========================

/**
 * @brief Finds minimum value in a float array
 * @param[in] source Input array
 * @param[in] count Number of elements
 * @return Minimum value (NAN if empty or invalid)
 * @throws EINVAL if count=0
 * @assert Fails if source=NULL
 */
stat_float_t stat_min_float_array(const stat_float_t* source, stat_size_t count);

/**
 * @brief Finds maximum value in a float array
 * @param[in] source Input array
 * @param[in] count Number of elements
 * @return Maximum value (NAN if empty or invalid)
 * @throws EINVAL if count=0
 * @assert Fails if source=NULL
 */
stat_float_t stat_max_float_array(const stat_float_t* source, stat_size_t count);

/**
 * @brief Computes range (max - min) of a float array
 * @param[in] source Input array
 * @param[in] count Number of elements
 * @return Range value (NAN if empty or invalid)
 * @throws EINVAL if count=0
 * @assert Fails if source=NULL
 */
stat_float_t stat_range_float_array(const stat_float_t* source, stat_size_t count);

/**
 * @brief Computes maximum value in an integer array
 * @param[in] source Input array
 * @param[in] count Number of elements
 * @return Maximum value (INT_MIN if empty or invalid)
 * @throws EINVAL if count=0
 * @assert Fails if source=NULL
 */
stat_int_t stat_max_int_array(const stat_int_t* source, stat_size_t count);

/**
 * @brief Computes minimum value in an integer array
 * @param[in] source Input array
 * @param[in] count Number of elements
 * @return Minimum value (INT_MAX if empty or invalid)
 * @throws EINVAL if count=0
 * @assert Fails if source=NULL
 */
stat_int_t stat_min_int_array(const stat_int_t* source, stat_size_t count);

/**
 * @brief Computes range (max - min) of an integer array
 * @param[in] source Input array
 * @param[in] count Number of elements
 * @return Range value (INT_MIN if empty or invalid)
 * @throws EINVAL if count=0
 * @assert Fails if source=NULL
 */
stat_int_t stat_range_int_array(const stat_int_t* source, stat_size_t count);

// ========================
// Array Conversion Functions
// ========================

/**
 * @brief Converts an array of floats to integers (truncating)
 * @param destination Pointer to pre-allocated integer array
 * @param source Pointer to source float array
 * @param count Number of elements to convert
 * @return Pointer to destination array
 */
stat_int_t* stat_cast_float_to_int_array(stat_int_t* destination, const stat_float_t* source, stat_size_t count);

/**
 * @brief Converts an array of integers to floats
 * @param destination Pointer to pre-allocated float array
 * @param source Pointer to source integer array
 * @param count Number of elements to convert
 * @return Pointer to destination array
 */
stat_float_t* stat_cast_int_to_float_array(stat_float_t* destination, const stat_int_t* source, stat_size_t count);

#endif // STAT_BASIC_H
