#ifndef STAT_PERCENTILES_H
#define STAT_PERCENTILES_H

#include "stat_types.h"

/**
 * @brief Compute a single percentile value from an array of floating-point data.
 * @param[in] data Pointer to the input array of floating-point values.
 *                 Will be copied and sorted internally. Must not be NULL.
 * @param[in] size Number of elements in the input array. Must be > 0.
 * @param[in] percentile The desired percentile (0.0 to 100.0).
 *                       Example: 50.0 for median.
 * @return The computed percentile value as stat_float_t.
 * @note Uses linear interpolation between data points for percentiles that don't
 *       fall exactly on an array element.
 * @details Creates an internal sorted copy of the data (original remains unchanged).
 */
stat_float_t stat_percentile_f(
    stat_float_t* data,
    stat_size_t size,
    stat_float_t percentile
);

/**
 * @brief Compute multiple percentiles from an array of floating-point data.
 * @param[in] data       Pointer to the input array of floating-point values.
 *                       Will be copied and sorted internally. Must not be NULL.
 * @param[in] data_size  Number of elements in the input array. Must be > 0.
 * @param[in,out] percentiles Array of desired percentiles (0.0 to 100.0 each).
 *                            Will be sorted in ascending order. Must not be NULL.
 * @param[out] results   Pre-allocated array to store results.
 *                       Must have at least p_count elements.
 * @param[in] p_count    Number of percentiles to compute.
 * @return Pointer to the results array (same as input results parameter).
 * @note More efficient than calling stat_percentile_f() repeatedly as it only
 *       sorts the data once.
 * @details Creates an internal sorted copy of the data (original remains unchanged).
 */
stat_float_t* stat_percentiles_array_f(
    const stat_float_t* data,
    stat_size_t data_size,
    stat_float_t* percentiles,
    stat_float_t* results,
    stat_size_t p_count
);

/**
 * @brief Compute a specific quartile from floating-point data.
 * @param[in] data    Pointer to the input array of floating-point values.
 *                    Will be copied and sorted internally. Must not be NULL.
 * @param[in] size    Number of elements in the input array. Must be > 0.
 * @param[in] quartile The desired quartile type:
 *                     STAT_Q1 (Q1/25th), STAT_MEDIAN (Q2/50th/median), STAT_Q3 (Q3/75th).
 * @return The computed quartile value as stat_float_t.
 * @note Uses the same interpolation method as stat_percentile_f().
 * @details Creates an internal sorted copy of the data (original remains unchanged).
 */
stat_float_t stat_quartile_f(
    stat_float_t* data,
    stat_size_t size,
    stat_quartile_t quartile
);

/**
 * @brief Compute the five-number summary (Tukey's hinges) from floating-point data.
 * @param[in] data Pointer to the input array of floating-point values.
 *                 Will be copied and sorted internally. Must not be NULL.
 * @param[in] size Number of elements in the input array. Must be > 0.
 * @return Struct containing:
 *         - minimum (smallest value)
 *         - first quartile (Q1, 25th percentile)
 *         - median (Q2, 50th percentile)
 *         - third quartile (Q3, 75th percentile)
 *         - maximum (largest value)
 *         - IQR (interquartile range, Q3-Q1)
 *         - lower fence (Q1 - 1.5*IQR)
 *         - upper fence (Q3 + 1.5*IQR)
 * @details The five-number summary provides a robust overview of data distribution.
 *          Tukey's fences are included for outlier detection.
 *          Creates an internal sorted copy of the data (original remains unchanged).
 */
stat_five_num_summary_t stat_five_num_summary_f(
    stat_float_t* data,
    stat_size_t size
);

// ======================== INTEGER VERSIONS ========================
/**
 * @brief Compute a percentile for integer data.
 * @param[in] data Input array of integers. Will be copied and sorted internally.
 * @param[in] size Number of elements in the array. Must be > 0.
 * @param[in] percentile The desired percentile (0.0 to 100.0).
 * @return Computed percentile as float (for interpolation).
 * @details Creates an internal sorted copy of the data (original remains unchanged).
 */
stat_float_t stat_percentile_i(
    const stat_int_t* data,
    stat_size_t size,
    stat_float_t percentile
);

/**
 * @brief Compute multiple percentiles for integer data.
 * @param[in] data Input array of integers. Will be copied and sorted internally.
 * @param[in] data_size Number of elements in the array. Must be > 0.
 * @param[in] percentiles Array of desired percentiles (0.0 to 100.0 each).
 * @param[out] results Pre-allocated output buffer for results.
 * @param[in] p_count Number of percentiles to compute.
 * @return Pointer to results array.
 * @details Creates an internal sorted copy of the data (original remains unchanged).
 */
stat_float_t* stat_percentiles_array_i(
    const stat_int_t* data,
    stat_size_t data_size,
    const stat_float_t* percentiles,
    stat_float_t* results,
    stat_size_t p_count
);

/**
 * @brief Compute a quartile for integer data.
 * @param[in] data Input array of integers. Will be copied and sorted internally.
 * @param[in] size Number of elements in the array. Must be > 0.
 * @param[in] quartile The desired quartile (0=Q1, 1=Median, 2=Q3).
 * @return Computed quartile as float.
 * @details Creates an internal sorted copy of the data (original remains unchanged).
 */
stat_float_t stat_quartile_i(
    const stat_int_t* data,
    stat_size_t size,
    stat_size_t quartile
);

/**
 * @brief Compute five-number summary for integer data.
 * @param[in] data Input array of integers. Will be copied and sorted internally.
 * @param[in] size Number of elements in the array. Must be > 0.
 * @return Struct containing five-number summary with Tukey's fences.
 * @details Creates an internal sorted copy of the data (original remains unchanged).
 */
stat_five_num_summary_t stat_five_num_summary_i(
    const stat_int_t* data,
    stat_size_t size
);

#endif // STAT_PERCENTILES_H
