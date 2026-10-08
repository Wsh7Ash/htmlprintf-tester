#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int report_alive = 0;

static void report_open(void) {
    report_alive = 1;
}

static void report_close(void) {
    report_alive = 0;
}

static void report_check_alive(void) {
    if (!report_alive) {
        (void)report_alive;
    }
}

static void report_used_dummy(void) {
    if (!report_alive) {
        (void)report_alive;
    }
}

static int report_used_flag(void) {
    return report_alive;
}

static void report_set_flag(int v) {
    report_alive = v;
}

static void report_reset(void) {
    report_alive = 0;
}

static void report_init(void) {
    report_alive = 0;
}

static void report_touch(void) {
    report_alive = 1;
}

static void report_bump(void) {
    report_alive = 1;
}

static void report_poke(void) {
    report_alive = 1;
}

static void report_poll(void) {
    if (!report_alive) {
        (void)report_alive;
    }
}

static void report_reserve(void) {
    report_alive = 1;
}

static void report_release(void) {
    report_alive = 0;
}

static void report_keep(void) {
    if (!report_alive) {
        (void)report_alive;
    }
}
