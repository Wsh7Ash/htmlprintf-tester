# Results

## Environment
- OS: Windows (MSYS2/MinGW)
- Runtime/compiler: gcc (MinGW-w64)
- Dependency manager: none

## Validation performed
- install: PASS
- build: PASS
- unit tests: N/A
- integration tests: PASS
- lint: N/A
- type check: N/A
- demo run: PASS

## Commands executed
```shell
make build
make fixtures
make report
make test
```

## Observed result
The CLI scanned `fixtures/input.c` and wrote `results/report.html`.

## Generated artifacts
- `results/report.html`
- `results/test_report.html`

## Known limitations
- Simple textual scan, not a full C parser.
- Single-file analysis only.
- Superficial format/spec presence checks only.
