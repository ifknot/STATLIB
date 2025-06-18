#ifndef STAT_PRNG_H
#define STAT_PRNG_H

#include "stat_types.h"

// Initialize with Marsaglia's optimal seed
void stat_prng_init(stat_prng_state_t* state);

// Initialize with time-based seed
void stat_prng_init_time(stat_prng_state_t* state);

// Generate random double in [0, 1)
stat_float_t stat_prng_next(stat_prng_state_t* state);

// Generate random int32_t in [min, max]
int32_t stat_prng_next_int(stat_prng_state_t* state, int32_t min, int32_t max);

#endif // STAT_PRNG_H
