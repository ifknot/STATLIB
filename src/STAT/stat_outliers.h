#ifndef STAT_OUTLIERS_H
#define STAT_OUTLIERS_H

#include "stat_types.h"

/**
 * @brief Determines if a value is an outlier based on a five-number summary
 * 
 * @param value The value to check
 * @param summary Pointer to the five-number summary
 * @return true if the value is an outlier, false otherwise
 */
bool stat_is_outlier(stat_float_t value, const stat_five_num_summary_t* summary);

/**
 * @brief Counts outliers in an array based on a five-number summary
 * 
 * @param data Array of values to check
 * @param size Number of elements in the array
 * @param summary Pointer to the five-number summary
 * @return stat_size_t Number of outliers found
 */
stat_size_t stat_count_outliers(const stat_float_t* data, stat_size_t size, const stat_five_num_summary_t* summary);

/**
 * @brief Flags outliers in an array by setting boolean indicators
 * 
 * @param[in] values Input array of values
 * @param[in] count Number of values in input array
 * @param[in] summary Five-number summary reference
 * @param[out] flags Output array for outlier flags (must be pre-allocated)
 * @return stat_size_t Number of outliers found
 */
stat_size_t stat_flag_outliers(const stat_float_t* values, stat_size_t count, const stat_five_num_summary_t* summary, bool* flags);

/**
 * @brief Collects outlier values into a destination array
 * 
 * @param[in] src Input array of values
 * @param[in] src_count Number of values in input array
 * @param[in] summary Five-number summary reference
 * @param[out] dst Destination array for outlier values (must be pre-allocated)
 * @return stat_size_t Number of outliers collected
 */
stat_size_t stat_collect_outliers(const stat_float_t* values, stat_size_t src_count, const stat_five_num_summary_t* summary, stat_float_t* outliers);


#endif 
