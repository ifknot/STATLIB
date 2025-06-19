#ifndef STAT_COMPARE_H
#define STAT_COMPARE_H

#include "stat_types.h"

int stat_compare_floats(stat_float_t a, stat_float_t b, stat_float_t epsilon);
bool stat_almost_equal(stat_float_t a, stat_float_t b, stat_float_t rel_epsilon, stat_float_t abs_epsilon);
bool stat_is_near_zero(stat_float_t value, stat_float_t threshold);

// Qsort-compatible wrapper
int stat_compare_floats_qsort(const void* a, const void* b);

int stat_compare_ints_qsort(const void* a, const void* b);

#endif // STAT_COMPARE_H
