#include <iostream>
#include <string>

int main() {
    std::string s = "https://example.com";

    // The buggy test: treat find() as a bool.
    // Direction 1 -- a MISS returns npos (huge, so truthy): "found" is printed
    // for something that is not there.
    if (s.find("ftp"))
        std::cout << "[buggy] thinks \"ftp\" is present (it is not)\n";

    // Direction 2 -- a match at index 0 returns 0 (falsy): the prefix that IS
    // there is reported missing.
    if (!s.find("https"))
        std::cout << "[buggy] correct only by luck: !0 is true, so this branch is taken\n";
    else
        std::cout << "[buggy] thinks \"https\" is absent (it is at index 0)\n";

    // The correct test: compare against npos.
    std::cout << "[correct] has \"ftp\"?   "
              << (s.find("ftp") != std::string::npos ? "yes" : "no") << '\n';   // no
    std::cout << "[correct] has \"https\"? "
              << (s.find("https") != std::string::npos ? "yes" : "no") << '\n'; // yes
    std::cout << "(find(\"https\") returned index " << s.find("https") << ")\n"; // 0
    return 0;
}
