#include <iostream>
#include <regex>
#include <string>

int main() {
    std::regex digits(R"(\d+)");   // one or more digits

    std::string a = "12345";
    std::string b = "order 12345 shipped";

    std::cout << std::boolalpha;

    // regex_match: the WHOLE string must match the pattern, end to end.
    std::cout << "match(\"12345\")             = " << std::regex_match(a, digits) << '\n';  // true
    std::cout << "match(\"order 12345 ...\")   = " << std::regex_match(b, digits) << '\n';  // false

    // regex_search: the pattern only has to appear SOMEWHERE in the string.
    std::cout << "search(\"order 12345 ...\")  = " << std::regex_search(b, digits) << '\n'; // true

    // To make match accept the number inside, the pattern must cover the rest too.
    std::regex line(R"(\w+ \d+ \w+)");
    std::cout << "match(full-line pattern)   = " << std::regex_match(b, line) << '\n';       // true
    return 0;
}
