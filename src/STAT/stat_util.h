#ifndef STAT_UTIL_H
#define STAT_UTIL_H

#include "stat_types.h"

void stat_sort_f(stat_float_t* data, stat_size_t size);
void stat_sort_i(stat_int_t* data, stat_size_t size);
bool stat_is_finite(stat_float_t value);
bool stat_is_normal(stat_float_t value);

#endif // STAT_UTIL_H
