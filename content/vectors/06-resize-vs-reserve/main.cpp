#include <cstddef>
#include <iostream>
#include <vector>

static void contents(const std::vector<int>& v, const char* label) {
    std::cout << label << ": size=" << v.size() << " {";
    for (std::size_t i = 0; i < v.size(); ++i) {
        std::cout << (i ? "," : "") << v[i];
    }
    std::cout << "}\n";
}

int main() {
    // reserve: capacity grows, size and contents stay put.
    {
        std::vector<int> v{1, 2, 3};
        v.reserve(10);
        std::cout << "after reserve(10): size=" << v.size()
                  << " capacity=" << v.capacity() << " (contents unchanged)\n";
        // v[5] here would be undefined behaviour -- those slots are not constructed.
    }

    // resize: size changes; grown slots are value-initialised, shrunk ones destroyed.
    {
        std::vector<int> v{1, 2, 3};
        v.resize(6);
        contents(v, "after resize(6)");   // three new zero-initialised ints appear
        v.resize(2);
        contents(v, "after resize(2)");   // the tail is destroyed
    }
    return 0;
}
