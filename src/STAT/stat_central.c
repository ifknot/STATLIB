#include "stat_central.h"
#include "stat_util.h" // for stat_sort
#include <stdlib.h>
#include <math.h>

stat_float_t stat_mean(stat_float_t* data, stat_size_t size) {
    if (size == 0) return NAN;

    stat_float_t sum = 0.0;
    for (stat_size_t i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum / size;
}

stat_float_t stat_median(stat_float_t* data, stat_size_t size) {
    if (size == 0) return NAN;

    stat_float_t* sorted = malloc(size * sizeof(stat_float_t));
    if (!sorted) return NAN;

    for (stat_size_t i = 0; i < size; i++) {
        sorted[i] = data[i];
    }
    stat_sort(sorted, size);

    stat_float_t median;
    if (size % 2 == 0) {
        median = (sorted[size/2 - 1] + sorted[size/2]) / 2.0;
    } else {
        median = sorted[size/2];
    }

    free(sorted);
    return median;
}

stat_float_t stat_mode(stat_float_t* data, stat_size_t size) {
    if (size == 0) return NAN;

    stat_sort(data, size);

    stat_float_t mode = data[0];
    stat_size_t max_count = 1, current_count = 1;

    for (stat_size_t i = 1; i < size; i++) {
        if (data[i] == data[i-1]) {
            current_count++;
        } else {
            if (current_count > max_count) {
                max_count = current_count;
                mode = data[i-1];
            }
            current_count = 1;
        }
    }

    // Check last element
    if (current_count > max_count) {
        mode = data[size-1];
    }

    return mode;
}
