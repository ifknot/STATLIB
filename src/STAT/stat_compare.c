#include "stat_compare.h"
#include "stat_constants.h"
#include "stat_types.h"
#include <math.h>

int stat_compare_floats(stat_float_t a, stat_float_t b, stat_float_t epsilon) {
    if (fabs(a - b) <= epsilon) return 0;
    return (a < b) ? -1 : 1;
}

bool stat_almost_equal(stat_float_t a, stat_float_t b, stat_float_t rel_epsilon, stat_float_t abs_epsilon) {
    if (a == b) return true;
    stat_float_t diff = fabs(a - b);
    if (diff <= abs_epsilon) return true;
    return diff <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * rel_epsilon );
}

bool stat_is_near_zero(stat_float_t value, stat_float_t threshold) {
    return fabs(value) <= threshold;
}

int stat_compare_floats_qsort(const void* a, const void* b) {
    stat_float_t fa = *(const stat_float_t*)a;
    stat_float_t fb = *(const stat_float_t*)b;
    return stat_compare_floats(fa, fb, STAT_EPSILON);
}
