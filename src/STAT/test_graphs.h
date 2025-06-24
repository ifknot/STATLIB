#ifndef TEST_GRAPHS_H
#define TEST_GRAPHS_H

#include <stdlib.h>

#include "../TDD/tdd_macros.h"
#include "../STAT/stat_graphs.h"
#include "../STAT/stat_distributions.h"
#include "stat_types.h"

#define GRAPHS_TEST_SUITE &graph_uniform

stat_float_t* allocate_samples(size_t count) {
    stat_float_t* mem = (stat_float_t*)malloc(count * sizeof(stat_float_t));
    if (!mem) {
        fprintf(stderr, "Failed to allocate %zu samples\n", count);
        return NULL;
    }
    return mem;
}

TEST(graph_uniform) {
    prng_state_t rng;
    //prng_init(&rng, PRNG_XORSHIFT, prng_default_seed(), 16, NULL);
    stat_float_t* samples = allocate_samples(1000);

    prng_init(&rng, PRNG_C99, 0xDEADBEEF, 16, NULL);

    stat_generate_uniform_dist(samples, 1000, 5.0, 10.0, &rng);
    // Print first 10 samples
    for (int i = 0; i < 10; i++) {
         printf("%f ", samples[i]);
    }

    free(samples);
}



#endif
