#include <iostream>
#include <string>
#include <string_view>

// Trim ASCII blanks by moving the window inwards -- returns a view, no copy.
static std::string_view trim(std::string_view sv) {
    const char* ws = " \t";
    std::size_t b = sv.find_first_not_of(ws);
    if (b == std::string_view::npos) return {};      // all blank -> empty view
    std::size_t e = sv.find_last_not_of(ws);
    sv.remove_suffix(sv.size() - (e + 1));
    sv.remove_prefix(b);
    return sv;
}

// ONE signature serves a std::string, a literal, or a char buffer -- all borrow,
// none is forced into a std::string at the call site.
static void parseLine(std::string_view line) {
    std::size_t eq = line.find('=');
    if (eq == std::string_view::npos) { std::cout << "(no '=')\n"; return; }
    std::string_view key = trim(line.substr(0, eq));
    std::string_view val = trim(line.substr(eq + 1));
    std::cout << "key=[" << key << "] val=[" << val << "]\n";
}

int main() {
    std::string fromFile = "  host =  localhost  ";
    parseLine(fromFile);        // a std::string argument
    parseLine("port= 8080");    // a literal -- no std::string is built
    parseLine("  bare line ");  // no '='
    return 0;
}
