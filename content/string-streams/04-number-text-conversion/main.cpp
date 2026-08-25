#include <iostream>
#include <sstream>
#include <string>

int main() {
    // Text -> number: feed the text to an istringstream and extract.
    std::string text = "255";
    std::istringstream in(text);
    int n = 0;
    in >> n;
    std::cout << "parsed: " << (n + 1) << '\n';   // 256

    // Number -> text: stream the value into an ostringstream and take .str().
    std::ostringstream out;
    out << 3.14159;
    std::cout << "formatted: " << out.str() << '\n';   // 3.14159

    // For a SINGLE value the standalone helpers are simpler and faster:
    //   std::to_string(n)   number -> text
    //   std::stoi(text)     text -> int (throws on bad input)
    //   std::to_chars / std::from_chars (C++17): fastest, no locale, no throw.
    // Keep stringstream for converting SEVERAL values or when you need formatting.
    std::cout << "to_string: " << std::to_string(42) << '\n';        // 42
    std::cout << "stoi:      " << (std::stoi("100") * 2) << '\n';     // 200
    return 0;
}
