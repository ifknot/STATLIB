#ifndef STAT_CENTRAL_H
#define STAT_CENTRAL_H

#include "stat_types.h"
#include <stdbool.h>

/**
 * @brief Computes arithmetic mean of float array
 * @param[in] data Input array (must not be NULL)
 * @param[in] count Number of elements
 * @return Mean value (NAN if invalid input)
 * @throws EINVAL if count=0, EDOM if NaN encountered
 * @assert Fails if data=NULL
 */
stat_float_t stat_mean_f(const stat_float_t* data, stat_size_t count);

/**
 * @brief Computes median of float array
 * @param[in] data Input array (must not be NULL)
 * @param[in] count Number of elements
 * @return Median value (NAN if invalid input)
 * @throws EINVAL if count=0, EDOM if NaN encountered
 * @assert Fails if data=NULL
 * @note Creates temporary sorted copy (does not modify input)
 */
stat_float_t stat_median_f(const stat_float_t* data, stat_size_t count);

/**
 * @brief Finds mode(s) of float array
 * @param[in] data Input array (must not be NULL)
 * @param[in] count Number of elements
 * @param[out] modes Pre-allocated output array (size >= count)
 * @param[out] mode_count Number of modes found
 * @return True on success, False on error
 * @throws EINVAL if count=0, EDOM if NaN encountered
 * @assert Fails if data or modes is NULL
 * @note Returns all modes if multimodal
 */
bool stat_mode_f(const stat_float_t* data, stat_size_t count, stat_float_t* modes, stat_size_t* mode_count);

// Integer versions (return float for mean/median)
stat_float_t stat_mean_i(const stat_int_t* data, stat_size_t count);
stat_float_t stat_median_i(const stat_int_t* data, stat_size_t count);
bool stat_mode_i(const stat_int_t* data, stat_size_t count, stat_int_t* modes, stat_size_t* mode_count);

#endif // STAT_CENTRAL_H
