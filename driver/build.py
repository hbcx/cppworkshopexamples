#!/usr/bin/env python3
"""cppworkshop build/CI driver -- the correctness gate for the example corpus.

This script is the single source of truth for "does an example actually compile
under the standard it claims?". It lives in cppworkshop-examples (public), next
to the content it checks, and is run both locally and by this repo's GitHub
Actions CI (see .github/workflows/ci.yml). See CLAUDE.md sec. 5.

What it does, per example dir (content/<section>/<example>/):
  * read meta.yaml (standard, run, sanitizers, werror, sources, libs);
  * map "C++NN" -> -std=c++NN;
  * pick sources BY CONVENTION -- every *.cpp in the dir, compiled together with
    -I <dir> so local headers resolve; one dir = one executable. .hpp files are
    never compiled alone. `sources:` in meta.yaml overrides this for rare cases;
  * assert a real `int main` exists (guardrail: templates are only checked at
    instantiation, so every example must instantiate/use the feature via main);
  * compile with g++ AND clang++ using -Wall -Wextra -pedantic (+ -Werror unless
    werror: false), adding -fsanitize=<...> when sanitizers: is set, and -O<lvl>
    when optimize: is set (examples default to -O0: the generated code stays
    close to the source a reader is looking at -- but a BENCHMARK must set
    optimize: O2, or it measures the un-inlined debug shape of the standard
    library instead of the algorithm);
  * put content/_shared/ on the include path, so examples can pull in shared
    header-only helpers (benchmark datasets, the timing harness) with
    #include "bench/datasets.hpp". Dirs starting with _ are not examples and are
    skipped by discovery. meta.yaml lists them in shared: purely so the SITE
    knows to display them next to main.cpp; the compiler finds them via -I;
  * when run: true, execute the binary and fail on non-zero exit / sanitizer
    report (TSan etc. only catch problems at runtime);
  * print a clear pass/fail report and exit non-zero if anything failed.

Usage:
  driver/build.py [path-to-cppworkshop-examples] [options]

Content path resolution order: CLI arg > CONTENT_REPO_PATH env > this repo root
(the driver lives in driver/, a sibling of content/). No arg is needed when run
from inside cppworkshop-examples.

Options:
  --compilers g++,clang++   Compilers to use (default: both). Missing ones are
                            skipped with a warning unless --strict.
  --filter SUBSTR           Only build examples whose "section/slug" contains SUBSTR.
  --std-lower-check         Also compile one standard lower and EXPECT failure;
                            warn if it still compiles (example may not truly need
                            its declared standard). Nice-to-have, never fatal.
  --keep                    Keep build artifacts (default: temp dir is removed).
  --dry-run                 Print the plan (sources + flags) without compiling.
  --strict                  Treat a missing requested compiler as an error.
  --write-output            For run: true examples, capture the program's stdout
                            and write it to output.txt next to the example (for
                            the site to display). Opt-in so CI never mutates the
                            repo; the author regenerates and commits these files.
  --bench                   Also RUN examples marked bench: true. Benchmarks are
                            timing measurements, not correctness checks: their
                            numbers are meaningless on a shared CI runner, so CI
                            only compiles them (a benchmark is code and can rot)
                            and the author executes them locally, which is also
                            when output.txt is regenerated. Combine with
                            --write-output to capture the timing tables.
  --write-trace             For run: true examples, record a gdb single-step
                            execution trace to trace.json next to the example (for
                            the site's step-through player). Needs gdb; skipped
                            with a note if absent. Concurrency examples (thread
                            sanitizer) and `trace: false` are excluded -- there is
                            no single deterministic trace. Like --write-output,
                            opt-in; the author regenerates and commits the file.
  -q, --quiet               Only print failures and the final summary.
"""

from __future__ import annotations

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

# --- standard label -> -std= flag -------------------------------------------
# Single source of truth for the flag; the label in meta.yaml is authoritative
# (guardrail #2: the standard is declared in exactly one place).
STD_ORDER = ["C++98", "C++03", "C++11", "C++14", "C++17", "C++20", "C++23", "C++26"]
STD_FLAG = {
    "C++98": "c++98", "C++03": "c++03", "C++11": "c++11", "C++14": "c++14",
    "C++17": "c++17", "C++20": "c++20", "C++23": "c++23", "C++26": "c++26",
}

SOURCE_EXTS = (".cpp", ".cc", ".cxx")
EXE_SUFFIX = ".exe" if os.name == "nt" else ""

# Optimization levels an example may request via `optimize:` in meta.yaml. Most
# examples want none (the default -O0 keeps the generated code close to the
# source, which is what a reader is comparing against); benchmarks REQUIRE one,
# because at -O0 you measure the un-inlined debug shape of the standard library
# rather than the algorithm.
OPT_LEVELS = ("O0", "O1", "O2", "O3", "Os", "Og")

# Header-only helpers shared by several examples (benchmark datasets, the timing
# harness). Not examples themselves -- the leading underscore keeps them out of
# discovery, and every example gets this dir on its include path.
SHARED_DIRNAME = "_shared"
_shared_dir: Path | None = None


# --- tiny YAML reader for our controlled meta.yaml --------------------------
# We only need a handful of scalar/list fields; a full YAML lib would be an
# unnecessary dependency for this controlled format.
def parse_meta(text: str) -> dict:
    meta: dict = {}
    for raw in text.splitlines():
        line = raw.rstrip()
        stripped = line.strip()
        if not stripped or stripped.startswith("#"):
            continue
        # Only care about top-level "key: value" lines (skip nested "- {...}").
        if line[:1] in (" ", "\t", "-"):
            continue
        m = re.match(r"^([A-Za-z_]+):\s*(.*)$", line)
        if not m:
            continue
        key, val = m.group(1), m.group(2).strip()
        meta[key] = _coerce(val)
    return meta


def _coerce(val: str):
    if val == "" or val == "[]":
        return [] if val == "[]" else ""
    if val.startswith("[") and val.endswith("]"):
        inner = val[1:-1].strip()
        if not inner:
            return []
        return [_unquote(x.strip()) for x in inner.split(",") if x.strip()]
    low = val.lower()
    if low in ("true", "false"):
        return low == "true"
    return _unquote(val)


def _unquote(s: str) -> str:
    if len(s) >= 2 and s[0] == s[-1] and s[0] in ("'", '"'):
        return s[1:-1]
    return s


# --- example discovery ------------------------------------------------------
class Example:
    def __init__(self, section: str, slug: str, path: Path, meta: dict):
        self.section = section
        self.slug = slug
        self.path = path
        self.meta = meta

    @property
    def name(self) -> str:
        return f"{self.section}/{self.slug}"

    def sources(self) -> list[Path]:
        override = self.meta.get("sources")
        if override:
            return [self.path / s for s in override]
        return sorted(
            p for p in self.path.iterdir()
            if p.suffix in SOURCE_EXTS and p.is_file()
        )


def discover(content_dir: Path, filt: str | None) -> list[Example]:
    examples: list[Example] = []
    for section_dir in sorted(p for p in content_dir.iterdir() if p.is_dir()):
        if section_dir.name.startswith("_"):
            continue  # _shared/ and friends hold library code, not examples
        for ex_dir in sorted(p for p in section_dir.iterdir() if p.is_dir()):
            if not re.match(r"^\d+-", ex_dir.name):
                continue
            meta_file = ex_dir / "meta.yaml"
            if not meta_file.is_file():
                continue
            ex = Example(section_dir.name, ex_dir.name, ex_dir,
                         parse_meta(meta_file.read_text(encoding="utf-8")))
            if filt and filt not in ex.name:
                continue
            examples.append(ex)
    return examples


# --- per-example checks -----------------------------------------------------
class Result:
    def __init__(self, name: str):
        self.name = name
        self.failures: list[str] = []   # human-readable reasons
        self.warnings: list[str] = []

    @property
    def ok(self) -> bool:
        return not self.failures


def has_int_main(sources: list[Path]) -> bool:
    pat = re.compile(r"\bint\s+main\s*\(")
    for s in sources:
        try:
            if pat.search(s.read_text(encoding="utf-8", errors="replace")):
                return True
        except OSError:
            pass
    return False


# --- toolchain path flavor (MSYS/Windows) -----------------------------------
# A Cygwin/MSYS-flavored compiler (e.g. clang++ targeting *-windows-cygnus)
# understands POSIX paths (/tmp, /m/projects/...), while a native mingw g++ and
# a native-Windows Python speak drive-letter paths (M:/..., C:/...). When Python
# hands drive-letter paths to a cygwin compiler it can't even create its output
# ("unable to make temporary file"). We detect the flavor once per compiler and
# convert every path argument through `cygpath -u` for cygwin ones. On Linux
# nothing is cygwin-flavored, so this is a no-op.
_flavor_cache: dict = {}
_CYGPATH = shutil.which("cygpath")


def compiler_flavor(cc: str) -> str:
    if cc not in _flavor_cache:
        machine = ""
        try:
            r = subprocess.run([cc, "-dumpmachine"], capture_output=True, text=True)
            machine = (r.stdout or "").strip().lower()
        except OSError:
            pass
        _flavor_cache[cc] = ("cygwin"
                             if any(k in machine for k in ("cygnus", "cygwin", "msys"))
                             else "native")
    return _flavor_cache[cc]


def to_compiler_path(p, flavor: str) -> str:
    p = str(p)
    if flavor == "cygwin" and _CYGPATH:
        r = subprocess.run([_CYGPATH, "-u", p], capture_output=True, text=True)
        if r.returncode == 0 and r.stdout.strip():
            return r.stdout.strip()
    return p


# --- sanitizer capability (per compiler) ------------------------------------
# A sanitizer requested in meta.yaml is only usable if the compiler can actually
# build+link with it. ThreadSanitizer in particular does NOT exist on the Windows
# toolchains -- native mingw g++ has no libtsan (-ltsan is missing) and Cygwin
# clang rejects the flag outright ("unsupported option for target ...-cygnus").
# So on those a concurrency example tagged `sanitizers: [thread]` would fail to
# build even though its code is fine. We probe each (compiler, sanitizer) once by
# compiling a trivial TU; unsupported ones are dropped for THAT compiler, which
# lets it still build and RUN the example (real threads, just no race detector),
# while a sanitizer-capable toolchain (Linux g++/clang locally under WSL, and CI)
# runs it under the sanitizer for the real correctness check. Flavor detection
# cannot decide this: native mingw reports the same "native" flavor as Linux.
_san_cache: dict = {}


def sanitizer_supported(cc: str, san: str) -> bool:
    key = (cc, san)
    if key not in _san_cache:
        flavor = compiler_flavor(cc)
        tmpdir = Path(tempfile.mkdtemp(prefix="cppws_santest_"))
        try:
            src = tmpdir / "probe.cpp"
            src.write_text("int main() { return 0; }\n")
            out = tmpdir / ("probe" + EXE_SUFFIX)
            cmd = [cc, f"-fsanitize={san}",
                   to_compiler_path(src, flavor), "-o", to_compiler_path(out, flavor)]
            try:
                r = subprocess.run(cmd, capture_output=True, text=True)
                _san_cache[key] = (r.returncode == 0)
            except OSError:
                _san_cache[key] = False
        finally:
            shutil.rmtree(tmpdir, ignore_errors=True)
    return _san_cache[key]


# --- runtime capability (per compiler) --------------------------------------
# Some standard-library features COMPILE everywhere but do not RUN on a given
# platform's runtime. The concrete case: std::shared_mutex on the MSYS2 UCRT64
# g++ (winpthreads) aborts at run time inside lock_shared() -- an assertion in
# winpthreads' pthread_rwlock, not a bug in the example -- while Cygwin clang and
# every Linux toolchain (WSL, CI) run it fine. An example can declare the runtime
# features it needs with `requires_runtime: [shared_mutex]`; we probe each
# (compiler, feature) once by compiling AND running a tiny program that exercises
# it. If the probe aborts, we still BUILD the example on that compiler but SKIP
# running it there (with a warning), and rely on a runtime-capable toolchain
# (WSL/CI) for the real execution + sanitizer check -- the same philosophy as the
# per-compiler sanitizer skip above.
_rt_cache: dict = {}

RUNTIME_PROBES: dict = {
    "shared_mutex": (
        "#include <shared_mutex>\n#include <mutex>\n#include <thread>\n"
        "int main(){ std::shared_mutex sm;\n"
        "  auto rd=[&]{ for(int i=0;i<200;++i){ std::shared_lock<std::shared_mutex> l(sm); } };\n"
        "  std::thread a(rd), b(rd); a.join(); b.join();\n"
        "  { std::unique_lock<std::shared_mutex> w(sm); } return 0; }\n"
    ),
}


def runtime_supported(cc: str, feature: str, std_flag: str) -> bool:
    key = (cc, feature)
    if key not in _rt_cache:
        probe = RUNTIME_PROBES.get(feature)
        if probe is None:
            _rt_cache[key] = True          # unknown feature: do not block running
            return True
        flavor = compiler_flavor(cc)
        tmpdir = Path(tempfile.mkdtemp(prefix="cppws_rttest_"))
        try:
            src = tmpdir / "probe.cpp"
            src.write_text(probe)
            out = tmpdir / ("probe" + EXE_SUFFIX)
            cmd = [cc, f"-std={std_flag}",
                   to_compiler_path(src, flavor), "-o", to_compiler_path(out, flavor)]
            try:
                c = subprocess.run(cmd, capture_output=True, text=True)
                if c.returncode != 0:
                    # Cannot even build the probe -> treat as unsupported here.
                    _rt_cache[key] = False
                else:
                    r = subprocess.run([str(out)], capture_output=True, text=True,
                                       timeout=30)
                    _rt_cache[key] = (r.returncode == 0)
            except (OSError, subprocess.TimeoutExpired):
                _rt_cache[key] = False
        finally:
            shutil.rmtree(tmpdir, ignore_errors=True)
    return _rt_cache[key]


def sanitizer_run_env() -> dict:
    env = os.environ.copy()
    # Make any sanitizer report abort with a non-zero exit so run: true fails.
    env["ASAN_OPTIONS"] = "abort_on_error=1:halt_on_error=1"
    env["UBSAN_OPTIONS"] = "halt_on_error=1:print_stacktrace=1"
    env["TSAN_OPTIONS"] = "halt_on_error=1:exitcode=66"
    return env


def compile_cmd(cc: str, std_flag: str, ex: Example, sources: list[Path],
                out: Path, sanitizers: list[str], werror: bool) -> list[str]:
    flavor = compiler_flavor(cc)
    cmd = [cc, f"-std={std_flag}", "-I", to_compiler_path(ex.path, flavor)]
    if _shared_dir is not None:
        cmd += ["-I", to_compiler_path(_shared_dir, flavor)]
    cmd += ["-Wall", "-Wextra", "-pedantic"]
    opt = ex.meta.get("optimize") or ""
    if opt:
        cmd.append(f"-{opt}")
    if werror:
        cmd.append("-Werror")
    if sanitizers:
        cmd.append("-fsanitize=" + ",".join(sanitizers))
        cmd.append("-g")
    cmd += [to_compiler_path(s, flavor) for s in sources]
    # Extra link libraries (e.g. stdc++exp for std::print's terminal support on
    # libstdc++). Placed after the sources so the linker resolves them correctly.
    for lib in (ex.meta.get("libs") or []):
        cmd.append(f"-l{lib}")
    cmd += ["-o", to_compiler_path(out, flavor)]
    return cmd


def build_one(ex: Example, compilers: list[str], build_root: Path,
              args) -> Result:
    res = Result(ex.name)

    standard = ex.meta.get("standard", "")
    std_flag = STD_FLAG.get(standard)
    if not std_flag:
        res.failures.append(f"unknown/missing standard {standard!r} in meta.yaml")
        return res

    sources = ex.sources()
    if not sources:
        res.failures.append("no source files (.cpp) found")
        return res
    missing = [s for s in sources if not s.is_file()]
    if missing:
        res.failures.append("sources listed but not found: "
                            + ", ".join(m.name for m in missing))
        return res

    # Guardrail: every example must actually instantiate/use the feature.
    if not (ex.path / "main.cpp").is_file():
        res.failures.append("no main.cpp (every example needs one)")
    if not has_int_main(sources):
        res.failures.append("no `int main` found in the compiled sources")
    if res.failures:
        return res

    opt = ex.meta.get("optimize") or ""
    if opt and opt not in OPT_LEVELS:
        res.failures.append(f"unknown optimize {opt!r} (expected one of "
                            + ", ".join(OPT_LEVELS) + ")")
        return res

    sanitizers = ex.meta.get("sanitizers") or []
    required_runtime = ex.meta.get("requires_runtime") or []
    werror = ex.meta.get("werror", True)

    # Benchmarks are timing measurements, not correctness checks: their numbers
    # are meaningless on a shared CI runner. They are still COMPILED on every
    # push (a benchmark is code and can rot); they are only EXECUTED when the
    # author asks for it locally with --bench, which is also when output.txt is
    # regenerated.
    bench = bool(ex.meta.get("bench", False))
    run = bool(ex.meta.get("run", False))
    if bench and not args.bench:
        run = False

    run_stdouts: list[tuple[str, str]] = []

    for cc in compilers:
        # Drop any requested sanitizer this compiler cannot provide (e.g. thread
        # on the Windows toolchains), so it still builds and runs the example; a
        # sanitizer-capable toolchain (Linux/WSL, CI) does the real check.
        eff_sanitizers = sanitizers
        if sanitizers:
            eff_sanitizers = [s for s in sanitizers if sanitizer_supported(cc, s)]
            dropped = [s for s in sanitizers if s not in eff_sanitizers]
            if dropped:
                res.warnings.append(
                    f"{cc}: sanitizer(s) {','.join(dropped)} unsupported here, "
                    "built without them (run under a sanitizer-capable toolchain "
                    "-- Linux g++/clang or CI -- for the real check)")

        out = build_root / f"{ex.section}__{ex.slug}__{cc.replace('/', '_')}{EXE_SUFFIX}"
        cmd = compile_cmd(cc, std_flag, ex, sources, out, eff_sanitizers, werror)

        if args.dry_run:
            res.warnings.append(f"[dry-run] {cc}: {' '.join(cmd)}"
                                + ("  (would run binary)" if run else ""))
            continue

        proc = subprocess.run(cmd, capture_output=True, text=True)
        if proc.returncode != 0:
            res.failures.append(f"{cc}: compile failed\n"
                                + _indent(proc.stderr or proc.stdout))
            continue

        # Optional negative check: one standard lower should NOT compile if the
        # example genuinely requires its declared version. Never fatal.
        if args.std_lower_check:
            _lower_check(res, cc, ex, sources, standard, eff_sanitizers, werror,
                         build_root)

        # Skip RUNNING (not building) on a compiler whose runtime cannot execute a
        # feature this example needs (e.g. shared_mutex on winpthreads g++). A
        # runtime-capable toolchain (WSL/CI) still runs it for the real check.
        run_here = run
        if run_here and required_runtime:
            unsupported = [f for f in required_runtime
                           if not runtime_supported(cc, f, std_flag)]
            if unsupported:
                run_here = False
                res.warnings.append(
                    f"{cc}: runtime feature(s) {','.join(unsupported)} not usable "
                    "here (built, not run -- run under a runtime-capable toolchain "
                    "-- Linux g++/clang or CI -- for the real check)")

        if run_here:
            renv = sanitizer_run_env() if eff_sanitizers else None
            # A benchmark legitimately runs for minutes (many sizes x samples).
            limit = 900 if bench else 60
            try:
                rp = subprocess.run([str(out)], capture_output=True, text=True,
                                    env=renv, timeout=limit)
            except subprocess.TimeoutExpired:
                res.failures.append(f"{cc}: run timed out ({limit}s)")
                continue
            if rp.returncode != 0:
                reason = "sanitizer report" if eff_sanitizers else "non-zero exit"
                res.failures.append(
                    f"{cc}: run failed ({reason}, exit {rp.returncode})\n"
                    + _indent((rp.stderr or rp.stdout)))
            else:
                run_stdouts.append((cc, rp.stdout))

    if run and args.write_output and run_stdouts and not args.dry_run:
        _write_output_file(ex, run_stdouts, res, bench)

    if run and args.write_trace and not args.dry_run and not bench:
        _write_trace_file(ex, compilers, std_flag, build_root, res)

    return res


def _write_output_file(ex: Example, run_stdouts: list, res: Result,
                       bench: bool = False) -> None:
    """Persist a run's stdout to output.txt next to the example so the site can
    show it. All compilers should print identically; warn (don't fail) if not."""
    distinct = {out for _, out in run_stdouts}
    first_cc, first_out = run_stdouts[0]
    if len(distinct) > 1 and not bench:
        res.warnings.append(
            f"compilers produced DIFFERENT stdout; wrote {first_cc}'s. Output may "
            "be nondeterministic -- review before committing (or drop output.txt).")
    if len(distinct) > 1 and bench:
        # Expected: a benchmark prints timings, which never match to the ns. The
        # committed output.txt is one reference run and says so in its header.
        res.warnings.append(f"benchmark: wrote {first_cc}'s timings as the "
                            "reference run (each compiler times differently)")
    target = ex.path / "output.txt"
    if first_out.strip() == "":
        res.warnings.append("run produced no stdout; output.txt not written")
        return
    # Keep LF endings so the committed file is stable across platforms.
    with open(target, "w", encoding="utf-8", newline="\n") as f:
        f.write(first_out)
    res.warnings.append(f"wrote output.txt ({len(first_out.splitlines())} lines)")


def _gdb_arg_path(p) -> str:
    """Path for gdb's OWN consumption (the binary to debug, the -x script). An
    MSYS-built gdb mangles drive-letter paths when creating the inferior process
    (it resolves C:/... relative to $HOME), so hand it a POSIX path via cygpath.
    On Linux there is no cygpath and this is identity."""
    if _CYGPATH:
        r = subprocess.run([_CYGPATH, "-u", str(p)], capture_output=True, text=True)
        if r.returncode == 0 and r.stdout.strip():
            return r.stdout.strip()
    return str(p).replace("\\", "/")


def _inferior_path(p) -> str:
    """Path handed to the INFERIOR's C runtime (freopen target) and read back by
    gdb's embedded Python. A native-Windows inferior cannot resolve an MSYS
    /tmp/... path, so convert to a real Windows path (cygpath -m gives a mixed,
    forward-slash form like C:/msys64/tmp/...). On Linux there is no cygpath and
    a normal POSIX path is what the inferior wants, so this is identity."""
    if _CYGPATH:
        r = subprocess.run([_CYGPATH, "-m", str(p)], capture_output=True, text=True)
        if r.returncode == 0 and r.stdout.strip():
            return r.stdout.strip()
    return str(p).replace("\\", "/")


def _write_trace_file(ex: Example, compilers: list[str], std_flag: str,
                      build_root: Path, res: Result) -> None:
    """Record a gdb single-step execution trace to trace.json next to the example
    (for the site's step-through player). Built from a dedicated -O0 -g binary so
    line stepping is faithful; runs under gdb via trace_gdb.py. Never fatal --
    any problem is a note, since this is display sugar, not the correctness gate.
    """
    sanitizers = ex.meta.get("sanitizers") or []
    if "thread" in sanitizers:
        return  # concurrency: no single deterministic trace to animate
    if not ex.meta.get("trace", True):
        return  # author opted this example out (e.g. nondeterministic)

    gdb = shutil.which("gdb")
    if not gdb:
        res.warnings.append("gdb not found; trace.json not written (skipped)")
        return
    # gdb pairs most reliably with g++; fall back to whatever compiler we have.
    cc = "g++" if "g++" in compilers else (compilers[0] if compilers else None)
    if cc is None:
        return

    flavor = compiler_flavor(cc)
    out = build_root / f"trace__{ex.section}__{ex.slug}{EXE_SUFFIX}"
    cmd = [cc, f"-std={std_flag}", "-I", to_compiler_path(ex.path, flavor),
           "-O0", "-g"]
    cmd += [to_compiler_path(s, flavor) for s in ex.sources()]
    for lib in (ex.meta.get("libs") or []):
        cmd.append(f"-l{lib}")
    cmd += ["-o", to_compiler_path(out, flavor)]
    proc = subprocess.run(cmd, capture_output=True, text=True)
    if proc.returncode != 0:
        res.warnings.append("trace: -O0 -g build failed; trace.json not written\n"
                            + _indent(proc.stderr or proc.stdout))
        return

    tracer = Path(__file__).resolve().parent / "trace_gdb.py"
    trace_out = build_root / f"trace__{ex.section}__{ex.slug}.json"
    stdout_file = build_root / f"trace__{ex.section}__{ex.slug}.stdout"
    # Record steps in any file that belongs to this example (main.cpp + headers/
    # sources), so the trace follows calls into class members defined in .hpp.
    header_exts = (".hpp", ".h", ".hh", ".hxx")
    own_files = sorted({p.name for p in ex.path.iterdir()
                        if p.is_file() and p.suffix in SOURCE_EXTS + header_exts})

    env = os.environ.copy()
    env["TRACE_OUT"] = _inferior_path(trace_out)
    env["TRACE_STDOUT"] = _inferior_path(stdout_file)
    env["TRACE_TARGET"] = "main.cpp"
    env["TRACE_FILES"] = "|".join(own_files)

    gdb_cmd = [gdb, "-q", "-batch", "-x", _gdb_arg_path(tracer), _gdb_arg_path(out)]
    try:
        gp = subprocess.run(gdb_cmd, capture_output=True, text=True,
                            env=env, timeout=120)
    except subprocess.TimeoutExpired:
        res.warnings.append("trace: gdb timed out; trace.json not written")
        return

    if not trace_out.is_file():
        res.warnings.append("trace: gdb produced no trace; trace.json not written\n"
                            + _indent(gp.stderr or gp.stdout))
        return
    import json
    try:
        data = json.loads(trace_out.read_text(encoding="utf-8"))
    except ValueError:
        res.warnings.append("trace: tracer emitted invalid JSON; skipped")
        return
    steps = data.get("steps") or []
    if not steps:
        res.warnings.append("trace: empty step list; trace.json not written")
        return

    target = ex.path / "trace.json"
    with open(target, "w", encoding="utf-8", newline="\n") as f:
        json.dump(data, f, ensure_ascii=False)
        f.write("\n")
    n_out = sum(1 for s in steps if isinstance(s, dict) and s.get("out"))
    res.warnings.append(f"wrote trace.json ({len(steps)} steps, {n_out} with output)")


def _lower_check(res, cc, ex, sources, standard, sanitizers, werror, build_root):
    idx = STD_ORDER.index(standard) if standard in STD_ORDER else 0
    if idx == 0:
        return
    lower = STD_ORDER[idx - 1]
    out = build_root / f"lower__{ex.section}__{ex.slug}__{cc.replace('/', '_')}{EXE_SUFFIX}"
    cmd = compile_cmd(cc, STD_FLAG[lower], ex, sources, out, sanitizers, werror)
    proc = subprocess.run(cmd, capture_output=True, text=True)
    if proc.returncode == 0:
        res.warnings.append(
            f"{cc}: also compiles under {lower} -- example may not truly "
            f"require {standard} (or the lower-std check is not meaningful here)")


def _indent(text: str, prefix: str = "      ") -> str:
    text = (text or "").strip("\n")
    if not text:
        return prefix + "(no compiler output)"
    return "\n".join(prefix + line for line in text.splitlines())


# --- driver setup -----------------------------------------------------------
def resolve_content_repo(cli_path: str | None) -> Path:
    # The driver lives inside cppworkshop-examples (driver/ is a sibling of
    # content/), so by default the content repo is the driver's OWN repo root.
    # CLI arg / CONTENT_REPO_PATH still override, e.g. to build another checkout.
    repo_root = Path(__file__).resolve().parent.parent
    candidate = (cli_path
                 or os.environ.get("CONTENT_REPO_PATH")
                 or str(repo_root))
    return Path(candidate).resolve()


def resolve_compilers(requested: list[str], strict: bool) -> list[str]:
    available = []
    for cc in requested:
        if shutil.which(cc):
            available.append(cc)
        else:
            msg = f"WARNING: compiler not found, skipping: {cc}"
            if strict:
                print(f"ERROR: required compiler not found: {cc}", file=sys.stderr)
                sys.exit(2)
            print(msg, file=sys.stderr)
    return available


def main() -> int:
    ap = argparse.ArgumentParser(description="cppworkshop build/CI driver")
    ap.add_argument("content_repo", nargs="?", help="path to cppworkshop-examples")
    ap.add_argument("--compilers", default="g++,clang++")
    ap.add_argument("--filter", default=None)
    ap.add_argument("--std-lower-check", action="store_true")
    ap.add_argument("--keep", action="store_true")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--strict", action="store_true")
    ap.add_argument("--write-output", action="store_true")
    ap.add_argument("--write-trace", action="store_true")
    ap.add_argument("--bench", action="store_true")
    ap.add_argument("-q", "--quiet", action="store_true")
    args = ap.parse_args()

    content_repo = resolve_content_repo(args.content_repo)
    content_dir = content_repo / "content"
    if not content_dir.is_dir():
        print(f"ERROR: no content/ dir under {content_repo}", file=sys.stderr)
        return 2

    global _shared_dir
    shared = content_dir / SHARED_DIRNAME
    _shared_dir = shared if shared.is_dir() else None

    requested = [c.strip() for c in args.compilers.split(",") if c.strip()]
    if args.dry_run:
        compilers = requested  # no need for real toolchains to print the plan
    else:
        compilers = resolve_compilers(requested, args.strict)
        if not compilers:
            print("ERROR: none of the requested compilers are available "
                  f"({', '.join(requested)}).", file=sys.stderr)
            return 2

    examples = discover(content_dir, args.filter)
    if not examples:
        print(f"No examples found under {content_dir}"
              + (f" matching {args.filter!r}" if args.filter else ""))
        return 0

    print(f"Driver: {content_repo}")
    print(f"Compilers: {', '.join(compilers)}"
          + ("   (DRY RUN)" if args.dry_run else ""))
    print(f"Examples: {len(examples)}\n")

    build_root = Path(tempfile.mkdtemp(prefix="cppws-build-"))
    results: list[Result] = []
    try:
        for ex in examples:
            res = build_one(ex, compilers, build_root, args)
            results.append(res)
            _print_result(res, args.quiet)
    finally:
        if args.keep:
            print(f"\nBuild artifacts kept in: {build_root}")
        else:
            shutil.rmtree(build_root, ignore_errors=True)

    passed = sum(1 for r in results if r.ok)
    failed = len(results) - passed
    print("\n" + "=" * 60)
    print(f"SUMMARY: {passed} passed, {failed} failed, {len(results)} total")
    if failed:
        print("Failed: " + ", ".join(r.name for r in results if not r.ok))
    return 1 if failed else 0


def _print_result(res: Result, quiet: bool) -> None:
    if res.ok:
        if not quiet:
            print(f"  PASS  {res.name}")
            for w in res.warnings:
                print(f"        note: {w}")
    else:
        print(f"  FAIL  {res.name}")
        for f in res.failures:
            print(f"        {f}")
        for w in res.warnings:
            print(f"        note: {w}")


if __name__ == "__main__":
    sys.exit(main())
