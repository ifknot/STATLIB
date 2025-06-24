/**
 * @file stat.h
 * @brief Master include file for the Statistics Library
 *
 * This is the umbrella header that includes all module headers.
 * For detailed documentation, see the individual module headers.
 */

#ifndef STAT_H
#define STAT_H

#include "stat_constants.h"
#include "stat_types.h"       ///< Core type definitions and structures
#include "stat_abs.h"         ///< Absolute value functions: stat_abs_float(), stat_abs_int32(), stat_safe_abs_int32()
#include "stat_basic.h"       ///< Basic statistics: stat_min(), stat_max(), stat_range()
#include "stat_central.h"     ///< Central tendency: stat_mean(), stat_median(), stat_mode()
#include "stat_clamp.h"       ///< Clamping functions: stat_clamp(), stat_clamp_int32(), stat_clamp_array()
#include "stat_compare.h"     ///< Comparison functions: stat_compare_floats(), stat_almost_equal(), stat_is_near_zero()
#include "stat_describe.h"    ///< Comprehensive statistics: stat_describe()
#include "stat_dispersion.h"  ///< Dispersion metrics: stat_variance(), stat_std_dev(), stat_mad(), stat_iqr()
#include "stat_distributions.h" ///< Distribution generators: stat_generate_uniform_dist(), stat_generate_normal_dist(), stat_generate_exponential_dist()
#include "stat_division.h"    ///< Integer division: stat_safe_div_int32(), stat_div_round_up(), stat_div_round_nearest()
#include "stat_outliers.h"    ///< Outlier detection: stat_is_outlier(), stat_count_outliers()
#include "stat_percentiles.h" ///< Percentile functions: stat_percentile(), stat_quartile(), stat_five_num_summary()
#include "stat_round.h"       ///< Rounding functions: stat_round_to_int32(), stat_floor_to_int32(), stat_ceil_to_int32(), stat_round_decimal()
#include "stat_sign.h"        ///< Sign functions: stat_sign_float(), stat_sign_int32(), stat_copysign_float()
#include "stat_util.h"        ///< Utilities: stat_sort(), stat_is_finite(), stat_is_normal()

#endif // STAT_H
