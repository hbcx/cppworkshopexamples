#include <source_location>
#include <iostream>
#include <string>

const char* baseName(const char* path) {
    const char* b = path;
    for (const char* p = path; *p; ++p)
        if (*p == '/' || *p == '\\')
            b = p + 1;
    return b;
}

enum class Level { Info, Warn, Error };
const char* name(Level l) {
    switch (l) {
        case Level::Info:  return "INFO";
        case Level::Warn:  return "WARN";
        case Level::Error: return "ERROR";
    }
    return "?";
}

// A logger that records the call site, as a plain function -- no macro needed. The
// caller writes log(Level::Warn, "..."); the location is filled in automatically.
void log(Level lvl, const std::string& msg,
         std::source_location loc = std::source_location::current()) {
    std::cout << "[" << name(lvl) << "] "
              << baseName(loc.file_name()) << ":" << loc.line()
              << " -- " << msg << "\n";
}

int main() {
    log(Level::Info, "starting up");
    log(Level::Warn, "low on memory");
    return 0;
}
