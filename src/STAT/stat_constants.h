/**
 * @file stat_constants.h
 * @brief Mathematical and statistical constants
 * @note All values are defined as stat_float_t (typically double precision)
 */

#ifndef STAT_CONSTANTS_H
#define STAT_CONSTANTS_H

#include "stat_types.h"
#include <float.h>

// =============================================
// Core Mathematical Constants
// =============================================

#ifndef M_PI
#define M_PI ((stat_float_t)3.14159265358979323846)       ///< π
#endif

#ifndef M_E
#define M_E  ((stat_float_t)2.7182818284590452354)        ///< Euler's number
#endif

#define STAT_SQRT2   ((stat_float_t)1.41421356237309504880)  ///< √2
#define STAT_SQRT1_2 ((stat_float_t)0.70710678118654752440)  ///< 1/√2

// =============================================
// Floating-Point Precision Constants
// =============================================

#define STAT_EPSILON       DBL_EPSILON                   ///< Machine epsilon (~2.22e-16 for double)
#define STAT_SQRT_EPSILON  ((stat_float_t)1.4901161193847656e-8)  ///< √DBL_EPSILON
#define STAT_SAFE_EPSILON  ((stat_float_t)1e-12)         ///< General safety threshold

// =============================================
// Statistical Comparison Thresholds
// =============================================

#define STAT_ABS_TOL  ((stat_float_t)1e-12)              ///< Absolute tolerance
#define STAT_REL_TOL  ((stat_float_t)1e-8)               ///< Relative tolerance
#define STAT_SMALL_TOL ((stat_float_t)1e-30)             ///< For very small numbers

// =============================================
// Special Values
// =============================================

#define STAT_INFINITY ((stat_float_t)1.0/0.0)            ///< Positive infinity
#define STAT_NAN      ((stat_float_t)0.0/0.0)            ///< Quiet NaN

// =============================================
// Distribution Constants
// =============================================

#define STAT_GAUSS_COEF ((stat_float_t)0.3989422804014327) ///< 1/√(2π)
#define STAT_LOG2PI     ((stat_float_t)1.8378770664093456) ///< log(2π)

// =============================================
// Conversion Factors
// =============================================

#define STAT_DEG_TO_RAD (M_PI / (stat_float_t)180.0)     ///< Degrees to radians
#define STAT_RAD_TO_DEG ((stat_float_t)180.0 / M_PI)     ///< Radians to degrees

// =============================================
// Utility Macros
// =============================================

/**
 * @brief Safe comparison macro with absolute and relative tolerance
 * @param a First value
 * @param b Second value
 * @param rel_tol Relative tolerance (default STAT_REL_TOL)
 * @param abs_tol Absolute tolerance (default STAT_ABS_TOL)
 */
#define STAT_FLOAT_EQ(a, b, rel_tol, abs_tol) \
    (fabs((a) - (b)) <= ((abs_tol) + (rel_tol) * fmin(fabs(a), fabs(b)))

/**
 * @brief Default float comparison using library tolerances
 */
#define STAT_APPROX_EQ(a, b) STAT_FLOAT_EQ(a, b, STAT_REL_TOL, STAT_ABS_TOL)

#endif // STAT_CONSTANTS_H
