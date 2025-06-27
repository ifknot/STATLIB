#include <stdlib.h>

//#include "PRNG/test_prng.h"
#include "STAT/test_stats.h"
//#include "STAT/test_graphs.h"

RUN_TESTS(
    ABS_TEST_SUITE//,
    //STATS_TEST_BASIC
    //STATS_TEST_CENTRAL,
    //STATS_TEST_CLAMP
    //STATS_TEST_COMPARE
    //GRAPHS_TEST_SUITE
    //PRNG_TEST_SUITE,
    //PRNG_TEST_RANGE,
    //PRNG_TEST_SUITE_FP
)

int main(int argc, char** argv) {

    return (run_tests()) ? EXIT_FAILURE : EXIT_SUCCESS;

}
