#include "stat_division.h"
//#include <limits.h>
#include <errno.h>

bool stat_safe_div_i32(int32_t numerator, int32_t denominator, int32_t* result) {
    if (denominator == 0 ||
        (numerator == INT32_MIN && denominator == -1)) {
        errno = ERANGE;
        return false;
    }
    *result = numerator / denominator;
    return true;
}

int32_t stat_div_round_up(int32_t dividend, int32_t divisor) {
    if (divisor == 0) return 0;
    return (dividend + divisor - 1) / divisor;
}

int32_t stat_div_round_nearest(int32_t dividend, int32_t divisor) {
    if (divisor == 0) return 0;
    return (dividend + divisor/2) / divisor;
}
