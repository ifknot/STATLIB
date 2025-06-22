#ifndef STAT_DISPERSION_H
#define STAT_DISPERSION_H

#include "stat_types.h"

/**
 * @brief Computes the range (max - min) of a float array.
 * @param[in] data Pointer to the input array of floating-point values. Must not be NULL.
 * @param[in] size Number of elements in the array. Must be > 0.
 * @return The range of the data as stat_float_t, or NAN if invalid input.
 * @details
 * - Single-pass O(n) algorithm
 * - Detects and rejects NaN values (returns NAN with errno=EDOM)
 * - Returns NAN with errno=EDOM for size=0
 */
stat_float_t stat_range_f(const stat_float_t* data, stat_size_t size);

/**
 * @brief Computes the Interquartile Range (IQR) of a float array.
 * @param[in,out] data Pointer to the input array. Will be copied and sorted internally.
 * @param[in] size Number of elements in the array. Must be > 1.
 * @return IQR (Q3 - Q1) as stat_float_t, or NAN if invalid input.
 * @details
 * - Robust measure of spread (resistant to outliers)
 * - Creates internal sorted copy (preserves input data)
 * - Uses linear interpolation for percentile calculation
 * - Returns NAN with errno=EDOM for size < 2
 */
stat_float_t stat_interquartile_range_f(stat_float_t* data, stat_size_t size);

/**
 * @brief Computes the Mean Absolute Deviation (MAD) of a float array.
 * @param[in] data Pointer to the input array. Must not be NULL.
 * @param[in] size Number of elements. Must be > 0.
 * @param[in] scale Scaling factor (typically 1.0 for raw MAD).
 * @return Scaled MAD as stat_float_t, or NAN if invalid input.
 * @details
 * - MAD = mean(abs(x - mean(x))) * scale
 * - Two-pass algorithm (first computes mean)
 * - Returns NAN with errno=EDOM for size=0
 */
stat_float_t stat_mean_absolute_deviation_f(stat_float_t* data, stat_size_t size, stat_float_t scale);

/**
 * @brief Computes the standard deviation of a float array.
 * @param[in] data Pointer to the input array. Must not be NULL.
 * @param[in] size Number of elements. Must be > 1.
 * @return Sample standard deviation (σ) as stat_float_t, or NAN if invalid.
 * @details
 * - Computed as sqrt(variance)
 * - Uses unbiased variance estimator (Bessel's correction)
 * - Returns NAN with errno=EDOM for size < 2
 */
stat_float_t stat_std_dev_f(stat_float_t* data, stat_size_t size);

/**
 * @brief Computes the Median Absolute Deviation (MAD) of a float array.
 * @param[in,out] data Pointer to the input array. Will be copied and sorted internally.
 * @param[in] size Number of elements. Must be > 0.
 * @return MAD as stat_float_t, or NAN if invalid input.
 * @details
 * - MAD = median(abs(x - median(x))) * 1.4826
 * - Includes scaling factor for consistency with normal distribution
 * - Creates internal sorted copies (preserves input data)
 * - Returns NAN with errno=EDOM for size=0
 */
stat_float_t stat_median_absolute_deviation_f(stat_float_t* data, stat_size_t size);

/**
 * @brief Robust, location-free scale estimator (Qn).
 * @param[in,out] data Pointer to the input array. Will be copied internally.
 * @param[in] size Number of elements. Must be > 1.
 * @return Qn estimator as stat_float_t, or NAN if invalid input.
 * @details
 * - First quartile of all pairwise absolute differences
 * - Scaled by 2.21914 for consistency with normal distribution
 * - O(n²) time complexity due to pairwise differences
 * - Returns NAN with errno=EDOM for size < 2
 */
stat_float_t stat_qn_estimator_f(stat_float_t* data, stat_size_t size);

/**
 * @brief Computes the variance of a float array (unbiased estimator).
 * @param[in] data Pointer to the input array. Must not be NULL.
 * @param[in] size Number of elements. Must be > 1.
 * @return Sample variance (σ²) as stat_float_t, or NAN if invalid.
 * @details
 * - Uses Bessel's correction (divides by n-1)
 * - Two-pass algorithm for numerical stability
 * - Returns NAN with errno=EDOM for size < 2
 */
stat_float_t stat_variance_f(stat_float_t* data, stat_size_t size);

// ======================== I32 (INTEGER) VERSIONS ========================
/**
 * @brief Range (max - min) of an int32_t array. Returns float to avoid overflow.
 */
stat_float_t stat_range_i(stat_int_t* data, stat_size_t size);

/**
 * @brief Sample variance (σ²) of an int32_t array. Uses float intermediates.
 */
stat_float_t stat_variance_i(stat_int_t* data, stat_size_t size);

/**
 * @brief Sample standard deviation (σ) of an int32_t array.
 */
stat_float_t stat_std_dev_i(stat_int_t* data, stat_size_t size);

/**
 * @brief Mean Absolute Deviation (MAD) of an int32_t array.
 */
stat_float_t stat_mean_absolute_deviation_i(stat_int_t* data, stat_size_t size);

/**
 * @brief Interquartile Range (IQR) of an int32_t array. Robust to outliers.
 */
stat_float_t stat_interquartile_range_i(stat_int_t* data, stat_size_t size);

/**
 * @brief Median Absolute Deviation (MAD) of an int32_t array. Robust to outliers.
 */
stat_float_t stat_median_absolute_deviation_i(stat_int_t* data, stat_size_t size);

/**
 * @brief Robust Qn scale estimator for int32_t arrays. Resistant to outliers.
 */
stat_float_t stat_qn_estimator_i(stat_int_t* data, stat_size_t size);

#endif // STAT_DISPERSION_H
