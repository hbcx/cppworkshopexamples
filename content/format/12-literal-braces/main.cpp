#include <format>
#include <iostream>
#include <string>

int main() {
    int n = 3;

    // WRONG (won't compile): a lone { or } is read as a placeholder delimiter, so
    // a JSON-looking literal breaks the parse:
    //   std::format("{ \"count\": {} }", n);        // error: stray '{' and '}'

    // RIGHT: double the braces you want literally -- {{ prints { and }} prints }.
    std::string json = std::format("{{ \"count\": {} }}", n);
    std::cout << json << '\n';                        // { "count": 3 }

    // The doubling rule is ONLY about the format string. Braces inside an
    // ARGUMENT are printed as-is; they are not re-parsed.
    std::string value = "{not a placeholder}";
    std::cout << std::format("value: {}\n", value);   // value: {not a placeholder}
    return 0;
}
