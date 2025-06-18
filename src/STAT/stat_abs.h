#ifndef STAT_ABS_H
#define STAT_ABS_H

#include "stat_types.h"


// Floating-point absolute value
stat_float_t stat_abs_f(stat_float_t value);

// Signed 32-bit absolute value
int32_t stat_abs_i32(int32_t value);
bool stat_safe_abs_i32(int32_t value, int32_t* result);

// Unsigned 32-bit absolute value (identity function)
uint32_t stat_abs_u32(uint32_t value);

#endif // STAT_ABS_H
