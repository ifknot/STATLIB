#include "stat_basic.h"
#include "stat_types.h"
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stddef.h>

stat_float_t stat_min_float_array(const stat_float_t* source, stat_size_t count) {
    assert(source != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }

    stat_float_t min = source[0];
    for (stat_size_t i = 1; i < count; i++) {
        if (source[i] < min) min = source[i];
    }
    return min;
}

stat_float_t stat_max_float_array(const stat_float_t* source, stat_size_t count) {
    assert(source != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }

    stat_float_t max = source[0];
    for (stat_size_t i = 1; i < count; i++) {
        if (source[i] > max) max = source[i];
    }
    return max;
}

stat_float_t stat_range_float_array(const stat_float_t* source, stat_size_t count) {
    assert(source != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return NAN;
    }
    return stat_max_float_array(source, count) - stat_min_float_array(source, count);
}

stat_int_t stat_max_int_array(const stat_int_t* source, stat_size_t count) {
    assert(source != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return INT32_MIN;
    }

    stat_int_t max = source[0];
    for (stat_size_t i = 1; i < count; i++) {
        if (source[i] > max) max = source[i];
    }
    return max;
}

stat_int_t stat_min_int_array(const stat_int_t* source, stat_size_t count) {
    assert(source != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return INT32_MAX;
    }

    stat_int_t min = source[0];
    for (stat_size_t i = 1; i < count; i++) {
        if (source[i] < min) min = source[i];
    }
    return min;
}

stat_int_t stat_range_int_array(const stat_int_t* source, stat_size_t count) {
    assert(source != NULL && "Input array cannot be NULL");

    if (count == 0) {
        errno = EINVAL;
        return INT32_MIN;
    }
    return stat_max_int_array(source, count) - stat_min_int_array(source, count);
}

stat_int_t* stat_cast_float_to_int_array(stat_int_t* destination, const stat_float_t* source, stat_size_t count) {
    assert(destination != NULL && source != NULL && "Arrays cannot be NULL");

    for (stat_size_t i = 0; i < count; i++) {
        destination[i] = (stat_int_t)source[i];
    }
    return destination;
}

stat_float_t* stat_cast_int_to_float_array(stat_float_t* destination, const stat_int_t* source, stat_size_t count) {
    assert(destination != NULL && source != NULL && "Arrays cannot be NULL");

    for (stat_size_t i = 0; i < count; i++) {
        destination[i] = (stat_float_t)source[i];
    }
    return destination;
}
