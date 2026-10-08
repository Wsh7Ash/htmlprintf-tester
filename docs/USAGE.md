# Usage

## Build

```shell
make build
```

If you do not have `make`, compile directly:

```shell
gcc -std=c11 -Wall -Wextra -Werror -pedantic -O2 -I. -o htmlprintf-tester.exe main.c
```

## Run a scan

```shell
./htmlprintf-tester.exe --input <file> --output <html-file>
```

Example:

```shell
./htmlprintf-tester.exe --input fixtures/input.c --output results/report.html
```

## Options

- `--input` - source file to scan
- `--output` - HTML report path
- `--target` - optional function name focus
- `--max-sites` - maximum sites to record
- `--max-fixtures` - maximum fixture slots

## Exit codes

- `0` - sites reviewed with no bad/warn findings
- `2` - usage error
- `3` - no printf-family sites found
- `4` - bad or warn findings present

## Typical workflow

```shell
make fixtures
make report
make test
```
