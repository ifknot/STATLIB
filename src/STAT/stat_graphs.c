#include "stat_graphs.h"
#include <stdio.h>
#include <assert.h>
#include "stat_basic.h"
#include "stat_types.h"

static void private_draw_bar(stat_size_t full_blocks, bool has_half_block) {
    for (stat_size_t i = 0; i < full_blocks; i++) {
        printf("%c", CP437_FULL_BLOCK);
    }
    if (has_half_block) {
        printf("%c", CP437_LEFT_HALF_BLOCK);
    }
}

void stat_graph_smooth_histogram(const stat_size_t* bins, const stat_binning_config_t* schema, stat_size_t max_height, bool show_bin_info
) {
    assert(bins && "NULL bins!");
    assert(schema && "NULL schema!");

    const stat_size_t max_count = stat_max_i((stat_int_t*)bins, schema->count);
    const stat_float_t scale = max_count ? (2.0 * max_height) / max_count : 0;

    for (stat_size_t i = 0; i < schema->count; i++) {
        if (show_bin_info) {
            printf("[%6.2f-%6.2f] ",
                   schema->edges[i],
                   schema->edges[i+1]);
        } else {
            printf("%3zu ", i);
        }

        const stat_float_t scaled = bins[i] * scale;
        const stat_size_t full = (stat_size_t)(scaled / 2);
        const bool half = (scaled - full * 2) >= 1.0;

        private_draw_bar(full, half);
        printf(" %zu\n", bins[i]);
    }
}
