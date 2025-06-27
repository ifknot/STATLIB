/**
 * @file stat_sign.h
 * @brief Sign manipulation functions for floating-point and integer values
 * 
 * Provides functions for extracting and manipulating the sign of numerical values,
 * with consistent behavior across both floating-point and integer types.
 */
#ifndef STAT_SIGN_H
#define STAT_SIGN_H

#include "stat_types.h"

/**
 * @brief Determines the sign of a floating-point value
 * @param value The input value to check
 * @return -1 if value is negative, 
 *          1 if value is positive, 
 *          0 if value is zero or NaN
 * @note Returns 0 for NaN values to maintain consistency with integer version
 */
int stat_sign_f(stat_float_t value);

/**
 * @brief Determines the sign of an integer value
 * @param value The input value to check
 * @return -1 if value is negative, 
 *          1 if value is positive, 
 *          0 if value is zero
 */
int stat_sign_i(int32_t value);

/**
 * @brief Copies the sign from one float to another
 * @param magnitude The value whose magnitude to use
 * @param sign The value whose sign to use
 * @return A value with magnitude from first parameter and sign from second
 * @note Handles NaN values consistently with standard copysign implementations
 * @see copysign() in math.h
 */
stat_float_t stat_copysign_f(stat_float_t magnitude, stat_float_t sign);

#endif // STAT_SIGN_H
