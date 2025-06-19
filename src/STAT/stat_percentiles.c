#include "stat_percentiles.h"
#include "stat_util.h" // for stat_sort
#include <stdlib.h>
#include <math.h>
#include <string.h>

stat_float_t stat_percentile(stat_float_t* data, stat_size_t size, stat_float_t percentile) {
    if (size == 0 || percentile < 0 || percentile > 100) return NAN;

    stat_float_t* sorted = malloc(size * sizeof(stat_float_t));
    if (!sorted) return NAN;

    memcpy(sorted, data, size * sizeof(stat_float_t));
    stat_sort_f(sorted, size);

    stat_float_t index = (percentile / 100.0) * (size - 1);
    stat_size_t lower = (stat_size_t)floor(index);
    stat_size_t upper = (stat_size_t)ceil(index);

    stat_float_t result;
    if (lower == upper) {
        result = sorted[lower];
    } else {
        stat_float_t weight = index - lower;
        result = sorted[lower] * (1 - weight) + sorted[upper] * weight;
    }

    free(sorted);
    return result;
}

stat_float_t stat_quartile(stat_float_t* data, stat_size_t size, stat_size_t quartile) {
    if (quartile < 1 || quartile > 3) return NAN;
    return stat_percentile(data, size, quartile * 25.0);
}

stat_five_num_summary_t stat_five_num_summary(stat_float_t* data, stat_size_t size) {
    stat_five_num_summary_t summary;

    summary.min = stat_percentile(data, size, 0);
    summary.q1 = stat_percentile(data, size, 25);
    summary.median = stat_percentile(data, size, 50);
    summary.q3 = stat_percentile(data, size, 75);
    summary.max = stat_percentile(data, size, 100);

    summary.iqr = summary.q3 - summary.q1;
    summary.lower_fence = summary.q1 - 1.5 * summary.iqr;
    summary.upper_fence = summary.q3 + 1.5 * summary.iqr;

    return summary;
}
