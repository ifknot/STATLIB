#ifndef TDD_GRAPHS_H
#define TDD_GRAPHS_H

#include <stdio.h>
#include <assert.h>

#include "tdd_constants.h"
#include "tdd_types.h"

void tdd_histogram(const tdd_size_t* values, tdd_size_t count, tdd_size_t width) {
    assert(count && "ZERO values!");
    assert(values && "NULL values!");
    assert(width && "ZERO max width!");

    uint32_t max = values[0];
    for (tdd_size_t i = 1; i < count; i++) {
        if (values[i] > max) max = values[i];
    }
    printf("\nHistogram\n");
    for (tdd_size_t i = 0; i < count; i++) {
        tdd_size_t bar_length = (tdd_size_t)(values[i] * width / max); // Calculate bar length (scaled to max_width)
        printf("%3zu ", i);
        for (tdd_size_t j = 0; j < bar_length; j++) {
            putchar(CP437_UPPER_HALF_BLOCK);
        }
        printf(" %u\n", values[i]);
    }
}

#endif
