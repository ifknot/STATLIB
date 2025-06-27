#ifndef STAT_TYPES_H
#define STAT_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>

/**
 * @file stat_types.h
 * @brief Core type definitions for statistics library
 *
 * Defines the fundamental types, constants, and structures used throughout
 * the statistics library, ensuring consistent numeric handling.
 */

/** Maximum size value for array operations */
#define STAT_SIZE_MAX UINT32_MAX

/** Type for array sizes and indices */
typedef uint32_t stat_size_t;

/** Standard signed integer type used throughout the library */
typedef int32_t stat_int_t;

/** Standard floating-point type used throughout the library */
typedef double stat_float_t;

/**
 * @brief Enumeration of quartile positions
 * @note Used for percentile calculation functions
 */
typedef enum {
    STAT_Q1 = 0,     /**< First quartile (25th percentile) */
    STAT_MEDIAN = 1, /**< Second quartile/median (50th percentile) */
    STAT_Q3 = 2      /**< Third quartile (75th percentile) */
} stat_quartile_t;

/**
 * @brief Five-number summary of a dataset
 * @details Contains the key quartiles and range metrics used in
 *          box plots and outlier detection.
 */
typedef struct {
    stat_float_t min;          /**< Minimum observed value */
    stat_float_t q1;           /**< First quartile (25th percentile) */
    stat_float_t median;       /**< Median value (50th percentile) */
    stat_float_t q3;           /**< Third quartile (75th percentile) */
    stat_float_t max;          /**< Maximum observed value */
    stat_float_t iqr;          /**< Interquartile range (Q3 - Q1) */
    stat_float_t lower_fence;  /**< Lower boundary for outlier detection */
    stat_float_t upper_fence;  /**< Upper boundary for outlier detection */
} stat_five_num_summary_t;

/**
 * @brief Comprehensive descriptive statistics
 * @details Contains all commonly used measures of central tendency,
 *          dispersion, and shape for a dataset.
 */
typedef struct {
    stat_float_t mean;         /**< Arithmetic mean */
    stat_float_t median;       /**< Median value */
    stat_float_t mode;         /**< Most frequent value */
    stat_float_t variance;     /**< Population variance */
    stat_float_t std_dev;      /**< Population standard deviation */
    stat_float_t range;        /**< Total range (max - min) */
    stat_five_num_summary_t five_num_summary; /**< Quartile breakdown */
} stat_descriptive_stats_t;

#endif // STAT_TYPES_H
