#include <ranges>
#include <vector>
#include <string>
#include <string_view>
#include <iostream>

int main() {
    std::vector<std::string> parts{"2024", "08", "12"};

    // join_with flattens a range of ranges, inserting a separator BETWEEN the
    // inner ranges but not at the ends -- the range version of a string join.
    auto date = parts | std::views::join_with('-') | std::ranges::to<std::string>();
    std::cout << "join_with('-'): " << date << '\n';

    // The separator can be a range of its own, not just a single element.
    std::vector<std::string> words{"lazy", "by", "default"};
    auto sentence = words | std::views::join_with(std::string_view{", "})
                          | std::ranges::to<std::string>();
    std::cout << "join_with(\", \"): " << sentence << '\n';
    return 0;
}
