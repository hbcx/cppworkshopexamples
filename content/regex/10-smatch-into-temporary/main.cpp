#include <iostream>
#include <regex>
#include <string>

// Safe: the temporary argument lives for the whole call, and .str() COPIES the
// group out before returning, so nothing points back into a string that is gone.
static std::string extractYear(const std::string& s) {
    std::smatch m;
    static const std::regex re(R"((\d{4}))");
    if (std::regex_search(s, m, re))
        return m[1].str();      // copy out -> caller gets an owning std::string
    return "";
}

int main() {
    // WRONG (won't compile, C++14+): searching a TEMPORARY string. The overload
    // taking std::string&& is =deleted precisely because the smatch would hold
    // iterators into a string destroyed at the semicolon:
    //   std::smatch m;
    //   std::regex_search(std::string("year 2026"), m, std::regex(R"(\d{4})"));
    //   std::cout << m[0];        // would dangle -> the overload is deleted

    // WRONG (described): an smatch outliving the subject it points into.
    //   std::smatch m;
    //   { std::string local = "year 2026"; std::regex_search(local, m, re); }
    //   std::cout << m[0];        // local is gone; m's iterators now dangle

    // RIGHT: keep the subject alive while the smatch is read.
    std::string text = "invoice 2026 total";
    std::smatch m;
    static const std::regex re(R"((\d{4}))");
    if (std::regex_search(text, m, re))
        std::cout << "year (subject alive) : " << m[1].str() << '\n';

    // RIGHT: or copy out immediately, so the result owns its characters.
    std::cout << "year (copied out)    : " << extractYear("dated 1999 ok") << '\n';
    return 0;
}
