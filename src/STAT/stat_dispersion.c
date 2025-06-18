#include "stat_dispersion.h"
#include "stat_central.h" // for mean/median
#include "stat_percentiles.h" // for iqr
#include "stat_util.h" // for sorting
#include <math.h>
#include <stdlib.h>

stat_float_t stat_range(stat_float_t* data, stat_size_t size) {
    if (size == 0) return NAN;

    stat_float_t min = data[0], max = data[0];
    for (stat_size_t i = 1; i < size; i++) {
        if (data[i] < min) min = data[i];
        if (data[i] > max) max = data[i];
    }
    return max - min;
}

stat_float_t stat_variance(stat_float_t* data, stat_size_t size) {
    if (size < 2) return NAN;

    stat_float_t mean = stat_mean(data, size);
    stat_float_t sum_sq_diff = 0.0;

    for (stat_size_t i = 0; i < size; i++) {
        sum_sq_diff += (data[i] - mean) * (data[i] - mean);
    }

    return sum_sq_diff / (size - 1); // Sample variance
}

stat_float_t stat_std_dev(stat_float_t* data, stat_size_t size) {
    return sqrt(stat_variance(data, size));
}

stat_float_t stat_mean_absolute_deviation(stat_float_t* data, stat_size_t size) {
    if (size == 0) return NAN;

    stat_float_t mean = stat_mean(data, size);
    stat_float_t sum_abs_diff = 0.0;

    for (stat_size_t i = 0; i < size; i++) {
        sum_abs_diff += fabs(data[i] - mean);
    }

    return sum_abs_diff / size;
}

stat_float_t stat_iqr(stat_float_t* data, stat_size_t size) {
    stat_five_num_summary_t fns = stat_five_num_summary(data, size);
    return fns.iqr;
}

stat_float_t stat_mad(stat_float_t* data, stat_size_t size) {
    if (size == 0) return NAN;

    stat_float_t med = stat_median(data, size);
    stat_float_t* abs_deviations = malloc(size * sizeof(stat_float_t));
    if (!abs_deviations) return NAN;

    for (stat_size_t i = 0; i < size; i++) {
        abs_deviations[i] = fabs(data[i] - med);
    }

    stat_float_t mad = stat_median(abs_deviations, size);
    free(abs_deviations);
    return mad;
}

stat_float_t stat_qn_estimator(stat_float_t* data, stat_size_t size) {
    if (size < 2) return NAN;

    stat_sort(data, size);
    stat_size_t h = (size / 2) + 1;
    stat_size_t k = h * (h - 1) / 2;

    stat_float_t* diffs = malloc(k * sizeof(stat_float_t));
    if (!diffs) return NAN;

    stat_size_t idx = 0;
    for (stat_size_t i = 0; i < size; i++) {
        for (stat_size_t j = i + 1; j < size && j < i + h; j++) {
            diffs[idx++] = fabs(data[i] - data[j]);
        }
    }

    stat_sort(diffs, k);
    stat_float_t qn = diffs[k/2];
    free(diffs);

    // Consistency factor for normal distribution
    return qn / (2.21914 * 1.0 / sqrt(size));
}
