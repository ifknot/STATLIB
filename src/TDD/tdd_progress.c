#include "tdd_progress.h"

#include <stdio.h>
#include <assert.h>

tdd_progress_t tdd_progress_make(tdd_size_t total, tdd_size_t current, tdd_size_t step, tdd_size_t width) {
    tdd_progress_t progress;
    progress.total = total;
    progress.current = current;
    progress.step = (step) ? total / step : total / width;
    progress.width = width;
    return progress;
}

void tdd_progress_bar(tdd_progress_t* progress) {
    assert(progress->current < progress->total);
    if((progress->current++) % progress->step > 0 && progress->current < progress->total) {
        return;    // current mod step != 0
    }
    printf("\r[");
    tdd_size_t limit = (progress->current * progress->width) / progress->total;
    for (tdd_size_t i = 0; i < progress->width; i++) {
        putchar(i < limit ? 0xB0 : ' ');
    }
    printf("] %3lu%%", (tdd_size_t)((progress->current * 100) / progress->total));
    fflush(stdout);
}

void tdd_progress_percent(tdd_progress_t* progress) {
    assert(progress->current < progress->total);
    if((progress->current++) % progress->step > 0 && progress->current < progress->total) {
        return;    // current mod step != 0
    }
    printf("\r %3lu%%", (tdd_size_t)((progress->current * 100) / progress->total));
    fflush(stdout);
}
