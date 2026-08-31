// A flat_map keeps its keys and values in two SEPARATE sorted vectors (a
// struct-of-arrays layout). keys() and values() hand you each column, so the
// keys are a contiguous sorted array you can binary-search or pass to a C API.

#include <algorithm>
#include <flat_map>
#include <iostream>
#include <string>

int main() {
    std::flat_map<int, std::string> codes{{404, "Not Found"},
                                          {200, "OK"},
                                          {500, "Server Error"}};

    const auto& keys = codes.keys();       // contiguous, sorted: 200 404 500
    const auto& vals = codes.values();     // parallel:           OK, Not Found, ...

    std::cout << "keys:  ";
    for (int k : keys) std::cout << k << ' ';
    std::cout << "\nvalues in step:\n";
    for (std::size_t i = 0; i < keys.size(); ++i)
        std::cout << "  keys[" << i << "]=" << keys[i]
                  << " values[" << i << "]=" << vals[i] << '\n';

    // The key column is a real sorted array: binary_search works directly on it.
    bool has404 = std::binary_search(keys.begin(), keys.end(), 404);
    std::cout << "binary_search on keys() for 404: " << std::boolalpha << has404 << '\n';
}
