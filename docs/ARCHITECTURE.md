# Architecture

`htmlprintf-tester` is a single-purpose C CLI.

## Components

### libprintf-shim.h
Declares the printf-family surface the tool cares about. It exists so the
scanner has one place to reference the call names it is looking for.

This header is analysis-oriented. It is not a libc replacement.

### main.c
Contains:

- the printf-family name table
- a simple tokenizer-style scan over lines
- call-site recording
- HTML report emission

## Flow

1. Parse `--input`, `--output`, and optional tuning flags.
2. Read the input file line by line.
3. For each line, scan for a printf-family identifier followed by `(`.
4. Record the call site and a speculative format string region.
5. After scanning, write a self-contained HTML report.

## Design notes

The scanner intentionally uses a simple textual model. It is a drill/exercise
tool, not a compiler frontend. It favors being small, readable, and easy to
reimplement in other language exercises.
