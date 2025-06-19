#ifndef STAT_GRAPHS_H
#define STAT_GRAPHS_H

#include <math.h>

#include "cp437_constants.h"
#include "stat_types.h"
#include "stat_basic.h" 

/**
 * @brief ASCII histograms for stat_int_t and stat_float_t  using full+half blocks to double density and smoother appearance 
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
void stat_graph_smooth_histogram_i(
    const stat_int_t* values,
    stat_size_t count,
    stat_size_t max_cols,
    bool show_stats,
    bool log_scale
);

void stat_graph_smooth_histogram_f(
    const stat_float_t* values,
    stat_size_t count,
    stat_size_t max_cols,
    bool show_stats,
    bool log_scale
);

#endif
