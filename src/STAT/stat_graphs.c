#include "stat_graphs.h"

#include <assert.h>
#include <stdio.h>

#include "stat_basic.h"
#include "stat_percentiles.h"
#include "cp437_constants.h"

void private_draw_bar(stat_size_t nblocks, bool is_half) {
    for (stat_size_t j = 0; j < nblocks; ++j)  {
        printf("%c", CP437_FULL_BLOCK);
    }
    if (is_half) {
        printf("%c", CP437_LEFT_HALF_BLOCK);
    }
}

void stat_graph_smooth_histogram_i(
    const stat_int_t* values,
    stat_size_t count,
    stat_size_t max_cols,
    bool show_stats,
    bool log_scale
) {
    assert(values && "NULL values!");
    assert(count && "ZERO values!");
    assert(max_cols && "ZERO columns!");

    stat_int_t max_val = stat_max_i(values, count);
    //assert(max_val && "ALL bins empty!");

    if(log_scale) {
        stat_float_t scale = (2.0 * max_cols / log10(max_val + 1));
    } else {
        stat_float_t scale =(2.0 * max_cols / (stat_float_t)max_val);
    }

    if (show_stats) {
        stat_float_t percentiles[3];
        const stat_float_t cut_offs[] = {25, 50, 75};
        stat_percentiles_array_i(values, count, (stat_float_t*)&cut_offs, (stat_float_t*)&percentiles, 3);
        printf("\nMax:%u  P25:%.1f  Med:%.1f  P75:%.1f\n\n", max_val, percentiles[0], percentiles[1], percentiles[2]);
    }

    stat_float_t scale = log_scale ?
        (2 * max_cols / log10(max_val + 1)) :
        (2 * max_cols / max_val);

    for (stat_size_t i = 0; i < count; ++i) {
        printf("%3zu ", i);

        stat_float_t val = log_scale ? log10(values[i] + 1) : (stat_float_t)values[i];
        stat_float_t scaled = val * scale;
        stat_size_t full_units = (stat_size_t)(scaled / 2);
        bool has_half_unit = (scaled - full_units * 2) >= 1.0;

        private_draw_bar(full_units, has_half_unit);
        printf(" %u\n", values[i]);
    }
}

void stat_graph_smooth_histogram_f(
    const stat_float_t* values,
    stat_size_t count,
    stat_size_t max_cols,
    bool show_stats,
    bool log_scale
) {
    assert(values && "NULL values!");
    assert(count && "ZERO values!");
    assert(max_cols && "ZERO columns!");

    // Calculate max value and percentiles
    stat_float_t max_val = values[0];
    for (stat_size_t i = 1; i < count; i++) {
        if (values[i] > max_val) max_val = values[i];
    }

    stat_float_t percentiles[3];
    stat_size_t cut_offs[] = {25, 50, 75};
    stat_percentiles_array_f(values, count, (stat_float_t*)&cut_offs, (stat_float_t*)&percentiles, 3);

    // Header
    printf("\n");
    if (show_stats) {
        printf("Max:%.2f  P25:%.2f  Med:%.2f  P75:%.2f\n\n",
               max_val, percentiles[0], percentiles[1], percentiles[2]);
    }

    // Scaling (2x resolution)
    stat_float_t scale = log_scale ?
        (2 * max_cols / log10(max_val + 1)) :
        (2 * max_cols / max_val);

    for (stat_size_t i = 0; i < count; i++) {
        printf("%2zu ", i); // Bin index

        stat_float_t val = log_scale ?
            log10(values[i] + 1) :
            values[i];

        stat_float_t scaled = val * scale;
        stat_size_t full_units = (stat_size_t)(scaled / 2);
        bool has_half_unit = (scaled - full_units * 2) >= 1.0;

        private_draw_bar(full_units, has_half_unit);
        printf(" %.2f\n", values[i]); // Show float value
    }
}
