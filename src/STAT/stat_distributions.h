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
 * @code{.c}
 * // Example: Generate 1000 uniform random numbers between 5.0 and 10.0
 * prng_state_t rng;
 * prng_init(&rng, PRNG_XORSHIFT, prng_default_seed(), 16, NULL);
 * stat_float_t samples[1000];
 * stat_generate_uniform_dist(samples, 1000, 5.0, 10.0, &rng);
 *
 * // Print first 10 samples
 * for (int i = 0; i < 10; i++) {
 *     printf("%f ", samples[i]);
 * }
 * @endcode
 */
void stat_generate_uniform_dist(
    stat_float_t* output,
    stat_size_t size,
    stat_float_t min,
    stat_float_t max,
    prng_state_t* state
);

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
 * @code{.c}
 * // Example: Generate 500 normally distributed numbers with mean=0 and std_dev=1
 * prng_state_t rng
 * prng_init(&rng, PRNG_XORSHIFT, prng_default_seed(), 16, NULL);
 * stat_float_t normal_samples[500];
 * stat_generate_normal_dist(normal_samples, 500, 0.0, 1.0, &rng);
 *
 * // Calculate sample mean (should be close to 0)
 * stat_float_t sum = 0.0;
 * for (int i = 0; i < 500; i++) {
 *     sum += normal_samples[i];
 * }
 * printf("Sample mean: %f\n", sum / 500);
 * @endcode
 */
void stat_generate_normal_dist(
    stat_float_t* output,
    stat_size_t size,
    stat_float_t mean,
    stat_float_t std_dev,
    prng_state_t* state
);

/**
 * @brief Generates exponential distribution with rate lambda
 * @param[out] output Pre-allocated output array
 * @param[in] size Number of samples
 * @param[in] lambda Rate parameter (must be > 0)
 * @param[in,out] state PRNG state
 * @throws EINVAL if size=0 or lambda <= 0
 * @assert Fails if output or state is NULL
 * @note Uses inverse transform sampling
 * @code{.c}
 * // Example: Generate exponential waiting times with rate 0.5 (mean=2.0)
 * prng_state_t rng;
 * prng_init(&rng, PRNG_XORSHIFT, prng_default_seed(), 16, NULL);
 * stat_float_t exp_samples[200];
 * stat_generate_exponential_dist(exp_samples, 200, 0.5, &rng);
 *
 * // Count how many samples are > 4.0 (should be about e^-2 ≈ 13.5%)
 * int count = 0;
 * for (int i = 0; i < 200; i++) {
 *     if (exp_samples[i] > 4.0) count++;
 * }
 * printf("Percentage >4.0: %.1f%%\n", (count/200.0)*100.0);
 * @endcode
 */
void stat_generate_exponential_dist(
    stat_float_t* output,
    stat_size_t size,
    stat_float_t lambda,
    prng_state_t* state
);

/**
 * @brief Generates Poisson distribution Pois(lambda)
 * @param[out] output Pre-allocated output array
 * @param[in] size Number of samples
 * @param[in] lambda Rate parameter (must be > 0)
 * @param[in,out] state PRNG state
 * @throws EINVAL if size=0 or lambda <= 0
 * @assert Fails if output or state is NULL
 * @note Uses Knuth's algorithm
 * @code{.c}
 * // Example: Simulate event counts with average rate of 3.5 events per interval
 * prng_state_t rng;
 * prng_init(&rng, PRNG_XORSHIFT, prng_default_seed(), 16, NULL);
 * stat_size_t poisson_samples[1000];
 * stat_generate_poisson_dist(poisson_samples, 1000, 3.5, &rng);
 *
 * // Calculate empirical probability of 5 events
 * int count5 = 0;
 * for (int i = 0; i < 1000; i++) {
 *     if (poisson_samples[i] == 5) count5++;
 * }
 * printf("Empirical P(X=5): %.3f\n", count5/1000.0);
 * @endcode
 */
void stat_generate_poisson_dist(
    stat_size_t* output,
    stat_size_t size,
    stat_float_t lambda,
    prng_state_t* state
);

/**
 * @brief Generates binomial distribution B(n, p)
 * @param[out] output Pre-allocated output array
 * @param[in] size Number of samples
 * @param[in] n Number of trials
 * @param[in] p Success probability [0,1]
 * @param[in,out] state PRNG state
 * @throws EINVAL if size=0, n=0, or p outside [0,1]
 * @assert Fails if output or state is NULL
 * @code{.c}
 * // Example: Simulate 50 coin flips (n=50, p=0.5) repeated 200 times
 * prng_state_t rng;
 * prng_init(&rng, PRNG_XORSHIFT, prng_default_seed(), 16, NULL);
 * stat_size_t binomial_samples[200];
 * stat_generate_binomial_dist(binomial_samples, 200, 50, 0.5, &rng);
 *
 * // Calculate how often we got exactly 25 heads
 * int count25 = 0;
 * for (int i = 0; i < 200; i++) {
 *     if (binomial_samples[i] == 25) count25++;
 * }
 * printf("Got exactly 25 heads in %d of 200 trials\n", count25);
 * @endcode
 */
void stat_generate_binomial_dist(
    stat_size_t* output,
    stat_size_t size,
    stat_size_t n,
    stat_float_t p,
    prng_state_t* state
);

#endif // STAT_DISTRIBUTIONS_H
