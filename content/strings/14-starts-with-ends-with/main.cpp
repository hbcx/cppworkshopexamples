#include <iostream>
#include <string>

int main() {
    std::string url = "https://example.com/api/v1";

    std::cout << "is https? " << (url.starts_with("https://") ? "yes" : "no") << '\n';
    std::cout << "is ftp?   " << (url.starts_with("ftp://") ? "yes" : "no") << '\n';
    std::cout << "ends /v1? " << (url.ends_with("/v1") ? "yes" : "no") << '\n';

    // Takes a single char too.
    std::string name = "report.csv";
    std::cout << "csv file? " << (name.ends_with(".csv") ? "yes" : "no") << '\n';

    // Correct on the short-string case, where substr(0, n) == prefix would build
    // a temporary and the old idioms were easy to get wrong.
    std::string tiny = "ht";
    std::cout << "\"ht\".starts_with(\"https\")? "
              << (tiny.starts_with("https") ? "yes" : "no") << '\n';   // no, no crash
    return 0;
}
