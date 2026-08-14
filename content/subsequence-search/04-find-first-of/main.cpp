#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

int main() {
    std::string text = "the quick brown fox";
    std::string vowels = "aeiou";

    // std::find_first_of finds the first element of the range that equals ANY of
    // the elements in the second range. Here: the first vowel in the text. This
    // is NOT search -- it does not look for "aeiou" as a block, it matches any
    // single one of those characters.
    auto v = std::find_first_of(text.begin(), text.end(),
                                vowels.begin(), vowels.end());
    if (v != text.end()) {
        std::cout << "first vowel is '" << *v << "' at index "
                  << std::distance(text.begin(), v) << '\n';
    }

    // A typical use: scan for the next delimiter out of a set of separators --
    // the start of tokenizing when several characters can end a token.
    std::string path = "user=admin;role=root";
    std::string seps = "=;";
    auto d = std::find_first_of(path.begin(), path.end(), seps.begin(), seps.end());
    std::cout << "first separator '" << *d << "' at index "
              << std::distance(path.begin(), d) << '\n';
    return 0;
}
