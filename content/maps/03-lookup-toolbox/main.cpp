#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

int main() {
    std::map<std::string, int> stock{{"pen", 12}, {"ruler", 5}};

    // find: no insertion, returns an iterator (end() if absent). The safe default.
    auto it = stock.find("pen");
    if (it != stock.end()) {
        std::cout << "find pen -> " << it->second << '\n';
    }

    // at: checked access by value; throws std::out_of_range if the key is absent.
    try {
        std::cout << "at(ruler) -> " << stock.at("ruler") << '\n';
        std::cout << "at(eraser) -> " << stock.at("eraser") << '\n';  // throws
    } catch (const std::out_of_range&) {
        std::cout << "at(eraser) threw out_of_range\n";
    }

    // count: 0 or 1 for a map (keys are unique). A pre-C++20 existence check.
    std::cout << "count(pen) = " << stock.count("pen")
              << ", count(eraser) = " << stock.count("eraser") << '\n';

    // None of the above inserted anything.
    std::cout << "size unchanged = " << stock.size() << '\n';
    return 0;
}
