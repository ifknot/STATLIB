#ifndef STAT_PERCENTILES_H
#define STAT_PERCENTILES_H

#include "stat_types.h"

stat_float_t stat_percentile(stat_float_t* data, stat_size_t size, stat_float_t percentile);
stat_float_t stat_quartile(stat_float_t* data, stat_size_t size, stat_size_t quartile);
stat_five_num_summary_t stat_five_num_summary(stat_float_t* data, stat_size_t size);

#endif // STAT_PERCENTILES_H
