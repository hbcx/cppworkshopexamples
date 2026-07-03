# Build / CI driver

`build.py` is the **correctness gate** for the example corpus (CLAUDE.md §5).
Examples may be AI-drafted, but nothing is "done" until it compiles here — under
the exact standard it declares, with two compilers, and (for concurrency) run
clean under sanitizers.

It lives in **this** repo (`cppworkshop-examples`, public), next to the content
it checks, and runs both locally and in CI via `.github/workflows/ci.yml`. Because
this repo is public, GitHub Actions minutes are free and unlimited.

## Requirements

- Python 3.8+ (no third-party packages — `meta.yaml` is parsed by a tiny built-in
  reader for our controlled format).
- `g++` and `clang++` on `PATH`. Missing compilers are skipped with a warning
  unless you pass `--strict`.

## Usage

```bash
# from the repo root: build every example with g++ and clang++
python driver/build.py

# or point at another checkout (CLI arg or CONTENT_REPO_PATH)
python driver/build.py /path/to/cppworkshopexamples

# see the plan (sources + exact compile flags) without needing a toolchain
python driver/build.py --dry-run

# one section / one example while authoring
driver/build.py --filter move-semantics
driver/build.py --filter 05-noexcept-move --keep
```

Exit code is non-zero if any example fails, so it drops straight into CI.

## What it checks per example

- `standard: "C++NN"` → `-std=c++NN` (the label is the single source of truth).
- Sources by **convention**: all `*.cpp` in the dir, compiled together with
  `-I <dir>`; `.hpp` never compiled alone. `sources:` in `meta.yaml` overrides.
- A real `int main` exists (templates are only checked at instantiation).
- Compiles with `g++` **and** `clang++`, flags
  `-Wall -Wextra -pedantic` (+ `-Werror` unless `werror: false`).
- `sanitizers: [thread, ...]` → `-fsanitize=...`; with `run: true` the binary is
  executed and any sanitizer report / non-zero exit fails the example.

## Options

| Flag | Effect |
|------|--------|
| `--compilers g++,clang++` | which compilers to use |
| `--filter SUBSTR` | only examples whose `section/slug` contains SUBSTR |
| `--std-lower-check` | also compile one standard lower and warn if it still compiles (nice-to-have, never fatal) |
| `--keep` | keep build artifacts (prints the temp dir) |
| `--dry-run` | print the plan, compile nothing |
| `--strict` | a missing requested compiler is an error |
| `-q, --quiet` | only print failures + summary |

## Where it sits in the workflow

```
skill writes files -> driver/build.py (THIS, the gate) -> sync -> MySQL -> site
```

The driver never touches the database. It only answers: *does this code compile
and run clean under the standard it claims?*
