#ifndef TEST_STAT_H
#define TEST_STAT_H

#include "stat_abs.h"
#include "tdd_macros.h"

// =============================================
// Test Suite Declaration
// =============================================

#define ABS_TEST_SUITE &test_abs_basic, \
                       &test_abs_edge_cases, \
                       &test_abs_error_handling

#define BASIC_TEST_SUITE &test_basic_scalar, \
                         &test_basic_array_operations, \
                         &test_basic_array_conversions, \
                         &test_basic_error_handling



// =============================================
// Test Cases
// =============================================

TEST(test_abs_basic) {
    // Test scalar float operations
    EXPECT_EQ(stat_abs_scalar_f(3.5), 3.5);
    EXPECT_EQ(stat_abs_scalar_f(-2.25), 2.25);
    EXPECT_EQ(stat_abs_scalar_f(0.0), 0.0);
    
    // Test scalar int operations
    EXPECT_EQ(stat_abs_scalar_i32(10), 10);
    EXPECT_EQ(stat_abs_scalar_i32(-5), 5);
    EXPECT_EQ(stat_abs_scalar_i32(0), 0);
    
    // Test array float operations
    stat_float_t src_f[] = {1.5, -2.5, 0.0, -0.0, 3.75};
    stat_float_t dst_f[5];
    stat_abs_f(src_f, dst_f, 5);
    
    EXPECT_ALMOST_EQ(dst_f[0], 1.5, 0.0001);
    EXPECT_ALMOST_EQ(dst_f[1], 2.5, 0.0001);
    EXPECT_EQ(dst_f[2], 0.0);
    EXPECT_EQ(dst_f[3], 0.0);
    EXPECT_ALMOST_EQ(dst_f[4], 3.75, 0.0001);
    
    // Test array int operations
    int32_t src_i[] = {1, -2, 0, 100, -200};
    int32_t dst_i[5];
    stat_abs_i(src_i, dst_i, 5);
    
    EXPECT_EQ(dst_i[0], 1);
    EXPECT_EQ(dst_i[1], 2);
    EXPECT_EQ(dst_i[2], 0);
    EXPECT_EQ(dst_i[3], 100);
    EXPECT_EQ(dst_i[4], 200);
}

TEST(test_abs_edge_cases) {
    // Test in-place operation (src == dst)
    stat_float_t inplace_f[] = {-1.1, 2.2, -3.3};
    stat_abs_f(inplace_f, inplace_f, 3);
    EXPECT_ALMOST_EQ(inplace_f[0], 1.1, 0.0001);
    EXPECT_ALMOST_EQ(inplace_f[1], 2.2, 0.0001);
    EXPECT_ALMOST_EQ(inplace_f[2], 3.3, 0.0001);
    
    int32_t inplace_i[] = {-1, 2, -3};
    stat_abs_i(inplace_i, inplace_i, 3);
    EXPECT_EQ(inplace_i[0], 1);
    EXPECT_EQ(inplace_i[1], 2);
    EXPECT_EQ(inplace_i[2], 3);
    
    // Test single element arrays
    stat_float_t single_f = -5.5;
    stat_abs_f(&single_f, &single_f, 1);
    EXPECT_ALMOST_EQ(single_f, 5.5, 0.0001);
    
    int32_t single_i = -5;
    stat_abs_i(&single_i, &single_i, 1);
    EXPECT_EQ(single_i, 5);
}

TEST(test_abs_error_handling) {
    stat_float_t src_f[] = {1.0, 2.0};
    stat_float_t dst_f[2];
    int32_t src_i[] = {1, 2};
    int32_t dst_i[2];
    
    // Test NULL inputs (should assert)
    #ifndef NDEBUG
    //EXPECT_ASSERT(stat_abs_f(NULL, dst_f, 2));
    //EXPECT_ASSERT(stat_abs_f(src_f, NULL, 2));
    //EXPECT_ASSERT(stat_abs_i(NULL, dst_i, 2));
    //EXPECT_ASSERT(stat_abs_i(src_i, NULL, 2));
    #endif
    
    // Test size=0 (should set errno)
    errno = 0;
    stat_abs_f(src_f, dst_f, 0);
    EXPECT_EQ(errno, EINVAL);
    
    errno = 0;
    stat_abs_i(src_i, dst_i, 0);
    EXPECT_EQ(errno, EINVAL);
    
    // Test NaN handling
    stat_float_t nan_src[] = {1.0, NAN, 2.0};
    errno = 0;
    stat_abs_f(nan_src, dst_f, 3);
    EXPECT_EQ(errno, EDOM);
    
    // Test INT32_MIN handling
    int32_t min_src[] = {1, INT32_MIN, 2};
    errno = 0;
    stat_abs_i(min_src, dst_i, 3);
    EXPECT_EQ(errno, ERANGE);
    
    // Test scalar NaN
    errno = 0;
    stat_abs_scalar_f(NAN);
    EXPECT_EQ(errno, EDOM);
    
    // Test scalar INT32_MIN
    errno = 0;
    stat_abs_scalar_i32(INT32_MIN);
    EXPECT_EQ(errno, ERANGE);
}

// =============================================
// BASIC Test Cases
// =============================================

TEST(test_basic_scalar) {
    // Float min/max
    EXPECT_EQ(stat_min_float(1.5, 2.5), 1.5);
    EXPECT_EQ(stat_min_float(-1.5, -2.5), -2.5);
    EXPECT_EQ(stat_max_float(1.5, 2.5), 2.5);
    EXPECT_EQ(stat_max_float(-1.5, -2.5), -1.5);
    EXPECT_EQ(stat_min_float(1.5, 1.5), 1.5);
    
    // Int min/max
    EXPECT_EQ(stat_min_int(1, 2), 1);
    EXPECT_EQ(stat_min_int(-1, -2), -2);
    EXPECT_EQ(stat_max_int(1, 2), 2);
    EXPECT_EQ(stat_max_int(-1, -2), -1);
    EXPECT_EQ(stat_min_int(1, 1), 1);
}

TEST(test_basic_array_operations) {
    // Float array operations
    stat_float_t float_data[] = {1.5, -2.5, 3.0, 0.0, -1.0};
    const stat_size_t float_count = sizeof(float_data)/sizeof(float_data[0]);
    
    EXPECT_ALMOST_EQ(stat_min_float_array(float_data, float_count), -2.5, 0.0001);
    EXPECT_ALMOST_EQ(stat_max_float_array(float_data, float_count), 3.0, 0.0001);
    EXPECT_ALMOST_EQ(stat_range_float_array(float_data, float_count), 5.5, 0.0001);
    
    // Int array operations
    stat_int_t int_data[] = {1, -2, 3, 0, -1, 5};
    const stat_size_t int_count = sizeof(int_data)/sizeof(int_data[0]);
    
    EXPECT_EQ(stat_min_int_array(int_data, int_count), -2);
    EXPECT_EQ(stat_max_int_array(int_data, int_count), 5);
    EXPECT_EQ(stat_range_int_array(int_data, int_count), 7);
    
    // Single element arrays
    stat_float_t single_float = 42.0;
    EXPECT_EQ(stat_min_float_array(&single_float, 1), 42.0);
    EXPECT_EQ(stat_max_float_array(&single_float, 1), 42.0);
    EXPECT_EQ(stat_range_float_array(&single_float, 1), 0.0);
    
    stat_int_t single_int = -10;
    EXPECT_EQ(stat_min_int_array(&single_int, 1), -10);
    EXPECT_EQ(stat_max_int_array(&single_int, 1), -10);
    EXPECT_EQ(stat_range_int_array(&single_int, 1), 0);
}

TEST(test_basic_array_conversions) {
    // Float to int conversion (truncating)
    stat_float_t float_src[] = {1.5, -2.9, 3.0, 0.0, -1.1};
    stat_int_t int_dst[5];
    stat_cast_float_to_int_array(int_dst, float_src, 5);
    
    EXPECT_EQ(int_dst[0], 1);
    EXPECT_EQ(int_dst[1], -2);
    EXPECT_EQ(int_dst[2], 3);
    EXPECT_EQ(int_dst[3], 0);
    EXPECT_EQ(int_dst[4], -1);
    
    // Int to float conversion
    stat_int_t int_src[] = {1, -2, 3, 0, -1};
    stat_float_t float_dst[5];
    stat_cast_int_to_float_array(float_dst, int_src, 5);
    
    EXPECT_EQ(float_dst[0], 1.0);
    EXPECT_EQ(float_dst[1], -2.0);
    EXPECT_EQ(float_dst[2], 3.0);
    EXPECT_EQ(float_dst[3], 0.0);
    EXPECT_EQ(float_dst[4], -1.0);
    
    // In-place conversion test (src == dst)
    stat_int_t int_buffer[3] = {1, 2, 3};
    stat_float_t* float_ptr = (stat_float_t*)int_buffer;
    stat_cast_int_to_float_array(float_ptr, int_buffer, 3);
    EXPECT_EQ(float_ptr[0], 1.0);
    EXPECT_EQ(float_ptr[1], 2.0);
    EXPECT_EQ(float_ptr[2], 3.0);
}

TEST(test_basic_error_handling) {
    stat_float_t float_data[] = {1.0, 2.0};
    stat_int_t int_data[] = {1, 2};
    
    // Test NULL inputs (should assert)
    #ifndef NDEBUG
    //EXPECT_ASSERT(stat_min_float_array(NULL, 2));
    //EXPECT_ASSERT(stat_max_int_array(NULL, 2));
    //EXPECT_ASSERT(stat_cast_float_to_int_array(NULL, float_data, 2));
    //EXPECT_ASSERT(stat_cast_int_to_float_array(NULL, int_data, 2));
    #endif
    
    // Test size=0 (should set errno)
    errno = 0;
    EXPECT_TRUE(isnan(stat_min_float_array(float_data, 0)));
    EXPECT_EQ(errno, EINVAL);
    
    errno = 0;
    EXPECT_EQ(stat_min_int_array(int_data, 0), INT32_MAX);
    EXPECT_EQ(errno, EINVAL);
    
    errno = 0;
    EXPECT_TRUE(isnan(stat_range_float_array(float_data, 0)));
    EXPECT_EQ(errno, EINVAL);
    
    errno = 0;
    EXPECT_EQ(stat_range_int_array(int_data, 0), INT32_MIN);
    EXPECT_EQ(errno, EINVAL);
    
    // Test NaN handling in float arrays
    stat_float_t nan_data[] = {1.0, NAN, 2.0};
    EXPECT_TRUE(isnan(stat_min_float_array(nan_data, 3)));
    EXPECT_TRUE(isnan(stat_max_float_array(nan_data, 3)));
    EXPECT_TRUE(isnan(stat_range_float_array(nan_data, 3)));
}


#endif
