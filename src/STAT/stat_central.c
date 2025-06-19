#include "stat_central.h"
#include "stat_util.h"
//#include "stat_abs.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <errno.h>

// Helper function for mode calculation
static void private_find_modes(const stat_float_t* sorted, stat_size_t count,
                       stat_float_t* modes, stat_size_t* mode_count) {
    stat_size_t max_count = 1, current_count = 1;
    *mode_count = 0;

    // First pass: find max frequency
    for (stat_size_t i = 1; i < count; i++) {
        if (sorted[i] == sorted[i-1]) {
            current_count++;
            if (current_count > max_count) {
                max_count = current_count;
            }
        } else {
            current_count = 1;
        }
    }

    // Second pass: collect all values with max frequency
    current_count = 1;
    for (stat_size_t i = 1; i < count; i++) {
        if (sorted[i] == sorted[i-1]) {
            current_count++;
        } else {
            if (current_count == max_count) {
                modes[(*mode_count)++] = sorted[i-1];
            }
            current_count = 1;
        }
    }
    // Check last element
    if (current_count == max_count) {
        modes[(*mode_count)++] = sorted[count-1];
    }
}

stat_float_t stat_mean_f(const stat_float_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }

    stat_float_t sum = 0.0;
    for (stat_size_t i = 0; i < count; i++) {
        if (isnan(data[i])) {
            errno = EDOM;
            return NAN;
        }
        sum += data[i];
    }
    return sum / count;
}

stat_float_t stat_median_f(const stat_float_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }

    // Create sorted copy (preserve input)
    stat_float_t* sorted = malloc(count * sizeof(stat_float_t));
    if (!sorted) {
        errno = ENOMEM;
        return NAN;
    }
    memcpy(sorted, data, count * sizeof(stat_float_t));
    stat_sort_f(sorted, count);

    stat_float_t result;
    if (count % 2 == 1) {
        result = sorted[count / 2];
    } else {
        result = (sorted[count/2 - 1] + sorted[count/2]) / 2.0f;
    }

    free(sorted);
    return result;
}

bool stat_mode_f(const stat_float_t* data, stat_size_t count, stat_float_t* modes, stat_size_t* mode_count) {
    assert(data != NULL && "Input array cannot be NULL");
    assert(modes != NULL && "Output array cannot be NULL");
    assert(mode_count != NULL && "Mode count pointer cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return false;
    }

    // Create sorted copy
    stat_float_t* sorted = malloc(count * sizeof(stat_float_t));
    if (!sorted) {
        errno = ENOMEM;
        return false;
    }
    memcpy(sorted, data, count * sizeof(stat_float_t));
    stat_sort_f(sorted, count);

    // Check for NaN
    for (stat_size_t i = 0; i < count; i++) {
        if (isnan(sorted[i])) {
            free(sorted);
            errno = EDOM;
            return false;
        }
    }

    private_find_modes(sorted, count, modes, mode_count);
    free(sorted);
    return true;
}

// Integer versions

stat_float_t stat_mean_i(const stat_int_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }

    int64_t sum = 0; // Use 64-bit to prevent overflow
    for (stat_size_t i = 0; i < count; i++) {
        sum += data[i];
    }
    return (stat_float_t)sum / count;
}

stat_float_t stat_median_i(const stat_int_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }

    // Create sorted copy
    stat_int_t* sorted = malloc(count * sizeof(stat_int_t));
    if (!sorted) {
        errno = ENOMEM;
        return NAN;
    }
    memcpy(sorted, data, count * sizeof(stat_int_t));
    stat_sort_i(sorted, count);

    stat_float_t result;
    if (count % 2 == 1) {
        result = (stat_float_t)sorted[count / 2];
    } else {
        result = ((stat_float_t)sorted[count/2 - 1] + sorted[count/2]) / 2.0f;
    }

    free(sorted);
    return result;
}

bool stat_mode_i(const stat_int_t* data, stat_size_t count,
                 stat_int_t* modes, stat_size_t* mode_count) {
    assert(data != NULL && "Input array cannot be NULL");
    assert(modes != NULL && "Output array cannot be NULL");
    assert(mode_count != NULL && "Mode count pointer cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return false;
    }

    // Create sorted copy
    stat_int_t* sorted = malloc(count * sizeof(stat_int_t));
    if (!sorted) {
        errno = ENOMEM;
        return false;
    }
    memcpy(sorted, data, count * sizeof(stat_int_t));
    stat_sort_i(sorted, count);

    // Reuse float mode logic with type conversion
    stat_float_t* float_modes = malloc(count * sizeof(stat_float_t));
    if (!float_modes) {
        free(sorted);
        errno = ENOMEM;
        return false;
    }

    private_find_modes((stat_float_t*)sorted, count, float_modes, mode_count);

    // Convert back to int32
    for (stat_size_t i = 0; i < *mode_count; i++) {
        modes[i] = (stat_int_t)float_modes[i];
    }

    free(sorted);
    free(float_modes);
    return true;
}
