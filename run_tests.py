#!/usr/bin/env python3
"""
run_tests.py — Compile a C++ solution and run it against one or more
input/expected-output test cases, reporting PASS/FAIL with diffs.

SUPPORTED TEST LAYOUTS
-----------------------
1. A folder of paired files (most common / recommended):
       tests/
         1.in   1.out
         2.in   2.out
         sample1.in  sample1.out
   Any file "X.in" is paired with "X.out" (same stem, any extension pair
   you like via --in-ext/--out-ext, default .in/.out).

2. A single "combined" text file with tests separated by a delimiter line
   (handy for quickly pasting samples straight off a problem page):

       tests.txt:
       ---INPUT---
       3 2
       1 2
       2 3
       ---OUTPUT---
       2
       ---INPUT---
       4 3
       1 2
       2 3
       3 4
       ---OUTPUT---
       3

   Use --combined tests.txt for this format.

3. Quick single-shot ad hoc test straight from the command line:
       python3 run_tests.py A.cpp --stdin "3 2\\n1 2\\n2 3" --expect "2"

FEATURES
--------
- Compiles once (g++ -O2 -std=c++17 by default, configurable).
- Per-test timeout (default 2s), reports TLE distinctly from wrong answer.
- Whitespace-tolerant comparison by default (trailing spaces/newlines,
  and optionally all whitespace runs, are ignored) since most judges
  normalize whitespace too — use --strict to disable this.
- On failure, shows a compact diff: input, expected, got, and the first
  line where they differ.
- Exit code reflects overall pass/fail (0 = all passed, 1 = compile error,
  2 = at least one test failed), so it composes fine in scripts/CI.
- Colorized PASS/FAIL if the terminal supports it, plain text otherwise.

USAGE
-----
    python3 run_tests.py A.cpp -t tests/
    python3 run_tests.py A.cpp --combined tests.txt
    python3 run_tests.py A.cpp --stdin "3 2\\n1 2\\n2 3" --expect "2"
    python3 run_tests.py A.cpp -t tests/ --timeout 5 --strict
"""

from __future__ import annotations
import argparse
import glob
import os
import re
import subprocess
import sys
import tempfile
import time
from dataclasses import dataclass


# ----------------------------------------------------------------------------
# Terminal color helpers (no-op if not a tty)
# ----------------------------------------------------------------------------

def _supports_color() -> bool:
    return sys.stdout.isatty() and os.environ.get("NO_COLOR") is None


USE_COLOR = _supports_color()


def c(text: str, code: str) -> str:
    if not USE_COLOR:
        return text
    return f"\033[{code}m{text}\033[0m"


def green(t): return c(t, "32")
def red(t): return c(t, "31")
def yellow(t): return c(t, "33")
def bold(t): return c(t, "1")


# ----------------------------------------------------------------------------
# Data model
# ----------------------------------------------------------------------------

@dataclass
class TestCase:
    name: str
    input_text: str
    expected_text: str


@dataclass
class TestResult:
    case: TestCase
    passed: bool
    got_output: str = ""
    elapsed: float = 0.0
    timed_out: bool = False
    runtime_error: str | None = None


# ----------------------------------------------------------------------------
# Test discovery
# ----------------------------------------------------------------------------

def load_paired_dir(tests_dir: str, in_ext: str, out_ext: str) -> list[TestCase]:
    cases = []
    in_files = sorted(glob.glob(os.path.join(tests_dir, f"*{in_ext}")))
    if not in_files:
        print(f"warning: no *{in_ext} files found in {tests_dir}", file=sys.stderr)
    for in_path in in_files:
        stem = in_path[: -len(in_ext)] if in_path.endswith(in_ext) else os.path.splitext(in_path)[0]
        out_path = stem + out_ext
        if not os.path.isfile(out_path):
            print(f"warning: no matching {out_ext} for {in_path}, skipping", file=sys.stderr)
            continue
        with open(in_path, encoding="utf-8") as f:
            inp = f.read()
        with open(out_path, encoding="utf-8") as f:
            exp = f.read()
        cases.append(TestCase(name=os.path.basename(stem), input_text=inp, expected_text=exp))
    return cases


def load_combined_file(path: str) -> list[TestCase]:
    with open(path, encoding="utf-8") as f:
        text = f.read()

    # Split on lines that are exactly ---INPUT--- / ---OUTPUT--- (case-insensitive,
    # dashes optional in count) to stay forgiving of minor formatting differences.
    tokens = re.split(r'^-*\s*(INPUT|OUTPUT)\s*-*\s*$', text, flags=re.MULTILINE | re.IGNORECASE)
    # tokens looks like: ['', 'INPUT', ' 3 2\n1 2\n2 3\n', 'OUTPUT', ' 2\n', 'INPUT', ...]
    cases = []
    i = 1
    idx = 1
    pending_input = None
    while i < len(tokens):
        label = tokens[i].upper()
        content = tokens[i + 1] if i + 1 < len(tokens) else ""
        if label == "INPUT":
            pending_input = content.strip("\n")
            # keep internal structure but strip only leading/trailing blank lines
            pending_input = content.strip()
        elif label == "OUTPUT":
            expected = content.strip()
            cases.append(TestCase(name=f"test{idx}", input_text=pending_input or "", expected_text=expected))
            idx += 1
            pending_input = None
        i += 2
    if not cases:
        print(f"warning: no INPUT/OUTPUT pairs parsed from {path}", file=sys.stderr)
    return cases


# ----------------------------------------------------------------------------
# Compilation
# ----------------------------------------------------------------------------

def compile_cpp(source: str, std: str, extra_flags: list[str]) -> tuple[str | None, str]:
    """Returns (binary_path or None, compiler_output)."""
    out_dir = tempfile.mkdtemp(prefix="run_tests_")
    binary = os.path.join(out_dir, "sol")
    cmd = ["g++", f"-std={std}", "-O2", "-o", binary, source] + extra_flags
    proc = subprocess.run(cmd, capture_output=True, text=True)
    if proc.returncode != 0:
        return None, proc.stdout + proc.stderr
    return binary, proc.stdout + proc.stderr


# ----------------------------------------------------------------------------
# Comparison
# ----------------------------------------------------------------------------

def normalize(text: str, strict: bool) -> str:
    if strict:
        return text
    # Split into tokens across all whitespace, rejoin with single spaces per
    # line-agnostic comparison EXCEPT we still want to catch cases where line
    # structure matters for correctness on some problems. We do a middle
    # ground: strip trailing whitespace on each line, drop trailing blank
    # lines, and collapse trailing spaces within a line.
    lines = [ln.rstrip() for ln in text.splitlines()]
    while lines and lines[-1] == "":
        lines.pop()
    return "\n".join(lines)


def first_diff_line(a: str, b: str) -> int | None:
    a_lines = a.splitlines()
    b_lines = b.splitlines()
    for idx, (x, y) in enumerate(zip(a_lines, b_lines)):
        if x != y:
            return idx + 1
    if len(a_lines) != len(b_lines):
        return min(len(a_lines), len(b_lines)) + 1
    return None


# ----------------------------------------------------------------------------
# Execution
# ----------------------------------------------------------------------------

def run_case(binary: str, case: TestCase, timeout: float, strict: bool) -> TestResult:
    start = time.time()
    try:
        proc = subprocess.run(
            [binary],
            input=case.input_text,
            capture_output=True,
            text=True,
            timeout=timeout,
        )
    except subprocess.TimeoutExpired:
        return TestResult(case=case, passed=False, timed_out=True, elapsed=timeout)
    elapsed = time.time() - start

    if proc.returncode != 0:
        return TestResult(
            case=case, passed=False, got_output=proc.stdout, elapsed=elapsed,
            runtime_error=f"exited with code {proc.returncode}; stderr: {proc.stderr.strip()[:500]}"
        )

    got_norm = normalize(proc.stdout, strict)
    exp_norm = normalize(case.expected_text, strict)
    passed = got_norm == exp_norm
    return TestResult(case=case, passed=passed, got_output=proc.stdout, elapsed=elapsed)


# ----------------------------------------------------------------------------
# Reporting
# ----------------------------------------------------------------------------

def print_result(result: TestResult, verbose_on_fail: bool = True):
    case = result.case
    if result.passed:
        print(f"{green('PASS')}  {case.name}  ({result.elapsed:.3f}s)")
        return

    if result.timed_out:
        print(f"{red('TLE ')}  {case.name}  (> {result.elapsed:.1f}s)")
        return

    if result.runtime_error:
        print(f"{red('RTE ')}  {case.name}  {result.runtime_error}")
        return

    print(f"{red('FAIL')}  {case.name}  ({result.elapsed:.3f}s)")
    if verbose_on_fail:
        diff_line = first_diff_line(
            normalize(result.got_output, False), normalize(case.expected_text, False)
        )
        indent = "      "
        print(f"{indent}{bold('input:')}")
        for ln in case.input_text.splitlines()[:20]:
            print(f"{indent}  {ln}")
        print(f"{indent}{bold('expected:')}")
        for ln in case.expected_text.splitlines()[:20]:
            print(f"{indent}  {ln}")
        print(f"{indent}{bold('got:')}")
        for ln in result.got_output.splitlines()[:20]:
            print(f"{indent}  {ln}")
        if diff_line:
            print(f"{indent}{yellow(f'first differing line: {diff_line}')}")


# ----------------------------------------------------------------------------
# CLI
# ----------------------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser(description="Compile and test a C++ solution against sample cases.")
    ap.add_argument("source", help="Path to the .cpp source file")
    ap.add_argument("-t", "--tests-dir", help="Directory of paired *.in/*.out files")
    ap.add_argument("--in-ext", default=".in", help="Input file extension (default .in)")
    ap.add_argument("--out-ext", default=".out", help="Expected-output file extension (default .out)")
    ap.add_argument("--combined", help="Single file with ---INPUT---/---OUTPUT--- sections")
    ap.add_argument("--stdin", help="Ad hoc single test: raw input text")
    ap.add_argument("--expect", help="Ad hoc single test: expected output text (used with --stdin)")
    ap.add_argument("--std", default="c++17", help="C++ standard (default c++17)")
    ap.add_argument("--timeout", type=float, default=2.0, help="Per-test timeout in seconds (default 2.0)")
    ap.add_argument("--strict", action="store_true", help="Exact byte comparison (no whitespace tolerance)")
    ap.add_argument("--cflag", action="append", default=[], help="Extra flag to pass to g++ (repeatable)")
    args = ap.parse_args()

    if not os.path.isfile(args.source):
        print(f"error: source file not found: {args.source}", file=sys.stderr)
        return 1

    cases: list[TestCase] = []
    if args.tests_dir:
        cases += load_paired_dir(args.tests_dir, args.in_ext, args.out_ext)
    if args.combined:
        cases += load_combined_file(args.combined)
    if args.stdin is not None:
        cases.append(TestCase(name="adhoc", input_text=args.stdin, expected_text=args.expect or ""))

    if not cases:
        print("error: no test cases found (use -t, --combined, or --stdin/--expect)", file=sys.stderr)
        return 1

    print(bold(f"Compiling {args.source} ..."))
    binary, compiler_out = compile_cpp(args.source, args.std, args.cflag)
    if binary is None:
        print(red("COMPILE ERROR"))
        print(compiler_out)
        return 1
    if compiler_out.strip():
        print(yellow("compiler warnings:"))
        print(compiler_out)

    print(bold(f"Running {len(cases)} test case(s)...\n"))
    passed = 0
    for case in cases:
        result = run_case(binary, case, args.timeout, args.strict)
        print_result(result)
        if result.passed:
            passed += 1

    total = len(cases)
    print()
    summary = f"{passed}/{total} passed"
    print(bold(green(summary)) if passed == total else bold(red(summary)))

    return 0 if passed == total else 2


if __name__ == "__main__":
    sys.exit(main())
