#include <source_location>
#include <iostream>

const char* baseName(const char* path) {
    const char* b = path;
    for (const char* p = path; *p; ++p)
        if (*p == '/' || *p == '\\')
            b = p + 1;
    return b;
}

// ANTI-PATTERN: the helper calls current() in its OWN body. That records the line
// INSIDE logBad, so every message points at the logger, not at the code that
// logged it -- useless for finding where something happened.
void logBad(const char* msg) {
    auto loc = std::source_location::current();   // always this line
    std::cout << "[bad]  " << baseName(loc.file_name()) << ":" << loc.line() << "  " << msg << "\n";
}

// FIX: take the location as a default argument so it is captured at the CALL site.
void logGood(const char* msg, std::source_location loc = std::source_location::current()) {
    std::cout << "[good] " << baseName(loc.file_name()) << ":" << loc.line() << "  " << msg << "\n";
}

int main() {
    logBad("hello");    // reports a line inside logBad -- the same for every call
    logBad("world");    // ...same line again
    logGood("hello");   // reports THIS line
    logGood("world");   // reports THIS line (different)
    return 0;
}
