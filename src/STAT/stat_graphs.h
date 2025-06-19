#ifndef STAT_GRAPHS_H
#define STAT_GRAPHS_H

#include <math.h>

#include "cp437_constants.h"
#include "stat_types.h"
#include "stat_basic.h" 

/**
 * @brief High-resolution DOS histogram using full+half blocks
 * @param values Array of bin counts - uint32_t
 * @param count Number of bins
 * @param max_cols Max display columns
 * @param show_stats Show distribution stats
 * @param log_scale Use logarithmic scaling
 * @example
 * [Log Scale Histogram]
 * Max:1000  Med:50.0  P25:10.0  P75:100.0
 * 0 █ 1
 * 1 ██ 10
 * 2 █████ 100
 * 3 ███ 50
 * 4 ████████████ 1000
 */
void stat_graph_histogram_u32(
    const uint32_t* values,
    stat_size_t count,
    stat_size_t max_cols,
    bool show_stats,
    bool log_scale
) {
    assert(values && "NULL values!");
    assert(count && "ZERO values!");
    assert(max_cols && "ZERO columns!");

    uint32_t max_val = stat_max_u32(values, count);
    //assert(max_val && "ALL bins empty!");

    if(log_scale) {
        stat_float_t scale = (2.0 * max_cols / log10(max_val + 1));
    } else {
        stat_float_t scale =(2.0 * max_cols / (stat_float_t)max_val);
    }

    if (show_stats) {
        stat_float_t percentiles[3];
        stat_percentiles(values, count, (stat_float_t[]){25,50,75}, percentiles, 3);
        printf("\nMax:%u  P25:%.1f  Med:%.1f  P75:%.1f\n\n", max_val, percentiles[0], percentiles[1], percentiles[2]);
    }

    for (stat_size_t i = 0; i < count; ++i) {
        printf("%3zu ", i); 
        
        stat_float_t val = log_scale ? log10(values[i] + 1) : (stat_float_t)values[i];
        stat_float_t scaled = val * scale;
        stat_size_t full_units = (stat_size_t)(scaled / 2);
        bool has_half_unit = (scaled - full_units * 2) >= 1.0;

        for (stat_size_t j = 0; j < full_units; ++j)  {
            printf(CP437_FULL_BLOCK);
        }
        if (has_half_unit) {
            printf(CP437_LEFT_HALF);
        }
        printf(" %u\n", values[i]);
    }

#endif
