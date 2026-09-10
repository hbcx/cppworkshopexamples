#include <source_location>
#include <iostream>

const char* baseName(const char* path) {
    const char* b = path;
    for (const char* p = path; *p; ++p)
        if (*p == '/' || *p == '\\')
            b = p + 1;
    return b;
}

// The key trick: a source_location default argument is evaluated at the CALL site,
// not inside note(). So each call reports where IT is, not where note() is defined.
void note(const char* msg, std::source_location loc = std::source_location::current()) {
    std::cout << baseName(loc.file_name()) << ":" << loc.line() << "  " << msg << "\n";
}

int main() {
    note("first call");
    note("second call");
    return 0;
}
