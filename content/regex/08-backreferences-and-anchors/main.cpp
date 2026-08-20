#include <iostream>
#include <regex>
#include <string>

int main() {
    std::cout << std::boolalpha;

    // Backreference \1: the SAME captured text must appear again -- a doubled word.
    std::regex doubled(R"(\b(\w+)\s+\1\b)");
    std::smatch m;
    std::string s = "the the cat sat sat down";
    while (std::regex_search(s, m, doubled)) {
        std::cout << "doubled word: " << m[1].str() << '\n';
        s = m.suffix().str();                 // continue past this match
    }

    // Anchors ^ and $ pin the pattern to the ends of the whole string.
    std::regex ident(R"(^[A-Za-z_]\w*$)");
    std::cout << "'user_id' is an identifier : " << std::regex_match(std::string("user_id"), ident) << '\n'; // true
    std::cout << "'9lives' is an identifier  : " << std::regex_match(std::string("9lives"), ident) << '\n';  // false

    // \b is a word boundary: 'cat' as a whole word, not inside 'category'.
    std::regex catWord(R"(\bcat\b)");
    std::cout << "whole word 'cat' in 'a cat.'  : " << std::regex_search(std::string("a cat."), catWord) << '\n';   // true
    std::cout << "whole word 'cat' in 'category': " << std::regex_search(std::string("category"), catWord) << '\n'; // false
    return 0;
}
