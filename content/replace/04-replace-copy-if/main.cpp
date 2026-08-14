#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <string>

int main() {
    // Redact a log line for display: keep the source, write a masked copy where
    // every digit becomes '*'. A condition, copied out -- so replace_copy_if.
    std::string line = "user 42 paid 1500";
    std::string masked;

    std::replace_copy_if(line.begin(), line.end(),
                         std::back_inserter(masked),
                         [](char c) { return std::isdigit(static_cast<unsigned char>(c)); },
                         '*');

    std::cout << "original: " << line << '\n';
    std::cout << "masked:   " << masked << '\n';

    // replace_copy_if is to replace_copy what replace_if is to replace: the
    // predicate version of the copying substitute. Same one-for-one length, same
    // untouched source. Writing straight to a stream works too:
    std::cout << "to stream: ";
    std::replace_copy_if(line.begin(), line.end(),
                         std::ostream_iterator<char>(std::cout),
                         [](char c) { return c == ' '; }, '_');
    std::cout << '\n';
    return 0;
}
