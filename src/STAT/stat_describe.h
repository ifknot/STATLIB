#ifndef STAT_DESCRIBE_H
#define STAT_DESCRIBE_H

#include "stat_types.h"
#include <stdbool.h>

typedef struct {
    stat_float_t min;
    stat_float_t max;
    stat_float_t mean;
    stat_float_t median;
    stat_float_t variance;
    stat_float_t stddev;
    stat_float_t q25;
    stat_float_t q75;
    stat_size_t count;
} stat_summary_t;

/**
 * @brief Computes comprehensive statistical summary
 * @param[in] data Input array (must not be NULL)
 * @param[in] count Number of elements
 * @return stat_summary_t structure with all metrics
 * @throws EINVAL if count=0, EDOM if NaN encountered
 * @assert Fails if data=NULL
 */
stat_summary_t stat_describe_f(const stat_float_t* data, stat_size_t count);

/**
 * @brief Computes summary statistics for int32 array
 * @param[in] data Input array (must not be NULL)
 * @param[in] count Number of elements
 * @return stat_summary_t structure (floating-point results)
 * @throws EINVAL if count=0
 * @assert Fails if data=NULL
 */
stat_summary_t stat_describe_i(const stat_int_t* data, stat_size_t count);

#endif // STAT_DESCRIBE_H
