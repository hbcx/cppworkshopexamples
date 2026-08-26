// std::stacktrace (C++23, <stacktrace>) captures the chain of function calls that
// led to the current point -- the same call stack a debugger shows. std::
// stacktrace::current() takes a snapshot; it is a range of stacktrace_entry, one
// per frame, newest first. Here we only look at INVARIANTS (it is non-empty, it
// has frames): the actual frame text depends on the build and platform.
#include <stacktrace>
#include <iostream>

int main() {
    std::stacktrace trace = std::stacktrace::current();

    std::cout << std::boolalpha;
    std::cout << "captured a trace: " << !trace.empty() << '\n';           // true
    std::cout << "current() is never empty while running: "
              << (trace.size() >= 1) << '\n';                              // true
    return 0;
}
