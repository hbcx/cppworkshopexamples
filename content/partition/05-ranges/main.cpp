#include <algorithm>
#include <ranges>
#include <string>
#include <vector>
#include <iostream>

struct Task {
    std::string name;
    bool urgent;
};

int main() {
    std::vector<Task> tasks{
        {"a", false}, {"b", true}, {"c", false}, {"d", true}, {"e", true}};

    // ranges::stable_partition takes the whole range and an optional PROJECTION,
    // and returns a SUBRANGE covering the second group (the elements that did not
    // satisfy the predicate). Here: urgent tasks first, order preserved.
    auto rest = std::ranges::stable_partition(tasks, [](bool u) { return u; },
                                              &Task::urgent);

    std::cout << "urgent first: ";
    for (const auto& t : tasks) std::cout << t.name << (t.urgent ? "! " : " ");
    std::cout << '\n';
    std::cout << "non-urgent count: " << std::ranges::distance(rest) << '\n';
    return 0;
}
