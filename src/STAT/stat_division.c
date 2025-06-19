#include "stat_division.h"
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stddef.h>

stat_float_t stat_safe_div_f(stat_float_t numerator, stat_float_t denominator) {
    if (denominator == 0.0f) {
        errno = EDOM;
        return NAN;
    }
    return numerator / denominator;
}

bool stat_safe_div_i(stat_int_t numerator, stat_int_t denominator,
                      stat_int_t* result) {
    assert(result != NULL && "Result pointer cannot be NULL");

    if (denominator == 0) {
        errno = EDOM;
        return false;
    }

    // Handle INT_MIN / -1 special case
    if (numerator == INT32_MIN && denominator == -1) {
        errno = ERANGE;
        return false;
    }

    *result = numerator / denominator;
    return true;
}

bool stat_divmod_i32(stat_int_t numerator, stat_int_t denominator, stat_int_t* quotient, stat_int_t* remainder) {
    assert(quotient != NULL && "Quotient pointer cannot be NULL");
    assert(remainder != NULL && "Remainder pointer cannot be NULL");

    if (denominator == 0) {
        errno = EDOM;
        return false;
    }

    // Handle INT_MIN / -1 special case
    if (numerator == INT32_MIN && denominator == -1) {
        errno = ERANGE;
        return false;
    }

    *quotient = numerator / denominator;
    *remainder = numerator % denominator;
    return true;
}
