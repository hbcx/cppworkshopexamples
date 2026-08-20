#include <iostream>
#include <regex>
#include <string>

int main() {
    std::cout << std::boolalpha;

    // icase: case-insensitive matching.
    std::regex ci("color", std::regex_constants::icase);
    std::cout << "icase 'color' in 'Color'      : "
              << std::regex_search(std::string("Color"), ci) << '\n';   // true

    // A different GRAMMAR: POSIX extended (ERE) instead of the default ECMAScript.
    std::regex ere("colou?r", std::regex_constants::extended);
    std::cout << "extended 'colou?r' on 'colour': "
              << std::regex_search(std::string("colour"), ere) << '\n'; // true

    // optimize: spend longer building the regex to make later matching faster.
    std::regex fast(R"(\d{3}-\d{4})", std::regex_constants::optimize);
    std::cout << "optimize on '555-1234'        : "
              << std::regex_search(std::string("555-1234"), fast) << '\n'; // true

    // nosubs: do not track capture groups when you only need a yes/no answer.
    std::regex nosub(R"((\d+))", std::regex_constants::nosubs);
    std::smatch m;
    std::string num = "42";
    std::regex_search(num, m, nosub);
    std::cout << "nosubs -> extra groups kept   : " << (m.size() - 1) << '\n'; // 0
    return 0;
}
