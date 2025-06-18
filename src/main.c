#include <stdlib.h>

#include "STAT/test_stats.h"

RUN_TESTS(
    STATS_TEST_SUITE
)

int main(int argc, char** argv) {

    return (run_tests()) ? EXIT_FAILURE : EXIT_SUCCESS;

}
