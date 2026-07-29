#include <iostream>
#include <string>

int main() {
    std::string setting = "timeout=30";

    // Split "key=value" at the '=' -- the shape of a small parser.
    std::string::size_type eq = setting.find('=');
    if (eq != std::string::npos) {                 // MUST compare against npos
        std::string key = setting.substr(0, eq);   // [0, eq)
        std::string val = setting.substr(eq + 1);  // to the end
        std::cout << "key = " << key << ", value = " << val << '\n';
    }

    // A miss returns npos, NOT a false-y zero. if (find(...)) would be a bug.
    std::string::size_type miss = setting.find('?');
    std::cout << "'?' found? " << (miss != std::string::npos ? "yes" : "no") << '\n';

    // rfind and find_first_of.
    std::string path = "/usr/local/bin";
    std::cout << "last slash at " << path.rfind('/') << '\n';
    std::cout << "first vowel at " << path.find_first_of("aeiou") << '\n';

    // substr clamps an oversized length to what remains.
    std::cout << "substr(11, 999) = " << path.substr(11, 999) << '\n';
    return 0;
}
