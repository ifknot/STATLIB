#include "stat_abs.h"
#ifndef STAT_ABS_H
#define STAT_ABS_H

#include "stat_types.h"
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

void stat_abs_f(stat_float_t* values, stat_size_t count) {
    for (stat_size_t i = 0; i < count; i++) {
        values[i] = values[i] < 0 ? -values[i] : values[i];
    }
}

stat_size_t stat_abs_i32(stat_int_t* values, stat_size_t count) {
    stat_size_t successes = 0;
    for (stat_size_t i = 0; i < count; i++) {
        if (values[i] != INT32_MIN) {
            values[i] = values[i] < 0 ? -values[i] : values[i];
            successes++;
        }
    }
    return successes;
}

stat_size_t stat_safe_abs_i32(
    const stat_int_t* input, 
    stat_int_t* output, 
    stat_size_t count
) {
    stat_size_t successes = 0;
    for (stat_size_t i = 0; i < count; i++) {
        if (input[i] == INT32_MIN) {
            output[i] = input[i]; // Preserve original
            errno = ERANGE;
        } else {
            output[i] = input[i] < 0 ? -input[i] : input[i];
            successes++;
        }
    }
    return successes;
}

#endif // STAT_ABS_H
