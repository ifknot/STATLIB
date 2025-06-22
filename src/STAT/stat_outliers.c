#include "stat_outliers.h"

#include <assert.h>
#include <stddef.h>

bool stat_is_outlier(stat_float_t value, const stat_five_num_summary_t* summary) {
    assert(summary != NULL && "Summary pointer cannot be NULL");

    const stat_float_t iqr = summary->q3 - summary->q1;
    const stat_float_t lower_bound = summary->q1 - 1.5f * iqr;
    const stat_float_t upper_bound = summary->q3 + 1.5f * iqr;

    return (value < lower_bound) || (value > upper_bound);
}

stat_size_t stat_count_outliers(const stat_float_t* data, stat_size_t size, const stat_five_num_summary_t* summary) {
    assert(data != NULL && "Data pointer cannot be NULL");
    assert(summary != NULL && "Summary pointer cannot be NULL");

    stat_size_t count = 0;
    for (stat_size_t i = 0; i < size; i++) {
        if (stat_is_outlier(data[i], summary)) {
            count++;
        }
    }
    return count;
}

stat_size_t stat_flag_outliers(const stat_float_t* values, stat_size_t count, const stat_five_num_summary_t* summary, bool* flags) {
    assert(values != NULL && "Values pointer cannot be NULL");
    assert(summary != NULL && "Summary pointer cannot be NULL");
    assert(flags != NULL && "Flags pointer cannot be NULL");

    stat_size_t outlier_count = 0;
    for (stat_size_t i = 0; i < count; i++) {
        flags[i] = stat_is_outlier(values[i], summary);
        outlier_count += flags[i] ? 1 : 0;
    }
    return outlier_count;
}

stat_size_t stat_collect_outliers(const stat_float_t* values, stat_size_t src_count, const stat_five_num_summary_t* summary, stat_float_t* outliers) {
    assert(values != NULL && "Source pointer cannot be NULL");
    assert(summary != NULL && "Summary pointer cannot be NULL");
    assert(outliers != NULL && "Destination pointer cannot be NULL");

    stat_size_t outlier_count = 0;
    for (stat_size_t i = 0; i < src_count; i++) {
        if (stat_is_outlier(values[i], summary)) {
            outliers[outlier_count++] = values[i];
        }
    }
    return outlier_count;
}
