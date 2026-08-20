#include <iostream>
#include <iterator>
#include <regex>
#include <string>

int main() {
    std::string text = "ip 10.0.0.1, gw 10.0.0.254, dns 8.8.8.8";
    std::regex ip(R"(\d+\.\d+\.\d+\.\d+)");

    // A regex_iterator walks EVERY non-overlapping match in the subject.
    // The default-constructed iterator is the end sentinel.
    std::sregex_iterator begin(text.begin(), text.end(), ip);
    std::sregex_iterator end;

    std::cout << "found " << std::distance(begin, end) << " addresses:\n";
    for (std::sregex_iterator it = begin; it != end; ++it) {
        const std::smatch& m = *it;                  // each step yields a full match
        std::cout << "  " << m.str() << " at offset " << m.position() << '\n';
    }
    return 0;
}
