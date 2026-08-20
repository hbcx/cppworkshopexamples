#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string date = "2026-08-20";
    std::regex pat(R"((\d{4})-(\d{2})-(\d{2}))");   // three parenthesised groups

    std::smatch m;                                   // results for a std::string subject
    if (std::regex_match(date, m, pat)) {
        std::cout << "whole match : " << m[0].str() << '\n';   // group 0 = the whole match
        std::cout << "year        : " << m[1].str() << '\n';   // group 1 = first ( )
        std::cout << "month       : " << m[2].str() << '\n';
        std::cout << "day         : " << m[3].str() << '\n';
        std::cout << "group count : " << m.size() << '\n';     // 4 = whole + 3 groups

        // Each group also knows where it sits in the subject and how long it is.
        std::cout << "month at    : offset " << m.position(2)
                  << ", length " << m.length(2) << '\n';
    }
    return 0;
}
