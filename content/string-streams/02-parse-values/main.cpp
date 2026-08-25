#include <iostream>
#include <sstream>
#include <string>

int main() {
    // std::istringstream reads values OUT of a string with >>, skipping the
    // whitespace between them -- the mirror image of building one.
    std::string line = "Ada 1815 3.5";
    std::istringstream in(line);

    std::string name;
    int year = 0;
    double score = 0.0;
    in >> name >> year >> score;

    std::cout << "name:  " << name << '\n';
    std::cout << "year:  " << year << '\n';
    std::cout << "score: " << score << '\n';
    std::cout << "all read ok: " << static_cast<bool>(in) << '\n';   // 1
    return 0;
}
