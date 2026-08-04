#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Split on a single-character delimiter. istringstream + getline keeps empty
// fields, which find/substr by hand tends to drop.
static std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> out;
    std::istringstream iss(s);
    std::string field;
    while (std::getline(iss, field, delim))
        out.push_back(field);
    return out;
}

int main() {
    // The classic line loop, over an in-memory stream (no real stdin needed).
    std::string document = "first line\nsecond line\nthird line";
    std::istringstream lines(document);
    std::string line;
    int n = 0;
    while (std::getline(lines, line))
        std::cout << "line " << ++n << ": " << line << '\n';

    // Split a CSV row into fields.
    std::vector<std::string> fields = split("timeout,30,seconds", ',');
    std::cout << "fields: " << fields.size() << " ->";
    for (const std::string& f : fields) std::cout << " [" << f << ']';
    std::cout << '\n';

    // An empty field between two delimiters is preserved.
    std::vector<std::string> withEmpty = split("a,,c", ',');
    std::cout << "\"a,,c\" middle field empty? "
              << (withEmpty.size() == 3 && withEmpty[1].empty() ? "yes" : "no") << '\n';
    return 0;
}
