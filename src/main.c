#include <stdlib.h>

#include "PRNG/test_prng.h"
#include "STAT/test_stats.h"


RUN_TESTS(
    //STATS_TEST_ABS,
    //STATS_TEST_BASIC
    //STATS_TEST_CENTRAL,
    //STATS_TEST_CLAMP
    //STATS_TEST_COMPARE
    //PRNG_TEST_SUITE
    PRNG_TEST_RANGE
)

int main(int argc, char** argv) {

    return (run_tests()) ? EXIT_FAILURE : EXIT_SUCCESS;

}
