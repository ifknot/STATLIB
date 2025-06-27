/**
 * @file stat_ieee754.h
 * @brief IEEE 754 floating-point special value definitions
 *
 * Provides portable definitions for NaN, infinity, and related constants
 * using explicit IEEE 754 bit patterns. Works on all compliant systems.
 */
#ifndef STAT_IEEE754_H
#define STAT_IEEE754_H

#include <stdint.h>

/**
 * @brief Union for type-punning between float and its bit representation
 * @warning Usage must comply with strict aliasing rules
 */
typedef union {
    float f;        ///< Floating-point representation
    uint32_t u;     ///< Raw 32-bit unsigned integer representation
} stat_float_bits;

/* -------------------------------
 * IEEE 754 Single-Precision Constants
 * -------------------------------
 *
 * IEEE 754 binary32 format (float):
 *   [1 sign bit | 8 exponent bits | 23 fraction bits]
 *
 * Special values:
 * - Exponent = 0xFF (all 1s)
 *   - Fraction = 0:      ±Infinity
 *   - Fraction != 0:     ±NaN
 * - Exponent = 0x00:     ±Zero or subnormal numbers
 */

/**
 * @brief Positive infinity constant (0x7F800000)
 * @details Bit pattern:
 *   - Sign: 0 (positive)
 *   - Exponent: 0xFF (all 1s)
 *   - Fraction: 0 (all 0s)
 */
#define STAT_POS_INF (*(float*)(uint32_t)0x7F800000)

/**
 * @brief Negative infinity constant (0xFF800000)
 * @details Bit pattern:
 *   - Sign: 1 (negative)
 *   - Exponent: 0xFF (all 1s)
 *   - Fraction: 0 (all 0s)
 */
#define STAT_NEG_INF (*(float*)(uint32_t)0xFF800000)

/**
 * @brief Canonical quiet NaN (0x7FC00000)
 * @details Bit pattern:
 *   - Sign: 0 (positive)
 *   - Exponent: 0xFF (all 1s)
 *   - Fraction: 0x400000 (quiet NaN flag)
 * @note Quiet NaNs propagate through operations without signaling
 */
#define STAT_POS_NAN (*(float*)(uint32_t)0x7FC00000)

/**
 * @brief Negative quiet NaN (0xFFC00000)
 * @details Bit pattern:
 *   - Sign: 1 (negative)
 *   - Exponent: 0xFF (all 1s)
 *   - Fraction: 0x400000 (quiet NaN flag)
 */
#define STAT_NEG_NAN (*(float*)(uint32_t)0xFFC00000)

/* -------------------------------
 * Detection Macros
 * -------------------------------
 */

/**
 * @brief Tests if value is NaN (Not a Number)
 * @param x Floating-point value to test
 * @return true if x is NaN, false otherwise
 * @note Relies on IEEE 754 property that NaN != NaN
 */
#define stat_isnan(x) ((x) == STAT_POS_NAN)

/**
 * @brief Tests if value is infinity
 * @param x Floating-point value to test
 * @return true if x is ±Infinity, false otherwise
 */
#define stat_isinf(x) (((x) == STAT_POS_INF) || ((x) == STAT_NEG_INF))

#endif /* STAT_IEEE754_H */
