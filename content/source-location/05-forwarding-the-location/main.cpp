#include <source_location>
#include <iostream>

const char* baseName(const char* path) {
    const char* b = path;
    for (const char* p = path; *p; ++p)
        if (*p == '/' || *p == '\\')
            b = p + 1;
    return b;
}

// The low-level sink: reports whatever location it is given.
void record(const char* what, std::source_location loc = std::source_location::current()) {
    std::cout << baseName(loc.file_name()) << ":" << loc.line() << "  " << what << "\n";
}

// A wrapper that wants record() to blame the REAL caller, not this wrapper. It
// takes its own source_location default -- capturing the caller -- and FORWARDS it
// down to record(), overriding record()'s own default.
void audit(const char* action, std::source_location loc = std::source_location::current()) {
    record(action, loc);
}

int main() {
    audit("login");    // record() reports THIS line, not a line inside audit()
    audit("logout");
    return 0;
}
