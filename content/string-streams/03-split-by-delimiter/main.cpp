#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    // getline with a delimiter splits a string into fields. The three-argument
    // form reads up to the next delimiter (here a comma) instead of a newline,
    // and it keeps EMPTY fields between two adjacent delimiters.
    std::string csv = "red,green,,blue";   // note the empty field
    std::istringstream in(csv);

    std::vector<std::string> fields;
    std::string field;
    while (std::getline(in, field, ','))
        fields.push_back(field);

    std::cout << "fields: " << fields.size() << '\n';   // 4
    for (std::size_t i = 0; i < fields.size(); ++i)
        std::cout << i << ": [" << fields[i] << "]\n";
    return 0;
}
