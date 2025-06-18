#ifndef STAT_DIVISION_H
#define STAT_DIVISION_H

#include "stat_types.h"

bool stat_safe_div_i32(int32_t numerator, int32_t denominator, int32_t* result);
int32_t stat_div_round_up(int32_t dividend, int32_t divisor);
int32_t stat_div_round_nearest(int32_t dividend, int32_t divisor);

#endif // STAT_DIVISION_H
