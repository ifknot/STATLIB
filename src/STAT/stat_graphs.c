#include "stat_graphs.h"

#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "stat_basic.h"
#include "stat_percentiles.h"
#include "stat_types.h"

static void calculate_bin_edges(
    stat_float_t* bin_edges,
    stat_float_t min,
    stat_float_t max, stat_size_t bin_count,
    bool log_scale
) {
    assert(bin_edges && "NULL bin_edges!");
    assert(bin_count > 0 && "Zero bins requested!");

    if (log_scale) {
        const stat_float_t log_min = log10(min + 1);
        const stat_float_t log_max = log10(max + 1);
        const stat_float_t log_step = (log_max - log_min) / bin_count;

        for (stat_size_t i = 0; i <= bin_count; ++i) {
            bin_edges[i] = pow(10, log_min + i * log_step) - 1;
            assert(!isnan(bin_edges[i]) && "Invalid log scale bin edge");
        }
    } else {
        const stat_float_t step = (max - min) / bin_count;
        for (stat_size_t i = 0; i <= bin_count; ++i) {
            bin_edges[i] = min + i * step;
            assert(!isnan(bin_edges[i]) && "Invalid linear bin edge");
        }
    }
}

static void draw_histogram(
    const stat_size_t* bin_counts,
    stat_size_t bin_count,
    stat_size_t max_rows,
    bool is_integer
) {
    const stat_size_t max_bin = stat_max_i((stat_int_t*)bin_counts, bin_count);
    const stat_float_t scale = (2.0 * max_rows) / (max_bin ? max_bin : 1);

    for (stat_size_t i = 0; i < bin_count; ++i) {
        printf("%3zu ", i);

        const stat_float_t scaled = bin_counts[i] * scale;
        const stat_size_t full_units = (stat_size_t)(scaled / 2);
        const bool has_half_unit = (scaled - full_units * 2) >= 1.0;

        for (stat_size_t j = 0; j < full_units; ++j) {
                printf("%c", CP437_FULL_BLOCK);
            }
            if (has_half_unit) {
                printf("%c", CP437_LEFT_HALF_BLOCK);
            }

        if (is_integer) {
            printf(" %zu\n", bin_counts[i]);
        } else {
            printf(" %.2f\n", (stat_float_t)bin_counts[i]);
        }
    }
}

void stat_graph_smooth_histogram_i(
    const stat_int_t* values,
    stat_size_t count,
    stat_size_t max_rows,
    bool show_stats,
    bool log_scale
) {
    assert(values && "NULL values!");
    assert(count && "ZERO values!");
    assert(max_rows && "ZERO columns!");

    // Calculate stats
    const stat_int_t min_val = stat_min_i(values, count);
    const stat_int_t max_val = stat_max_i(values, count);

    stat_float_t percentiles[3];
    const stat_float_t cut_offs[] = {25, 50, 75};
    stat_percentiles_array_i(values, count, cut_offs, percentiles, 3);

    if (show_stats) {
        printf("\nMax:%d  P25:%.1f  Med:%.1f  P75:%.1f\n\n",
               max_val, percentiles[0], percentiles[1], percentiles[2]);
    }

    // Allocate and calculate bin edges
    stat_float_t* bin_edges = malloc((max_rows + 1) * sizeof(stat_float_t));
    assert(bin_edges && "Failed to allocate bin edges");
    calculate_bin_edges(bin_edges, min_val, max_val, max_rows, log_scale);

    // Count values in bins
    stat_size_t* bin_counts = malloc(max_rows * sizeof(stat_size_t));
    for (stat_size_t i = 0; i < max_rows; ++i) {
        bin_counts[i] = 0;
    }

    for (stat_size_t i = 0; i < count; ++i) {
        const stat_float_t val = log_scale ? log10(values[i] + 1) : values[i];
        const stat_float_t min_edge = log_scale ? log10(bin_edges[0] + 1) : bin_edges[0];
        const stat_float_t max_edge = log_scale ? log10(bin_edges[max_rows] + 1) : bin_edges[max_rows];
        const stat_float_t normalized = (val - min_edge) / (max_edge - min_edge);
        stat_size_t bin_idx = (stat_size_t)(normalized * max_rows);

        if (bin_idx >= max_rows) bin_idx = max_rows - 1;
        bin_counts[bin_idx]++;
    }

    draw_histogram(bin_counts, max_rows, max_rows, true);
    free(bin_edges);
    free(bin_counts);
}

void stat_graph_smooth_histogram_f(
    const stat_float_t* values,
    stat_size_t count,
    stat_size_t max_rows,
    bool show_stats,
    bool log_scale
) {
    assert(values && count && max_rows);

    // Calculate stats
    const stat_float_t min_val = stat_min_f(values, count);
    const stat_float_t max_val = stat_max_f(values, count);
    const stat_float_t range = max_val - min_val;

    // Allocate bins
    stat_size_t* bin_counts = calloc(max_rows, sizeof(stat_size_t));
    assert(bin_counts);

    // Bin the values directly without explicit edges
    for (stat_size_t i = 0; i < count; i++) {
        const stat_float_t val = values[i];
        stat_float_t normalized = (val - min_val) / range;

        // Calculate bin index with careful edge handling
        stat_size_t bin_idx = (stat_size_t)(normalized * max_rows);

        // Handle values exactly at max_val
        if (val == max_val) {
            bin_idx = max_rows - 1;
        }
        // Handle other edge cases
        else if (bin_idx >= max_rows) {
            bin_idx = max_rows - 1;
        }

        bin_counts[bin_idx]++;
    }

    // Display results
    if (show_stats) {
        printf("\nRange: [%.2f, %.2f]  Bins: %zu\n\n", min_val, max_val, max_rows);
    }

    // Print the actual bin ranges for verification
    for (stat_size_t i = 0; i < max_rows; i++) {
        stat_float_t bin_start = min_val + (range * i) / max_rows;
        stat_float_t bin_end = min_val + (range * (i+1)) / max_rows;
        printf("Bin %2zu: [%.2f, %.2f) %zu\n",
               i, bin_start, bin_end, bin_counts[i]);
    }
}
