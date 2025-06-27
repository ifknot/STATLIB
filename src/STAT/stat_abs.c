#include "stat_abs.h"
#include "stat_IEEE754.h"
#include <assert.h>
#include <stddef.h>

stat_float_t* stat_abs_f(const stat_float_t* src, stat_float_t* dst, stat_size_t size) {
    assert(src != NULL && "Source array cannot be NULL");
    assert(dst != NULL && "Destination array cannot be NULL");

    if (size == 0) {
        errno = EINVAL;
        return dst;
    }

    for (stat_size_t i = 0; i < size; i++) {
        if (isnan(src[i])) {
            errno = EDOM;
            return dst;
        }
        dst[i] = src[i] < 0 ? -src[i] : src[i];
    }
    return dst;
}

int32_t* stat_abs_i(const int32_t* src, int32_t* dst, stat_size_t size) {
    assert(src != NULL && "Source array cannot be NULL");
    assert(dst != NULL && "Destination array cannot be NULL");

    if (!size) {
        errno = EINVAL;
        return dst;
    }

    for (stat_size_t i = 0; i <size; i++) {
        if (src[i] == INT32_MIN) {
            dst[i] = INT32_MIN; // Preserve value
            errno = ERANGE;
        } else {
            dst[i] = src[i] < 0 ? -src[i] : src[i];
        }
    }
    return dst;
}
