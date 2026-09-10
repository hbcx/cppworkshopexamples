#include <source_location>
#include <iostream>

const char* baseName(const char* path) {
    const char* b = path;
    for (const char* p = path; *p; ++p)
        if (*p == '/' || *p == '\\')
            b = p + 1;
    return b;
}

// ANTI-PATTERN (described, does NOT work as written):
//
//   template <typename... Args>
//   void logf(const char* fmt, Args... args,
//             std::source_location loc = std::source_location::current());
//
// A defaulted source_location cannot usefully follow a function parameter pack:
// the arguments are deduced into Args..., so there is nothing left for loc to
// default from at the call site. The natural "format, args, then location"
// signature simply does not capture the caller.

// FIX: bundle the format and the location into one type whose constructor captures
// the location, and take THAT first. The pack then trails freely. This is exactly
// how C++23's std::format_string carries a source location for std::print-style
// logging.
struct FmtLoc {
    const char* fmt;
    std::source_location loc;
    // Implicit, so a plain string literal at the call site becomes a FmtLoc and
    // captures the location there.
    FmtLoc(const char* f, std::source_location l = std::source_location::current())
        : fmt(f), loc(l) {}
};

template <typename... Args>
void logf(FmtLoc where, Args... args) {
    std::cout << baseName(where.loc.file_name()) << ":" << where.loc.line()
              << "  " << where.fmt << "  [" << sizeof...(args) << " args]\n";
}

int main() {
    logf("user %d logged in", 42);   // the string becomes a FmtLoc capturing THIS line
    logf("shutting down");
    return 0;
}
