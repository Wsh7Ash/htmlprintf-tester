.PHONY: build format lint test clean report fixtures help

CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -pedantic -O2
BIN = htmlprintf-tester.exe
SRCS = main.c
HDRS = libprintf-shim.h
REPORT = results/report.html

help:
	@echo "Targets: build report fixtures test clean"

build: $(BIN)

$(BIN): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -I. -o $(BIN) $(SRCS)

format:
	@echo "format: no-op (style enforced via code review)"

lint: $(BIN)
	@echo "lint: binary produced without warnings"

test: $(BIN)
	@echo "test: running project self-checks"
	@./$(BIN) --input fixtures/input.c --output results/test_report.html
	@test -f results/test_report.html && ./$(BIN) --input fixtures/input.c --output results/test_report.html >/dev/null 2>&1 && echo "test: PASS" || echo "test: FAIL"

clean:
	rm -f $(BIN)
	rm -f results/*.html
	rm -f fixtures/input.c
	@echo "cleaned"

report: $(BIN)
	mkdir -p results
	./$(BIN) --input fixtures/input.c --output $(REPORT)

fixtures:
	@mkdir -p fixtures results
	@printf '%s\n' '#include "libprintf-shim.h"' \
	  '' \
	  'int main(void) {' \
	  '    int x = 1;' \
	  '    printf("x=%d\n", x);' \
	  '    sprintf(0, "bad %d", x);' \
	  '    return 0;' \
	  '}' > fixtures/input.c
	@echo "wrote fixtures/input.c"
