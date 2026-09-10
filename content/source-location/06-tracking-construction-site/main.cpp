#include <source_location>
#include <iostream>

const char* baseName(const char* path) {
    const char* b = path;
    for (const char* p = path; *p; ++p)
        if (*p == '/' || *p == '\\')
            b = p + 1;
    return b;
}

// A constructor with a source_location default records where each object was
// created -- useful for tracking leaks, dangling handles, or which call site
// produced a bad object.
struct Tracked {
    std::source_location origin;
    explicit Tracked(std::source_location loc = std::source_location::current())
        : origin(loc) {}

    void report() const {
        std::cout << "created at " << baseName(origin.file_name()) << ":" << origin.line() << "\n";
    }
};

int main() {
    Tracked a;
    Tracked b;
    a.report();
    b.report();
    return 0;
}
