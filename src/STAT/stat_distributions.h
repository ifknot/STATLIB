#ifndef STAT_DISTRIBUTIONS_H
#define STAT_DISTRIBUTIONS_H

#include "stat_constants.h"
#include "stat_types.h"
#include "stat_prng.h"  // Now depends on PRNG module

// Uniform distribution in [min, max]
void stat_generate_uniform_dist(stat_float_t* output, stat_size_t size,
                              stat_float_t min, stat_float_t max,
                              stat_prng_state_t* state);

// Normal distribution N(mean, std_dev^2)
void stat_generate_normal_dist(stat_float_t* output, stat_size_t size,
                             stat_float_t mean, stat_float_t std_dev,
                             stat_prng_state_t* state);

// Exponential distribution with rate lambda
void stat_generate_exponential_dist(stat_float_t* output, stat_size_t size,
                                  stat_float_t lambda, stat_prng_state_t* state);

#endif // STAT_DISTRIBUTIONS_H
