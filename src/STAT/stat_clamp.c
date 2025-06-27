#include "stat_clamp.h"
#include <math.h>

stat_float_t* stat_clamp_float_array(
    stat_float_t* destination,
    const stat_float_t* source,
    stat_size_t count,
    stat_float_t min,
    stat_float_t max
) {
    // Irrecoverable errors (assert)
    assert(source != NULL && "Source array cannot be NULL");
    assert(destination != NULL && "Destination array cannot be NULL");

    // Recoverable errors (errno)
    if (count == 0 || min > max) {
        errno = EINVAL;
        return destination;
    }

    for (stat_size_t i = 0; i < count; i++) {
        if (isnan(source[i])) {
            errno = EDOM;
            return destination;
        }
        destination[i] = stat_clamp_float(source[i], min, max);
    }
    return destination;
}

stat_int_t* stat_clamp_int_array(
    stat_int_t* destination,
    const stat_int_t* source,
    stat_size_t count,
    stat_int_t min,
    stat_int_t max
) {
    // Irrecoverable errors (assert)
    assert(source != NULL && "Source array cannot be NULL");
    assert(destination != NULL && "Destination array cannot be NULL");

    // Recoverable errors (errno)
    if (count == 0 || min > max) {
        errno = EINVAL;
        return destination;
    }

    for (stat_size_t i = 0; i < count; i++) {
        destination[i] = stat_clamp_int(source[i], min, max);
    }
    return destination;
}
