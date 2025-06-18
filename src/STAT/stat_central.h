#ifndef STAT_CENTRAL_H
#define STAT_CENTRAL_H

#include "stat_types.h"

stat_float_t stat_mean(stat_float_t* data, stat_size_t size);
stat_float_t stat_median(stat_float_t* data, stat_size_t size);
stat_float_t stat_mode(stat_float_t* data, stat_size_t size);

#endif // STAT_CENTRAL_H
