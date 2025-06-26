#ifndef STAT_BINNING_H
#define STAT_BINNING_H

#include "stat_types.h"

typedef enum {
    BIN_LINEAR,
    BIN_LOGARITHMIC
} stat_binning_strategy_t;

typedef struct {
    stat_float_t min;
    stat_float_t max;
    stat_size_t count;
    stat_float_t* edges;  // Array of count+1 edges
} stat_binning_schema_t;

// Core binning functions
stat_binning_schema_t stat_binning_create_schema(
  stat_float_t min, 
  stat_float_t max, 
  stat_size_t bin_count, 
  stat_binning_strategy_t strategy
);

void stat_binning_free_schema(stat_binning_schema_t* schema);

// Integer value binning
void stat_bin_values_i(
  const stat_int_t* values, 
  stat_size_t count,
  const stat_binning_schema_t* schema, 
  stat_size_t* bins
);

// Float value binning (with epsilon comparison)
void stat_bin_values_f(
  const stat_float_t* values, 
  stat_size_t count,
  const stat_binning_schema_t* schema, 
  stat_size_t* bins,
  stat_float_t epsilon
);

// Automatic binning from data
stat_binning_schema_t stat_auto_bin_f(
  const stat_float_t* values, 
  stat_size_t count,
  stat_size_t bin_count, 
  stat_binning_strategy_t strategy
);

// Utility functions
stat_float_t stat_bin_center(const stat_binning_schema_t* schema, stat_size_t bin_idx);

stat_float_t stat_bin_width(const stat_binning_schema_t* schema, stat_size_t bin_idx);

#endif // STAT_BINNING_H
