#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <vector>

int main() {
    std::string csv = "a,  b ,c ,   d";
    std::regex sep(R"(\s*,\s*)");     // a comma with any spaces around it

    // Submatch -1 selects the text BETWEEN the matches -> this is a split.
    std::sregex_token_iterator it(csv.begin(), csv.end(), sep, -1);
    std::sregex_token_iterator end;

    std::vector<std::string> fields(it, end);
    std::cout << "fields: " << fields.size() << '\n';
    for (const std::string& f : fields)
        std::cout << "  [" << f << "]\n";

    // Submatch 0 selects the MATCHES themselves -- here, the separators.
    std::sregex_token_iterator sepIt(csv.begin(), csv.end(), sep, 0);
    std::cout << "separators: " << std::distance(sepIt, std::sregex_token_iterator()) << '\n';
    return 0;
}
