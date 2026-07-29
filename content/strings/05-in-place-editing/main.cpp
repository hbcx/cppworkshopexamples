#include <algorithm>
#include <iostream>
#include <string>

// Replace every occurrence of `from` with `to`, advancing past each replacement
// so we do not rescan (and never loop forever when `to` contains `from`).
static void replaceAll(std::string& s, const std::string& from, const std::string& to) {
    std::string::size_type pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos) {
        s.replace(pos, from.size(), to);
        pos += to.size();
    }
}

int main() {
    std::string s = "hello world";

    s.insert(5, ",");                 // "hello, world"
    std::cout << "after insert:  " << s << '\n';

    s.erase(5, 1);                    // remove the comma again -> "hello world"
    std::cout << "after erase:   " << s << '\n';

    s.replace(0, 5, "HELLO");         // "HELLO world"
    std::cout << "after replace: " << s << '\n';

    replaceAll(s, "o", "0");          // "HELL0 w0rld"
    std::cout << "replaceAll:    " << s << '\n';

    // Strip EVERY space with the erase-remove idiom -- erase alone cannot filter.
    std::string spaced = "a b  c   d";
    spaced.erase(std::remove(spaced.begin(), spaced.end(), ' '), spaced.end());
    std::cout << "spaces removed: " << spaced << '\n';   // "abcd"
    return 0;
}
