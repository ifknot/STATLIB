#include "stat_dispersion.h"
#include "stat_central.h"
#include "stat_util.h"
#include "stat_abs.h"

#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

stat_float_t stat_range_f(const stat_float_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
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

stat_float_t stat_iqr_f(stat_float_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }

    // Sort in-place
    stat_sort_f(data, count);

    // Calculate quartiles
    stat_float_t q1 = stat_percentile_f(data, count, 25.0f);
    stat_float_t q3 = stat_percentile_f(data, count, 75.0f);
    return q3 - q1;
}

stat_float_t stat_mad_f(stat_float_t* data, stat_size_t count, stat_float_t scale) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }

    // Sort in-place
    stat_sort_f(data, count);

    // Calculate median
    stat_float_t med = stat_median_f(data, count);

    // Compute absolute deviations
    stat_float_t* deviations = malloc(count * sizeof(stat_float_t));
    if (!deviations) {
        errno = ENOMEM;
        return NAN;
    }

    for (stat_size_t i = 0; i < count; i++) {
        deviations[i] = stat_abs_scalar_f(data[i] - med);
    }

    // Sort and get median of deviations
    stat_sort_f(deviations, count);
    stat_float_t mad = stat_median_f(deviations, count);

    free(deviations);
    return mad * scale;
}

stat_float_t stat_cv_f(const stat_float_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }

    stat_float_t mean = stat_mean_f(data, count);
    if (mean == 0.0f) {
        errno = EINVAL;
        return NAN;
    }

    stat_float_t stddev = sqrtf(stat_variance_f(data, count, true));
    return stddev / mean;
}

// Integer versions (convert to float for calculations)

stat_float_t stat_range_i32(const stat_int_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");
    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }

    stat_int_t min = data[0], max = data[0];
    for (stat_size_t i = 1; i < count; i++) {
        min = data[i] < min ? data[i] : min;
        max = data[i] > max ? data[i] : max;
    }
    return (stat_float_t)(max - min);
}

stat_float_t stat_iqr_i32(stat_int_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");
    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }

    stat_sort_i(data, count);
    stat_float_t q1 = stat_percentile_i(data, count, 25.0f);
    stat_float_t q3 = stat_percentile_i(data, count, 75.0f);
    return q3 - q1;
}

stat_float_t stat_mad_i32(stat_int_t* data, stat_size_t count, stat_float_t scale) {
    assert(data != NULL && "Input array cannot be NULL");
    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }

    stat_sort_i(data, count);
    stat_float_t med = stat_median_i(data, count);

    stat_float_t* deviations = malloc(count * sizeof(stat_float_t));
    if (!deviations) {
        errno = ENOMEM;
        return NAN;
    }

    for (stat_size_t i = 0; i < count; i++) {
        deviations[i] = stat_abs_scalar_f((stat_float_t)data[i] - med);
    }

    stat_sort_f(deviations, count);
    stat_float_t mad = stat_median_f(deviations, count);

    free(deviations);
    return mad * scale;
}

stat_float_t stat_cv_i32(const stat_int_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");
    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }

    stat_float_t mean = stat_mean_i(data, count);
    if (mean == 0.0f) {
        errno = EINVAL;
        return NAN;
    }

    stat_float_t stddev = sqrtf(stat_variance_i(data, count, true));
    return stddev / mean;
}
