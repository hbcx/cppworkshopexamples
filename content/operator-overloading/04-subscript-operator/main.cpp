#include <iostream>
#include <vector>
#include <cstddef>

// A fixed-size row of numbers. We want row[i] to read AND write like an array,
// and to still be indexable when the row is const (read-only).

class Row {
public:
    explicit Row(std::size_t n) : data_(n, 0) {}

    // Non-const: called on a mutable Row, hands back a writable reference so
    // row[i] = v works.
    int& operator[](std::size_t i) { return data_[i]; }

    // Const: called on a const Row, hands back a read-only reference.
    const int& operator[](std::size_t i) const { return data_[i]; }

    std::size_t size() const { return data_.size(); }

private:
    std::vector<int> data_;
};

// Takes a const Row, so only the const operator[] is available here.
int sum(const Row& r) {
    int total = 0;
    for (std::size_t i = 0; i < r.size(); ++i)
        total += r[i];               // const overload
    return total;
}

int main() {
    Row r{3};
    r[0] = 10;                       // non-const overload: writable
    r[1] = 20;
    r[2] = 30;

    std::cout << "r[1] = " << r[1] << "\n";    // 20
    std::cout << "sum  = " << sum(r) << "\n";  // 60  (via the const overload)
    return 0;
}
