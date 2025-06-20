#include "stat_prng.h"
#include <assert.h>
#include <string.h>

/* ===================== *
 * Engine-Specific Constants
 * ===================== */

// Marsaglia MWC
#define MWC_MULTIPLIER_A  36969U
#define MWC_MULTIPLIER_B  18000U
#define MWC_MULTIPLIER_C  29000U
#define MWC_MULTIPLIER_D  30970U
#define MWC_DEFAULT_SEED  1U     // Default seed if 0 provided

// XORShift128
#define XORSHIFT_INIT_SEED  0xBADF00D
#define XORSHIFT_SEED_STEP  0x11111111
#define XORSHIFT_SHIFT_1    11U
#define XORSHIFT_SHIFT_2    8U
#define XORSHIFT_SHIFT_3    19U

// PCG32
#define PCG_MULTIPLIER      6364136223846793005ULL
#define PCG_INCREMENT       1442695040888963407ULL  // Default stream selector
#define PCG_ROTATE_BITS     59U
#define PCG_XSHIFT_BITS     27U
#define PCG_OUTPUT_BITS     31U

// SplitMix
#define SPLITMIX_INCREMENT  0x9e3779b97f4a7c15ULL
#define SPLITMIX_MULT_1     0xbf58476d1ce4e5b9ULL
#define SPLITMIX_MULT_2     0x94d049bb133111ebULL
#define SPLITMIX_SHIFT_1    30U
#define SPLITMIX_SHIFT_2    27U

// Float generation
#define FLOAT_PRECISION_BITS 24U  // Bits for float mantissa
#define FLOAT_SHIFT_BITS    8U    // (32 - FLOAT_PRECISION_BITS)

/* ===================== *
 * Engine Implementations
 * ===================== */

static uint32_t marsaglia_next(stat_prng_state_t* s) {
    uint32_t t = s->state.mwc.a * MWC_MULTIPLIER_A + (s->state.mwc.a >> 16);
    s->state.mwc.a = t;
    t = s->state.mwc.b * MWC_MULTIPLIER_B + (s->state.mwc.b >> 16);
    s->state.mwc.b = t;
    return (s->state.mwc.a << 16) + s->state.mwc.b;
}

static uint32_t xorshift_next(stat_prng_state_t* s) {
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

static uint32_t pcg32_next(stat_prng_state_t* s) {
    uint64_t oldstate = s->state.pcg.state;
    s->state.pcg.state = oldstate * PCG_MULTIPLIER + PCG_INCREMENT;
    uint32_t xorshifted = ((oldstate >> (64 - PCG_ROTATE_BITS)) ^ oldstate) >> PCG_XSHIFT_BITS;
    uint32_t rot = oldstate >> PCG_ROTATE_BITS;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & PCG_OUTPUT_BITS));
}

static uint32_t splitmix_next(stat_prng_state_t* s) {
    uint64_t z = (s->state.splitmix += SPLITMIX_INCREMENT);
    z = (z ^ (z >> SPLITMIX_SHIFT_1)) * SPLITMIX_MULT_1;
    z = (z ^ (z >> SPLITMIX_SHIFT_2)) * SPLITMIX_MULT_2;
    return z >> 32;
}

/* ===================== *
 * Metadata and Core Functions
 * ===================== */

static const struct {
    const char* name;
    uint32_t period_log2;
    float speed_factor;
} engine_metadata[] = {
    [PRNG_MARSAGLIA] = {"Marsaglia MWC", 60, 1.0f},
    [PRNG_XORSHIFT]  = {"XORShift128**", 128, 1.3f},
    [PRNG_C99]       = {"C99 rand()", 31, 0.8f},
    [PRNG_PCG32]     = {"PCG32", 64, 1.1f},
    [PRNG_SPLITMIX]  = {"SplitMix64", 64, 1.2f}
};

void stat_prng_init(stat_prng_state_t* state, prng_engine_t engine, uint64_t seed) {
    assert(state != NULL);
    
    state->engine = engine;
    state->engine_name = engine_metadata[engine].name;
    state->period_log2 = engine_metadata[engine].period_log2;
    
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

uint32_t stat_prng_u32(stat_prng_state_t* state) {
    assert(state != NULL);
    
    switch(state->engine) {
        case PRNG_MARSAGLIA: return marsaglia_next(state);
        case PRNG_XORSHIFT:  return xorshift_next(state);
        case PRNG_C99:       return rand();
        case PRNG_PCG32:     return pcg32_next(state);
        case PRNG_SPLITMIX:  return splitmix_next(state);
        default:            return 0;
    }
}

stat_float_t stat_prng_float(stat_prng_state_t* state) {
    return (stat_prng_u32(state) >> FLOAT_SHIFT_BITS) * 
           (1.0f / (1U << FLOAT_PRECISION_BITS));
}

const char* stat_prng_get_metadata(const stat_prng_state_t* state) {
    static char buf[64];
    snprintf(buf, sizeof(buf), "%s (Period=2^%u, Speed=%.1fx)",
             state->engine_name, state->period_log2, 
             engine_metadata[state->engine].speed_factor);
    return buf;
}
