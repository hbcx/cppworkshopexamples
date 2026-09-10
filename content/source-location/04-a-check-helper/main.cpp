#include <source_location>
#include <iostream>

const char* baseName(const char* path) {
    const char* b = path;
    for (const char* p = path; *p; ++p)
        if (*p == '/' || *p == '\\')
            b = p + 1;
    return b;
}

// A check that reports WHERE it failed, as a normal function. Unlike assert (a
// macro), this is a first-class function you can pass around and test.
void check(bool condition, const char* what,
           std::source_location loc = std::source_location::current()) {
    if (condition)
        std::cout << "ok:     " << what << "\n";
    else
        std::cout << "FAILED: " << what << "  at "
                  << baseName(loc.file_name()) << ":" << loc.line() << "\n";
}

int main() {
    int x = 5;
    check(x > 0, "x is positive");
    check(x > 10, "x exceeds 10");   // fails; the report points at this line
    return 0;
}
