#include "stat_prng.h"
#include <assert.h>
#include <time.h>

// 32-bit version of xoshiro128** (C99-compatible)
static inline uint32_t rotl32(uint32_t x, int k) {
    return (x << k) | (x >> (32 - k));
}

static uint32_t xoshiro128_next(stat_prng_state_t* state) {
    const uint32_t result = rotl32(state->state[1] * 5, 7) * 9;
    const uint32_t t = state->state[1] << 9;

    state->state[2] ^= state->state[0];
    state->state[3] ^= state->state[1];
    state->state[1] ^= state->state[2];
    state->state[0] ^= state->state[3];

    state->state[2] ^= t;
    state->state[3] = rotl32(state->state[3], 11);

    return result;
}

void stat_prng_init(stat_prng_state_t* state, uint32_t seed) {
    assert(state != NULL && "PRNG state cannot be NULL");
    assert(seed != 0 && "ZERO seed forbidden!");

    // Simple seeding (Watcom-compatible)
    state->state[0] = seed ^ 0xBADF00D;
    state->state[1] = seed ^ 0xCAFEBABE;
    state->state[2] = seed ^ 0xDEADBEEF;
    state->state[3] = seed ^ 0xFEEDFACE;

    // Warm-up (10 iterations)
    for (int i = 0; i < 10; i++) {
        xoshiro128_next(state);
    }
}

void stat_prng_init_time(stat_prng_state_t* state) {
    uint32_t seed = (uint32_t)time(NULL);
    stat_prng_init(state, seed);
}

stat_float_t stat_prng_float(stat_prng_state_t* state) {
    assert(state != NULL && "PRNG state cannot be NULL");
    // 23-bit precision float in [0,1)
    return (xoshiro128_next(state) >> 9) * (1.0f / (1U << 23));
}

uint32_t stat_prng_u32(stat_prng_state_t* state) {
    assert(state != NULL && "PRNG state cannot be NULL");
    return xoshiro128_next(state);
}
