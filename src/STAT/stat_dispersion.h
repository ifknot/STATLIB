#ifndef STAT_DISTRIBUTIONS_H
#define STAT_DISTRIBUTIONS_H

#include "stat_types.h"
#include "../PRNG/prng.h"
#include <stdbool.h>

/**
 * @brief Generates uniform distribution in [min, max]
 * @param[out] output Pre-allocated output array
 * @param[in] size Number of samples
 * @param[in] min Lower bound (inclusive)
 * @param[in] max Upper bound (inclusive)
 * @param[in,out] state PRNG state (must be initialized)
 * @throws EINVAL if size=0 or min >= max
 * @assert Fails if output or state is NULL
 */
void stat_generate_uniform_dist(stat_float_t* output, stat_size_t size,
                              stat_float_t min, stat_float_t max,
                              prng_state_t* state);

/**
 * @brief Generates normal distribution N(mean, std_dev^2)
 * @param[out] output Pre-allocated output array
 * @param[in] size Number of samples
 * @param[in] mean Distribution mean
 * @param[in] std_dev Standard deviation
 * @param[in,out] state PRNG state
 * @throws EINVAL if size=0 or std_dev < 0
 * @assert Fails if output or state is NULL
 * @note Uses Box-Muller transform
 */
void stat_generate_normal_dist(stat_float_t* output, stat_size_t size,
                             stat_float_t mean, stat_float_t std_dev,
                             prng_state_t* state);

/**
 * @brief Generates exponential distribution with rate lambda
 * @param[out] output Pre-allocated output array
 * @param[in] size Number of samples
 * @param[in] lambda Rate parameter (must be > 0)
 * @param[in,out] state PRNG state
 * @throws EINVAL if size=0 or lambda <= 0
 * @assert Fails if output or state is NULL
 * @note Uses inverse transform sampling
 */
void stat_generate_exponential_dist(stat_float_t* output, stat_size_t size,
                                  stat_float_t lambda, prng_state_t* state);

/**
 * @brief Generates Poisson distribution Pois(lambda)
 * @param[out] output Pre-allocated output array
 * @param[in] size Number of samples
 * @param[in] lambda Rate parameter (must be > 0)
 * @param[in,out] state PRNG state
 * @throws EINVAL if size=0 or lambda <= 0
 * @assert Fails if output or state is NULL
 * @note Uses Knuth's algorithm
 */
void stat_generate_poisson_dist(stat_size_t* output, stat_size_t size,
                              stat_float_t lambda, prng_state_t* state);

/**
 * @brief Generates binomial distribution B(n, p)
 * @param[out] output Pre-allocated output array
 * @param[in] size Number of samples
 * @param[in] n Number of trials
 * @param[in] p Success probability [0,1]
 * @param[in,out] state PRNG state
 * @throws EINVAL if size=0, n=0, or p outside [0,1]
 * @assert Fails if output or state is NULL
 */
void stat_generate_binomial_dist(stat_size_t* output, stat_size_t size,
                               stat_size_t n, stat_float_t p,
                               prng_state_t* state);

#endif // STAT_DISTRIBUTIONS_H
