#ifndef STAT_PRNG_H
#define STAT_PRNG_H

#include "stat_types.h"

#define STAT_MARSAGLIA_SEED 0x2545F4914F6CDD1DULL

/**
 * @brief PRNG algorithm options with performance/quality tradeoffs
 */
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
} stat_prng_state_t;

/**
 * @brief Initializes PRNG with specific engine
 * @param[out] state PRNG state to initialize
 * @param[in] engine Engine type from prng_engine_t
 * @param[in] seed Seed value (0 uses engine-specific defaults)
 * @note For C99: seed=0 leaves the generator unseeded
 */
void stat_prng_init(stat_prng_state_t* state, prng_engine_t engine, uint64_t seed);

/**
 * @brief Generates uniform float in [0,1)
 * @details All engines provide at least 32-bit precision
 */
stat_float_t stat_prng_float(stat_prng_state_t* state);

/**
 * @brief Generates full-range 32-bit unsigned
 */
uint32_t stat_prng_u32(stat_prng_state_t* state);

/**
 * @brief Gets engine metadata string
 * @return Format: "Name (Period=2^N, Speed=X.Yx)"
 */
const char* stat_prng_get_metadata(const stat_prng_state_t* state);

#endif // STAT_PRNG_H
