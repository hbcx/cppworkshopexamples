// Each frame is a std::stacktrace_entry with three accessors: description() (the
// function), source_file() and source_line(). You iterate a stacktrace like any
// range. Which fields are filled depends on available debug info -- source_line()
// can be 0 and source_file() empty in an optimized or stripped build -- so read
// them defensively. This prints the top few frames (text is environment-specific).
#include <stacktrace>
#include <iostream>

void show() {
    std::stacktrace st = std::stacktrace::current();

    int shown = 0;
    for (const std::stacktrace_entry& frame : st) {
        std::cout << "#" << shown << " " << frame.description();
        if (frame.source_line() != 0) {                    // present only with debug info
            std::cout << " at " << frame.source_file() << ':' << frame.source_line();
        }
        std::cout << '\n';
        if (++shown == 3) break;                           // just the top few
    }
}

int main() {
    show();
    return 0;
}
