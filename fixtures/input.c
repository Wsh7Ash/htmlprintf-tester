#include "libprintf-shim.h"

void log_one(FILE *f) {
    fprintf(f, "count=%d\n", 1);
}

int main(void) {
    int x = 1;
    char buf[64];
    printf("x=%d\n", x);
    sprintf(0, "bad %d", x);
    log_one(stdout);
    printf("");
    sprintf(buf, "");
    asprintf(&buf, "%d", x);
    return 0;
}

