#include "stat_util.h"
#include <math.h>
#include <assert.h>
#include <stdbool.h>

// ========================
// Sorting Functions
// ========================

static void quicksort_f(stat_float_t* data, stat_size_t left, stat_size_t right) {
    if (left >= right) return;
    
    stat_float_t pivot = data[(left + right) / 2];
    stat_size_t i = left, j = right;
    
    while (i <= j) {
        while (data[i] < pivot) i++;
        while (data[j] > pivot) j--;
        if (i <= j) {
            stat_float_t temp = data[i];
            data[i] = data[j];
            data[j] = temp;
            i++;
            j--;
        }
    }
    
    quicksort_f(data, left, j);
    quicksort_f(data, i, right);
}

void stat_sort_f(stat_float_t* data, stat_size_t size) {
    assert(data != NULL);
    if (size > 1) {
        quicksort_f(data, 0, size - 1);
    }
}

static void insertion_sort_i(stat_int_t* data, stat_size_t size) {
    for (stat_size_t i = 1; i < size; i++) {
        stat_int_t key = data[i];
        stat_size_t j = i;
        while (j > 0 && data[j-1] > key) {
            data[j] = data[j-1];
            j--;
        }
        data[j] = key;
    }
}

void stat_sort_i(stat_int_t* data, stat_size_t size) {
    assert(data != NULL);
    if (size <= 20) {
        insertion_sort_i(data, size);
    } else {
        // Use library qsort for larger arrays
        qsort(data, size, sizeof(stat_int_t), 
              (int (*)(const void*, const void*))stat_compare_ints_qsort);
    }
}

// ========================
// Value Validation
// ========================

bool stat_is_finite(stat_float_t value) {
    return isfinite(value);
}

bool stat_is_normal(stat_float_t value) {
    return isnormal(value);
}

bool stat_is_valid(stat_float_t value) {
    return isfinite(value) && 
           (fabs(value) < 1e100) &&  // Reasonable bound
           (fabs(value) > 1e-100 || value == 0.0);
}

// ========================
// Array Validation
// ========================

bool stat_array_is_valid_f(const stat_float_t* data, stat_size_t size) {
    assert(data != NULL);
    for (stat_size_t i = 0; i < size; i++) {
        if (!stat_is_valid(data[i])) {
            return false;
        }
    }
    return true;
}

bool stat_array_is_sorted_f(const stat_float_t* data, stat_size_t size) {
    assert(data != NULL);
    for (stat_size_t i = 1; i < size; i++) {
        if (data[i-1] > data[i]) {
            return false;
        }
    }
    return true;
}

bool stat_array_is_finite_f(const stat_float_t* data, stat_size_t size) {
    assert(data != NULL);
    for (stat_size_t i = 0; i < size; i++) {
        if (!isfinite(data[i])) {
            return false;
        }
    }
    return true;
}

// ========================
// Array Operations
// ========================

stat_size_t stat_array_replace_nan_f(stat_float_t* data, stat_size_t size, stat_float_t substitute) {
    assert(data != NULL);
    stat_size_t count = 0;
    for (stat_size_t i = 0; i < size; i++) {
        if (isnan(data[i])) {
            data[i] = substitute;
            count++;
        }
    }
    return count;
}

stat_size_t stat_array_clip_f(stat_float_t* data, stat_size_t size, stat_float_t min, stat_float_t max) {
    assert(data != NULL);
    stat_size_t count = 0;
    for (stat_size_t i = 0; i < size; i++) {
        if (data[i] < min) {
            data[i] = min;
            count++;
        } else if (data[i] > max) {
            data[i] = max;
            count++;
        }
    }
    return count;
}
