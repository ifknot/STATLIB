#include "stat_prng.h"
#include <time.h>
//#include <limits.h>

#define STAT_MARSAGLIA_SEED 0x2545F4914F6CDD1DULL

void stat_prng_init(stat_prng_state_t* state) {
    state->x = STAT_MARSAGLIA_SEED;
}

void stat_prng_init_time(stat_prng_state_t* state) {
    uint64_t seed = (uint64_t)time(NULL);
    // Mix in additional entropy sources when possible
    //seed ^= (uint64_t)__rdtsc();  // Use CPU timestamp counter if available
    //seed ^= (uint64_t)clock();    // Fallback to process clock
    //state->x = seed ? seed : STAT_MARSAGLIA_SEED;

    // Warm-up the generator
    for (int i = 0; i < 10; i++) {
        stat_prng_next(state);
    }
}

stat_float_t stat_prng_next(stat_prng_state_t* state) {
    state->x ^= state->x >> 12;
    state->x ^= state->x << 25;
    state->x ^= state->x >> 27;
    return (stat_float_t)((state->x * STAT_MARSAGLIA_SEED) >> 11) * (1.0 / (UINT64_C(1) << 53));
}

int32_t stat_prng_next_int(stat_prng_state_t* state, int32_t min, int32_t max) {
    uint64_t range = (uint64_t)max - min + 1;
    uint64_t scaled = (uint64_t)(stat_prng_next(state) * range);
    return min + (int32_t)scaled;
}
