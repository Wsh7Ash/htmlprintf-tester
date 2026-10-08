# Security

`htmlprintf-tester` is a local static-analysis exercise tool.

## What it does

- Reads a C source file you provide.
- Scans it for printf-family call sites.
- Writes a static HTML report.

## What it does not do

- It does not execute the scanned code.
- It is not a compiler, linter, or security scanner.
- It does not guarantee discovery of all format-related issues.

## Authorized use

This tool is intended for local review, IDE/agent-interaction practice, and
educational demos on code you own or are authorized to analyze.

## Reporting issues

If you find a bug or unsafe behavior, open an issue or contact the maintainer.
