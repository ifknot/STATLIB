#ifndef PRNG_H
#define PRNG_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "prng_constants.h"

typedef enum {
    /**
     * @brief Marsaglia's MWC (Multiply With Carry)
     * @period ~2^60
     * @speed 1.0x (reference)
     * @quality Good for general statistics
     * @recommended_for Monte Carlo, game physics
     * @code{.c}
     * // Example: Basic initialization and usage
     * prng_state_t rng;
     * prng_init(&rng, PRNG_MARSAGLIA, 0xDEADBEEF, 16, NULL); // 16 warmup rounds no seed logging
     * double val = prng_next_float(&rng);  // [0,1) random value
     * @endcode
     */
    PRNG_MARSAGLIA,

    /**
     * @brief XORShift128** (shift-register based)
     * @period 2^128-1
     * @speed 1.3x
     * @quality Excellent statistical properties
     * @recommended_for High-performance simulations
     * @code{.c}
     * // Example: Generating multiple values
     * prng_state_t rng;
     * prng_init(&rng, PRNG_XORSHIFT, prng_default_seed(), 32, stderr); // 32 warmup rounds log seed to stderr
     * for (int i = 0; i < 10; i++) {
     *     printf("%u\n", prng_next_u32(&rng));
     * }
     * @endcode
     */
    PRNG_XORSHIFT,

    /**
     * @brief Standard C99 rand() (LCG implementation)
     * @period Typically 2^31-1
     * @speed 0.8x
     * @quality Poor, for compatibility only
     * @recommended_for Comparison/testing only
     * @code{.c}
     * // Example: Compatibility mode
     * prng_state_t rng;
     * prng_init(&rng, PRNG_C99, 12345, 0, NULL); // no warmup, no seed logging
     * // Matches standard rand() output:
     * printf("%d\n", (int)(prng_next_float(&rng) * RAND_MAX));
     * @endcode
     */
    PRNG_C99,

    /**
     * @brief PCG32 (Permuted Congruential Generator)
     * @period 2^64
     * @speed 1.1x
     * @quality Excellent, passes BigCrush
     * @recommended_for Cryptography-free security
     * @code{.c}
     * // Example: Seeding with system time
     * prng_state_t rng;
     * prng_init(&rng, PRNG_PCG32, prng_time_seed(), 64, NULL);
     * uint32_t secure_val = prng_next_u32(&rng);
     * @endcode
     */
    PRNG_PCG32,

    /**
     * @brief SplitMix64 (splittable generator)
     * @period 2^64
     * @speed 1.2x
     * @quality Good, not for cryptography
     * @recommended_for Parallel streams
     * @code{.c}
     * // Example: Parallel stream initialization
     * prng_state_t rng1, rng2;
     * uint64_t base_seed = prng_default_seed();
     * prng_init(&rng1, PRNG_SPLITMIX, base_seed, 16, NULL);
     * prng_init(&rng2, PRNG_SPLITMIX, base_seed + 1, 16, NULL);
     * // Now rng1 and rng2 produce independent streams
     * @endcode
     */
    PRNG_SPLITMIX

} prng_engine_t;

/**
 * @brief Array of all available PRNG engines
 * @code{.c}
 * // Example: Testing all engines
 * for (size_t i = 0; i < PRNG_ENGINE_COUNT; i++) {
 *     prng_state_t rng;
 *     prng_init(&rng, prng_engine_list[i], 42, 16, NULL);
 *     printf("Engine %zu: %f\n", i, prng_next_float(&rng));
 * }
 * @endcode
 */
const prng_engine_t prng_engine_list[PRNG_ENGINE_COUNT] = {
    PRNG_MARSAGLIA,
    PRNG_XORSHIFT,
    PRNG_C99,
    PRNG_PCG32,
    PRNG_SPLITMIX,
};

typedef struct {
    union {
        // Marsaglia MWC
        struct { uint32_t a, b, c, d; } mwc;
        // XORShift128
        struct { uint32_t x[4]; } xorshift;
        // C99 (uses global state)
        uint32_t c99_seed;
        // PCG32
        struct { uint64_t state; uint64_t inc; } pcg;
        // SplitMix
        uint64_t splitmix;
    } state;
    prng_engine_t engine;
} prng_state_t;

/**
 * @brief Generates a default seed using system time and process ID
 * @details Uses high-resolution clock bits if available. Seed is hashed for better
 *          bit dispersion. Guarantees non-zero return value.
 * @return uint64_t A non-zero seed value
 * @code{.c}
 * // Example: Typical usage with distributions
 * prng_state_t rng;
 * prng_init(&rng, PRNG_XORSHIFT, prng_default_seed(), 16, NULL);
 *
 * stat_float_t normal_samples[100];
 * stat_generate_normal_dist(normal_samples, 100, 0.0, 1.0, &rng);
 * @endcode
 */
uint64_t prng_default_seed(void);

/**
 * @brief Validates a seed for a specific PRNG engine
 * @param seed The seed value to validate
 * @param engine The PRNG engine to check against
 * @return true if seed is acceptable for the engine
 * @code{.c}
 * // Example: Seed validation
 * uint64_t seed = 0;
 * if (!prng_is_valid_seed(seed, PRNG_MARSAGLIA)) {
 *     seed = prng_default_seed(); // Fallback to valid seed
 * }
 * @endcode
 */
bool prng_is_valid_seed(uint64_t seed, prng_engine_t engine);

/**
 * @brief Generates a seed value using system time
 * @return uint64_t A time-based seed value
 * @note Combines time since epoch, stack address bits, and clock ticks
 * @code{.c}
 * // Example: Quick initialization
 * prng_state_t rng;
 * prng_init(&rng, PRNG_PCG32, prng_time_seed(), 0, NULL);
 * @endcode
 */
uint64_t prng_time_seed(void);

/**
 * @brief Initializes a PRNG state with explicit parameters
 * @param state Pointer to PRNG state to initialize
 * @param engine The PRNG algorithm to use
 * @param seed The seed value (must not be zero)
 * @param warmup_rounds Number of initialization rounds (clamped to [0, WARMUP_MAX])
 * @param seed_log Optional FILE pointer for logging seed information
 * @code{.c}
 * // Example: Full initialization with logging
 * prng_state_t rng;
 * FILE *log = fopen("rng_log.txt", "w");
 * prng_init(&rng, PRNG_MARSAGLIA, 0xCAFEBABE, 32, log);
 * fclose(log);
 *
 * // Generate some values
 * for (int i = 0; i < 10; i++) {
 *     printf("%f\n", prng_next_float(&rng));
 * }
 * @endcode
 */
void prng_init(prng_state_t* state, prng_engine_t engine, uint64_t seed, int warmup_rounds, FILE* seed_log);

/**
 * @brief Generates uniform float in [0,1)
 * @param state Initialized PRNG state
 * @return double Random value in [0,1) range
 * @code{.c}
 * // Example: Generating probabilities
 * prng_state_t rng;
 * prng_init(&rng, PRNG_PCG32, 123, 0, NULL);
 *
 * if (prng_next_float(&rng) < 0.3) {
 *     printf("30%% event occurred!\n");
 * }
 * @endcode
 */
double prng_next_float(prng_state_t* state);

/**
 * @brief Generates full-range 32-bit unsigned value
 * @param state Initialized PRNG state
 * @return uint32_t Random 32-bit value
 * @code{.c}
 * // Example: Generating random IDs
 * prng_state_t rng;
 * prng_init(&rng, PRNG_XORSHIFT, 456, 0, NULL);
 *
 * uint32_t session_id = prng_next_u32(&rng);
 * printf("New session ID: %08X\n", session_id);
 * @endcode
 */
uint32_t prng_next_u32(prng_state_t* state);

/**
 * @brief Generates a biased-but-fast random number within [min, max]
 * @param r 32-bit random input (from prng_next_u32())
 * @param min Lower bound (inclusive)
 * @param max Upper bound (inclusive)
 * @return uint32_t Random value in range
 * @note For range=6, bias per value is ~0.0000001%
 * @code{.c}
 * // Example: Fast dice roll simulation
 * prng_state_t rng;
 * prng_init(&rng, PRNG_MARSAGLIA, 789, 0, NULL);
 *
 * uint32_t die_roll = prng_range_u32(prng_next_u32(&rng), 1, 6);
 * printf("You rolled a %u\n", die_roll);
 * @endcode
 */
static inline uint32_t prng_range_u32(uint32_t r, uint32_t min, uint32_t max) {
    uint32_t range = max - min + 1;
    return min + ((uint64_t)r * range >> 32);
}

/**
 * @brief Generates perfectly uniform random number in [min, max]
 * @param state Initialized PRNG state
 * @param min Lower bound (inclusive)
 * @param max Upper bound (inclusive)
 * @return uint32_t Random value in range
 * @note Uses rejection sampling for perfect uniformity
 * @code{.c}
 * // Example: Shuffling a deck of cards
 * prng_state_t rng;
 * prng_init(&rng, PRNG_PCG32, 987, 0, NULL);
 *
 * uint8_t deck[52];
 * for (int i = 0; i < 52; i++) deck[i] = i + 1;
 *
 * // Fisher-Yates shuffle using exact range
 * for (int i = 51; i > 0; i--) {
 *     uint32_t j = prng_range_exact(&rng, 0, i);
 *     uint8_t temp = deck[i];
 *     deck[i] = deck[j];
 *     deck[j] = temp;
 * }
 * @endcode
 */
uint32_t prng_range_exact(prng_state_t* state, uint32_t min, uint32_t max);

/**
 * @brief Dumps PRNG state in compact single-line format
 * @param state Initialized PRNG state (must not be NULL)
 * @param output FILE stream (NULL = stdout)
 * @code{.c}
 * // Example outputs:
 * // [PRNG] Marsaglia's MWC State:a=0x12345678 b=0x9ABCDEF0
 * // [PRNG] PCG32 State:0x4D3C0A3B00000001 Seq:0x0001
 * @endcode
 */
void prng_dump(const prng_state_t* state, FILE* output);

#endif
