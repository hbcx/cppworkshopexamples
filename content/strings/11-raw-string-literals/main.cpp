#include <iostream>
#include <string>

int main() {
    // A Windows path: no doubled backslashes.
    std::string path = R"(C:\logs\app\today.txt)";
    std::cout << "path:  " << path << '\n';

    // A regex pattern, readable as the engine sees it.
    std::string pattern = R"(\d+\.\d+)";
    std::cout << "regex: " << pattern << '\n';

    // Multi-line, with quotes kept literally -- a custom delimiter (json) lets
    // the body contain )" safely; the literal ends only at )json".
    std::string blob = R"json({
  "name": "café",
  "path": "C:\temp"
})json";
    std::cout << "json blob has " << blob.size() << " bytes and contains a quote? "
              << (blob.find('"') != std::string::npos ? "yes" : "no") << '\n';

    // Raw only changes how the SOURCE is read: the escaped and raw spellings of
    // the same path produce identical bytes.
    std::string escaped = "C:\\logs\\app\\today.txt";
    std::cout << "raw == escaped equivalent? " << (path == escaped ? "yes" : "no") << '\n';
    return 0;
}
