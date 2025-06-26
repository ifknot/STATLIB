#ifndef STAT_BINNING_H
#define STAT_BINNING_H

#include "stat_types.h"

/**
 * @file stat_binning.h
 * @brief Data binning utilities with explicit memory management
 * 
 * All binning operations require the caller to manage all memory allocation.
 * The library only computes bin edges and assignments.
 */

typedef enum {
    BIN_LINEAR,      ///< Equal-width bins
    BIN_LOGARITHMIC, ///< Logarithmically spaced bins
    BIN_PERCENTILE   ///< Bins with equal data counts
} stat_binning_strategy_t;

/**
 * @brief Binning configuration structure
 * @note edges array must be allocated by caller with (count+1) elements
 */
typedef struct {
    stat_float_t min;    ///< Minimum bin edge
    stat_float_t max;    ///< Maximum bin edge
    stat_size_t count;   ///< Number of bins
    stat_float_t* edges; ///< Caller-allocated array of bin edges
} stat_binning_config_t;

/**
 * @brief Computes bin edges for given range and strategy
 * @param[out] config Pre-initialized config with allocated edges array
 * @param min Minimum value of range
 * @param max Maximum value of range
 * @param strategy Binning strategy to use
 * 
 * @code
 * // User-managed memory example
 * stat_binning_config_t cfg;
 * cfg.min = 0;
 * cfg.max = 100;
 * cfg.count = 10;
 * cfg.edges = malloc((cfg.count+1) * sizeof(stat_float_t));
 * 
 * stat_binning_calculate_edges(&cfg, BIN_LINEAR);
 * 
 * // Use edges...
 * free(cfg.edges);
 * @endcode
 */
void stat_binning_calculate_edges(
    stat_binning_config_t* config,
    stat_binning_strategy_t strategy
);

/**
 * @brief Bins integer values into pre-allocated bins
 * @param values Input values to bin
 * @param count Number of values
 * @param config Binning configuration
 * @param[out] bins Caller-allocated array (size=config->count)
 * 
 * @code
 * stat_int_t data[1000] = {...};
 * stat_binning_config_t cfg = {...};
 * stat_size_t bins[20] = {0}; // Stack-allocated bins
 * 
 * stat_bin_values_i(data, 1000, &cfg, bins);
 * @endcode
 */
void stat_bin_values_i(
    const stat_int_t* values,
    stat_size_t count,
    const stat_binning_config_t* config,
    stat_size_t* bins
);

/**
 * @brief Bins float values with epsilon comparison
 * @param epsilon Tolerance for edge comparisons
 * 
 * @code
 * stat_float_t measurements[500] = {...};
 * stat_binning_config_t cfg = {...};
 * stat_size_t bins[15] = {0};
 * 
 * stat_bin_values_f(measurements, 500, &cfg, bins, 1e-10f);
 * @endcode
 */
void stat_bin_values_f(
    const stat_float_t* values,
    stat_size_t count,
    const stat_binning_config_t* config,
    stat_size_t* bins,
    stat_float_t epsilon
);

/**
 * @brief Computes automatic bin edges from data
 * @param[out] config Initialized config with allocated edges array
 * 
 * @code
 * stat_float_t dataset[2000] = {...};
 * stat_binning_config_t cfg;
 * cfg.count = 10;
 * cfg.edges = malloc((cfg.count+1) * sizeof(stat_float_t));
 * 
 * stat_auto_bin_f(dataset, 2000, &cfg, BIN_PERCENTILE);
 * 
 * // Remember to free edges when done
 * free(cfg.edges);
 * @endcode
 */
void stat_auto_bin_f(
    const stat_float_t* values,
    stat_size_t count,
    stat_binning_config_t* config,
    stat_binning_strategy_t strategy
);

/* Utility functions (remain unchanged) */
stat_float_t stat_bin_center(const stat_binning_config_t* config, stat_size_t bin_idx);
stat_float_t stat_bin_width(const stat_binning_config_t* config, stat_size_t bin_idx);

#endif // STAT_BINNING_H
