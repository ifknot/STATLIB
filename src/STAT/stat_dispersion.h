#ifndef STAT_DISPERSION_H
#define STAT_DISPERSION_H

#include "stat_types.h"

// Basic dispersion
stat_float_t stat_range(stat_float_t* data, stat_size_t size);
stat_float_t stat_variance(stat_float_t* data, stat_size_t size);
stat_float_t stat_std_dev(stat_float_t* data, stat_size_t size);
stat_float_t stat_mean_absolute_deviation(stat_float_t* data, stat_size_t size);

// Robust dispersion (less sensitive to outliers)
stat_float_t stat_iqr(stat_float_t* data, stat_size_t size); // Q3-Q1
stat_float_t stat_mad(stat_float_t* data, stat_size_t size); // Median Absolute Deviation
stat_float_t stat_qn_estimator(stat_float_t* data, stat_size_t size); // Robust scale estimator

#endif // STAT_DISPERSION_H
