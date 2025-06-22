#ifndef STAT_ROUND_H
#define STAT_ROUND_H

#include "stat_types.h"

// Watcom C doesn't have the lround() math function in its standard library!
#ifdef __WATCOMC__
#define stat_lround(x) ((int32_t)((x) >= 0 ? (x) + 0.5 : (x) - 0.5))
#else
#define stat_lround(x) lround(x)
#endif

stat_int_t stat_round_to_i(stat_float_t value);
stat_int_t stat_floor_to_i(stat_float_t value);
stat_int_t stat_ceil_to_i(stat_float_t value);
stat_int_t stat_trunc_to_i(stat_float_t value);

stat_float_t stat_round_to_multiple(stat_float_t value, stat_float_t multiple);
stat_float_t stat_round_decimal(stat_float_t value, int decimals);

bool stat_safe_round_to_i(stat_float_t value, stat_int_t* result);

stat_float_t* stat_cast_int_to_float_array(stat_float_t* destination, stat_int_t* source);

// put the rest of the array versions of the float versions here


#endif // STAT_ROUND_H
