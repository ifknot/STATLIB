#include "stat_outliers.h"

bool stat_is_outlier(stat_float_t value, stat_five_num_summary_t* summary) {
    return (value < summary->lower_fence) || (value > summary->upper_fence);
}

stat_size_t stat_count_outliers(stat_float_t* data, stat_size_t size, stat_five_num_summary_t* summary) {
    stat_size_t count = 0;
    for (stat_size_t i = 0; i < size; i++) {
        if (stat_is_outlier(data[i], summary)) {
            count++;
        }
    }
    return count;
}
