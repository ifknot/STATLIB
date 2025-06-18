#ifndef STAT_CLAMP_H
#define STAT_CLAMP_H

#include "stat_types.h"

stat_float_t stat_clamp_f(stat_float_t value, stat_float_t min, stat_float_t max);
int32_t stat_clamp_i32(int32_t value, int32_t min, int32_t max);
bool stat_clamp_array(stat_float_t* array, stat_size_t size, stat_float_t min, stat_float_t max);

#endif // STAT_CLAMP_H
