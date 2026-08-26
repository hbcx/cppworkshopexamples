// A captured trace is meant to be printed. std::stacktrace has an operator<< and
// a std::to_string overload, both producing a multi-line, human-readable dump --
// frame index, function description, and file:line where debug info is present.
// This is what you put in a crash log. The exact text depends on the compiler,
// build flags and platform, so treat it as a diagnostic for humans, not data.
#include <stacktrace>
#include <iostream>
#include <string>

int inner() {
    // Stream the whole stack from here.
    std::cout << std::stacktrace::current() << '\n';
    return 0;
}
int middle() { return inner(); }

int main() {
    std::cout << "--- stack trace ---\n";
    middle();

    // std::to_string gives the same text as a std::string, for a log line.
    std::string text = std::to_string(std::stacktrace::current());
    std::cout << "--- captured " << (text.empty() ? "nothing" : "the trace")
              << " as a string ---\n";
    return 0;
}
