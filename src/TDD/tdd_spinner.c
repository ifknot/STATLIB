#include "tdd_spinner.h"

#include <stdio.h>
#include <string.h>

#include "tdd_constants.h"

tdd_spinner_t tdd_spinner_make(const char* frames) {
    tdd_spinner_t spinner;
    spinner.counter = 0;
    spinner.frames = (frames) ? frames : TDD_SPINNER_DEFAULT_CHARS;
    return spinner;
}

void tdd_spinner_step(tdd_spinner_t* spinner) {
    printf("\r%c", spinner->frames[spinner->counter++ % strlen(spinner->frames)]);
    fflush(stdout);
}

void tdd_spinner_clear(tdd_spinner_t* spinner) {
    printf("\r \r"); // Clear spinner
    fflush(stdout);
}

/** @} */ // end of test_utils group
