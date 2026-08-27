#include <iostream>
#include <vector>

// You can provide two versions of a member function that differ only in const:
// the const one is chosen on a const object and returns a read-only reference,
// the non-const one is chosen on a mutable object and returns a writable
// reference. This is exactly how the standard containers' operator[] works.

class IntGrid {
public:
    IntGrid(std::size_t n, int value) : data_(n, value) {}

    // Non-const: called on a mutable grid, hands out a writable reference.
    int& operator[](std::size_t i)             { return data_[i]; }
    // Const: called on a const grid, hands out a read-only reference.
    const int& operator[](std::size_t i) const { return data_[i]; }

    std::size_t size() const { return data_.size(); }

private:
    std::vector<int> data_;
};

int main() {
    IntGrid g(3, 0);
    g[0] = 10;              // non-const operator[] -> writable reference
    g[1] = 20;
    std::cout << "g[0]=" << g[0] << " g[1]=" << g[1] << " g[2]=" << g[2] << "\n";

    const IntGrid frozen(2, 7);
    // frozen[0] = 1;       // error: const operator[] returns const int&
    std::cout << "frozen[0]=" << frozen[0] << " frozen[1]=" << frozen[1] << "\n";
    return 0;
}
