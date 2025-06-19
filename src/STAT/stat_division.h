#ifndef STAT_DIVISION_H
#define STAT_DIVISION_H

#include "stat_types.h"
#include <stdbool.h>

/**
 * @brief Safe division for floats (returns NAN on divide-by-zero)
 * @param[in] numerator Dividend
 * @param[in] denominator Divisor
 * @return numerator/denominator or NAN
 * @note Sets errno=EDOM if denominator=0
 */
stat_float_t stat_safe_div_f(stat_float_t numerator, stat_float_t denominator);

/**
 * @brief Safe division for integers (avoids overflow/underflow)
 * @param[in] numerator Dividend
 * @param[in] denominator Divisor
 * @param[out] result Pointer to store result
 * @return True if successful, False on error
 * @throws ERANGE if result would overflow, EDOM if denominator=0
 * @assert Fails if result pointer is NULL
 */
bool stat_safe_div_i(stat_int_t numerator, stat_int_t denominator, stat_int_t* result);

/**
 * @brief Integer division with remainder
 * @param[in] numerator Dividend
 * @param[in] denominator Divisor
 * @param[out] quotient Result of division
 * @param[out] remainder Remainder
 * @return True if successful, False on error
 * @throws EDOM if denominator=0
 * @assert Fails if quotient or remainder pointers are NULL
 */
bool stat_divmod_i(stat_int_t numerator, stat_int_t denominator, stat_int_t* quotient, stat_int_t* remainder);

#endif // STAT_DIVISION_H
