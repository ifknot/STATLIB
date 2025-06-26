#include "../TDD/tdd_macros.h"

#include "stat_abs.h"
#include "stat_basic.h"
#include "stat_central.h"
#include "stat_clamp.h"
#include "stat_compare.h"
#include "stat_constants.h"

#include <math.h>
#include <limits.h>
#include <stdlib.h> // for abs()
#include <errno.h>
#include <float.h>

#define NAN (0.0 / 0.0)
#define INFINITY (1.0 / 0.0)
#define NEG_INFINITY (-1.0 / 0.0)

#define STATS_TEST_ABS &test_stat_abs_f_basic, \
    &test_stat_abs_f_inplace, \
    &test_stat_abs_f_nan, \
    &test_stat_abs_f_infinity, \
    &test_stat_abs_f_empty, \
    &test_stat_abs_f_large, \
    &test_stat_abs_i_basic, \
    &test_stat_abs_i_int32_min, \
    &test_stat_abs_i_empty, \
    &test_stat_abs_i_large, \
    &test_stat_abs_scalar_f, \
    &test_stat_abs_scalar_f_nan, \
    &test_stat_abs_scalar_f_bulk, \
    &test_stat_abs_scalar_i32, \
    &test_stat_abs_scalar_i32_range, \
    &test_stat_abs_scalar_i32_bulk


#define STATS_TEST_BASIC &test_stat_min_f_normal, \
    &test_stat_min_f_nan_handling, \
    &test_stat_min_f_empty, \
    &test_stat_min_f_large, \
    &test_stat_max_f_normal, \
    &test_stat_max_f_infinity, \
    &test_stat_range_f_known, \
    &test_stat_range_f_precision, \
    &test_stat_min_scalar_f_edge, \
    &test_stat_max_scalar_f_edge

#define STATS_TEST_CENTRAL &test_stat_mean_f_basic, \
    &test_stat_mean_f_empty, \
    &test_stat_mean_f_large, \
    &test_stat_median_f_odd, \
    &test_stat_median_f_even, \
    &test_stat_mode_f_unimodal, \
    &test_stat_mode_f_bimodal, \
    &test_stat_mean_i_basic, \
    &test_stat_mode_i_no_modes

#define STATS_TEST_CLAMP &test_stat_clamp_f_basic, \
    &test_stat_clamp_f_empty, \
    &test_stat_clamp_f_invalid_range, \
    &test_stat_clamp_i32_basic, \
    &test_stat_clamp_scalar_f_edge, \
    &test_stat_clamp_scalar_i32_edge, \
    &test_stat_clamp_f_large

#define STATS_TEST_COMPARE &test_stat_compare_floats_basic, \
    &test_stat_almost_equal, \
    &test_stat_is_near_zero, \
    &test_stat_compare_floats_qsort, \
    &test_stat_compare_ints_qsort, \
    &test_stat_compare_infinity, \
    &test_stat_almost_equal_extreme

#define STATS_TEST_BINNING &test_binning_linear_edges, \
        &test_binning_logarithmic_edges,    \
        &test_bin_integer_values,            \
        &test_bin_float_values,            \
        &test_auto_bin_percentile,            \
        &test_bin_center_width,            \
        &test_binning_with_rounding,        \
        &test_percentile_binning_with_rounding,    \
        &test_bin_center_with_rounding,        \
        &test_edge_cases

// ============================================================================
// TEST: stat_abs_f (stat_float_t* src, stat_float_t* dst, stat_size_t size)
// ============================================================================
TEST(test_stat_abs_f_basic) {
    stat_float_t src[] = {-1.5, 2.0, -3.14, 0.0};
    stat_float_t dst[4];

    stat_abs_f(src, dst, 4);
    EXPECT_EQ(dst[0], 1.5);
    EXPECT_EQ(dst[1], 2.0);
    EXPECT_EQ(dst[2], 3.14);
    EXPECT_EQ(dst[3], 0.0);
}

TEST(test_stat_abs_f_inplace) {
    stat_float_t data[] = {-1.0, -2.0, -3.0};
    stat_abs_f(data, data, 3);
    EXPECT_EQ(data[0], 1.0);
    EXPECT_EQ(data[1], 2.0);
    EXPECT_EQ(data[2], 3.0);
}

TEST(test_stat_abs_f_nan) {
    stat_float_t src[] = {0.0/0.0};
    stat_float_t dst[1];

    errno = 0;
    stat_abs_f(src, dst, 1);
    EXPECT_NEQ(dst[0], dst[0]); // NaN check
    EXPECT_EQ(errno, EDOM);
}

TEST(test_stat_abs_f_infinity) {
    stat_float_t src[] = {0};
    stat_float_t dst[1];

    stat_abs_f(src, dst, 1);
    EXPECT_EQ(dst[0], INFINITY);
}

TEST(test_stat_abs_f_empty) {
    errno = 0;
    stat_float_t dummy;
    stat_abs_f(&dummy, &dummy, 0);
    EXPECT_EQ(errno, EINVAL);
}

TEST(test_stat_abs_f_large) {
    stat_float_t big[100];
    stat_float_t out[100];
    for (int i = 0; i < 100; i++) {
        big[i] = (i % 2) ? -i*0.1 : i*0.1;
    }

    stat_abs_f(big, out, 100);
    EXPECT_EQ(out[99], 9.9);
}

// ============================================================================
// TEST: stat_abs_i (int32_t* src, int32_t* dst, stat_size_t size)
// ============================================================================
TEST(test_stat_abs_i_basic) {
    int32_t src[] = {-1, 2, -3, 0};
    int32_t dst[4];

    stat_abs_i(src, dst, 4);
    EXPECT_EQ(dst[0], 1);
    EXPECT_EQ(dst[1], 2);
    EXPECT_EQ(dst[2], 3);
    EXPECT_EQ(dst[3], 0);
}

TEST(test_stat_abs_i_int32_min) {
    int32_t src[] = {INT32_MIN};
    int32_t dst[1];

    errno = 0;
    stat_abs_i(src, dst, 1);
    EXPECT_EQ(errno, ERANGE);
}

TEST(test_stat_abs_i_empty) {
    errno = 0;
    int32_t dummy;
    stat_abs_i(&dummy, &dummy, 0);
    EXPECT_EQ(errno, EINVAL);
}

TEST(test_stat_abs_i_large) {
    int32_t big[100];
    int32_t out[100];
    for (int i = 0; i < 100; i++) {
        big[i] = (i % 2) ? -i : i;
    }

    stat_abs_i(big, out, 100);
    EXPECT_EQ(out[99], 99);
}

// ============================================================================
// TEST: stat_abs_scalar_f (stat_float_t x)
// ============================================================================
TEST(test_stat_abs_scalar_f) {
    EXPECT_EQ(stat_abs_scalar_f(-2.5), 2.5);
    EXPECT_EQ(stat_abs_scalar_f(0.0), 0.0);
    EXPECT_EQ(stat_abs_scalar_f(-0.0), 0.0);
}

TEST(test_stat_abs_scalar_f_nan) {
    errno = 0;
    stat_float_t result = stat_abs_scalar_f(NAN);
    EXPECT_NEQ(result, result);
    EXPECT_EQ(errno, EDOM);
}

TEST(test_stat_abs_scalar_f_bulk) {
    for (int i = 0; i < 100; i++) {
        stat_float_t x = (i % 2) ? -i * 0.1 : i * 0.1;
        EXPECT_EQ(stat_abs_scalar_f(x), fabs(x));
    }
}

// ============================================================================
// TEST: stat_abs_scalar_i32 (int32_t x)
// ============================================================================
TEST(test_stat_abs_scalar_i32) {
    EXPECT_EQ(stat_abs_scalar_i32(-42), 42);
    EXPECT_EQ(stat_abs_scalar_i32(42), 42);
}

TEST(test_stat_abs_scalar_i32_range) {
    errno = 0;
    stat_abs_scalar_i32(INT32_MIN);
    EXPECT_EQ(errno, ERANGE);
}

TEST(test_stat_abs_scalar_i32_bulk) {
    for (int32_t i = -50; i < 50; i++) {
        if (i != INT32_MIN) { // Skip undefined case
            EXPECT_EQ(stat_abs_scalar_i32(i), abs(i));
        }
    }
}

// ============================================================================
// TEST: stat_min_f (const stat_float_t* data, stat_size_t count)
// ============================================================================
TEST(test_stat_min_f_normal) {
    stat_float_t data[] = {5.0, -2.0, 3.14, 0.001, -DBL_MAX};
    EXPECT_EQ(stat_min_f(data, 5), -DBL_MAX);  // Dogma 1: Extreme value
}

TEST(test_stat_min_f_nan_handling) {
    stat_float_t data[] = {1.0, 0.0/0.0, 3.0};
    EXPECT_NEQ(stat_min_f(data, 3), stat_min_f(data, 3));  // NaN check
}

TEST(test_stat_min_f_empty) {
    errno = 0;
    stat_float_t dummy;
    EXPECT_TRUE(isnan(stat_min_f(&dummy, 0)));  // @throws EINVAL
    EXPECT_EQ(errno, EINVAL);                   // Dogma 4: Docs proven
}

TEST(test_stat_min_f_large) {
    stat_float_t* big = (stat_float_t*)malloc(1000 * sizeof(stat_float_t));
    EXPECT_MALLOC(big);  /* Your cleaner version */

    for (int i = 0; i < 1000; i++) {
        big[i] = 1000.0 - i;
    }

    EXPECT_EQ(stat_min_f(big, 1000), 1.0);
    free(big);
}

// ============================================================================
// TEST: stat_max_f (const stat_float_t* data, stat_size_t count)
// ============================================================================
TEST(test_stat_max_f_normal) {
    stat_float_t data[] = {-5.0, 2.0, DBL_MAX, 0.0};
    EXPECT_EQ(stat_max_f(data, 4), DBL_MAX);  // Dogma 1: Max float
}

TEST(test_stat_max_f_infinity) {
    stat_float_t data[] = {1.0, INFINITY, 3.0};
    EXPECT_EQ(stat_max_f(data, 3), INFINITY);  // Dogma 1: INF handling
}

// ============================================================================
// TEST: stat_range_f (const stat_float_t* data, stat_size_t count)
// ============================================================================
TEST(test_stat_range_f_known) {
    stat_float_t data[] = {10.0, -5.0, 0.0, DBL_MIN};
    EXPECT_EQ(stat_range_f(data, 4), 15.0);  // 10.0 - (-5.0)
}

TEST(test_stat_range_f_precision) {
    // Dogma 3: Math correctness
    stat_float_t data[] = {1.0000001, 1.0000000};
    EXPECT_EQ(stat_range_f(data, 2), 0.0000001);
}

// ============================================================================
// TEST: Scalar Functions (Dogma 11: Full coverage)
// ============================================================================
TEST(test_stat_min_scalar_f_edge) {
    EXPECT_EQ(stat_min_scalar_f(NAN, 2.0), 2.0);  // NaN handling
    EXPECT_EQ(stat_min_scalar_f(-0.0, +0.0), -0.0);  // IEEE 754
}

TEST(test_stat_max_scalar_f_edge) {
    EXPECT_EQ(stat_max_scalar_f(INFINITY, DBL_MAX), INFINITY);
    EXPECT_EQ(stat_max_scalar_f(NAN, 1.0), 1.0);
}

// ============================================================================
// TEST: stat_mean_f (const stat_float_t* data, stat_size_t count)
// ============================================================================
TEST(test_stat_mean_f_basic) {
    stat_float_t data[] = {1.0, 2.0, 3.0, 4.0};
    EXPECT_EQ(stat_mean_f(data, 4), 2.5);
}

TEST(test_stat_mean_f_empty) {
    errno = 0;
    stat_float_t dummy;
    EXPECT_TRUE(isnan(stat_mean_f(&dummy, 0)));  // @throws EINVAL
    EXPECT_EQ(errno, EINVAL);
}

TEST(test_stat_mean_f_large) {
    // Dogma 10: Heap-allocated large array
    stat_float_t* big = (stat_float_t*)malloc(1000 * sizeof(stat_float_t));
    EXPECT_MALLOC(big);

    for (int i = 0; i < 1000; i++) big[i] = i + 1.0;
    EXPECT_EQ(stat_mean_f(big, 1000), 500.5);  // (1+1000)/2

    free(big);
}

// ============================================================================
// TEST: stat_median_f (const stat_float_t* data, stat_size_t count)
// ============================================================================
TEST(test_stat_median_f_odd) {
    stat_float_t data[] = {5.0, 2.0, 3.0};
    EXPECT_EQ(stat_median_f(data, 3), 3.0);
}

TEST(test_stat_median_f_even) {
    stat_float_t data[] = {1.0, 5.0, 2.0, 4.0};
    EXPECT_EQ(stat_median_f(data, 4), 3.0);  // (2+4)/2
}

// ============================================================================
// TEST: stat_mode_f (const stat_float_t* data, stat_size_t count, ...)
// ============================================================================
TEST(test_stat_mode_f_unimodal) {
    stat_float_t data[] = {1.0, 2.0, 2.0, 3.0};
    stat_float_t modes[4];
    stat_size_t mode_count;

    EXPECT_TRUE(stat_mode_f(data, 4, modes, &mode_count));
    EXPECT_EQ(mode_count, 1);
    EXPECT_EQ(modes[0], 2.0);
}

TEST(test_stat_mode_f_bimodal) {
    stat_float_t data[] = {1.0, 2.0, 2.0, 3.0, 3.0};
    stat_float_t modes[5];
    stat_size_t mode_count;

    EXPECT_TRUE(stat_mode_f(data, 5, modes, &mode_count));
    EXPECT_EQ(mode_count, 2);
    EXPECT_EQ(modes[0], 2.0);  // Modes may be unordered
    EXPECT_EQ(modes[1], 3.0);
}

// ============================================================================
// Integer Version Tests (Same patterns as float)
// ============================================================================
TEST(test_stat_mean_i_basic) {
    stat_int_t data[] = {1, 2, 3, 4};
    EXPECT_EQ(stat_mean_i(data, 4), 2.5);
}

TEST(test_stat_mode_i_no_modes) {
    stat_int_t data[] = {1, 2, 3};
    stat_int_t modes[3];
    stat_size_t mode_count;

    EXPECT_TRUE(stat_mode_i(data, 3, modes, &mode_count));
    EXPECT_EQ(mode_count, 3);  // All values are modes when none repeat
}

// ============================================================================
// TEST: stat_clamp_f (float array clamp)
// ============================================================================
TEST(test_stat_clamp_f_basic) {
    stat_float_t src[] = {-1.5, 2.0, 3.5, NAN};
    stat_float_t dst[4];

    stat_clamp_f(src, dst, 4, 0.0, 2.0);
    EXPECT_EQ(dst[0], 0.0);   // Clamped from -1.5
    EXPECT_EQ(dst[1], 2.0);   // Unchanged
    EXPECT_EQ(dst[2], 2.0);   // Clamped from 3.5
    EXPECT_NEQ(dst[3], dst[3]); // NaN propagated
}

TEST(test_stat_clamp_f_empty) {
    errno = 0;
    stat_float_t dummy_src, dummy_dst;
    stat_clamp_f(&dummy_src, &dummy_dst, 0, 0.0, 1.0);
    EXPECT_EQ(errno, EINVAL);  // @throws EINVAL
}

TEST(test_stat_clamp_f_invalid_range) {
    stat_float_t src[] = {1.0};
    stat_float_t dst[1];

    errno = 0;
    stat_clamp_f(src, dst, 1, 2.0, 1.0);  // min > max
    EXPECT_EQ(errno, EINVAL);
}

// ============================================================================
// TEST: stat_clamp_i32 (int32 array clamp)
// ============================================================================
TEST(test_stat_clamp_i32_basic) {
    stat_int_t src[] = {INT32_MIN, 0, INT32_MAX};
    stat_int_t dst[3];

    stat_clamp_i32(src, dst, 3, -100, 100);
    EXPECT_EQ(dst[0], -100);  // Clamped from INT32_MIN
    EXPECT_EQ(dst[1], 0);     // Unchanged
    EXPECT_EQ(dst[2], 100);   // Clamped from INT32_MAX
}

// ============================================================================
// TEST: Scalar Clamps
// ============================================================================
TEST(test_stat_clamp_scalar_f_edge) {
    errno = 0;
    EXPECT_EQ(stat_clamp_scalar_f(-INFINITY, 0.0, 1.0), 0.0);
    EXPECT_EQ(stat_clamp_scalar_f(NAN, 0.0, 1.0), NAN);
    EXPECT_EQ(errno, EDOM);
}

TEST(test_stat_clamp_scalar_i32_edge) {
    errno = 0;
    EXPECT_EQ(stat_clamp_scalar_i32(42, 0, INT32_MAX), 42);
    EXPECT_EQ(stat_clamp_scalar_i32(42, 50, 10), 42);  // Invalid range
    EXPECT_EQ(errno, EINVAL);
}

// ============================================================================
// Large Array Tests (Heap-Allocated for DOS)
// ============================================================================
TEST(test_stat_clamp_f_large) {
    stat_float_t* big_src = (stat_float_t*)malloc(1000 * sizeof(stat_float_t));
    stat_float_t* big_dst = (stat_float_t*)malloc(1000 * sizeof(stat_float_t));
    EXPECT_MALLOC(big_src);
    EXPECT_MALLOC(big_dst);

    for (int i = 0; i < 1000; i++) {
        big_src[i] = i - 500.0;  // Values from -500 to 499
    }

    stat_clamp_f(big_src, big_dst, 1000, -100.0, 100.0);
    EXPECT_EQ(big_dst[0], -100.0);   // Clamped from -500
    EXPECT_EQ(big_dst[600], 100.0);  // Clamped from 100 (now 600-500=100)

    free(big_src);
    free(big_dst);
}

// ============================================================================
// TEST: stat_compare_floats
// ============================================================================
TEST(test_stat_compare_floats_basic) {
    // Exact equality
    EXPECT_EQ(stat_compare_floats(1.0, 1.0, STAT_SAFE_EPSILON), 0);

    // Within epsilon
    EXPECT_EQ(stat_compare_floats(1.0, 1.0 + 0.5*STAT_SAFE_EPSILON, STAT_SAFE_EPSILON), 0);

    // Outside epsilon (a > b)
    EXPECT_GT(stat_compare_floats(1.0 + 2*STAT_SAFE_EPSILON, 1.0, STAT_SAFE_EPSILON), 0);

    // NaN handling
    EXPECT_LT(stat_compare_floats(NAN, 1.0, STAT_SAFE_EPSILON), 0);
}

// ============================================================================
// TEST: stat_almost_equal
// ============================================================================
TEST(test_stat_almost_equal) {
    // Relative tolerance test
    EXPECT_TRUE(stat_almost_equal(1.0, 1.0 + STAT_REL_TOL*0.5, STAT_REL_TOL, STAT_ABS_TOL));

    // Absolute tolerance test for near-zero
    EXPECT_TRUE(stat_almost_equal(0.0, STAT_ABS_TOL*0.5, STAT_REL_TOL, STAT_ABS_TOL));

    // Failure case
    EXPECT_FALSE(stat_almost_equal(1.0, 1.0 + 2*STAT_REL_TOL, STAT_REL_TOL, STAT_ABS_TOL));
}

// ============================================================================
// TEST: stat_is_near_zero
// ============================================================================
TEST(test_stat_is_near_zero) {
    EXPECT_TRUE(stat_is_near_zero(0.5*STAT_SMALL_TOL, STAT_SMALL_TOL));
    EXPECT_FALSE(stat_is_near_zero(2*STAT_SMALL_TOL, STAT_SMALL_TOL));

    // Verify it uses absolute comparison only
    EXPECT_TRUE(stat_is_near_zero(1e-20, STAT_SMALL_TOL));  // Even though 1e-20 << SMALL_TOL
}

// ============================================================================
// TEST: Qsort Comparators
// ============================================================================
TEST(test_stat_compare_floats_qsort) {
    stat_float_t a = 1.0;
    stat_float_t b = 2.0;
    EXPECT_LT(stat_compare_floats_qsort(&a, &b), 0);
    EXPECT_GT(stat_compare_floats_qsort(&b, &a), 0);
}

TEST(test_stat_compare_ints_qsort) {
    stat_int_t a = 10;
    stat_int_t b = 20;
    EXPECT_LT(stat_compare_ints_qsort(&a, &b), 0);
    EXPECT_EQ(stat_compare_ints_qsort(&a, &a), 0);
}

// ============================================================================
// Edge Case Tests
// ============================================================================
TEST(test_stat_compare_infinity) {
    EXPECT_LT(stat_compare_floats(INFINITY, 1.0, STAT_SAFE_EPSILON), 0);
    EXPECT_GT(stat_compare_floats(1.0, -INFINITY, STAT_SAFE_EPSILON), 0);
}

TEST(test_stat_almost_equal_extreme) {
    // Large numbers with relative tolerance
    stat_float_t big = 1e20;
    EXPECT_TRUE(stat_almost_equal(big, big*(1.0 + 0.5*STAT_REL_TOL), STAT_REL_TOL, STAT_ABS_TOL));
}

// Binning tests
TEST(test_binning_linear_edges) {
    stat_binning_config_t cfg;
    cfg.min = 0.0;
    cfg.max = 100.0;
    cfg.count = 5;
    stat_float_t edges[6];  // count+1
    cfg.edges = edges;
    
    stat_binning_calculate_edges(&cfg, BIN_LINEAR);
    
    EXPECT_EQ(edges[0], 0.0);
    EXPECT_EQ(edges[1], 20.0);
    EXPECT_EQ(edges[2], 40.0);
    EXPECT_EQ(edges[3], 60.0);
    EXPECT_EQ(edges[4], 80.0);
    EXPECT_EQ(edges[5], 100.0);
}

TEST(test_binning_logarithmic_edges) {
    stat_binning_config_t cfg;
    cfg.min = 1.0;
    cfg.max = 1000.0;
    cfg.count = 3;
    stat_float_t edges[4];
    cfg.edges = edges;
    
    stat_binning_calculate_edges(&cfg, BIN_LOGARITHMIC);
    
    EXPECT_GT(edges[1], 1.0);
    EXPECT_LT(edges[1], edges[2]);
    EXPECT_EQ(edges[3], 1000.0);
}

TEST(test_bin_integer_values) {
    stat_binning_config_t cfg;
    cfg.min = 0;
    cfg.max = 100;
    cfg.count = 5;
    stat_float_t edges[6] = {0, 20, 40, 60, 80, 100};
    cfg.edges = edges;
    
    stat_int_t values[] = {5, 25, 35, 99, 100, 0};
    stat_size_t bins[5] = {0};
    
    stat_bin_values_i(values, 6, &cfg, bins);
    
    EXPECT_EQ(bins[0], 2);  // 0-20: 5, 0
    EXPECT_EQ(bins[1], 1);  // 20-40: 25
    EXPECT_EQ(bins[2], 1);  // 40-60: 35
    EXPECT_EQ(bins[4], 2);  // 80-100: 99, 100
}

TEST(test_bin_float_values) {
    stat_binning_config_t cfg;
    cfg.min = 0.0;
    cfg.max = 1.0;
    cfg.count = 2;
    stat_float_t edges[3] = {0.0, 0.5, 1.0};
    cfg.edges = edges;
    
    stat_float_t values[] = {0.1, 0.6, 0.4999999, 1.0};
    stat_size_t bins[2] = {0};
    
    stat_bin_values_f(values, 4, &cfg, bins, 1e-6f);
    
    EXPECT_EQ(bins[0], 2);  // 0.0-0.5: 0.1, 0.4999999
    EXPECT_EQ(bins[1], 2);  // 0.5-1.0: 0.6, 1.0
}

TEST(test_auto_bin_percentile) {
    stat_float_t values[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    stat_binning_config_t cfg;
    cfg.count = 5;
    stat_float_t edges[6];
    cfg.edges = edges;
    
    stat_auto_bin_f(values, 5, &cfg, BIN_PERCENTILE);
    
    EXPECT_EQ(edges[0], 1.0);
    EXPECT_EQ(edges[5], 5.0);
    EXPECT_IN_RANGE(edges[1], 1.9, 2.1);
    EXPECT_IN_RANGE(edges[2], 2.9, 3.1);
}

TEST(test_bin_center_width) {
    stat_binning_config_t cfg;
    cfg.count = 2;
    stat_float_t edges[3] = {0.0, 5.0, 10.0};
    cfg.edges = edges;
    
    EXPECT_EQ(stat_bin_center(&cfg, 0), 2.5);
    EXPECT_EQ(stat_bin_center(&cfg, 1), 7.5);
    EXPECT_EQ(stat_bin_width(&cfg, 0), 5.0);
    EXPECT_EQ(stat_bin_width(&cfg, 1), 5.0);
}

TEST(test_edge_cases) {
    // Empty bins check
    stat_binning_config_t cfg;
    cfg.count = 0;
    stat_float_t edges[1];
    cfg.edges = edges;
    
    // Should assert (count must be > 0)
    // Note: This would be wrapped in EXPECT_ASSERT in frameworks that support it
}

TEST(test_binning_with_rounding) {
    stat_binning_config_t cfg;
    cfg.min = 0.0;
    cfg.max = 1.0;
    cfg.count = 4;
    stat_float_t edges[5];
    cfg.edges = edges;
    
    stat_binning_calculate_edges(&cfg, BIN_LINEAR);
    
    // Verify edges are properly rounded
    EXPECT_EQ(edges[0], 0.0);
    EXPECT_EQ(edges[1], 0.25);
    EXPECT_EQ(edges[2], 0.5);
    EXPECT_EQ(edges[3], 0.75);
    EXPECT_EQ(edges[4], 1.0);
    
    // Test with values that need rounding
    stat_float_t values[] = {0.2499999, 0.2500001, 0.4999999, 0.5000001};
    stat_size_t bins[4] = {0};
    
    stat_bin_values_f(values, 4, &cfg, bins, 1e-7f);
    
    EXPECT_EQ(bins[0], 1);  // 0.2499999 -> bin 0
    EXPECT_EQ(bins[1], 2);  // 0.2500001 and 0.4999999 -> bin 1
    EXPECT_EQ(bins[2], 1);  // 0.5000001 -> bin 2
}

TEST(test_percentile_binning_with_rounding) {
    stat_float_t values[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    stat_binning_config_t cfg;
    cfg.count = 5;
    stat_float_t edges[6];
    cfg.edges = edges;
    
    stat_auto_bin_f(values, 5, &cfg, BIN_PERCENTILE);
    
    // Verify rounded percentiles
    EXPECT_EQ(edges[0], 1.1);
    EXPECT_EQ(edges[5], 5.5);
    EXPECT_EQ(stat_round_decimal(edges[1], 1), 2.2);
    EXPECT_EQ(stat_round_decimal(edges[2], 1), 3.3);
    EXPECT_EQ(stat_round_decimal(edges[3], 1), 4.4);
}

TEST(test_bin_center_with_rounding) {
    stat_binning_config_t cfg;
    cfg.count = 2;
    stat_float_t edges[3] = {1.234567, 3.456789, 5.678901};
    cfg.edges = edges;
    
    // Test center calculation with rounding
    stat_float_t center = stat_bin_center(&cfg, 0);
    EXPECT_EQ(stat_round_decimal(center, 6), 2.345678);
    
    center = stat_bin_center(&cfg, 1);
    EXPECT_EQ(stat_round_decimal(center, 6), 4.567845);
}
