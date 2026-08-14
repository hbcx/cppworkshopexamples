#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

// Compare two characters ignoring case. Cast to unsigned char before tolower:
// passing a negative char is undefined.
static bool sameLetter(char a, char b) {
    return std::tolower(static_cast<unsigned char>(a)) ==
           std::tolower(static_cast<unsigned char>(b));
}

int main() {
    std::cout << std::boolalpha;

    std::string x = "Hello";
    std::string y = "hello";

    // All three algorithms take an optional binary predicate as a last argument,
    // so "equal" and "less" can mean whatever you need. Here: case-insensitive
    // equality -- the default == would say these differ.
    bool same = std::equal(x.begin(), x.end(), y.begin(), y.end(), sameLetter);
    std::cout << "\"Hello\" equals \"hello\" ignoring case? " << same << '\n';

    // mismatch with the same predicate: where do they differ, case aside?
    std::string a = "coProGram";
    std::string b = "coprogramME";
    auto d = std::mismatch(a.begin(), a.end(), b.begin(), b.end(), sameLetter);
    std::cout << "case-insensitive common prefix length: "
              << (d.first - a.begin()) << '\n';

    // lexicographical_compare with a case-insensitive element order: "apple"
    // sorts before "Banana" even though 'B' (66) is less than 'a' (97) in ASCII.
    std::string s1 = "apple", s2 = "Banana";
    bool less = std::lexicographical_compare(
        s1.begin(), s1.end(), s2.begin(), s2.end(),
        [](char c1, char c2) {
            return std::tolower(static_cast<unsigned char>(c1)) <
                   std::tolower(static_cast<unsigned char>(c2));
        });
    std::cout << "\"apple\" < \"Banana\" ignoring case? " << less << '\n';
    return 0;
}
