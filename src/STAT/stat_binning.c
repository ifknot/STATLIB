#include "stat_binning.h"
#include "stat_basic.h"
#include "stat_percentiles.h"
#include "stat_compare.h"
#include "stat_round.h"  // Include your rounding module
#include <math.h>
#include <assert.h>

// Updated implementation using your rounding functions
void stat_binning_calculate_edges(stat_binning_config_t* config, stat_binning_strategy_t strategy)  {
    assert(config && "NULL config");
    assert(config->edges && "Edges array not provided");
    assert(config->count > 0 && "Must have at least 1 bin");
    assert(config->max > config->min && "Invalid range");

    switch (strategy) {
        case BIN_LINEAR: {
            const stat_float_t step = (config->max - config->min) / config->count;
            for (stat_size_t i = 0; i <= config->count; i++) {
                // Use your rounding function for consistent behavior
                config->edges[i] = stat_round_decimal(config->min + i * step, 6);
            }
            break;
        }

        case BIN_LOGARITHMIC: {
            const stat_float_t log_min = log10(config->min + 1);
            const stat_float_t log_max = log10(config->max + 1);
            const stat_float_t log_step = (log_max - log_min) / config->count;
            for (stat_size_t i = 0; i <= config->count; i++) {
                // Round the logarithmic edges for consistency
                stat_float_t edge = pow(10, log_min + i * log_step) - 1;
                config->edges[i] = stat_round_decimal(edge, 6);
            }
            break;
        }

        default:
            assert(false && "Invalid binning strategy");
    }
}


void stat_bin_values_i(const stat_int_t* values, stat_size_t count, const stat_binning_config_t* config, stat_size_t* bins) {
    assert(values && "NULL values");
    assert(config && "NULL config");
    assert(config->edges && "NULL edges");
    assert(bins && "NULL bins");

    const stat_float_t range = config->max - config->min;
    for (stat_size_t i = 0; i < count; i++) {
        stat_float_t normalized = (values[i] - config->min) / range;
        stat_size_t bin = (stat_size_t)(normalized * config->count);
        if (bin >= config->count) bin = config->count - 1;
        bins[bin]++;
    }
}

void stat_auto_bin_f(const stat_float_t* values, stat_size_t count, stat_binning_config_t* config, stat_binning_strategy_t strategy) {
    assert(values && "NULL values");
    assert(count > 0 && "Empty dataset");
    assert(config && "NULL config");
    assert(config->edges && "NULL edges");
    assert(config->count > 0 && "Must have at least 1 bin");

    config->min = stat_min_f(values, count);
    config->max = stat_max_f(values, count);

    if (strategy == BIN_PERCENTILE) {
        config->edges[0] = config->min;
        config->edges[config->count] = config->max;

        // Calculate percentile cuts
        stat_float_t percentiles[config->count - 1];
        stat_float_t cuts[config->count - 1];
        for (stat_size_t i = 0; i < config->count - 1; i++) {
            cuts[i] = 100.0 * (i + 1) / config->count;
        }

        stat_percentiles_array_f(values, count, cuts, percentiles, config->count - 1);

        for (stat_size_t i = 0; i < config->count - 1; i++) {
            // Round percentile edges for consistency
            config->edges[i + 1] = stat_round_decimal(percentiles[i], 6);
        }
    } else {
        stat_binning_calculate_edges(config, strategy);
    }
}
void stat_auto_bin_f(const stat_float_t* values, stat_size_t count, stat_binning_config_t* config, stat_binning_strategy_t strategy) {
    assert(values && "NULL values");
    assert(count > 0 && "Empty dataset");
    assert(config && "NULL config");
    assert(config->edges && "NULL edges");
    assert(config->count > 0 && "Must have at least 1 bin");

    config->min = stat_min_f(values, count);
    config->max = stat_max_f(values, count);

    if (strategy == BIN_PERCENTILE) {
        config->edges[0] = config->min;
        config->edges[config->count] = config->max;

        // Calculate percentile cuts
        stat_float_t percentiles[config->count - 1];
        stat_float_t cuts[config->count - 1];
        for (stat_size_t i = 0; i < config->count - 1; i++) {
            cuts[i] = 100.0 * (i + 1) / config->count;
        }

        stat_percentiles_array_f(values, count, cuts, percentiles, config->count - 1);

        for (stat_size_t i = 0; i < config->count - 1; i++) {
            config->edges[i + 1] = percentiles[i];
        }
    } else {
        stat_binning_calculate_edges(config, strategy);
    }
}

stat_float_t stat_bin_center(const stat_binning_config_t* config, stat_size_t bin_idx) {
    assert(config && "NULL config");
    assert(bin_idx < config->count && "Bin index out of range");
    return (config->edges[bin_idx] + config->edges[bin_idx + 1]) / 2;
}

stat_float_t stat_bin_width(const stat_binning_config_t* config, stat_size_t bin_idx) {
    assert(config && "NULL config");
    assert(bin_idx < config->count && "Bin index out of range");
    return config->edges[bin_idx + 1] - config->edges[bin_idx];
}
