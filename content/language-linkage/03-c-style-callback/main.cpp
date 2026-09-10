#include <cstdlib>   // std::qsort
#include <cstddef>
#include <iostream>

// A comparator for the C function std::qsort. C-style callbacks receive void*
// (the API has no idea about the element type) and return the ordering as an int.
// Giving it C linkage matches the C-linkage function pointer qsort expects.
extern "C" int compare_ints(const void* a, const void* b) {
    int x = *static_cast<const int*>(a);
    int y = *static_cast<const int*>(b);
    return (x > y) - (x < y);   // -1, 0, or 1, with no risk of subtraction overflow
}

int main() {
    int data[] = {5, 2, 9, 1, 7};
    const std::size_t n = sizeof(data) / sizeof(data[0]);

    std::qsort(data, n, sizeof(int), compare_ints);

    std::cout << "sorted:";
    for (std::size_t i = 0; i < n; ++i)
        std::cout << " " << data[i];
    std::cout << "\n";
    return 0;
}
