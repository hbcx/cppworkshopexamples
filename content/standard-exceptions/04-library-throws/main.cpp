// You do not only throw these yourself -- the standard library throws them, and
// catching the right type is how you handle those failures. This shows three
// library calls that throw, each caught by its specific type. The exact what()
// wording is chosen by the implementation, so we print our own label and match
// on the TYPE, not the message text.
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3};
    try {
        int x = v.at(10);                  // std::out_of_range
        std::cout << x;
    }
    catch (const std::out_of_range&) {
        std::cout << "vector::at rejected index 10\n";
    }

    try {
        int n = std::stoi("xyz");          // std::invalid_argument
        std::cout << n;
    }
    catch (const std::invalid_argument&) {
        std::cout << "stoi rejected non-numeric text\n";
    }

    try {
        std::string s = "abc";
        std::string sub = s.substr(10);    // std::out_of_range (pos > size)
        std::cout << sub;
    }
    catch (const std::out_of_range&) {
        std::cout << "substr rejected position 10\n";
    }
    return 0;
}
