#!/usr/bin/env python3
"""
merge_cpp.py — Merge a multi-file C++ project (local .h/.cpp includes) into
a single, judge-submittable .cpp file.

WHY THIS EXISTS
----------------
Competitive-programming setups often split code across files like:
    typ.h      - shared typedefs
    utils.h    - declarations of helper functions
    utils.cpp  - definitions of helper functions
    A.h        - declares int main_A();
    A.cpp      - the actual solution, entry point is main_A(), not main()

Online judges (Codeforces, vDuge, etc.) want ONE .cpp file with a single
real `main()`. This script:

  1. Recursively inlines every local `#include "file.h"` / `#include "file.cpp"`,
     starting from an entry file you point it at.
  2. Tracks every file it has already inlined (by resolved absolute path) so
     a file included from multiple places is only pasted once — this also
     transparently handles `#pragma once` and `#ifndef/#define/#endif`
     include guards, since we do our OWN guarding at the file level instead
     of relying on the preprocessor.
  3. Collects every *system* include (`#include <...>`) seen anywhere in the
     whole tree, de-duplicates them, and hoists them ALL to the very top of
     the output — so you never get "vector not declared" because some
     nested header's <vector> include got embedded in the wrong place.
  4. Detects a "fake main" function — by default anything matching
     `main_XXX` (e.g. main_A, main_B2, mainSolve) — and:
       - renames its definition to `main`
       - rewrites any call sites (e.g. `return main_A();`) to call `main`
       - if MULTIPLE fake-main candidates are found, refuses to guess and
         asks you to pick one explicitly with --entry-func, rather than
         silently doing the wrong thing.
  5. Leaves a comment trail (`// ---- begin utils.h ----` / `// ---- end ----`)
     so the merged file is still readable and debuggable.
  6. Is conservative: if it can't confidently resolve an include (file not
     found), it LEAVES THE #include LINE AS-IS and prints a warning, rather
     than crashing or silently dropping code.

LIMITATIONS (by design, kept simple & predictable)
---------------------------------------------------
- This is a text-level merger, not a real preprocessor. It does not evaluate
  #ifdef/#ifndef logic beyond stripping simple include guards; conditional
  compilation blocks are passed through untouched.
- It does not deduplicate function/typedef bodies that are copy-pasted in
  multiple files (only whole-FILE duplication, via already-included tracking,
  is deduplicated).
- It assumes reasonably standard, simple include-guard patterns.

USAGE
-----
    python3 merge_cpp.py A.cpp -o A_submit.cpp
    python3 merge_cpp.py A.cpp -o A_submit.cpp --entry-func main_A
    python3 merge_cpp.py A.cpp                       # prints to stdout
    python3 merge_cpp.py A.cpp -I ./include -I ./lib  # extra search dirs

EXIT CODES
----------
    0  success
    1  bad usage / file not found
    2  ambiguous fake-main detection (needs --entry-func)
"""

from __future__ import annotations
import argparse
import os
import re
import sys
from dataclasses import dataclass, field


# ----------------------------------------------------------------------------
# Regexes
# ----------------------------------------------------------------------------

# #include "local/file.h"   (quotes = local include)
RE_LOCAL_INCLUDE = re.compile(r'^\s*#\s*include\s*"([^"]+)"\s*(//.*)?$')

# #include <system/file>    (angle brackets = system include)
RE_SYSTEM_INCLUDE = re.compile(r'^\s*#\s*include\s*<([^>]+)>\s*(//.*)?$')

# #pragma once
RE_PRAGMA_ONCE = re.compile(r'^\s*#\s*pragma\s+once\s*$')

# Simple include-guard trio: #ifndef X / #define X ... #endif
RE_IFNDEF = re.compile(r'^\s*#\s*ifndef\s+(\w+)\s*$')
RE_DEFINE_GUARD = re.compile(r'^\s*#\s*define\s+(\w+)\s*$')
RE_ENDIF = re.compile(r'^\s*#\s*endif\b')

# A function definition whose name matches a "fake main" pattern, e.g.:
#   int main_A() {            int main_A(int argc, char** argv){
#   void mainSolve()  {
RE_FAKE_MAIN_DEF = re.compile(
    r'^\s*(?:static\s+|inline\s+)*'          # optional qualifiers
    r'(?:int|void)\s+'                        # return type
    r'(main[_A-Za-z0-9]*)\s*'                 # captured function name, must start with "main"
    r'\(([^)]*)\)\s*\{?\s*$'                  # arg list, optional opening brace on same line
)


@dataclass
class MergeResult:
    text: str
    system_includes: list[str] = field(default_factory=list)
    inlined_files: list[str] = field(default_factory=list)
    fake_main_candidates: list[str] = field(default_factory=list)
    warnings: list[str] = field(default_factory=list)


class CppMerger:
    def __init__(self, search_dirs: list[str] | None = None):
        # Extra directories to look for local includes, beyond the including
        # file's own directory.
        self.search_dirs = search_dirs or []
        self.system_includes: dict[str, None] = {}   # ordered set
        self.already_inlined: set[str] = set()        # resolved abs paths
        self.inlined_order: list[str] = []
        self.warnings: list[str] = []

    # -- include resolution ---------------------------------------------
    def _resolve(self, inc_name: str, including_file: str) -> str | None:
        candidates = [os.path.join(os.path.dirname(including_file), inc_name)]
        candidates += [os.path.join(d, inc_name) for d in self.search_dirs]
        for c in candidates:
            if os.path.isfile(c):
                return os.path.abspath(c)
        return None

    # -- core recursive inliner -------------------------------------------
    def _inline(self, path: str) -> str:
        abspath = os.path.abspath(path)
        if abspath in self.already_inlined:
            # Already pasted this file's contents once; skip silently.
            # (This is exactly what #pragma once / include guards would do.)
            return ""
        self.already_inlined.add(abspath)
        self.inlined_order.append(abspath)

        try:
            with open(abspath, encoding="utf-8", errors="replace") as f:
                raw_lines = f.readlines()
        except OSError as e:
            self.warnings.append(f"Could not open '{abspath}': {e}")
            return f"// [merge_cpp.py] ERROR: could not open {abspath}: {e}\n"

        out_lines: list[str] = []
        guard_define_name: str | None = None  # the X in #ifndef X / #define X we should strip
        skip_next_endif_for_guard = False

        i = 0
        n = len(raw_lines)
        while i < n:
            line = raw_lines[i]

            # Strip a leading #ifndef GUARD / #define GUARD pair (classic guard idiom)
            m_ifndef = RE_IFNDEF.match(line)
            if m_ifndef and guard_define_name is None and not out_lines:
                # Only treat as a guard if it's basically the first real content
                # and immediately followed by a matching #define.
                if i + 1 < n:
                    m_def = RE_DEFINE_GUARD.match(raw_lines[i + 1])
                    if m_def and m_def.group(1) == m_ifndef.group(1):
                        guard_define_name = m_ifndef.group(1)
                        skip_next_endif_for_guard = True
                        i += 2  # skip both lines
                        continue

            if RE_PRAGMA_ONCE.match(line):
                i += 1
                continue

            if skip_next_endif_for_guard and RE_ENDIF.match(line) and i == n - 1:
                # trailing #endif matching the guard we stripped
                i += 1
                continue

            m_sys = RE_SYSTEM_INCLUDE.match(line)
            if m_sys:
                self.system_includes.setdefault(m_sys.group(1), None)
                i += 1
                continue

            m_local = RE_LOCAL_INCLUDE.match(line)
            if m_local:
                inc_name = m_local.group(1)
                resolved = self._resolve(inc_name, abspath)
                if resolved is None:
                    self.warnings.append(
                        f"Could not resolve local include \"{inc_name}\" "
                        f"from {abspath}; leaving #include line as-is."
                    )
                    out_lines.append(line)
                else:
                    rel = os.path.basename(resolved)
                    out_lines.append(f"// ---- begin {rel} ----\n")
                    inlined = self._inline(resolved)
                    if inlined:
                        out_lines.append(inlined)
                    out_lines.append(f"// ---- end {rel} ----\n")
                i += 1
                continue

            out_lines.append(line)
            i += 1

        return "".join(out_lines)

    def merge(self, entry_file: str) -> MergeResult:
        if not os.path.isfile(entry_file):
            raise FileNotFoundError(entry_file)
        body = self._inline(entry_file)
        return MergeResult(
            text=body,
            system_includes=list(self.system_includes.keys()),
            inlined_files=self.inlined_order,
            warnings=self.warnings,
        )


# ----------------------------------------------------------------------------
# Fake-main detection & renaming
# ----------------------------------------------------------------------------

def find_fake_main_candidates(text: str) -> list[str]:
    """Return distinct function names matching the 'fake main' pattern,
    excluding real 'main' itself."""
    found = []
    for line in text.splitlines():
        m = RE_FAKE_MAIN_DEF.match(line)
        if m:
            name = m.group(1)
            if name != "main" and name not in found:
                found.append(name)
    return found


def rename_entry_function(text: str, func_name: str) -> str:
    """Rename `func_name` -> `main` at its definition and at call sites.

    Uses word-boundary regex so it won't touch e.g. `main_ABC` when asked
    to rename `main_A`.
    """
    pattern = re.compile(r'\b' + re.escape(func_name) + r'\b')
    return pattern.sub("main", text)


# ----------------------------------------------------------------------------
# Assembly
# ----------------------------------------------------------------------------

def assemble_output(result: MergeResult, entry_func: str | None) -> tuple[str, list[str]]:
    notes: list[str] = []
    body = result.text

    # Resolve which function should become main().
    candidates = find_fake_main_candidates(body)
    has_real_main = re.search(r'\bint\s+main\s*\(', body) is not None

    chosen = None
    if entry_func:
        chosen = entry_func
        if chosen not in candidates and not has_real_main:
            notes.append(
                f"WARNING: --entry-func '{chosen}' was not found as a function "
                f"definition in the merged text. No renaming performed."
            )
            chosen = None
    elif has_real_main:
        notes.append("A real main() already exists; no renaming needed.")
    elif len(candidates) == 1:
        chosen = candidates[0]
        notes.append(f"Auto-detected entry function '{chosen}' -> renaming to main().")
    elif len(candidates) > 1:
        notes.append(
            "AMBIGUOUS: multiple candidate entry functions found: "
            + ", ".join(candidates)
            + ". Re-run with --entry-func <name> to pick one. No renaming performed."
        )
    else:
        notes.append(
            "WARNING: no main() and no main_XXX-style function found. "
            "You may need to add --entry-func explicitly."
        )

    if chosen:
        body = rename_entry_function(body, chosen)

    # Build final file: header comment + deduped system includes + using namespace std + body
    header = []
    header.append("// Auto-merged by merge_cpp.py — do not hand-edit includes below.\n")
    header.append("// Source files inlined (in inclusion order):\n")
    for f in result.inlined_files:
        header.append(f"//   {os.path.basename(f)}\n")
    header.append("\n")

    for inc in result.system_includes:
        header.append(f"#include <{inc}>\n")
    if not any("bits/stdc++.h" in inc for inc in result.system_includes):
        pass  # don't force bits/stdc++.h if the user's own headers already list what they need

    if "using namespace std;" not in body and any(
        inc in ("iostream", "vector", "bits/stdc++.h") for inc in result.system_includes
    ):
        header.append("using namespace std;\n")

    header.append("\n")

    final_text = "".join(header) + body
    return final_text, notes


# ----------------------------------------------------------------------------
# CLI
# ----------------------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser(
        description="Merge a multi-file C++ project into one submittable .cpp file."
    )
    ap.add_argument("entry", help="Entry .cpp file (the one with your fake main, e.g. A.cpp)")
    ap.add_argument("-o", "--output", help="Output file (default: stdout)")
    ap.add_argument(
        "-I", "--include-dir", action="append", default=[],
        help="Extra directory to search for local includes (repeatable)"
    )
    ap.add_argument(
        "--entry-func",
        help="Explicitly name the function to rename to main() "
             "(e.g. main_A). Overrides auto-detection."
    )
    args = ap.parse_args()

    try:
        merger = CppMerger(search_dirs=args.include_dir)
        result = merger.merge(args.entry)
    except FileNotFoundError as e:
        print(f"error: entry file not found: {e}", file=sys.stderr)
        return 1

    final_text, notes = assemble_output(result, args.entry_func)

    for w in result.warnings:
        print(f"[merge_cpp.py] warning: {w}", file=sys.stderr)
    for n in notes:
        print(f"[merge_cpp.py] note: {n}", file=sys.stderr)

    if any(n.startswith("AMBIGUOUS") for n in notes):
        # Still write output (so the user can see it) but signal failure.
        if args.output:
            with open(args.output, "w", encoding="utf-8") as f:
                f.write(final_text)
            print(f"[merge_cpp.py] wrote (unresolved) output to {args.output}", file=sys.stderr)
        else:
            print(final_text)
        return 2

    if args.output:
        with open(args.output, "w", encoding="utf-8") as f:
            f.write(final_text)
        print(f"[merge_cpp.py] wrote merged file to {args.output}", file=sys.stderr)
    else:
        print(final_text)

    return 0


if __name__ == "__main__":
    sys.exit(main())
