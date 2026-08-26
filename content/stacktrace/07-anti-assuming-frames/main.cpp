// Anti-pattern: assuming a frame's details are always there. description(),
// source_file() and source_line() depend on debug info: an optimized or stripped
// release build can leave source_line() at 0 and source_file() empty, and
// inlining removes frames entirely, so the stack differs from the source. Never
// build program logic on frame text or counts -- read them defensively and treat
// a trace as a best-effort diagnostic.
#include <stacktrace>
#include <iostream>
#include <string>

int main() {
    std::cout << std::boolalpha;
    std::stacktrace st = std::stacktrace::current();
    const std::stacktrace_entry& top = st.at(0);

    // Defensive read: the location may be absent (no debug info / stripped build).
    std::string file = std::string(top.source_file());
    bool have_location = top.source_line() != 0 && !file.empty();
    std::string label = have_location
        ? file + ':' + std::to_string(top.source_line())
        : std::string("<location unavailable>");

    // Whatever the build gives us, we produced a label without assuming fields.
    std::cout << "produced a location label safely: " << !label.empty() << '\n';   // true

    // A description is for a human to read, never something to parse or branch on.
    bool has_description = !std::string(top.description()).empty();
    std::cout << "top frame has a description: " << has_description << '\n';        // true
    return 0;
}
