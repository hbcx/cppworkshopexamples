#include <source_location>
#include <iostream>

// file_name() returns the path as the compiler saw it, which differs between
// toolchains -- reduce it to the bare file name so output is stable.
const char* baseName(const char* path) {
    const char* b = path;
    for (const char* p = path; *p; ++p)
        if (*p == '/' || *p == '\\')
            b = p + 1;
    return b;
}

int main() {
    std::source_location loc = std::source_location::current();
    std::cout << "file:     " << baseName(loc.file_name()) << "\n";
    std::cout << "line:     " << loc.line() << "\n";
    std::cout << "function: " << loc.function_name() << "\n";
    return 0;
}
