#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main() {
    // Anti-pattern: gluing a path together with string "+". You have to get the
    // separator right yourself, and edge cases bite: a directory that already
    // ends in a slash gives a doubled separator.
    std::string dir = "output/";     // note the trailing slash
    std::string name = "result.csv";

    std::string glued = dir + "/" + name;   // output//result.csv  -- doubled '/'
    std::cout << "string glue: " << glued << '\n';

    // Fix: operator/ inserts exactly one separator and never doubles it.
    fs::path good = fs::path(dir) / name;    // output/result.csv
    std::cout << "operator/:   " << good.generic_string() << '\n';

    // It also does the right thing when the left side has no trailing slash.
    fs::path noSlash = fs::path("output") / name;
    std::cout << "no slash:    " << noSlash.generic_string() << '\n';
    return 0;
}
