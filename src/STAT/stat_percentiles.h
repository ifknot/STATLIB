#ifndef STAT_PERCENTILES_H
#define STAT_PERCENTILES_H

#include "stat_types.h"

/**
 * @brief Computes a single percentile value from sorted data
 * @param data Sorted array of values
 * @param size Number of elements in array
 * @param percentile Target percentile (0.0-100.0)
 * @return stat_float_t The computed percentile value
 * @note Data must be sorted in ascending order
 */
stat_float_t stat_percentile(const stat_float_t* data, stat_size_t size, stat_float_t percentile);

/**
 * @brief Computes multiple percentiles from sorted data (array version)
 * @param[in] data Sorted input data
 * @param[in] data_size Number of elements in input data
 * @param[in] percentiles Array of target percentiles (0.0-100.0)
 * @param[out] results Pre-allocated output array for results
 * @param[in] p_count Number of percentiles to compute
 * @return stat_float_t* Pointer to results for chaining
 * @note Data must be sorted in ascending order
 */
stat_float_t* stat_percentiles_array(const stat_float_t* data, stat_size_t data_size, const stat_float_t* percentiles, stat_float_t* results, stat_size_t p_count);

/**
 * @brief Computes a specific quartile (1-4) from sorted data
 * @param data Sorted array of values
 * @param size Number of elements in array
 * @param quartile Quartile number (1-4)
 * @return stat_float_t The computed quartile value
 * @note Data must be sorted in ascending order
 */
stat_float_t stat_quartile(const stat_float_t* data, stat_size_t size, stat_size_t quartile);

/**
 * @brief Computes the five-number summary (min, Q1, median, Q3, max)
 * @param data Sorted array of values
 * @param size Number of elements in array
 * @return stat_five_num_summary_t The computed summary
 * @note Data must be sorted in ascending order
 */
stat_five_num_summary_t stat_five_num_summary(const stat_float_t* data, stat_size_t size);

#endif // STAT_PERCENTILES_H
