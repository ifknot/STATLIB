#include "stat_basic.h"

#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>

stat_float_t stat_min_f(const stat_float_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }

    stat_float_t min = data[0];
    for (stat_size_t i = 1; i < count; i++) {
        if (data[i] < min) min = data[i];
    }
    return min;
}

stat_float_t stat_max_f(const stat_float_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }

    stat_float_t max = data[0];
    for (stat_size_t i = 1; i < count; i++) {
        if (data[i] > max) max = data[i];
    }
    return max;
}

stat_float_t stat_range_f(const stat_float_t* data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }
    return stat_max_f(data, count) - stat_min_f(data, count);
}

stat_int_t stat_max_i(const stat_int_t* values, stat_size_t count) {
    assert(values != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return INT32_MIN;
    }

    stat_int_t max = values[0];
    for (stat_size_t i = 1; i < count; i++) {
        if (values[i] > max) max = values[i];
    }
    return max;
}

stat_int_t stat_min_i(const stat_int_t* values, stat_size_t count) {
    assert(values != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return INT32_MIN;
    }

    stat_int_t min = values[0];
    for (stat_size_t i = 1; i < count; i++) {
        if (values[i] < min) min = values[i];
    }
    return min;
}

stat_int_t stat_range_i(const stat_int_t *data, stat_size_t count) {
    assert(data != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return INT32_MIN;
    }
    return stat_max_i(data, count) - stat_min_i(data, count);
}
