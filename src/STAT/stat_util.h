#ifndef STAT_UTIL_H
#define STAT_UTIL_H

#include "stat_types.h"

//int stat_compare_floats(const void* a, const void* b);
void stat_sort(stat_float_t* data, stat_size_t size); // Use the qsort-specific version here
bool stat_is_finite(stat_float_t value);
bool stat_is_normal(stat_float_t value);

#endif // STAT_UTIL_H
