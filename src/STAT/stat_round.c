#include "stat_round.h"
#include <math.h>
#include <errno.h>

int32_t stat_round_to_i32(stat_float_t value) {
    return stat_lround(value); // Watcom!!!
}

int32_t stat_floor_to_i32(stat_float_t value) {
    return (int32_t)floor(value);
}

int32_t stat_ceil_to_i32(stat_float_t value) {
    return (int32_t)ceil(value);
}

int32_t stat_trunc_to_i32(stat_float_t value) {
    return (int32_t)trunc(value);
}

stat_float_t stat_round_to_multiple(stat_float_t value, stat_float_t multiple) {
    if (multiple == 0.0) return value;
    return round(value / multiple) * multiple;
}

stat_float_t stat_round_decimal(stat_float_t value, int decimals) {
    const stat_float_t factor = pow(10.0, decimals);
    return round(value * factor) / factor;
}

bool stat_safe_round_to_i32(stat_float_t value, int32_t* result) {
    if (!isfinite(value)) goto overflow;
    if (value >= (stat_float_t)INT32_MAX + 0.5f) goto overflow;
    if (value <= (stat_float_t)INT32_MIN - 0.5f) goto overflow;

    *result = stat_round_to_i32(value);
    return true;

overflow:
    errno = ERANGE;
    return false;
}
