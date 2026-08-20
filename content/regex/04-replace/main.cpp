#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string text = "call 555-1234 or 555-5678";
    std::regex phone(R"((\d{3})-(\d{4}))");

    // $1, $2 in the replacement are backreferences to capture groups 1 and 2.
    std::string masked = std::regex_replace(text, phone, "$1-XXXX");
    std::cout << masked << '\n';                       // call 555-XXXX or 555-XXXX

    // format_first_only: rewrite just the first match, leave the rest alone.
    std::string once = std::regex_replace(text, phone, "[redacted]",
                                          std::regex_constants::format_first_only);
    std::cout << once << '\n';                         // call [redacted] or 555-5678

    // format_no_copy: emit ONLY the replacements, dropping the unmatched text.
    // $& stands for the whole match.
    std::string only = std::regex_replace(text, phone, "$&;",
                                          std::regex_constants::format_no_copy);
    std::cout << only << '\n';                         // 555-1234;555-5678;
    return 0;
}
