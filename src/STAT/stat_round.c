#include "stat_round.h"
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>

/* Scalar rounding functions */
stat_int_t stat_round_to_i(stat_float_t value) {
    return stat_lround(value);
}

stat_int_t stat_floor_to_i(stat_float_t value) {
    return (stat_int_t)floor(value);
}

stat_int_t stat_ceil_to_i(stat_float_t value) {
    return (stat_int_t)ceil(value);
}

stat_int_t stat_trunc_to_i(stat_float_t value) {
    return (stat_int_t)trunc(value);
}

stat_float_t stat_round_to_multiple(stat_float_t value, stat_float_t multiple) {
    if (multiple == 0.0) {
        errno = EDOM;
        return NAN;
    }
    return round(value / multiple) * multiple;
}

stat_float_t stat_round_decimal(stat_float_t value, int decimals) {
    const stat_float_t factor = pow(10.0, decimals);
    return round(value * factor) / factor;
}

bool stat_safe_round_to_i(stat_float_t value, stat_int_t* result) {
    if (value < INT32_MIN || value > INT32_MAX) {
        return false;
    }
    *result = stat_lround(value);
    return true;
}

stat_int_t* stat_round_float_to_int_array(stat_int_t* destination, const stat_float_t* source) {
    assert(destination != NULL && source != NULL);

    for (size_t i = 0; !isnan(source[i]); i++) {  // NAN as sentinel
        destination[i] = stat_lround(source[i]);
    }
    return destination;
}

stat_int_t* stat_floor_float_to_int_array(stat_int_t* destination, const stat_float_t* source) {
    assert(destination != NULL && source != NULL);

    for (size_t i = 0; !isnan(source[i]); i++) {
        destination[i] = (stat_int_t)floor(source[i]);
    }
    return destination;
}

stat_int_t* stat_ceil_float_to_int_array(stat_int_t* destination, const stat_float_t* source) {
    assert(destination != NULL && source != NULL);

    for (size_t i = 0; !isnan(source[i]); i++) {
        destination[i] = (stat_int_t)ceil(source[i]);
    }
    return destination;
}

stat_int_t* stat_trunc_float_to_int_array(stat_int_t* destination, const stat_float_t* source) {
    assert(destination != NULL && source != NULL);

    for (size_t i = 0; !isnan(source[i]); i++) {
        destination[i] = (stat_int_t)trunc(source[i]);
    }
    return destination;
}

stat_float_t* stat_round_float_decimal_array(stat_float_t* destination,
                                           const stat_float_t* source,
                                           int decimals) {
    assert(destination != NULL && source != NULL);

    const stat_float_t factor = pow(10.0, decimals);
    for (size_t i = 0; !isnan(source[i]); i++) {
        destination[i] = round(source[i] * factor) / factor;
    }
    return destination;
}

stat_float_t* stat_round_float_to_multiple_array(stat_float_t* destination,
                                              const stat_float_t* source,
                                              stat_float_t multiple) {
    assert(destination != NULL && source != NULL);

    if (multiple == 0.0) {
        errno = EDOM;
        return destination;
    }

    for (size_t i = 0; !isnan(source[i]); i++) {
        destination[i] = stat_round_to_multiple(source[i], multiple);
    }
    return destination;
}

stat_int_t* stat_safe_float_to_int_array(stat_int_t* destination, const stat_float_t* source, stat_size_t* success_count) {
    assert(destination != NULL && source != NULL && success_count != NULL);

    *success_count = 0;
    for (size_t i = 0; !isnan(source[i]); i++) {
        if (source[i] >= INT32_MIN && source[i] <= INT32_MAX) {
            destination[i] = stat_lround(source[i]);
            (*success_count)++;
        } else {
            destination[i] = 0;
        }
    }
    return destination;
}
