#include <source_location>
#include <iostream>

const char* baseName(const char* path) {
    const char* b = path;
    for (const char* p = path; *p; ++p)
        if (*p == '/' || *p == '\\')
            b = p + 1;
    return b;
}

void record(const char* what, std::source_location loc = std::source_location::current()) {
    std::cout << baseName(loc.file_name()) << ":" << loc.line() << "  " << what << "\n";
}

// ANTI-PATTERN: this wrapper does not forward a location, so record()'s own
// default fires -- capturing the call INSIDE auditBad. Every audited action is
// reported at the wrapper's line, never the real caller's.
void auditBad(const char* action) {
    record(action);
}

// FIX: give the wrapper its own source_location default (capturing the caller) and
// forward it into record().
void auditGood(const char* action, std::source_location loc = std::source_location::current()) {
    record(action, loc);
}

int main() {
    auditBad("login");     // reported at auditBad's body line
    auditBad("logout");    // ...the same line again
    auditGood("login");    // reported at THIS line
    auditGood("logout");   // reported at THIS line (different)
    return 0;
}
