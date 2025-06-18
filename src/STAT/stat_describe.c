#include "stat_describe.h"
#include "stat_central.h"
#include "stat_dispersion.h"
#include "stat_percentiles.h"

stat_descriptive_stats_t stat_describe(stat_float_t* data, stat_size_t size) {
    stat_descriptive_stats_t stats;

    stats.mean = stat_mean(data, size);
    stats.median = stat_median(data, size);
    stats.mode = stat_mode(data, size);
    stats.variance = stat_variance(data, size);
    stats.std_dev = stat_std_dev(data, size);
    stats.range = stat_range(data, size);
    stats.five_num_summary = stat_five_num_summary(data, size);

    return stats;
}
