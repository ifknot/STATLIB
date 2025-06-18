/**
 * @file tdd_report.h
 * @brief Enhanced test reporting with formatting options
 * @ingroup tdd_framework
 */
#ifndef TDD_REPORT_H
#define TDD_REPORT_H

#include <stdio.h>

typedef enum {
    REPORT_CONSOLE,    // Human-readable (default)
    REPORT_JSON,       // Machine-readable for CI/CD pipelines
    REPORT_VERBOSE,    // Detailed debug output
    REPORT_SILENT      // Only failures (benchmarking mode)
} report_format_t;

typedef struct {
    int total;
    int passed;
    int failed;
    const char* suite_name;
    double time_elapsed;  // Seconds
} test_summary_t;

// ========================
// Public Interface
// ========================

/**
 * @brief Sets global report format (default: REPORT_CONSOLE)
 */
void tdd_set_format(report_format_t format);

/**
 * @brief Generates a formatted test report
 * @param summary Test metrics and metadata
 * @param output  FILE* to write to (stdout/stderr/file)
 */
void tdd_generate_report(test_summary_t summary, FILE* output);

/**
 * @brief Saves historical data to a .tdd_history file
 * @note  Appends results for trend analysis (pass/fail over time)
 */
void tdd_save_history(test_summary_t summary);

#endif
