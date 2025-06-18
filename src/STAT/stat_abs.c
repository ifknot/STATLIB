#include "stat_abs.h"
//#include <limits.h>
#include <errno.h>

stat_float_t stat_abs_f(stat_float_t value) {
    return value < 0 ? -value : value;
}

int32_t stat_abs_i32(int32_t value) {
    return value < 0 ? -value : value;
}

bool stat_safe_abs_i32(int32_t value, int32_t* result) {
    if (value == INT32_MIN) {
        errno = ERANGE;
        return false;
    }
    *result = stat_abs_i32(value);
    return true;
}

uint32_t stat_abs_u32(uint32_t value) {
    return value < 0 ? -value : value;
}
