"""gdb-driven single-step tracer for the cppworkshop execution player.

Run by driver/build.py (never directly) as:  gdb -q -batch -x trace_gdb.py BIN
with these environment variables set:

  TRACE_OUT      where to write the trace JSON
  TRACE_STDOUT   file to redirect the inferior's stdout into (see below)
  TRACE_TARGET   source file whose lines we record (default: main.cpp)

It records the REAL execution: it single-steps the program and, for every stop
in TRACE_TARGET, appends the line number. Library calls compiled without debug
info are stepped over automatically, so only the example's own lines (including
descents into its own constructors/destructors) are captured. This is why the
trace must come from running, not static analysis -- loop counts, taken
branches, and how many destructors run are only knowable at runtime.

Per-step output: the inferior's stdout is redirected (via freopen, from inside
the inferior, because a native Windows CRT needs a real OS path) to TRACE_STDOUT.
After each step we flush and read what newly appeared, attributing it to the line
that just executed. Result format: {"file": T, "steps": [{"line": N, "out": S?}]}.
"""

import gdb
import json
import os

TARGET = os.environ.get("TRACE_TARGET", "main.cpp")
# Every file that belongs to THIS example (basenames): we record steps in any of
# them -- main.cpp plus headers/sources where the class' members live -- but not
# system headers. Falls back to just the primary file.
TARGETS = set(f for f in os.environ.get("TRACE_FILES", "").split("|") if f) or {TARGET}
OUT = os.environ["TRACE_OUT"]
OUTFILE = os.environ["TRACE_STDOUT"].replace("\\", "/")
MAX_STEPS = 20000


def basename(p):
    if not p:
        return None
    return p.replace("\\", "/").split("/")[-1]


def resolve_stdout_expr():
    """A gdb expression that evaluates to the inferior's stdout FILE*.

    glibc exposes a real `stdout` global; the Windows UCRT does not -- there the
    handle comes from __acrt_iob_func(1). Try the portable one first.
    """
    candidates = [
        "stdout",
        "((void*(*)(int))__acrt_iob_func)(1)",
    ]
    for expr in candidates:
        try:
            gdb.parse_and_eval(expr)
            return expr
        except gdb.error:
            continue
    return None


_seen = [0]


def read_new():
    try:
        with open(OUTFILE, "rb") as f:
            data = f.read()
    except OSError:
        return ""
    chunk = data[_seen[0]:]
    _seen[0] = len(data)
    return chunk.decode("utf-8", "replace").replace("\r\n", "\n")


def main():
    exited = {"v": False}
    gdb.events.exited.connect(lambda e: exited.__setitem__("v", True))

    gdb.execute("set pagination off")
    gdb.execute("set width 0")
    gdb.execute("break main")
    gdb.execute("run")

    # Redirect the inferior's stdout so we can poll it after each step.
    stdout_expr = resolve_stdout_expr()
    can_capture = False
    if stdout_expr is not None:
        try:
            gdb.parse_and_eval('(void*)freopen("%s", "w", %s)' % (OUTFILE, stdout_expr))
            can_capture = True
        except gdb.error:
            can_capture = False

    steps = []
    n = 0
    while n < MAX_STEPS and not exited["v"]:
        n += 1
        try:
            frame = gdb.selected_frame()
        except gdb.error:
            break
        sal = frame.find_sal()
        fname = basename(sal.symtab.filename) if sal.symtab else None
        line = sal.line
        rec = None
        if fname in TARGETS and line and line > 0:
            rec = {"file": fname, "line": line}
            steps.append(rec)
        try:
            gdb.execute("step")
        except gdb.error:
            break
        if can_capture and not exited["v"]:
            try:
                gdb.parse_and_eval("(int)fflush(0)")
            except gdb.error:
                pass
            out = read_new()
            if out:
                tgt = rec if rec is not None else (steps[-1] if steps else None)
                if tgt is not None:
                    tgt["out"] = tgt.get("out", "") + out

    if can_capture:
        tail = read_new()
        if tail and steps:
            steps[-1]["out"] = steps[-1].get("out", "") + tail

    with open(OUT, "w", encoding="utf-8") as f:
        json.dump({"file": TARGET, "steps": steps}, f, ensure_ascii=False)


main()
