#ifndef STAT_BASIC_H
#define STAT_BASIC_H

#include "stat_types.h"

// Float versions
stat_float_t stat_min_f(stat_float_t* data, stat_size_t size);
stat_float_t stat_max_f(stat_float_t* data, stat_size_t size);
stat_float_t stat_range_f(stat_float_t* data, stat_size_t size);

// int32 versions
int32_t stat_min_i32(int32_t* data, stat_size_t size);
int32_t stat_max_i32(int32_t* data, stat_size_t size);
int32_t stat_range_i32(int32_t* data, stat_size_t size);


// Unsigned uint32 ops
uint32_t stat_min_u32(uint32_t* data, stat_size_t size);
uint32_t stat_max_u32(uint32_t* data, stat_size_t size);
uint32_t stat_range_u32(uint32_t* data, stat_size_t size);

#endif // STAT_BASIC_H
