#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // operator/ (a NON-member function) joins two paths and inserts the
    // directory separator for you -- this is the right way to build a path,
    // never string "+".
    fs::path base = "var/log";
    fs::path full = base / "app" / "today.txt";
    std::cout << "joined: " << full.generic_string() << '\n';   // var/log/app/today.txt

    // /= appends as a new component (adds a separator); += concatenates onto the
    // last component (no separator). The difference matters:
    fs::path a = "report";
    a /= "final";                 // report/final
    std::cout << "append (/=): " << a.generic_string() << '\n';

    fs::path b = "report";
    b += "-final";                // report-final  (glued, no separator)
    std::cout << "concat (+=): " << b.generic_string() << '\n';

    // Building a suffixed filename mixes both: a new component, then a suffix.
    fs::path out = base / "backup";
    out += ".2026";               // var/log/backup.2026
    std::cout << "mixed:       " << out.generic_string() << '\n';
    return 0;
}
