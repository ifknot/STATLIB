#include "prng.h"

#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "prng_constants.h"

// Engine Implementations...

static uint32_t marsaglia_next(prng_state_t* s) {
    uint32_t t = s->state.mwc.a * MWC_MULTIPLIER_A + (s->state.mwc.a >> 16);
    s->state.mwc.a = t;
    t = s->state.mwc.b * MWC_MULTIPLIER_B + (s->state.mwc.b >> 16);
    s->state.mwc.b = t;
    return (s->state.mwc.a << 16) + s->state.mwc.b;
}

static uint32_t xorshift_next(prng_state_t* s) {
    uint32_t t = s->state.xorshift.x[3];
    t ^= t << XORSHIFT_SHIFT_1;
    t ^= t >> XORSHIFT_SHIFT_2;
    s->state.xorshift.x[3] = s->state.xorshift.x[2];
    s->state.xorshift.x[2] = s->state.xorshift.x[1];
    s->state.xorshift.x[1] = s->state.xorshift.x[0];
    t ^= s->state.xorshift.x[0];
    t ^= s->state.xorshift.x[0] >> XORSHIFT_SHIFT_3;
    s->state.xorshift.x[0] = t;
    return t;
}

static uint32_t pcg32_next(prng_state_t* s) {
    uint64_t oldstate = s->state.pcg.state;
    s->state.pcg.state = oldstate * PCG_MULTIPLIER + PCG_INCREMENT;
    uint32_t xorshifted = ((oldstate >> (64 - PCG_ROTATE_BITS)) ^ oldstate) >> PCG_XSHIFT_BITS;
    uint32_t rot = oldstate >> PCG_ROTATE_BITS;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & PCG_OUTPUT_BITS));
}

static uint32_t splitmix_next(prng_state_t* s) {
    uint64_t z = (s->state.splitmix += SPLITMIX_INCREMENT);
    z = (z ^ (z >> SPLITMIX_SHIFT_1)) * SPLITMIX_MULT_1;
    z = (z ^ (z >> SPLITMIX_SHIFT_2)) * SPLITMIX_MULT_2;
    return z >> 32;
}

// Interface implementation...

/**
 * Generates a default seed using system time and process ID.
 * - Clock bits add per-process variability.
 * - Final hash ensures bit dispersion.
 * - Guaranteed non-zero (hash_seed() ensures this).
 */
uint64_t prng_default_seed(void) {
    uint64_t seed = ((uint64_t)time(NULL)) << SEED_TIME_SHIFT;
    seed ^= (uint64_t)clock() << (SEED_TIME_SHIFT / 2);
    return seed ? seed : SEED_GOLDEN_RATIO; // Fallback if clock=0
}

/**
 * Strict validation for engine-specific seed rules.
 * - Marsaglia MWC: Zero seeds cause degenerate states.
 * - XORShift: Low-entropy seeds produce weak initial outputs.
 */
bool prng_is_valid_seed(uint64_t seed, prng_engine_t engine) {
    switch (engine) {
        case PRNG_MARSAGLIA: return seed >= SEED_MWC_MIN;
        case PRNG_XORSHIFT:  return (seed & SEED_XSHIFT_MASK) != 0;
        default: return true;
    }
}

uint64_t prng_time_seed(void) {
    uint64_t seed = 0;

    // 1. Time since epoch (seconds)
    seed ^= (uint64_t)time(NULL) << 32;

    // 2. Processor ticks (best effort)
    seed ^= (uint64_t)clock();

    // 3. Stack address entropy (ASLR bits)
    volatile uintptr_t stack_addr = (uintptr_t)&seed;
    seed ^= (stack_addr << 16) | (stack_addr >> 16);

    // 4. Guaranteed non-zero
    return seed ? seed : GOLDEN_RATIO_64; // Golden ratio fallback
}

/**
 * Core initialization with explicit control.
 * - seed=0 triggers assertion (use prng_default_seed() instead).
 * - Warm-up rounds=0 skips warm-up entirely.
 * - Logging is opt-in only (seed_log=NULL disables).
 */
void prng_init(prng_state_t* state, prng_engine_t engine, uint64_t seed, int warmup_rounds, FILE* seed_log) {
    assert(state != NULL);
    assert(seed != 0); // Enforce explicit seeding

    state->engine = engine;

    switch(engine) {
        case PRNG_MARSAGLIA:
            seed = seed ? seed : MWC_DEFAULT_SEED;
            state->state.mwc.a = seed;
            state->state.mwc.b = seed + 1;
            state->state.mwc.c = seed + 2;
            state->state.mwc.d = seed + 3;
            break;

        case PRNG_XORSHIFT:
            seed = seed ? seed : MWC_DEFAULT_SEED;
            for (int i = 0; i < 4; i++)
                state->state.xorshift.x[i] = seed ^ (XORSHIFT_INIT_SEED + i*XORSHIFT_SEED_STEP);
            break;

        case PRNG_C99:
            if (seed) srand((unsigned)seed);
            break;

        case PRNG_PCG32:
            state->state.pcg.state = seed ? seed : MWC_DEFAULT_SEED;
            state->state.pcg.inc = PCG_INCREMENT;
            break;

        case PRNG_SPLITMIX:
            state->state.splitmix = seed ? seed : MWC_DEFAULT_SEED;
            break;
    }
}

uint32_t prng_next_u32(prng_state_t* state) {
    assert(state != NULL);

    switch(state->engine) {
        case PRNG_MARSAGLIA: return marsaglia_next(state);
        case PRNG_XORSHIFT:  return xorshift_next(state);
        case PRNG_C99:       return rand();
        case PRNG_PCG32:     return pcg32_next(state);
        case PRNG_SPLITMIX:  return splitmix_next(state);
        default:             return 0;
    }
}

double prng_next_float(prng_state_t* state) {
    return prng_next_u32(state) * (1.0 / 4294967296.0);//FLOAT_INV_2POW32    1.0 / FLOAT_2POW32
}

// Rejection sampling for strict uniformity
uint32_t prng_range_exact(prng_state_t* state, uint32_t min, uint32_t max) {
    if (min == 0 && max == UINT32_MAX) { // Special case: no need for range reduction
        return prng_next_u32(state);
    }
    const uint32_t range = max - min + 1;
    const uint32_t threshold = -range % range; // Equiv to (2^32 - range) % range
    uint32_t r;
    do {
        r = prng_next_u32(state);
    } while (r < threshold);

    return min + r % range;
}

void prng_dump_compact(const prng_state_t* state, FILE* output) {
    if (!state) return;
    FILE *out = output ? output : stdout;

    switch (state->engine) {
        case PRNG_MARSAGLIA:
            fprintf(out, "[PRNG] %s State:a=0x%08X b=0x%08X",
                   prng_to_string[state->engine],
                   state->state.mwc.a, state->state.mwc.b);
            break;

        case PRNG_XORSHIFT:
            fprintf(out, "[PRNG] %s State:0x%08X..0x%08X",
                   prng_to_string[state->engine],
                   state->state.xorshift.x[0],
                   state->state.xorshift.x[3]);
            break;

        case PRNG_C99:
            fprintf(out, "[PRNG] %s Seed:0x%08X",
                   prng_to_string[state->engine],
                   state->state.c99_seed);
            break;

        case PRNG_PCG32:
            fprintf(out, "[PRNG] %s State:0x%08X%08X Seq:0x%04X",
                   prng_to_string[state->engine],
                   (unsigned)(state->state.pcg.state >> 32),
                   (unsigned)state->state.pcg.state,
                   (unsigned)state->state.pcg.inc);
            break;

        case PRNG_SPLITMIX:
            fprintf(out, "[PRNG] %s State:0x%016llX",
                   prng_to_string[state->engine],
                   (unsigned long long)state->state.splitmix);
            break;
    }

    #ifdef PRNG_TRACK_WARMUP
    fprintf(out, " (Warmup:%d)", state->warmup_remaining);
    #endif

    fprintf(out, "\n");
}
