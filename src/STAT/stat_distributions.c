#include "stat_distributions.h"
#include <math.h>

void stat_generate_uniform_dist(stat_float_t* output, stat_size_t size,
                              stat_float_t min, stat_float_t max,
                              stat_prng_state_t* state) {
    stat_float_t range = max - min;
    for (stat_size_t i = 0; i < size; i++) {
        output[i] = min + stat_prng_next(state) * range;
    }
}

void stat_generate_normal_dist(stat_float_t* output, stat_size_t size,
                             stat_float_t mean, stat_float_t std_dev,
                             stat_prng_state_t* state) {
    for (stat_size_t i = 0; i < size; i += 2) {
        stat_float_t u1 = stat_prng_next(state);
        stat_float_t u2 = stat_prng_next(state);
        stat_float_t z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        stat_float_t z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);
        output[i] = mean + std_dev * z0;
        if (i+1 < size) output[i+1] = mean + std_dev * z1;
    }
}

void stat_generate_exponential_dist(stat_float_t* output, stat_size_t size,
                                  stat_float_t lambda, stat_prng_state_t* state) {
    for (stat_size_t i = 0; i < size; i++) {
        stat_float_t u = stat_prng_next(state);
        output[i] = -log(1 - u) / lambda;
    }
}
