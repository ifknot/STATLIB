#include "stat_dispersion.h"
#include "stat_central.h"
#include "stat_percentiles.h"
#include "stat_types.h"
#include "stat_util.h"
#include "stat_abs.h"

#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// ======================== FLOAT IMPLEMENTATIONS ========================

stat_float_t stat_range_f(const stat_float_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EDOM;
        return NAN;
    }

    stat_float_t min = data[0], max = data[0];
    for (stat_size_t i = 1; i < count; i++) {
        if (isnan(data[i])) {
            errno = EDOM;
            return NAN;
        }
        min = data[i] < min ? data[i] : min;
        max = data[i] > max ? data[i] : max;
    }
    return max - min;
}

stat_float_t stat_interquartile_range_f(stat_float_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EDOM;
        return NAN;
    }

    stat_float_t* sorted = malloc(count * sizeof(stat_float_t));
    if (!sorted) {
        errno = ENOMEM;
        return NAN;
    }

    memcpy(sorted, data, count * sizeof(stat_float_t));
    stat_sort_f(sorted, count);

    stat_float_t q1 = stat_percentile_f(sorted, count, 25.0f);
    stat_float_t q3 = stat_percentile_f(sorted, count, 75.0f);

    free(sorted);
    return q3 - q1;
}

stat_float_t stat_mean_absolute_deviation_f(stat_float_t* data, stat_size_t count, stat_float_t scale) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EDOM;
        return NAN;
    }

    stat_float_t mean = stat_mean_f(data, count);
    if (isnan(mean)) {
        return NAN;
    }

    stat_float_t sum = 0.0f;
    for (stat_size_t i = 0; i < count; i++) {
        sum += stat_abs_scalar_f(data[i] - mean);
    }

    return (sum / count) * scale;
}

stat_float_t stat_median_absolute_deviation_f(stat_float_t* data, stat_size_t count) {
    return stat_mean_absolute_deviation_f(data, count, 1.4826f); // Scaling factor for normal dist
}

stat_float_t stat_std_dev_f(stat_float_t* data, stat_size_t count) {
    stat_float_t var = stat_variance_f(data, count);
    return isnan(var) ? NAN : sqrtf(var);
}

stat_float_t stat_variance_f(stat_float_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count < 2) {
        errno = EDOM;
        return NAN;
    }

    stat_float_t mean = stat_mean_f(data, count);
    if (isnan(mean)) {
        return NAN;
    }

    stat_float_t sum_sq = 0.0f;
    for (stat_size_t i = 0; i < count; i++) {
        stat_float_t diff = data[i] - mean;
        sum_sq += diff * diff;
    }

    return sum_sq / (count - 1); // Unbiased estimator
}

stat_float_t stat_qn_estimator_f(stat_float_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count < 2) {
        errno = EDOM;
        return NAN;
    }

    // Create array for pairwise differences
    stat_size_t n_pairs = count * (count - 1) / 2;
    stat_float_t* diffs = malloc(n_pairs * sizeof(stat_float_t));
    if (!diffs) {
        errno = ENOMEM;
        return NAN;
    }

    // Compute all pairwise absolute differences
    stat_size_t idx = 0;
    for (stat_size_t i = 0; i < count; i++) {
        for (stat_size_t j = i + 1; j < count; j++) {
            diffs[idx++] = stat_abs_scalar_f(data[i] - data[j]);
        }
    }

    // Find first quartile of differences
    stat_sort_f(diffs, n_pairs);
    stat_float_t qn = stat_percentile_f(diffs, n_pairs, 25.0f);

    free(diffs);
    return qn * 2.21914f; // Scaling factor for consistency
}

// ======================== INTEGER IMPLEMENTATIONS ========================

stat_float_t stat_range_i(stat_int_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EDOM;
        return NAN;
    }

    stat_int_t min = data[0], max = data[0];
    for (stat_size_t i = 1; i < count; i++) {
        min = data[i] < min ? data[i] : min;
        max = data[i] > max ? data[i] : max;
    }
    return (stat_float_t)(max - min);
}

stat_float_t stat_variance_i(stat_int_t* data, stat_size_t count) {
    if (count < 2) {
        errno = EDOM;
        return NAN;
    }

    stat_float_t mean = stat_mean_i(data, count);
    stat_float_t sum_sq = 0.0f;

    for (stat_size_t i = 0; i < count; i++) {
        stat_float_t diff = (stat_float_t)data[i] - mean;
        sum_sq += diff * diff;
    }

    return sum_sq / (count - 1);
}

stat_float_t stat_std_dev_i(stat_int_t* data, stat_size_t count) {
    stat_float_t var = stat_variance_i(data, count);
    return isnan(var) ? NAN : sqrtf(var);
}

stat_float_t stat_mean_absolute_deviation_i(stat_int_t* data, stat_size_t count) {
    if (count == 0) {
        errno = EDOM;
        return NAN;
    }

    stat_float_t mean = stat_mean_i(data, count);
    stat_float_t sum = 0.0f;

    for (stat_size_t i = 0; i < count; i++) {
        sum += stat_abs_scalar_f((stat_float_t)data[i] - mean);
    }

    return sum / count;
}

stat_float_t stat_interquartile_range_i(stat_int_t* data, stat_size_t count) {
    stat_float_t* fdata = malloc(count * sizeof(stat_float_t));
    if (!fdata) {
        errno = ENOMEM;
        return NAN;
    }

    for (stat_size_t i = 0; i < count; i++) {
        fdata[i] = (stat_float_t)data[i];
    }

    stat_float_t result = stat_interquartile_range_f(fdata, count);
    free(fdata);
    return result;
}

stat_float_t stat_median_absolute_deviation_i(stat_int_t* data, stat_size_t count) {
    stat_float_t* fdata = malloc(count * sizeof(stat_float_t));
    if (!fdata) {
        errno = ENOMEM;
        return NAN;
    }

    for (stat_size_t i = 0; i < count; i++) {
        fdata[i] = (stat_float_t)data[i];
    }

    stat_float_t result = stat_median_absolute_deviation_f(fdata, count);
    free(fdata);
    return result;
}

stat_float_t stat_qn_estimator_i(stat_int_t* data, stat_size_t count) {
    stat_float_t* fdata = malloc(count * sizeof(stat_float_t));
    if (!fdata) {
        errno = ENOMEM;
        return NAN;
    }

    for (stat_size_t i = 0; i < count; i++) {
        fdata[i] = (stat_float_t)data[i];
    }

    stat_float_t result = stat_qn_estimator_f(fdata, count);
    free(fdata);
    return result;
}
