#ifndef STAT_GRAPHS_H
#define STAT_GRAPHS_H

#include <math.h>

#include "cp437_constants.h"
#include "stat_types.h"
#include "stat_basic.h"
#include "stat_binning.h"

/**
 * @brief ASCII histograms for stat_int_t and stat_float_t  using full+half blocks to double density and smoother appearance
 * @example
 * [Log Scale Histogram]
 * Max:1000  Med:50.0  P25:10.0  P75:100.0
 * 0 █ 1
 * 1 ██ 10
 * 2 █████ 100
 * 3 ███ 50
 * 4 ████████████ 1000
 */
void stat_graph_smooth_histogram(
    const stat_size_t* bins,
    const stat_binning_config_t* config,
    stat_size_t max_height,
    bool show_bin_info
);

#endif // STAT_GRAPHS_H
