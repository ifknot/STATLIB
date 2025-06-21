#include "stat_percentiles.h"
#include "stat_sort.h"
#include "stat_util.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

static stat_float_t private_compute_percentile(const stat_float_t* sorted, stat_size_t size, stat_float_t percentile) {
    const stat_float_t rank = (percentile / 100.0f) * (size - 1);
    const stat_size_t lower = (stat_size_t)rank;
    const stat_float_t frac = rank - lower;
    
    return sorted[lower] + frac * (sorted[lower + 1] - sorted[lower]);
}

stat_float_t stat_percentile(stat_float_t* data, stat_size_t size, stat_float_t percentile) {
    assert(data != NULL && "Data pointer cannot be NULL");
    if (size == 0 || percentile < 0 || percentile > 100) {
        errno = EDOM;
        return NAN;
    }

    stat_float_t* sorted = malloc(size * sizeof(stat_float_t));
    if (!sorted) {
        errno = ENOMEM;
        return NAN;
    }

    memcpy(sorted, data, size * sizeof(stat_float_t));
    stat_sort_f(sorted, size);
    
    stat_float_t result = private_compute_percentile(sorted, size, percentile);
    free(sorted);
    return result;
}

stat_float_t* stat_percentiles_array(stat_float_t* data, stat_size_t data_size,
                                   const stat_float_t* percentiles,
                                   stat_float_t* results, stat_size_t p_count) {
    assert(data != NULL && "Data pointer cannot be NULL");
    assert(percentiles != NULL && "Percentiles pointer cannot be NULL");
    assert(results != NULL && "Results pointer cannot be NULL");
    
    if (data_size == 0) {
        errno = EDOM;
        return results;
    }

    stat_float_t* sorted = malloc(data_size * sizeof(stat_float_t));
    if (!sorted) {
        errno = ENOMEM;
        return results;
    }

    memcpy(sorted, data, data_size * sizeof(stat_float_t));
    stat_sort_f(sorted, data_size);

    for (stat_size_t i = 0; i < p_count; i++) {
        if (percentiles[i] < 0 || percentiles[i] > 100) {
            results[i] = NAN;
            errno = EDOM;
        } else {
            results[i] = private_compute_percentile(sorted, data_size, percentiles[i]);
        }
    }

    free(sorted);
    return results;
}

stat_float_t stat_quartile(stat_float_t* data, stat_size_t size, stat_size_t quartile) {
    assert(data != NULL && "Data pointer cannot be NULL");
    if (quartile < 1 || quartile > 3) {
        errno = EDOM;
        return NAN;
    }
    return stat_percentile(data, size, quartile * 25.0f);
}

stat_five_num_summary_t stat_five_num_summary(stat_float_t* data, stat_size_t size) {
    assert(data != NULL && "Data pointer cannot be NULL");
    
    stat_five_num_summary_t summary = {0};
    if (size == 0) {
        errno = EDOM;
        return summary;
    }

    stat_float_t* sorted = malloc(size * sizeof(stat_float_t));
    if (!sorted) {
        errno = ENOMEM;
        return summary;
    }

    memcpy(sorted, data, size * sizeof(stat_float_t));
    stat_sort_f(sorted, size);

    summary.min = sorted[0];
    summary.q1 = private_compute_percentile(sorted, size, 25.0f);
    summary.median = private_compute_percentile(sorted, size, 50.0f);
    summary.q3 = private_compute_percentile(sorted, size, 75.0f);
    summary.max = sorted[size - 1];
    summary.iqr = summary.q3 - summary.q1;
    summary.lower_fence = summary.q1 - 1.5f * summary.iqr;
    summary.upper_fence = summary.q3 + 1.5f * summary.iqr;

    free(sorted);
    return summary;
}
