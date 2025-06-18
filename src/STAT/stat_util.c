#include "stat_util.h"
#include "stat_compare.h"
#include <math.h>
#include <stdlib.h>

void stat_sort(stat_float_t* data, stat_size_t size) {
    qsort(data, size, sizeof(stat_float_t), stat_compare_floats_qsort);
}

bool stat_is_finite(stat_float_t value) {
    return isfinite(value);
}

bool stat_is_normal(stat_float_t value) {
    return isnormal(value);
}
