/**
 * @file tdd_progress.h
 * @brief Test progress reporting utilities
 * @defgroup test_utils Test Utilities
 * @{
 */
#ifndef TDD_PROGRESS_H
#define TDD_PROGRESS_H

#include "tdd_types.h"
#include "tdd_on_escape.h"

typedef struct {
    tdd_size_t total;
    tdd_size_t current;
    tdd_size_t width;       // bar width - 0 if not a width widget
    tdd_size_t step;        // step size for percent or rotation - if 0 step is calculated
} tdd_progress_t;

tdd_progress_t tdd_progress_make(tdd_size_t total, tdd_size_t current, tdd_size_t step, tdd_size_t width);

void tdd_progress_bar(tdd_progress_t* p);

void tdd_progress_percent(tdd_progress_t* p);

#endif

/** @} */ // end of test_utils group
