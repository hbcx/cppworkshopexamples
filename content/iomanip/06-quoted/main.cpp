#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

int main() {
    // std::quoted (C++14) wraps a string in quotes on output and strips them on
    // input, ESCAPING embedded quotes -- so a value containing spaces or quotes
    // round-trips through a stream without being split or mangled.
    std::string original = "she said \"hi\", then left";

    std::stringstream ss;
    ss << std::quoted(original);
    std::cout << "encoded: " << ss.str() << '\n';

    // Read it back with quoted: you get the exact original, spaces and all.
    std::string round;
    ss >> std::quoted(round);
    std::cout << "decoded matches original: " << (round == original) << '\n';   // 1

    // Without quoted, >> would stop at the first space and the quotes would be
    // part of the data -- this is why quoted exists for delimited records.
    return 0;
}
