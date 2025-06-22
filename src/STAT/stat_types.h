#ifndef STAT_TYPES_H
#define STAT_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <float.h>

#define STAT_SIZE_MAX UINT32_MAX
typedef uint32_t stat_size_t;
typedef int32_t stat_int_t;
typedef double stat_float_t;

typedef enum {
    STAT_Q1 = 0,     // 25th percentile
    STAT_MEDIAN = 1,  // 50th percentile
    STAT_Q3 = 2       // 75th percentile
} stat_quartile_t;

// Five-number summary
typedef struct {
    stat_float_t min;
    stat_float_t q1;
    stat_float_t median;
    stat_float_t q3;
    stat_float_t max;
    stat_float_t iqr;
    stat_float_t lower_fence;
    stat_float_t upper_fence;
} stat_five_num_summary_t;

// Descriptive statistics
typedef struct {
    stat_float_t mean;
    stat_float_t median;
    stat_float_t mode;
    stat_float_t variance;
    stat_float_t std_dev;
    stat_float_t range;
    stat_five_num_summary_t five_num_summary;
} stat_descriptive_stats_t;

#endif // STAT_TYPES_H
