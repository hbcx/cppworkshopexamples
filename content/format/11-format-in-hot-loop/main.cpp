#include <format>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main() {
    std::vector<int> ids = {1, 2, 3, 4, 5};

    // WRONG for a hot path: every std::format call allocates a brand-new
    // std::string, which is then copied into the result and thrown away -- one
    // wasted allocation per line.
    std::string report;
    for (int id : ids)
        report += std::format("row {}\n", id);       // fresh string each iteration

    // RIGHT: format straight into the buffer you are building with format_to,
    // reusing its capacity and allocating no per-line temporary.
    std::string report2;
    report2.reserve(ids.size() * 8);
    auto out = std::back_inserter(report2);
    for (int id : ids)
        std::format_to(out, "row {}\n", id);

    std::cout << std::boolalpha << "same result: " << (report == report2) << '\n';
    std::cout << report2;
    return 0;
}
