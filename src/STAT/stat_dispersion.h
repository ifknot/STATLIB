#ifndef STAT_DISPERSION_H
#define STAT_DISPERSION_H

#include "stat_types.h"

/**
 * @brief Computes the range (max - min) of a float array.
 * @param data Pointer to the input array of floating-point values.
 * @param size Number of elements in the array.
 * @return The range of the data as a stat_float_t.
 */
stat_float_t stat_range_f(stat_float_t* data, stat_size_t size);

/**
 * @brief Computes the variance of a float array (unbiased estimator).
 * @param data Pointer to the input array of floating-point values.
 * @param size Number of elements in the array.
 * @return The sample variance (σ²) as a stat_float_t.
 */
stat_float_t stat_variance_f(stat_float_t* data, stat_size_t size);

/**
 * @brief Computes the standard deviation of a float array.
 * @param data Pointer to the input array of floating-point values.
 * @param size Number of elements in the array.
 * @return The sample standard deviation (σ) as a stat_float_t.
 */
stat_float_t stat_std_dev_f(stat_float_t* data, stat_size_t size);

/**
 * @brief Computes the Mean Absolute Deviation (MAD) of a float array.
 * @details MAD = average of absolute deviations from the mean.
 * @param data Pointer to the input array of floating-point values.
 * @param size Number of elements in the array.
 * @return The MAD as a stat_float_t.
 */
stat_float_t stat_mean_absolute_deviation_f(stat_float_t* data, stat_size_t size);

/**
 * @brief Computes the Interquartile Range (IQR) of a float array.
 * @details IQR = Q3 (75th percentile) - Q1 (25th percentile). A robust measure of spread.
 * @param data Pointer to the input array of floating-point values.
 * @param size Number of elements in the array.
 * @return The IQR as a stat_float_t.
 */
stat_float_t stat_interquartile_range_f(stat_float_t* data, stat_size_t size);

/**
 * @brief Computes the Median Absolute Deviation (MAD) of a float array.
 * @details MAD = median of absolute deviations from the median. Highly robust to outliers.
 * @param data Pointer to the input array of floating-point values.
 * @param size Number of elements in the array.
 * @return The MAD as a stat_float_t.
 */
stat_float_t stat_median_absolute_deviation_f(stat_float_t* data, stat_size_t size);

/**
 * @brief Robust, location-free scale estimator (Qn).
 * @details Estimates spread without assuming a central value. Efficient for normal data.
 * Based on the first quartile of all pairwise absolute differences between points.
 * @param data Pointer to the input array of floating-point values.
 * @param size Number of elements in the array.
 * @return The Qn estimator as a stat_float_t.
 */
stat_float_t stat_qn_estimator_f(stat_float_t* data, stat_size_t size);

// ======================== I32 (INTEGER) VERSIONS ========================
/**
 * @brief Range (max - min) of an int32_t array. Returns float to avoid overflow.
 */
stat_float_t stat_range_i32(stat_int_t* data, stat_size_t size);

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
