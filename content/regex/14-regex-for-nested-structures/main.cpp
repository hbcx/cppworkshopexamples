#include <iostream>
#include <regex>
#include <string>

int main() {
    // A pattern that tries to grab the text inside braces.
    std::regex greedy(R"(\{(.*)\})");
    std::smatch m;

    // Looks correct on a flat, single-level input.
    std::string flat = "{ level1 }";
    if (std::regex_search(flat, m, greedy))
        std::cout << "flat   greedy -> [" << m[1].str() << "]\n";   // " level1 "

    // WRONG on nesting: a regex cannot count matching pairs, so greedy .* runs
    // from the first { to the LAST } and swallows the inner braces instead of
    // returning the outermost balanced group.
    std::string nested = "{ a { b } c } tail";
    if (std::regex_search(nested, m, greedy))
        std::cout << "nested greedy -> [" << m[1].str() << "]\n";   // " a { b } c "

    // A lazy .*? does not fix it -- it stops at the FIRST }, cutting the group short.
    std::regex lazy(R"(\{(.*?)\})");
    if (std::regex_search(nested, m, lazy))
        std::cout << "nested lazy   -> [" << m[1].str() << "]\n";   // " a { b "

    std::cout << "neither is the balanced group: regex cannot count pairs. "
                 "Use a real parser (a stack, or a grammar) for nested structures.\n";
    return 0;
}
