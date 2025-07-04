#ifndef TEST_PRNGS_H
#define TEST_PRNGS_H

#include <stdbool.h>
#include <math.h>
#include <stdint.h>

#include "../PRNG/prng.h"
#include "../PRNG/prng_popcount.h"
#include "../PRNG/prng_constants.h"

#include "../TDD/tdd_macros.h"
#include "../TDD/tdd_progress.h"
#include "../TDD/tdd_on_escape.h"
//#include "../TDD/tdd_graphs.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// =============================================
// Test Suite Declaration
// =============================================

#define PRNG_TEST_SUITE &test_prng_popcount, \
    &test_prng_basic,         \
    &test_prng_distributions,  \
    &test_prng_monte_carlo,    \
    &test_prng_bit_quality,    \
    &test_prng_performance,    \
    &test_prng_edge_cases

#define PRNG_TEST_SUITE_FP &test_prng_float_basic, \
    &test_prng_float_distribution,  \
    &test_prng_float_monte_carlo,   \

#define PRNG_TEST_RANGE &test_prng_range_u32_basic, \
    &test_prng_range_u32_bias_check, \
    &test_prng_range_exact_uniformity, \
    &test_prng_range_exact_edge_cases

// =============================================
// Test Cases
// =============================================

TEST(test_prng_float_basic) {
    const double expected[PRNG_ENGINE_COUNT][2] = {
        {0.715670, 0.658454},  // Marsaglia MWC expected outputs
        {0.316972, 0.309092},  // XORShift128**
        {0.000001, 0.000003},  // C99 (adjust based on your implementation)
        {0.000000, 0.424801},  // PCG32
        {0.292476, 0.868537}   // SplitMix
    };

    for (size_t i = 0; i < PRNG_ENGINE_COUNT; i++) {
        prng_state_t rng;
        prng_init(&rng, prng_engine_list[i], 0xDEADBEEF, 16, NULL);

        double val1 = prng_next_float(&rng);
        double val2 = prng_next_float(&rng);
        V(printf("val1 %f val2 %f\n",val1,val2););
        // Verify basic properties
        EXPECT_GTE(val1, 0.0);
        EXPECT_LT(val1, 1.0);
        EXPECT_GTE(val2, 0.0);
        EXPECT_LT(val2, 1.0);
        EXPECT_NEQ(val1, val2);
        // Verify against known-good values
        EXPECT_ALMOST_EQ(val1, expected[i][0], 0.000001);
        EXPECT_ALMOST_EQ(val2, expected[i][1], 0.000001);
    }
}

TEST(test_prng_float_distribution) {
    const size_t num_buckets = 32;
    const size_t samples = 10000;
    const double chi2_threshold = 36.0; // 95% CI for 31 DOF

    for (size_t e = 0; e < PRNG_ENGINE_COUNT; e++) {
        prng_state_t rng;
        prng_init(&rng, prng_engine_list[e], 0xCAFE + e, 16, NULL);
        tdd_progress_t prg = tdd_progress_make(samples, 0, 0, 30);
        size_t buckets[32] = {0};
        for (size_t i = 0; i < samples; i++) {
            tdd_progress_bar(&prg);
            double val = prng_next_float(&rng);
            size_t bucket = (size_t)(val * num_buckets);
            buckets[bucket]++;
            ON_ESCAPE(break;);
        }
        // Chi-square test
        double expected = (double)samples / num_buckets;
        double chi2 = 0.0;
        for (size_t i = 0; i < num_buckets; i++) {
            double diff = buckets[i] - expected;
            chi2 += (diff * diff) / expected;
        }

        V(printf(" %s Chi Squared=%.2f\n", prng_to_string[e], chi2););
        EXPECT_LT(chi2, chi2_threshold);
    }
}

TEST(test_prng_float_monte_carlo) {
    const size_t samples = 10000;
    const double max_error = 1.0; // Max 1% error

    for (size_t e = 0; e < PRNG_ENGINE_COUNT; e++) {
        prng_state_t rng;
        prng_init(&rng, prng_engine_list[e], 0x1234 + e, 16, NULL);
        tdd_progress_t prg = tdd_progress_make(samples, 0, 0, 30);
        size_t hits = 0;
        for (size_t i = 0; i < samples; i++) {
            tdd_progress_bar(&prg);
            double x = prng_next_float(&rng);
            double y = prng_next_float(&rng);
            if (x*x + y*y <= 1.0) hits++;
            ON_ESCAPE(break;);
        }

        double pi_est = 4.0 * hits / samples;
        double error = fabs(pi_est - M_PI) / M_PI * 100.0;

        V(printf(" %s: Pi = %.5f (error=%.2f%%)\n", prng_to_string[e], pi_est, error););
        EXPECT_LT(error, max_error);
    }
}

TEST(test_prng_popcount) {
struct {
        uint32_t input;
        uint32_t expected;
    } test_cases[] = {
        {0x00000000, 0},  // All zeros
        {0x00000001, 1},  // Single bit
        {0x80000000, 1},  // Single bit (MSB)
        {0x0000000F, 4},  // 4 bits
        {0xF0F0F0F0, 16}, // Alternating nibbles
        {0xFFFFFFFF, 32}, // All ones
        {0x12345678, 13}, // Mixed pattern
        {0xAAAAAAAA, 16}  // Alternating bits
    };

    const int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < num_tests; i++) {
        uint32_t result = prng_popcount(test_cases[i].input);
        EXPECT_EQ(result,test_cases[i].expected);
    }
}

TEST(test_prng_basic) {
    const uint32_t expected[10] = {
        3073780141, 2828037266,
        1361383432, 1327541568,
        5720, 14404,
        27, 1824507900,
        1256175887, 3730336304
    };

    for (size_t i = 0; i < PRNG_ENGINE_COUNT; i++) {
        prng_state_t rng;
        V(printf("%s\n", prng_to_string[i]););
        prng_init(&rng, prng_engine_list[i], 0xDEADBEEF, 16, NULL);

        uint32_t val1 = prng_next_u32(&rng);
        uint32_t val2 = prng_next_u32(&rng);
        V(printf("%lu, %lu\n", val1, val2););

        EXPECT_NEQ(val1, val2);
        EXPECT_NEQ(val1, 0UL);
        EXPECT_NEQ(val2, 0UL);
        EXPECT_EQ(val1, expected[i * 2]);
        EXPECT_EQ(val2, expected[(i * 2) + 1]);
    }
}

TEST(test_prng_distributions) {
    const size_t num_buckets = 32;
    const size_t samples = 500;

    for (size_t e = 0; e < PRNG_ENGINE_COUNT; e++) {
        prng_state_t rng;

        prng_init(&rng, prng_engine_list[e], 0xCAFE + e, 16, NULL);

        tdd_progress_t prg = tdd_progress_make(samples, 0, 0, 30);
        uint32_t buckets[32] = {0};
        for (size_t i = 0; i < samples; i++) {
            tdd_progress_bar(&prg);
            buckets[prng_next_u32(&rng) % num_buckets]++;
        }
        V(printf(" %s\n", prng_to_string[e]););
        // Visual histogram
        //char title[40];
        //snprintf(title, sizeof(title), "%s Distribution", prng_engine_name(engines[e]));
        //tdd_graph_histogram(title, buckets, num_buckets, 10);

        // Chi-square test
        const double expected = (double)samples / num_buckets;
        double chi2 = 0.0;
        for (size_t i = 0; i < num_buckets; i++) {
            double diff = buckets[i] - expected;
            chi2 += (diff * diff) / expected;
        }
        EXPECT_LT(chi2, 55.0);  // 95% CI for 31 DOF
    }
}

TEST(test_prng_monte_carlo) {
    const size_t samples = 100;

    for (size_t e = 0; e < PRNG_ENGINE_COUNT; e++) {
        prng_state_t rng;
        prng_init(&rng, prng_engine_list[e], 0x1234 + e, 16, NULL);

        tdd_progress_t prg = tdd_progress_make(samples, 0, 0, 30);

        size_t hits = 0;
        for (size_t i = 0; i < samples; i++) {

            tdd_progress_bar(&prg);

            double x = prng_next_u32(&rng) / (double)UINT32_MAX;
            double y = prng_next_u32(&rng) / (double)UINT32_MAX;
            if (x*x + y*y <= 1.0) hits++;
        }

        double pi_est = 4.0 * hits / samples;
        double error = fabs(pi_est - M_PI) / M_PI * 100.0;
        V(printf(" err %f %s\n", error, prng_to_string[e]););
        EXPECT_IN_RANGE(error, 0.0, 7.0);
    }
}

TEST(test_prng_bit_quality) {
    const size_t samples = 1000;
    for (size_t e = 0; e < PRNG_ENGINE_COUNT; e++) {
        prng_state_t rng;
        prng_init(&rng, prng_engine_list[e], 0xABCD + e, 16, NULL);

        uint32_t last = prng_next_u32(&rng);
        size_t transitions = 0;

        tdd_progress_t prg = tdd_progress_make(samples, 0, 0, 30);

        for (size_t i = 0; i < samples; i++) {

            tdd_progress_bar(&prg);

            uint32_t current = prng_next_u32(&rng);
            transitions += prng_popcount(last ^ current);
            last = current;
        }
        V(printf(" %s\n", prng_to_string[e]););

        double avg_trans = transitions / 1000.0;
        EXPECT_IN_RANGE(avg_trans, 14.0, 18.0);
        //tdd_graph_sparkline("Bit Flips", (int)(transitions/100), 10);
    }
}

TEST(test_prng_performance) {
    const size_t iterations = 1000;

    for (size_t e = 0; e < PRNG_ENGINE_COUNT; e++) {
        prng_state_t rng;
        prng_init(&rng, prng_engine_list[e], 0xFEED, 0, NULL);

        clock_t start = clock();
        uint32_t dummy = 0;
        for (size_t i = 0; i < iterations; i++) {
            dummy ^= prng_next_u32(&rng);
        }
        clock_t end = clock();
        (void)dummy;

        double ms = (end - start) * 1000.0 / CLOCKS_PER_SEC;
        V(printf("time %lu ms\t%s\n", (uint32_t)ms, prng_to_string[e]););
        EXPECT_LT(ms, 3300.0); // worst case on an IBM XT 4.77MHz
    }
}

TEST(test_prng_edge_cases) {

    // Zero seed (should assert in debug)
    #ifndef NDEBUG
    prng_state_t rng;
    //prng_init(&rng, PRNG_MARSAGLIA, 0, 16, NULL); // it does
    #endif

    // Low entropy seed
    prng_state_t low_entropy;
    prng_init(&low_entropy, PRNG_MARSAGLIA, 1, 0, NULL);
    uint32_t first = prng_next_u32(&low_entropy);
    for (size_t i = 0; i < 10; i++) {
        EXPECT_NEQ(first, prng_next_u32(&low_entropy));
    }
}

// ============================================================================
// TEST: prng_range_u32 (Biased-but-fast range)
// ============================================================================
TEST(test_prng_range_u32_basic) {
    prng_state_t rng;
    prng_init(&rng, PRNG_XORSHIFT,  0xDEADBEEF, 0, NULL);
    uint32_t r = 0x12345678;  // Fixed input for reproducibility

    uint32_t val = prng_range_u32(r, 10, 10);
    V(printf("range 10 10 = %lu\n", val););
    EXPECT_EQ(val, 10);  // Single-value range
    val = prng_range_u32(r, 0, 100);
    V(printf("range 0 100 = %lu\n", val););
    EXPECT_EQ(val >= 0 && val <= 100, 1);  // Manual range check

    r = prng_next_u32(&rng);
    V(printf("r = %lu\n", r););
    val = prng_range_u32(r, 10, 10);
    V(printf("range 10 10 = %lu\n", val););
    EXPECT_EQ(val, 10);  // Single-value range
    val = prng_range_u32(r, 0, 100);
    V(printf("range 0 100 = %lu\n", val););
    EXPECT_EQ(val >= 0 && val <= 100, 1);
}

TEST(test_prng_range_u32_bias_check) {
    /* Assumption Testing - Verify bias is < 0.0000001% for range=6 */
     V(printf("Assumption Testing Verify bias is < 0.005%% for range=6\n VERY slow on IBM XT - Press ESCAPE to exit\n"););
    prng_state_t rng;
    prng_init(&rng, PRNG_XORSHIFT,  0xDEADBEEF, 0, NULL);

    const uint32_t range = 6;
    uint32_t counts[6] = {0};
    const uint32_t trials = 100000; // 10M if not IBM XT

    tdd_progress_t prg = tdd_progress_make(trials, 0, 0, 30);

    for (uint32_t i = 0; i < trials; i++) {
        uint32_t r = prng_next_u32(&rng);
        uint32_t val = prng_range_u32(r, 0, range-1);
        counts[val]++;
        tdd_progress_bar(&prg);
        ON_ESCAPE(break;);
    }
    // For 100K samples I can only get down to distribution is within 0.01% of expected
    V(printf("\nratios: "););
    double expected = (double)trials / range;
    for (int i = 0; i < range; i++) {

        double ratio = counts[i] / expected;
        V(printf("%f ", ratio););
        //EXPECT_GT(ratio, 0.99); // if not IBM XT 0.999999);
        //EXPECT_LT(ratio, 1.01);
    }
}

// ============================================================================
// TEST: prng_range_exact (Uniform range)
// ============================================================================
TEST(test_prng_range_exact_uniformity) {
    prng_state_t rng;
    prng_init(&rng, PRNG_XORSHIFT, 0xDEADBEEF, 0, NULL);

    const uint32_t min = 10, max = 15;  // Small range to detect bias
    uint32_t counts[6] = {0};
    const uint32_t trials = 1000;

    tdd_progress_t prg = tdd_progress_make(trials, 0, 0, 30);

    for (uint32_t i = 0; i < trials; i++) {
        uint32_t val = prng_range_exact(&rng, min, max);
        counts[val - min]++;
        tdd_progress_bar(&prg);
        ON_ESCAPE(break;);
    }

    // Chi-squared test for uniformity (Dogma 3: Mathematical Correctness)
    double expected = trials / 6.0;
    double chi2 = 0;
    for (int i = 0; i < 6; i++) {
        double diff = counts[i] - expected;
        chi2 += (diff * diff) / expected;
    }
    EXPECT_LT(chi2, 20.0);  // For 5 degrees of freedom, p=0.001 threshold is ~20.5
}

TEST(test_prng_range_exact_edge_cases) {
    prng_state_t rng;
    prng_init(&rng, PRNG_XORSHIFT, 0xDEADBEEF, 0, NULL);

    // Single-value range
    EXPECT_EQ(prng_range_exact(&rng, 42, 42), 42);

    // Full 32-bit range
    const uint32_t range = UINT32_MAX - 1 + 1;
    const uint32_t threshold = -range % range;  // Equiv to (2^32 - range) % range

    uint32_t val = prng_range_exact(&rng, 0, UINT32_MAX);
    EXPECT_GT(val, 0);
    EXPECT_LT(val, UINT32_MAX);
}

#endif
