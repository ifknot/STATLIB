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
     */
    PRNG_MARSAGLIA,

    /**
     * @brief XORShift128** (shift-register based)
     * @period 2^128-1
     * @speed 1.3x
     * @quality Excellent statistical properties
     * @recommended_for High-performance simulations
     */
    PRNG_XORSHIFT,

    /**
     * @brief Standard C99 rand() (LCG implementation)
     * @period Typically 2^31-1
     * @speed 0.8x
     * @quality Poor, for compatibility only
     * @recommended_for Comparison/testing only
     */
    PRNG_C99,

    /**
     * @brief PCG32 (Permuted Congruential Generator)
     * @period 2^64
     * @speed 1.1x
     * @quality Excellent, passes BigCrush
     * @recommended_for Cryptography-free security
     */
    PRNG_PCG32,

    /**
     * @brief SplitMix64 (splittable generator)
     * @period 2^64
     * @speed 1.2x
     * @quality Good, not for cryptography
     * @recommended_for Parallel streams
     */
    PRNG_SPLITMIX

} prng_engine_t;

const prng_engine_t prng_engine_list[PRNG_ENGINE_COUNT] = {
    PRNG_MARSAGLIA,
    PRNG_XORSHIFT,
    PRNG_PCG32,
    PRNG_SPLITMIX,
    PRNG_C99
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
    const char* engine_name;
    uint32_t period_log2;  // log2 of period
} prng_state_t;


/**
 * Generates a default seed using system time and process ID.
 * - Uses high-resolution clock bits if available.
 * - Seed is hashed for better bit dispersion.
 * - Guarantees non-zero return value.
 *
 * Example:
 *   uint64_t seed = prng_default_seed(); // 0x3A7B...
 */
uint64_t prng_default_seed(void);

/**
 * Validates a seed for a specific PRNG engine.
 * - Returns true if seed is acceptable for the engine.
 * - For Marsaglia MWC: seed must be >= SEED_MWC_MIN (avoids zero-state).
 * - For XORShift: seed must not have low 4 bits all zero (avoids low entropy).
 * - Other engines: always returns true.
 *
 * Example:
 *   if (!prng_is_valid_seed(0, PRNG_MARSAGLIA)) // false
 */
bool prng_is_valid_seed(uint64_t seed, prng_engine_t engine);

/**
 * Barebones C99 implementation using only standard library.
 * Combines:
 * - Time since epoch (seconds + nanoseconds)
 * - Stack address ASLR bits
 * - Clock ticks (if available)
 *
 * Not cryptographically secure but sufficient for PRNG seeding.
 */
uint64_t prng_time_seed(void);

/**
 * Initializes a PRNG state with explicit parameters.
 * - Requires: state != NULL, seed != 0 (asserts otherwise).
 * - Warm-up rounds: clamped to [0, WARMUP_MAX].
 * - seed_log: if non-NULL, writes seed metadata to the specified FILE.
 * - Never auto-seeds or modifies inputs silently.
 *
 * Example:
 *   prng_init(&rng, PRNG_XORSHIFT, 0xABCD, 16, stderr);
 *   // Logs: "[PRNG] Engine=xorshift Seed=0xABCD..."
 */
void prng_init(prng_state_t* state, prng_engine_t engine, uint64_t seed, int warmup_rounds, FILE* seed_log);

/**
 * @brief Generates uniform float in [0,1)
 * @details All engines provide at least 32-bit precision
 */
double prng_next_float(prng_state_t* state);

/**
 * @brief Generates full-range 32-bit unsigned
 */
uint32_t prng_next_u32(prng_state_t* state);

/**
 * @brief Gets engine metadata string
 * @return Format: "Name (Period=2^N, Speed=X.Yx)"
 */
const char* prng_get_metadata(const prng_state_t* state);


#endif
