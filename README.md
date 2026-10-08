# htmlprintf-tester

A small static-analysis exercise tool that scans a C source file for
`printf`-family call sites and emits a compact HTML audit report.

It is meant for IDE/agent-interaction practice: the scanner deliberately
works on a narrow textual model of C call sites, so it is suitable for
code review drills, agent-driven audits, and teaching printf hygiene
rather than as a replacement for compiler warnings or a full parser.

## Problem

Printf-family formatting bugs are easy to miss during review:

- missing or empty format specs
- mismatched argument counts
- copy-pasted format strings that no longer match the data

This tool gives a fast, visible, HTML-side view of every printf-family
call site in a file so an editor or agent can inspect them in one place.

## Key features

- Detects a broad printf-family surface: `printf`, `fprintf`, `sprintf`,
  `snprintf`, `vprintf`, `vfprintf`, `vsprintf`, `vsnprintf`, `wprintf`,
  `fwprintf`, `swprintf`, `vwprintf`, `vfwprintf`, `vswprintf`, `asprintf`,
  `vasprintf`.
- Emit a single HTML report with a site table.
- Designed to run as a small CLI with `--input` and `--output`.

## Requirements

- A C11 compiler such as `gcc` or `clang`.
- GNU Make (optional; the Makefile is provided for convenience).

## Installation

Build the binary:

```shell
make build
```

Or manually:

```shell
gcc -std=c11 -Wall -Wextra -Werror -pedantic -O2 -I. -o htmlprintf-tester.exe main.c
```

## Quick start

Generate a fixture file, then run the scanner:

```shell
make fixtures
make report
```

Open `results/report.html` in a browser.

## Usage

```shell
./htmlprintf-tester.exe --input <file> --output <html-file> [--target <func>] [--max-sites N] [--max-fixtures N]
```

Example:

```shell
./htmlprintf-tester.exe --input fixtures/input.c --output results/report.html
```

## Architecture

- `libprintf-shim.h` declares the printf-family surface the scanner cares
  about. It is an analysis-oriented shim, not a libc replacement.
- `main.c` contains the call-site scanner and HTML report writer.

The scanner uses a deliberately simple textual model:

1. Read the input line by line.
2. Look for printf-family identifiers followed by `(`.
3. Extract the text between `(` and the matching `)`.
4. Record each site and emit an HTML audit page.

This is intentionally rule-of-thumb, not a full C parser. It is a
teaching/exercise tool.

## Testing

Build and run the project self-checks:

```shell
make test
```

That target builds the binary, runs it against the fixture input, and
verifies the HTML report was created.

## Project structure

```text
.
├── libprintf-shim.h
├── main.c
├── Makefile
├── fixtures/
│   └── input.c
└── results/
    └── report.html
```

## Limitations

- Only analyzes one source file at a time.
- Uses a simple textual scan, so it can miss call sites hidden by macros,
  odd formatting, or non-obvious tokenization.
- It does not validate format-argument correctness deeply; it reports
  obvious spec presence only.
- It is not a compiler, linter, or security scanner.

## Roadmap

- Add a small set of multiple fixture inputs for review drills.
- Add a summary of empty-format sites.
- Keep the tool intentionally small and readable.

## Security notes

This is a local read-only analyzer. It does not execute the scanned code.
Do not use it as the sole mechanism for catching format-security issues.
Compiler warnings and code review remain necessary.

## License

MIT
