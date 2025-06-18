#ifndef TEST_STATS_H
#define TEST_STATS_H

#include <math.h>
#include <errno.h>

#include "../TDD/tdd_macros.h"
#include "../STAT/stat.h"
#include "stat_compare.h"
#include "stat_types.h"

#define STATS_TEST_SUITE &test_basic_stats,          \
                        &test_central_tendency,      \
                        &test_dispersion,            \
                        &test_percentiles,           \
                        &test_outliers,              \
                        &test_distributions,         \
                        &test_descriptive_stats,     \
                        &test_clamping,              \
                        &test_comparisons,           \
                        &test_division,              \
                        &test_rounding,              \
                        &test_sign_ops,              \
                        &test_abs_ops,               \
                        &test_prng,                  \
                        &test_utility,               \
                        &test_type_safety,           \
                        &test_edge_cases

// =============================================
// Test Groups
// =============================================

TEST(test_basic_stats) {
    stat_float_t data[] = {1.5, 2.5, 3.5};
    stat_size_t size = 3;

    EXPECT_EQ(stat_min_f(data, size), 1.5);
    EXPECT_EQ(stat_max_f(data, size), 3.5);
    EXPECT_EQ(stat_range_f(data, size), 2.0);
}

TEST(test_central_tendency) {
    stat_float_t data[] = {1, 2, 3, 4};
    EXPECT_EQ(stat_mean(data, 4), 2.5);
    EXPECT_EQ(stat_median(data, 4), 2.5);
    EXPECT_EQ(stat_mode(data, 4), 1); // All values are modes
}

TEST(test_dispersion) {
    stat_float_t data[] = {1, 2, 3, 4, 5};
    EXPECT_EQ(stat_variance(data, 5), 2.5);
    EXPECT_EQ(stat_std_dev(data, 5), sqrt(2.5));
    EXPECT_EQ(stat_mad(data, 5), 1.0); // NB *median* absolute deviations
    // Single element
    stat_float_t single[] = {42};
    EXPECT_EQ(stat_mad(single, 1), 0.0);
    // Even-sized array
    stat_float_t even[] = {1, 2, 3, 4};
    EXPECT_EQ(stat_mad(even, 4), 1.0);  // Deviations: {1, 0, 0, 1}
    // All identical values
    stat_float_t identical[] = {5, 5, 5};
    EXPECT_EQ(stat_mad(identical, 3), 0.0);
    //floats
    stat_float_t frac[] =  {1.1, 2.2, 3.3, 4.02, 5.1};
    EXPECT_EQ(stat_compare_floats(stat_mad(frac, 5), 1.1, STAT_SAFE_EPSILON), 0);
}

TEST(test_percentiles) {
    stat_float_t data[] = {1,2,3,4,5,6,7,8,9,10};
    stat_five_num_summary_t fns = stat_five_num_summary(data, 10);
    EXPECT_EQ(fns.min, 1);
    EXPECT_EQ(fns.median, 5.5);
    EXPECT_EQ(fns.max, 10);
}

TEST(test_outliers) {
    stat_float_t data[] = {1,2,3,4,5,100}; // 100 is an outlier
    stat_five_num_summary_t fns = stat_five_num_summary(data, 6);
    EXPECT(stat_is_outlier(100, &fns));
    EXPECT(!stat_is_outlier(3, &fns));
}

TEST(test_distributions) {
    stat_prng_state_t prng;
    stat_prng_init(&prng);

    stat_float_t normal[100];
    stat_generate_normal_dist(normal, 100, 0.0, 1.0, &prng);

    // Check mean is near 0
    EXPECT_IN_RANGE(stat_mean(normal, 100), -0.1, 0.1);
}

TEST(test_descriptive_stats) {
    stat_float_t data[] = {1,2,3,4,5};
    stat_descriptive_stats_t stats = stat_describe(data, 5);
    EXPECT_EQ(stats.mean, 3);
    EXPECT_EQ(stats.median, 3);
    EXPECT_EQ(stats.std_dev, sqrt(2.5));
}

TEST(test_clamping) {
    // Float clamping
    EXPECT_EQ(stat_clamp_f(3.14, 0.0, 5.0), 3.14);
    EXPECT_EQ(stat_clamp_f(-1.0, 0.0, 5.0), 0.0);
    EXPECT_EQ(stat_clamp_f(10.0, 0.0, 5.0), 5.0);

    // Integer clamping
    EXPECT_EQ(stat_clamp_i32(50, 0, 100), 50);
    EXPECT_EQ(stat_clamp_i32(-10, 0, 100), 0);
    EXPECT_EQ(stat_clamp_i32(150, 0, 100), 100);

    // Array clamping
    stat_float_t arr[] = {-1.0, 2.5, 10.0};
    stat_clamp_array(arr, 3, 0.0, 5.0);
    EXPECT_EQ(arr[0], 0.0);
    EXPECT_EQ(arr[1], 2.5);
    EXPECT_EQ(arr[2], 5.0);
}

TEST(test_comparisons) {
    // Float comparisons with epsilon
    EXPECT(stat_almost_equal(1.0, 1.0000001, 1e-6, 1e-9));
    EXPECT(!stat_almost_equal(1.0, 1.1, 1e-6, 1e-9));

    // Near-zero checks
    EXPECT(stat_is_near_zero(0.0000001, 1e-6));
    EXPECT(!stat_is_near_zero(0.1, 1e-6));
}

TEST(test_division) {
    int32_t result;

    // Safe division
    EXPECT(stat_safe_div_i32(10, 2, &result) && result == 5);
    EXPECT(!stat_safe_div_i32(10, 0, &result)); // division by zero
    EXPECT(!stat_safe_div_i32(INT32_MIN, -1, &result)); // overflow

    // Rounding division
    EXPECT_EQ(stat_div_round_up(10, 3), 4);
    EXPECT_EQ(stat_div_round_nearest(10, 4), 3);
}

TEST(test_rounding) {
    // Basic rounding
    EXPECT_EQ(stat_round_to_i32(3.4), 3);
    EXPECT_EQ(stat_round_to_i32(3.6), 4);
    EXPECT_EQ(stat_floor_to_i32(3.9), 3);
    EXPECT_EQ(stat_ceil_to_i32(3.1), 4);

    // Precision rounding
    EXPECT_EQ(stat_round_decimal(3.14159, 2), 3.14);
    EXPECT_EQ(stat_round_to_multiple(17, 5), 15);
}

/**
 * @brief Test absolute value operations
 * @details Covers all absolute value functions:
 *          - stat_abs_f()
 *          - stat_abs_i32()
 *          - stat_abs_u32()
 *          - stat_safe_abs_i32()
 */
TEST(test_abs_ops) {
    // ======================
    // Float absolute values
    // ======================
    EXPECT_EQ(stat_abs_f(3.14), 3.14);
    EXPECT_EQ(stat_abs_f(-2.718), 2.718);
    EXPECT_EQ(stat_abs_f(0.0), 0.0);
    EXPECT(isnan(stat_abs_f(NAN)));

    // ======================
    // int32_t absolute values
    // ======================
    EXPECT_EQ(stat_abs_i32(42), 42);
    EXPECT_EQ(stat_abs_i32(-42), 42);
    EXPECT_EQ(stat_abs_i32(0), 0);

    // Test INT32_MIN edge case (can't be represented positively)
    EXPECT_EQ(stat_abs_i32(INT32_MIN), INT32_MIN); // Known limitation

    // ======================
    // uint32_t absolute values
    // ======================
    EXPECT_EQ(stat_abs_u32(42), 42);
    EXPECT_EQ(stat_abs_u32(0), 0);

    // ======================
    // Safe absolute values
    // ======================
    int32_t result;
    EXPECT(stat_safe_abs_i32(42, &result) && result == 42);
    EXPECT(stat_safe_abs_i32(-42, &result) && result == 42);
    EXPECT(stat_safe_abs_i32(0, &result) && result == 0);

    // Test INT32_MIN failure case
    bool success = stat_safe_abs_i32(INT32_MIN, &result);
    EXPECT(!success && errno == ERANGE);

    // ======================
    // Array operations
    // ======================
    stat_float_t f_arr[] = {1.5, -2.5, 3.0};
    int32_t i_arr[] = {1, -2, 3};
    uint32_t u_arr[] = {1, 2, 3};

    EXPECT_EQ(stat_min_f(f_arr, 3), -2.5);
    EXPECT_EQ(stat_min_i32(i_arr, 3), -2);
    EXPECT_EQ(stat_min_u32(u_arr, 3), 1);
}

TEST(test_sign_ops) {
    // Sign detection
    EXPECT_EQ(stat_sign_f(3.14), 1);
    EXPECT_EQ(stat_sign_f(-2.71), -1);
    EXPECT_EQ(stat_sign_f(0.0), 0);

    // Integer signs
    EXPECT_EQ(stat_sign_i32(42), 1);
    EXPECT_EQ(stat_sign_i32(-42), -1);

    // Sign copying
    EXPECT_EQ(stat_copysign_f(5.0, -1.0), -5.0);
}

TEST(test_prng) {
    stat_prng_state_t prng;
    stat_prng_init(&prng);

    // Basic PRNG properties
    stat_float_t r1 = stat_prng_next(&prng);
    stat_float_t r2 = stat_prng_next(&prng);
    EXPECT_NEQ(r1, r2);
    EXPECT_IN_RANGE(r1, 0.0, 1.0);

    // Integer generation
    int32_t ir = stat_prng_next_int(&prng, 1, 6);
    EXPECT_IN_RANGE(ir, 1, 6);
}

TEST(test_utility) {
    stat_float_t data[] = {3.0, 1.0, 2.0};

    // Sorting
    stat_sort(data, 3);
    EXPECT_EQ(data[0], 1.0);
    EXPECT_EQ(data[1], 2.0);

    // FP classification
    EXPECT(stat_is_finite(1.0));
    EXPECT(!stat_is_normal(NAN));
}

TEST(test_type_safety) {
    // Cross-type comparisons
    EXPECT_EQ(stat_abs_i32(-5), stat_abs_u32(5));

    // Mixed-type operations
    stat_float_t f = 3.0;
    int32_t i = 3;
    EXPECT_EQ(stat_round_to_i32(f), i);
}

TEST(test_edge_cases) {
    // Empty arrays
    EXPECT(isnan(stat_mean(NULL, 0)));

    // Single element
    stat_float_t single[] = {42.0};
    EXPECT_EQ(stat_median(single, 1), 42.0);

    // Extreme values
    int32_t extreme[] = {INT32_MIN, INT32_MAX};
    EXPECT_EQ(stat_range_i32(extreme, 2), UINT32_MAX);
}

#endif // TEST_STATS_H
