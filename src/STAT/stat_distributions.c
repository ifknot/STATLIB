#include "stat_distributions.h"
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stddef.h>

#include "stat_constants.h"
#include "../PRNG/prng.h"

void stat_generate_uniform_dist(
    stat_float_t* output,
    stat_size_t size,
    stat_float_t min,
    stat_float_t max,
    prng_state_t* state
) {
    assert(output != NULL && "Output array cannot be NULL");
    assert(state != NULL && "PRNG state cannot be NULL");

    if (size == 0 || min >= max) {
        errno = EINVAL;
        return;
    }

    const stat_float_t range = max - min;
    for (stat_size_t i = 0; i < size; i++) {
        printf("%f ", prng_next_float(state));
        output[i] = min + prng_next_float(state) * range;
    }
}

void stat_generate_normal_dist(
    stat_float_t* output,
    stat_size_t size,
    stat_float_t mean, stat_float_t std_dev,
    prng_state_t* state
) {
    assert(output != NULL && "Output array cannot be NULL");
    assert(state != NULL && "PRNG state cannot be NULL");

    if (size == 0 || std_dev < 0) {
        errno = EINVAL;
        return;
    }

    // Box-Muller transform (generates pairs)
    for (stat_size_t i = 0; i < size; i += 2) {
        const stat_float_t u1 = prng_next_float(state);
        const stat_float_t u2 = prng_next_float(state);

        const stat_float_t mag = std_dev * sqrt(-2.0f * log(u1));
        const stat_float_t z0 = mag * cos(TWO_PI * u2) + mean;
        const stat_float_t z1 = mag * sin(TWO_PI * u2) + mean;

        output[i] = z0;
        if (i + 1 < size) output[i+1] = z1;
    }
}

void stat_generate_exponential_dist(
    stat_float_t* output,
    stat_size_t size,
    stat_float_t lambda,
    prng_state_t* state
) {
    assert(output != NULL && "Output array cannot be NULL");
    assert(state != NULL && "PRNG state cannot be NULL");

    if (size == 0 || lambda <= 0) {
        errno = EINVAL;
        return;
    }

    for (stat_size_t i = 0; i < size; i++) {
        const stat_float_t u = prng_next_float(state);
        output[i] = -log(1.0f - u) / lambda;
    }
}

void stat_generate_poisson_dist(
    stat_size_t* output,
    stat_size_t size,
    stat_float_t lambda,
    prng_state_t* state
) {
    assert(output != NULL && "Output array cannot be NULL");
    assert(state != NULL && "PRNG state cannot be NULL");

    if (size == 0 || lambda <= 0) {
        errno = EINVAL;
        return;
    }

    const stat_float_t exp_lambda = exp(-lambda);

    for (stat_size_t i = 0; i < size; i++) {
        stat_size_t k = 0;
        stat_float_t p = 1.0f;

        do {
            p *= prng_next_float(state);
            k++;
        } while (p > exp_lambda);

        output[i] = k - 1;
    }
}

void stat_generate_binomial_dist(
    stat_size_t* output,
    stat_size_t size,
    stat_size_t n,
    stat_float_t p,
    prng_state_t* state
) {
    assert(output != NULL && "Output array cannot be NULL");
    assert(state != NULL && "PRNG state cannot be NULL");

    if (size == 0 || n == 0 || p < 0 || p > 1) {
        errno = EINVAL;
        return;
    }

    for (stat_size_t i = 0; i < size; i++) {
        stat_size_t successes = 0;
        for (stat_size_t j = 0; j < n; j++) {
            if (prng_next_float(state) < p) successes++;
        }
        output[i] = successes;
    }
}
