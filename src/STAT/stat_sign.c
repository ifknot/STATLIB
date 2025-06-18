#include "stat_sign.h"
#include <math.h>

int stat_sign_f(stat_float_t value) {
    if (value > 0.0) return 1;
    if (value < 0.0) return -1;
    return 0;
}

int stat_sign_i32(int32_t value) {
    return (value > 0) - (value < 0);
}

stat_float_t stat_copysign_f(stat_float_t magnitude, stat_float_t sign) {
    return copysign(magnitude, sign);
}
