#ifndef STAT_PRNG_H
#define STAT_PRNG_H

#include "stat_types.h"
#include <stdint.h>

#define STAT_MARSAGLIA_SEED 0x2545F4914F6CDD1DULL

/**
 * @brief PRNG state structure (32 bytes)
 */
typedef struct {
    uint32_t state[8];  // 256-bit state (C99-compatible)
} stat_prng_state_t;

/**
 * @brief Initializes PRNG with a 32-bit seed
 * @param[out] state PRNG state to initialize
 * @param[in] seed Seed value (0 uses time() as seed)
 * @note Uses time(0) if seed=0 (not cryptographically secure)
 */
void stat_prng_init(stat_prng_state_t* state, uint32_t seed);

void stat_prng_init_time(stat_prng_state_t* state);

/**
 * @brief Generates uniform float in [0,1)
 * @param[in,out] state PRNG state
 * @return Random float in [0,1)
 * @assert Fails if state=NULL
 */
stat_float_t stat_prng_float(stat_prng_state_t* state);

/**
 * @brief Generates uniform uint32 in [0,UINT32_MAX]
 * @param[in,out] state PRNG state
 * @return Random uint32
 * @assert Fails if state=NULL
 */
uint32_t stat_prng_u32(stat_prng_state_t* state);

#endif // STAT_PRNG_H
