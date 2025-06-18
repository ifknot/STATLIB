/**
 * @file test_tdd_framework.h
 * @brief Self-testing harness for the TDD framework
 * @note Dogfoods all test macros and report formats
 * @ingroup tdd_framework
 */

#ifndef TEST_TDD_FRAMEWORK_H
#define TEST_TDD_FRAMEWORK_H

#include "tdd_macros.h"
#include "tdd_report.h"

#define TDD_FRAMEWORK_TESTS \
    &test_expect_macros,     \
    &test_string_macros,     \
    &test_pointer_macros,    \
    &test_report_formats,    \
    &test_historical_tracking

// =============================================
// Test Cases (Testing the Tester!)
// =============================================

TEST(test_expect_macros) {
    // Numeric comparisons
    EXPECT_EQ(42, 42);
    EXPECT_NEQ(42, 99);
    EXPECT_GT(100, 42);
    EXPECT_LT(42, 100);

    // Edge cases
    EXPECT_EQ(0, 0);
    EXPECT_NEQ(0, 1);
}

TEST(test_string_macros) {
    const char* test_str = "Hello TDD";

    // Exact matches
    EXPECT_STREQ(test_str, "Hello TDD");
    EXPECT_STRNEQ(test_str, "Goodbye TDD");

    // Substrings
    EXPECT_STR_CONTAINS(test_str, "TDD");
    EXPECT_STR_CONTAINS(test_str, "Hello");

    // Negative cases
    EXPECT(!strstr(test_str, "XYZ")); // Sanity check
}

TEST(test_pointer_macros) {
    int x = 42;
    int* valid_ptr = &x;
    int* null_ptr = NULL;

    EXPECT_NOT_NULL(valid_ptr);
    EXPECT_NULL(null_ptr);

    // Pointer comparison
    EXPECT_EQ(valid_ptr, &x);
    EXPECT_NEQ(valid_ptr, (int*)0xDEADBEEF);
}

TEST(test_report_formats) {
    test_summary_t dummy = {
        .total = 10,
        .passed = 8,
        .failed = 2,
        .suite_name = "test_dummy",
        .time_elapsed = 1.23
    };

    // Verify all formats execute without crashing
    tdd_set_format(REPORT_CONSOLE);
    tdd_generate_report(dummy, stdout);

    tdd_set_format(REPORT_JSON);
    tdd_generate_report(dummy, stdout);

    tdd_set_format(REPORT_VERBOSE);
    tdd_generate_report(dummy, stdout);

    tdd_set_format(REPORT_SILENT);
    tdd_generate_report(dummy, stdout);

    // Reset to default
    tdd_set_format(REPORT_CONSOLE);
}

TEST(test_historical_tracking) {
    test_summary_t dummy = {
        .total = 3,
        .passed = 3,
        .failed = 0,
        .suite_name = "test_historical",
        .time_elapsed = 0.45
    };

    // Test history logging
    tdd_save_history(dummy);

    // Verify file was touched (simplistic check)
    FILE* hist = fopen("history.tdd", "r");
    EXPECT_NOT_NULL(hist);
    if (hist) {
        char line[256];
        EXPECT_NOT_NULL(fgets(line, sizeof(line), hist));
        fclose(hist);
    }
}

#endif
