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
//#include "../TDD/tdd_graphs.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// =============================================
// Test Suite Declaration
// =============================================

#define PRNG_TEST_SUITE &test_prng_popcount, \
    &test_prng_basic,         \
    //&test_prng_distributions,  \
    //&test_prng_monte_carlo,    \
    &test_prng_bit_quality,    \
    &test_prng_performance,    \
    &test_prng_edge_cases

// =============================================
// Test Cases
// =============================================

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
        27, 1824507900,
        1256175887, 3730336304,
        5720, 14404
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
    const prng_engine_t engines[] = {PRNG_MARSAGLIA, PRNG_XORSHIFT, PRNG_PCG32};
    const size_t num_buckets = 32;
    const size_t samples = 10000;

    for (size_t e = 0; e < sizeof(engines)/sizeof(engines[0]); e++) {
        prng_state_t rng;
        prng_init(&rng, engines[e], 0xCAFE + e, 16, NULL);

        uint32_t buckets[32] = {0};
        for (size_t i = 0; i < samples; i++) {
            buckets[prng_next_u32(&rng) % num_buckets]++;
        }

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
    const prng_engine_t engines[] = {PRNG_MARSAGLIA, PRNG_XORSHIFT, PRNG_PCG32};
    const size_t samples = 10000;

    for (size_t e = 0; e < sizeof(engines)/sizeof(engines[0]); e++) {
        prng_state_t rng;
        prng_init(&rng, engines[e], 0x1234 + e, 16, NULL);

        size_t hits = 0;
        for (size_t i = 0; i < samples; i++) {
            double x = prng_next_u32(&rng) / (double)UINT32_MAX;
            double y = prng_next_u32(&rng) / (double)UINT32_MAX;
            if (x*x + y*y <= 1.0) hits++;
        }

        double pi_est = 4.0 * hits / samples;
        double error = fabs(pi_est - M_PI) / M_PI * 100.0;

        EXPECT_IN_RANGE(error, 0.0, 5.0);
    }
}

TEST(test_prng_bit_quality) {
    const prng_engine_t engines[] = {PRNG_MARSAGLIA, PRNG_XORSHIFT};

    for (size_t e = 0; e < sizeof(engines)/sizeof(engines[0]); e++) {
        prng_state_t rng;
        prng_init(&rng, engines[e], 0xABCD + e, 16, NULL);

        uint32_t last = prng_next_u32(&rng);
        size_t transitions = 0;
        for (size_t i = 0; i < 1000; i++) {
            uint32_t current = prng_next_u32(&rng);
            transitions += prng_popcount(last ^ current);
            last = current;
        }

        double avg_trans = transitions / 1000.0;
        EXPECT_IN_RANGE(avg_trans, 14.0, 18.0);
        //tdd_graph_sparkline("Bit Flips", (int)(transitions/100), 10);
    }
}

TEST(test_prng_performance) {
    const prng_engine_t engines[] = {PRNG_MARSAGLIA, PRNG_XORSHIFT, PRNG_PCG32};
    const size_t iterations = 100000;

    for (size_t e = 0; e < sizeof(engines)/sizeof(engines[0]); e++) {
        prng_state_t rng;
        prng_init(&rng, engines[e], 0xFEED, 0, NULL);

        clock_t start = clock();
        uint32_t dummy = 0;
        for (size_t i = 0; i < iterations; i++) {
            dummy ^= prng_next_u32(&rng);
        }
        clock_t end = clock();
        (void)dummy;

        double ms = (end - start) * 1000.0 / CLOCKS_PER_SEC;
        EXPECT_LT(ms, 1000.0);
    }
}

TEST(test_prng_edge_cases) {

    // Zero seed (should assert in debug)
    #ifndef NDEBUG
    prng_state_t rng;
    prng_init(&rng, PRNG_MARSAGLIA, 0, 16, NULL);
    #endif

    // Low entropy seed
    prng_state_t low_entropy;
    prng_init(&low_entropy, PRNG_MARSAGLIA, 1, 0, NULL);
    uint32_t first = prng_next_u32(&low_entropy);
    for (size_t i = 0; i < 10; i++) {
        EXPECT_NEQ(first, prng_next_u32(&low_entropy));
    }
}

#endif
