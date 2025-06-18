#include "tdd_report.h"
#include <time.h>

static report_format_t current_format = REPORT_CONSOLE;

void tdd_set_format(report_format_t format) {
    current_format = format;
}

void tdd_generate_report(test_summary_t s, FILE* output) {
    time_t now;
    time(&now);

    switch (current_format) {
        case REPORT_CONSOLE: {
            fprintf(output, "\n__| %s |________________________________\n", s.suite_name);
            fprintf(output, "  Tests:  %d\n  Passed: %d\n  Failed: %d\n  Time:   %.3fs\n",
                   s.total, s.passed, s.failed, s.time_elapsed);
            fprintf(output, "  Status: %s\n",
                   (s.failed == 0) ? ":) ALL PASSED" : ":( FAILURES");
            fprintf(output, "----------------------------------------------\n");
            break;
        }
        case REPORT_JSON: {
            fprintf(output,
                "{\"suite\":\"%s\",\"passed\":%d,\"failed\":%d,"
                "\"total\":%d,\"time\":%.3f,\"timestamp\":%ld}\n",
                s.suite_name, s.passed, s.failed, s.total,
                s.time_elapsed, now);
            break;
        }
        case REPORT_VERBOSE: {
            // Extended debug view
            fprintf(output, "\n[TEST REPORT] %s\n", ctime(&now));
            fprintf(output, "--------------------------------\n");
            fprintf(output, "Configuration:\n");
            fprintf(output, "  Format:    Verbose Debug\n");
            fprintf(output, "  Timestamp: %ld\n", now);
            fprintf(output, "Results:\n");
            fprintf(output, "  Pass Rate: %.1f%%\n",
                   (s.passed / (double)s.total) * 100.0);
            fprintf(output, "  Duration:  %.3f seconds\n", s.time_elapsed);
            break;
        }
        case REPORT_SILENT:
            if (s.failed > 0) {
                fprintf(output, "FAILURES: %d/%d\n", s.failed, s.total);
            }
            break;
    }
}

void tdd_save_history(test_summary_t s) {
    FILE* hist = fopen("history.tdd", "a");
    if (hist) {
        time_t now;
        time(&now);
        fprintf(hist, "%ld|%s|%d|%d|%.3f\n",
                now, s.suite_name, s.passed, s.failed, s.time_elapsed);
        fclose(hist);
    }
}
