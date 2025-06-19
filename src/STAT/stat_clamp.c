#include "stat_clamp.h"

#include <math.h>
#include <stddef.h>

stat_float_t* stat_clamp_f(
    const stat_float_t* src,
    stat_float_t* dst,
    stat_size_t count,
    stat_float_t min,
    stat_float_t max
) {
    // Irrecoverable errors (assert)
    assert(src != NULL && "Source array cannot be NULL");
    assert(dst != NULL && "Destination array cannot be NULL");

    // Recoverable errors (errno)
    if (count == 0 || min > max) {
        errno = EINVAL;
        return dst;
    }

    for (stat_size_t i = 0; i < count; i++) {
        if (isnan(src[i])) {
            errno = EDOM;
            return dst;
        }
        dst[i] = (src[i] < min) ? min : ((src[i] > max) ? max : src[i]);
    }
    return dst;
}

stat_int_t* stat_clamp_i32(
    const stat_int_t* src,
    stat_int_t* dst,
    stat_size_t count,
    stat_int_t min,
    stat_int_t max
) {
    // Irrecoverable errors (assert)
    assert(src != NULL && "Source array cannot be NULL");
    assert(dst != NULL && "Destination array cannot be NULL");

    // Recoverable errors (errno)
    if (count == 0 || min > max) {
        errno = EINVAL;
        return dst;
    }

    for (stat_size_t i = 0; i < count; i++) {
        dst[i] = (src[i] < min) ? min : ((src[i] > max) ? max : src[i]);
    }
    return dst;
}
