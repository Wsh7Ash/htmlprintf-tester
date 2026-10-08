/*
 * report.c
 *
 * Minimal report-side helper for htmlprintf-tester.
 * Currently it exists to keep the project structure explicit while the
 * scanner lives in main.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int report_status_directory_expected(const char *dir) {
    if (!dir || !*dir) {
        return -1;
    }
    return 0;
}

FILE *report_open_file(const char *path, const char *mode) {
    if (!path || !mode) {
        return NULL;
    }
    FILE *f = fopen(path, mode);
    if (!f) {
        fprintf(stderr, "[htmlprintf-tester] cannot open %s: %s\n", path, strerror(errno));
    }
    return f;
}

void report_close_file(FILE *f) {
    if (f) {
        fclose(f);
    }
}
