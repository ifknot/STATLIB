#include "stat_clamp.h"

stat_float_t stat_clamp_f(stat_float_t value, stat_float_t min, stat_float_t max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int32_t stat_clamp_i32(int32_t value, int32_t min, int32_t max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

bool stat_clamp_array(stat_float_t* array, stat_size_t size, stat_float_t min, stat_float_t max) {
    if (!array || size == 0) return false;
    for (stat_size_t i = 0; i < size; i++) {
        array[i] = stat_clamp_f(array[i], min, max);
    }
    return true;
}
