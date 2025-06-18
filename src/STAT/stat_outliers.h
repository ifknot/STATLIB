#ifndef STAT_OUTLIERS_H
#define STAT_OUTLIERS_H

#include "stat_types.h"

bool stat_is_outlier(stat_float_t value, stat_five_num_summary_t* summary);
stat_size_t stat_count_outliers(stat_float_t* data, stat_size_t size, stat_five_num_summary_t* summary);

#endif // STAT_OUTLIERS_H
