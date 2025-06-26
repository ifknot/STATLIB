#include "stat_binning.h"
#include "stat_basic.h"
#include "stat_percentiles.h"
#include "stat_compare.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>

binning_schema_t stat_binning_create_schema(stat_float_t min, stat_float_t max, stat_size_t bin_count, stat_binning_strategy_t strategy) {
    binning_schema_t schema = {
        .min = min,
        .max = max,
        .count = bin_count,
        .edges = malloc((bin_count + 1) * sizeof(stat_float_t))
    };
    assert(schema.edges && "MALLOC failure!");

    switch (strategy) {
        case BIN_LINEAR: {
            const stat_float_t step = (max - min) / bin_count;
            for (stat_size_t i = 0; i <= bin_count; i++) {
                schema.edges[i] = min + i * step;
            }
            break;
        }
        case BIN_LOGARITHMIC: {
            const stat_float_t log_min = log10(min + 1);
            const stat_float_t log_max = log10(max + 1);
            const stat_float_t log_step = (log_max - log_min) / bin_count;
            for (stat_size_t i = 0; i <= bin_count; i++) {
                schema.edges[i] = pow(10, log_min + i * log_step) - 1;
            }
            break;
        }
        default: assert(false && "INVALID binning strategy!");
    }

    return schema;
}

void stat_binning_free_schema(binning_schema_t* schema) {
    if (schema && schema->edges) {
        free(schema->edges);
        schema->edges = NULL;
    }
}

void stat_bin_values_i(const stat_int_t* values, stat_size_t count, const binning_schema_t* schema, stat_size_t* bins) {
    assert(values && "NULL values!");
    assert(schema && "NULL schema!");
    assert(bins && "Null bins!");
    
    const stat_float_t range = schema->max - schema->min;
    for (stat_size_t i = 0; i < count; i++) {
        stat_float_t normalized = (values[i] - schema->min) / range;
        stat_size_t bin = (stat_size_t)(normalized * schema->count);
        if (bin >= schema->count) bin = schema->count - 1;
        bins[bin]++;
    }
}

void stat_bin_values_f(const stat_float_t* values, stat_size_t count, const binning_schema_t* schema, stat_size_t* bins, stat_float_t epsilon) {
    assert(values && "NULL values!");
    assert(schema && "NULL schema!");
    assert(bins && "Null bins!");
    
    for (stat_size_t i = 0; i < count; i++) {
        stat_size_t bin = 0;
        if (stat_almost_equal(values[i], schema->max, epsilon, epsilon)) {
            bin = schema->count - 1;
        } else {
            stat_float_t normalized = (values[i] - schema->min) / (schema->max - schema->min);
            bin = (stat_size_t)(normalized * schema->count);
            if (bin >= schema->count) bin = schema->count - 1;
        }
        bins[bin]++;
    }
}

binning_schema_t stat_auto_bin_f(const stat_float_t* values, stat_size_t count, stat_size_t bin_count, stat_binning_strategy_t strategy) {
    assert(values && "NULL values!");
    assert(count && "ZERO values count!");
    assert(bin_count && "ZERO nins count!");
    
    stat_float_t min = stat_min_f(values, count);
    stat_float_t max = stat_max_f(values, count);
    
    if (strategy == BIN_PERCENTILE) {
        binning_schema_t schema = {
            .min = min,
            .max = max,
            .count = bin_count,
            .edges = malloc((bin_count + 1) * sizeof(stat_float_t))
        };
        assert(schema.edges);
        
        schema.edges[0] = min;
        schema.edges[bin_count] = max;
        
        // Calculate percentile points
        stat_float_t percentiles[bin_count - 1];
        stat_float_t cuts[bin_count - 1];
        for (stat_size_t i = 0; i < bin_count - 1; i++) {
            cuts[i] = 100.0 * (i + 1) / bin_count;
        }
        
        stat_percentiles_array_f(values, count, cuts, percentiles, bin_count - 1);
        
        for (stat_size_t i = 0; i < bin_count - 1; i++) {
            schema.edges[i + 1] = percentiles[i];
        }
        
        return schema;
    }
    
    return stat_binning_create_schema(min, max, bin_count, strategy);
}

stat_float_t stat_bin_center(const binning_schema_t* schema, stat_size_t bin_idx) {
    assert(schema && bin_idx < schema->count);
    return (schema->edges[bin_idx] + schema->edges[bin_idx + 1]) / 2;
}

stat_float_t stat_bin_width(const binning_schema_t* schema, stat_size_t bin_idx) {
    assert(schema && bin_idx < schema->count);
    return schema->edges[bin_idx + 1] - schema->edges[bin_idx];
}
