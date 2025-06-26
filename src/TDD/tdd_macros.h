/**
 * @file tdd_macros.h
 * @brief Minimalist Test-Driven Development (TDD) framework
 * @details Inspired by Axel Lindeberg aka u/SuperSmurfen but significantly extending
 * @see Original implementation: https://github.com/AxlLind/libwebb/blob/main/tests/libtest.h
 *
 * @defgroup tdd_framework Test Framework
 * @{
 */

#ifndef TDD_MACROS_H
#define TDD_MACROS_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "tdd_report.h"
#include "tdd_progress.h"

// ========================
// DEATH TEST INFRASTRUCTURE
// ========================
#include <setjmp.h>
#include <signal.h>

static jmp_buf tdd_death_jmp_buf;

/**
 * @brief Conditional verbose output macro
 * @param expr Expression to execute only in debug mode
 */
#ifndef NDEBUG
#define V(expr) do { { expr } } while(0)
#else
#define V(expr)
#endif

// =============================================
// Internal Implementations
// =============================================

/**
 * @brief Extracts filename from full path
 */
#define _FILENAME (strrchr("/" __FILE__, '/') + 1)

#define _EXPECT_COMPARE(a, b, op, tag)                                  \
    do {                                                                \
        if (!((a) op (b))) {                                            \
            printf("\n%s:%d - FAILED: %s %s %s\n"                       \
                   "  Actual:   %d\n"                                   \
                   "  Expected: %d\n",                                  \
                   _FILENAME, __LINE__, #a, #op, #b, (int32_t)(a), (int32_t)(b));\
            *pass = false;                                              \
        }                                                               \
    } while (0)

#define _EXPECT_STRCOMPARE(a, b, op, tag)                              \
    do {                                                                \
        int cmp = strcmp((a), (b));                                     \
        if (!(cmp op 0)) {                                              \
            printf("\n%s:%d - FAILED: %s %s %s\n"                       \
                   "  Actual:   \"%s\"\n"                               \
                   "  Expected: \"%s\"\n"                               \
                   "  strcmp() = %d\n",                                \
                   _FILENAME, __LINE__, #a, #op, #b, (a), (b), cmp);    \
            *pass = false;                                              \
        }                                                               \
    } while (0)

#define _EXPECT_NULLCHECK(ptr, should_be_null)                         \
    do {                                                                \
        bool is_null = (ptr == NULL);                                   \
        if (is_null != should_be_null) {                                \
            printf("\n%s:%d - FAILED: %s %s NULL\n"                     \
                   "  Pointer: %p\n",                                   \
                   _FILENAME, __LINE__, #ptr,                           \
                   should_be_null ? "should be" : "should not be",      \
                   (void*)(ptr));                                       \
            *pass = false;                                              \
        }                                                               \
    } while (0)

#define _EXPECT_STRCONTAINS(haystack, needle)                          \
    do {                                                                \
        const char* _hs = (haystack);                                   \
        const char* _nd = (needle);                                     \
        if (_hs == NULL || _nd == NULL || strstr(_hs, _nd) == NULL) {  \
            printf("\n%s:%d - FAILED: \"%s\" should contain \"%s\"\n"   \
                   "  Full string: \"%s\"\n",                           \
                   _FILENAME, __LINE__, #haystack, #needle, _hs);       \
            *pass = false;                                              \
        }                                                               \
    } while (0)

/**
 * @brief Internal assertion handler
 * @param expr Expression to evaluate
 * @param halt Whether to stop test execution on failure
 * @private
 */
#define _ASSERT(expr, halt)                                         \
    do {                                                                \
        if (!(expr)) {                                                  \
            printf("\n%s:%d - expected: %s\n", _FILENAME, __LINE__, #expr);\
            *pass = false;                                              \
            if (halt)                                                   \
                return;                                                 \
        }                                                               \
    } while (0)

/**
 * @brief Non-fatal test expectation
 * @param expr Expression to test (continues execution if false)
 */
#define EXPECT(expr) _ASSERT(expr, false)

/**
 * @brief Fatal test assertion
 * @param expr Expression to test (stops test if false)
 */
#define ASSERT(expr, ...) _ASSERT(expr, true)

// =============================================
// Numeric Comparisons
// =============================================

#define EXPECT_ALMOST_EQ(a, b, e) _EXPECT_COMPARE(fabs(a - b), e, <=, "EQ")


#define EXPECT_MALLOC(ptr) _ASSERT(ptr != NULL, false);

#define EXPECT_TRUE(expr) _ASSERT(expr, false);

#define EXPECT_FALSE(expr) _ASSERT(!(expr), false);

/**
 * @brief Expects two values to be equal (non-fatal)
 * @param a Actual value
 * @param b Expected value
 */
#define EXPECT_EQ(a, b) _EXPECT_COMPARE(a, b, ==, "EQ")

/**
 * @brief Expects two values to be unequal (non-fatal)
 * @param a First value
 * @param b Second value
 */
#define EXPECT_NEQ(a, b) _EXPECT_COMPARE(a, b, !=, "NEQ")

/**
 * @brief Expects a > b (non-fatal)
 * @param a Tested value
 * @param b Comparison threshold
 */
#define EXPECT_GT(a, b) _EXPECT_COMPARE(a, b, >, "GT")

#define EXPECT_GTE(a, b) _EXPECT_COMPARE(a,b,>=,"GTE")

/**
 * @brief Expects a < b (non-fatal)
 * @param a Tested value
 * @param b Comparison threshold
 */
#define EXPECT_LT(a, b) _EXPECT_COMPARE(a, b, <, "LT")

#define EXPECT_LTE(a,b) _EXPECT_COMPARE(c,b,<=,"LTE")

// =============================================
// String Comparisons
// =============================================

/**
 * @brief Expects two strings to be equal (strcmp == 0)
 * @param a Actual string
 * @param b Expected string
 */
#define EXPECT_STREQ(a, b) _EXPECT_STRCOMPARE(a, b, ==, "STREQ")

/**
 * @brief Expects two strings to be unequal (strcmp != 0)
 * @param a First string
 * @param b Second string
 */
#define EXPECT_STRNEQ(a, b) _EXPECT_STRCOMPARE(a, b, !=, "STRNEQ")

/**
 * @brief Expects a string to contain a substring (non-fatal)
 * @param haystack String to search in
 * @param needle Substring to find
 */
#define EXPECT_STR_CONTAINS(haystack, needle) _EXPECT_STRCONTAINS(haystack, needle)

// =============================================
// Pointer Comparisons
// =============================================

/**
 * @brief Expects pointer to be NULL (non-fatal)
 * @param ptr Pointer to check
 */
#define EXPECT_NULL(ptr) _EXPECT_NULLCHECK(ptr, true)

/**
 * @brief Expects pointer to not be NULL (non-fatal)
 * @param ptr Pointer to check
 */
#define EXPECT_NOT_NULL(ptr) _EXPECT_NULLCHECK(ptr, false)

/**
 * @brief Expects a value to be within [min,max] range (inclusive)
 * @param value The value to test
 * @param min Minimum allowed value
 * @param max Maximum allowed value
 * @param ... Optional failure message
 *
 * @code
 * EXPECT_IN_RANGE(temperature, 20.0, 25.0);
 * EXPECT_IN_RANGE(score, 0, 100, "Score out of bounds");
 * @endcode
 */
#define EXPECT_IN_RANGE(value, min, max, ...) do { \
    const double _val = (double)(value); \
    const double _min = (double)(min); \
    const double _max = (double)(max); \
    if (!(_val >= _min && _val <= _max)) { \
        printf("\n%s:%d - FAILED: %s not in [%s,%s]\n" \
               "  Actual:   %g\n" \
               "  Expected: [%g,%g]\n", \
               _FILENAME, __LINE__, #value, #min, #max, _val, _min, _max); \
        if (strlen("" #__VA_ARGS__) > 0) printf("  Message:  " __VA_ARGS__ "\n"); \
        *pass = false; \
    } \
} while (0)

/**
 * @brief Test case structure
 */
typedef struct {
    void (*fn)(bool*);   /**< Test function pointer */
    char *name;          /**< Test name */
} test_t;

/**
 * @brief Declares a test case
 * @param name Test case name
 *
 * @details Creates:
 *          1. Test function prototype
 *          2. Test case structure
 *          3. Test function implementation
 */
#define TEST(name)                                  \
    static void name##_fn(bool*);                   \
    static const test_t name = {name##_fn, #name};  \
    static void name##_fn(bool* pass)

/**
 * @brief Executes a test suite
 * @param ... Variable list of test cases
 * @return Number of failed tests
 *
 * @details Handles:
 *          - Test execution
 *          - Result reporting
 *          - Failure counting
 *          - Verbosity control
 */
 #define RUN_TESTS(...)                                                  \
     int run_tests(void) {                                               \
         const test_t* tests[] = {__VA_ARGS__};                          \
         test_summary_t summary = {0};                                   \
         summary.suite_name = _FILENAME;                                 \
         clock_t start = clock();                                        \
                                                                         \
         for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {   \
             bool passed = true;                                         \
             tests[i]->fn(&passed);                                      \
             summary.total++;                                            \
             passed ? summary.passed++ : summary.failed++;               \
         }                                                               \
                                                                         \
         summary.time_elapsed = (double)(clock() - start) / CLOCKS_PER_SEC; \
         tdd_generate_report(summary, stdout);                           \
         tdd_save_history(summary);                                      \
         return summary.failed;                                          \
     }

#endif

/** @} */ // end of tdd_framework group
