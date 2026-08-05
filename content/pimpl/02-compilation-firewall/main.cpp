#include "report.hpp"

#include <iostream>

int main() {
    Report r;
    r.addRow("cpu", 42);
    r.addRow("mem", 71);
    std::cout << "report: " << r.render() << "\n";

    // The public object is just a pointer, whatever Impl holds.
    std::cout << "sizeof(Report) = " << Report::footprint()
              << ", one pointer = " << sizeof(void*) << " -> "
              << (Report::footprint() == sizeof(void*) ? "stable ABI" : "?") << "\n";
}
