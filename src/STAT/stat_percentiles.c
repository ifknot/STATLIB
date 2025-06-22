#include "stat_percentiles.h"
#include "stat_types.h"
#include "stat_util.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static stat_float_t private_compute_percentile_f(const stat_float_t* sorted, stat_size_t size, stat_float_t percentile) {
    assert(sorted != NULL);
    assert(size > 0);
    assert(percentile >= 0.0f && percentile <= 100.0f);

    if (size == 1) {
        return sorted[0];
    }

    const stat_float_t rank = (percentile / 100.0f) * (size - 1);
    const stat_size_t lower = (stat_size_t)rank;
    const stat_float_t frac = rank - lower;

    return sorted[lower] + frac * (sorted[lower + 1] - sorted[lower]);
}

stat_float_t stat_percentile_f(stat_float_t* data, stat_size_t size, stat_float_t percentile) {
    assert(data != NULL && "Data pointer cannot be NULL");
    assert(size > 0 && "Data size cannot be 0");

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

    stat_float_t result = private_compute_percentile_f(sorted, size, percentile);
    free(sorted);
    return result;
}

stat_float_t* stat_percentiles_array_f(
    const stat_float_t* data,
    stat_size_t data_size,
    stat_float_t* percentiles,
    stat_float_t* results,
    stat_size_t p_count
) {
    assert(data != NULL && "Data pointer cannot be NULL");
    assert(percentiles != NULL && "Percentiles pointer cannot be NULL");
    assert(results != NULL && "Results pointer cannot be NULL");
    assert(data_size > 0 && "Data size cannot be 0");
    assert(p_count && "Percentiles count cannot be 0");

    stat_float_t* sorted = malloc(data_size * sizeof(stat_float_t));
    if (!sorted) {
        errno = ENOMEM;
        return results;
    }

    stat_sort_f(percentiles, p_count); // do not rely on user correctly ordering the centiles

    memcpy(sorted, data, data_size * sizeof(stat_float_t));
    stat_sort_f(sorted, data_size);

    for (stat_size_t i = 0; i < p_count; i++) {
        if (percentiles[i] < 0 || percentiles[i] > 100) {
            results[i] = NAN;
            errno = EDOM;
            break;
        } else {
            results[i] = private_compute_percentile_f(sorted, data_size, percentiles[i]);
        }
    }

    free(sorted);
    return results;
}

stat_float_t stat_quartile_f(stat_float_t* data, stat_size_t size, stat_quartile_t quartile) {
    assert(data != NULL && "Data pointer cannot be NULL");
    assert(size > 0 && "Data size cannot be 0");

    if (quartile > STAT_Q3) {
        errno = EDOM;
        return NAN;
    }

    static const stat_float_t QUARTILE_PERCENTS[] = {25.0f, 50.0f, 75.0f};
    return stat_percentile_f(data, size, QUARTILE_PERCENTS[quartile]);
}

stat_five_num_summary_t stat_five_num_summary_f(stat_float_t* data, stat_size_t size) {
    assert(data != NULL && "Data pointer cannot be NULL");
    assert(size > 0 && "Data size cannot be 0");

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
    summary.q1 = private_compute_percentile_f(sorted, size, 25.0f);

    if (size % 2 == 1) {
        summary.median = sorted[size/2]; // For odd sizes, median can be direct access
    } else {
        summary.median = private_compute_percentile_f(sorted, size, 50.0f);
    }

    summary.q3 = private_compute_percentile_f(sorted, size, 75.0f);
    summary.max = sorted[size - 1];
    summary.iqr = summary.q3 - summary.q1;
    summary.lower_fence = summary.q1 - 1.5f * summary.iqr;
    summary.upper_fence = summary.q3 + 1.5f * summary.iqr;

    free(sorted);
    return summary;
}

static stat_float_t private_compute_percentile_i(const stat_int_t* sorted, stat_size_t size, stat_float_t percentile) {
    assert(sorted != NULL);
    assert(size > 0);
    assert(percentile >= 0.0f && percentile <= 100.0f);

    if (size == 1) {
        return (stat_float_t)sorted[0];
    }

    const stat_float_t rank = (percentile / 100.0f) * (size - 1);
    const stat_size_t lower = (stat_size_t)rank;
    const stat_float_t frac = rank - lower;

    return (stat_float_t)sorted[lower] + frac * (sorted[lower + 1] - sorted[lower]);
}

stat_float_t stat_percentile_i(const stat_int_t* data, stat_size_t size, stat_float_t percentile) {
    assert(data != NULL && "Data pointer cannot be NULL");
    assert(size > 0 && "Data size cannot be 0");

    if (percentile < 0 || percentile > 100) {
        errno = EDOM;
        return NAN;
    }

    stat_int_t* sorted = malloc(size * sizeof(stat_int_t));
    if (!sorted) {
        errno = ENOMEM;
        return NAN;
    }

    memcpy(sorted, data, size * sizeof(stat_int_t));
    stat_sort_i(sorted, size);

    stat_float_t result = private_compute_percentile_i(sorted, size, percentile);
    free(sorted);
    return result;
}

stat_float_t* stat_percentiles_array_i(
    const stat_int_t* data,
    stat_size_t data_size,
    const stat_float_t* percentiles,
    stat_float_t* results,
    stat_size_t p_count
) {
    assert(data != NULL && "Data pointer cannot be NULL");
    assert(percentiles != NULL && "Percentiles pointer cannot be NULL");
    assert(results != NULL && "Results pointer cannot be NULL");
    assert(data_size > 0 && "Data size cannot be 0");
    assert(p_count && "Percentiles count cannot be 0");

    stat_int_t* sorted = malloc(data_size * sizeof(stat_int_t));
    if (!sorted) {
        errno = ENOMEM;
        return results;
    }

    stat_float_t* sorted_percentiles = malloc(p_count * sizeof(stat_float_t));
    if (!sorted_percentiles) {
        free(sorted);
        errno = ENOMEM;
        return results;
    }

    memcpy(sorted_percentiles, percentiles, p_count * sizeof(stat_float_t));
    stat_sort_f(sorted_percentiles, p_count);

    memcpy(sorted, data, data_size * sizeof(stat_int_t));
    stat_sort_i(sorted, data_size);

    for (stat_size_t i = 0; i < p_count; i++) {
        if (sorted_percentiles[i] < 0 || sorted_percentiles[i] > 100) {
            results[i] = NAN;
            errno = EDOM;
            break;
        } else {
            results[i] = private_compute_percentile_i(sorted, data_size, sorted_percentiles[i]);
        }
    }

    free(sorted);
    free(sorted_percentiles);
    return results;
}

stat_float_t stat_quartile_i(const stat_int_t* data, stat_size_t size, stat_size_t quartile) {
    assert(data != NULL && "Data pointer cannot be NULL");
    assert(size > 0 && "Data size cannot be 0");

    if (quartile > STAT_Q3) {
        errno = EDOM;
        return NAN;
    }

    static const stat_float_t QUARTILE_PERCENTS[] = {25.0f, 50.0f, 75.0f};
    return stat_percentile_i(data, size, QUARTILE_PERCENTS[quartile]);
}

stat_five_num_summary_t stat_five_num_summary_i(const stat_int_t* data, stat_size_t size) {
    assert(data != NULL && "Data pointer cannot be NULL");
    assert(size > 0 && "Data size cannot be 0");

    stat_five_num_summary_t summary = {0};
    if (size == 0) {
        errno = EDOM;
        return summary;
    }

    stat_int_t* sorted = malloc(size * sizeof(stat_int_t));
    if (!sorted) {
        errno = ENOMEM;
        return summary;
    }

    memcpy(sorted, data, size * sizeof(stat_int_t));
    stat_sort_i(sorted, size);

    summary.min = (stat_float_t)sorted[0];
    summary.q1 = private_compute_percentile_i(sorted, size, 25.0f);

    if (size % 2 == 1) {
        summary.median = (stat_float_t)sorted[size >> 1];
    } else {
        summary.median = private_compute_percentile_i(sorted, size, 50.0f);
    }

    summary.q3 = private_compute_percentile_i(sorted, size, 75.0f);
    summary.max = (stat_float_t)sorted[size - 1];
    summary.iqr = summary.q3 - summary.q1;
    summary.lower_fence = summary.q1 - 1.5f * summary.iqr;
    summary.upper_fence = summary.q3 + 1.5f * summary.iqr;

    free(sorted);
    return summary;
}
