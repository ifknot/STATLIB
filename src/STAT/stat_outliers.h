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
 * @brief Determines which values in an array are outliers (array version)
 * 
 * @param[in] values Input array of values
 * @param[in] count Number of values in input array
 * @param[in] summary Five-number summary reference
 * @param[out] results Output array for outlier flags (must be pre-allocated)
 * @return bool* Pointer to results array for chaining
 */
bool* stat_is_outlier_array(const stat_float_t* values, stat_size_t count, const stat_five_num_summary_t* summary, bool* results);

/**
 * @brief Enumerates outliers by storing pointers to them in a destination array
 * 
 * @param[in] values Input array of values
 * @param[in] count Number of values in input array
 * @param[in] summary Five-number summary reference
 * @param[out] dest Destination array for outlier pointers (must be pre-allocated)
 * @return stat_size_t Number of outliers found (may be less than array capacity)
 */
stat_size_t stat_enumerate_outliers(const stat_float_t* values, stat_size_t count, const stat_five_num_summary_t* summary, stat_float_t* dest);


#endif 
