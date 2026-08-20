#include <iostream>
#include <regex>
#include <string>

int main() {
    std::cout << std::boolalpha;
    std::smatch m;
    std::string s = "pay 50 dollars now";

    // SUPPORTED: lookahead. Match digits only when followed by " dollars".
    std::regex priceAhead(R"(\d+(?= dollars))");
    if (std::regex_search(s, m, priceAhead))
        std::cout << "lookahead (supported) : " << m.str() << '\n';   // 50

    // NOT SUPPORTED (described): std::regex ECMAScript has no LOOKBEHIND
    // ((?<=...)) and no NAMED groups ((?<name>...)) -- unlike PCRE, Python or
    // .NET. A pattern copied from a PCRE cheatsheet that uses them throws
    // std::regex_error at construction, so it fails only when the program runs:
    //   std::regex behind(R"((?<=\$)\d+)");   // lookbehind: not supported

    // WORKAROUND for "the number after a $": use a NUMBERED capture group and
    // read the piece you want, instead of a lookbehind or a named group.
    std::regex numbered(R"(\$(\d+))");
    std::string priced = "costs $50 today";
    if (std::regex_search(priced, m, numbered))
        std::cout << "numbered-group workaround : " << m[1].str() << '\n';  // 50
    return 0;
}
