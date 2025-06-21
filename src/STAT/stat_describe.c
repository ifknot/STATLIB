#include "stat_describe.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <memory.h>

#include "stat_basic.h"
#include "stat_central.h"
#include "stat_util.h"
#include "errno.h"

/*
stat_summary_t stat_describe_f(const stat_float_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    stat_summary_t summary = {0};
    if (count == 0) {
        errno = EINVAL;
        return summary;
    }

    // Create a copy for operations that need sorting
    stat_float_t* copy = malloc(count * sizeof(stat_float_t));
    if (!copy) {
        errno = ENOMEM;
        return summary;
    }
    memcpy(copy, data, count * sizeof(stat_float_t));

    // Basic metrics
    summary.count = count;
    summary.min = stat_min_f(data, count);
    summary.max = stat_max_f(data, count);
    summary.mean = stat_mean_f(data, count);

    // Variance/stddev
    summary.variance = 0;//stat_variance_f(data, count, true);
    summary.stddev = sqrt(summary.variance);

    // Percentiles (requires sorted data)
    stat_sort_f(copy, count);
    summary.median = stat_median_f(copy, count);
    summary.q25 = 0;//stat_percentile_f(copy, count, 25.0f);
    summary.q75 = 0;//stat_percentile_f(copy, count, 75.0f);

    free(copy);
    return summary;
}

stat_summary_t stat_describe_i32(const stat_int_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    stat_summary_t summary = {0};
    if (count == 0) {
        errno = EINVAL;
        return summary;
    }

    // Create float copy for consistent calculations
    stat_float_t* fdata = malloc(count * sizeof(stat_float_t));
    if (!fdata) {
        errno = ENOMEM;
        return summary;
    }
    for (stat_size_t i = 0; i < count; i++) {
        fdata[i] = (stat_float_t)data[i];
    }

    //summary = stat_describe_f(fdata, count);
    free(fdata);
    return summary;
}
*/
