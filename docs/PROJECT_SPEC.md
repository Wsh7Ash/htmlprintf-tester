# Project spec — htmlprintf-tester

Name: htmlprintf-tester
Category: dev-tool
Target user: developers and agents practicing printf-family review in C
Problem: printf-family call sites are easy to miss during review
Solution: scan a C source file for printf-family call sites and emit a compact HTML audit report
Core workflow:
1. build the CLI
2. run it against a source file
3. open the generated HTML report
Must-have features:
- detect printf-family call sites
- emit an HTML report
- run as a small CLI
Nice-to-have features:
- multiple fixtures
- clearer empty-spec summary
Stack: C11, GNU Make
Test strategy: build + run against fixture input + verify HTML report exists
Demo strategy: make fixtures, make report, open results/report.html
Safety considerations: local read-only analyzer; does not execute scanned code
Definition of done:
- make build succeeds
- make test passes
- make report produces a readable HTML report
- README commands were tested
