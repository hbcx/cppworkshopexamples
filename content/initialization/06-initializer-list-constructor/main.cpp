#include <iostream>
#include <initializer_list>
#include <vector>
#include <string>

// A constructor taking std::initializer_list<T> lets your type be built from a
// braced list of values, like the standard containers. When you use {}, the
// compiler PREFERS this constructor if the braced values fit -- even over
// another constructor that would also match.

class IntSet {
public:
    IntSet(std::initializer_list<int> values) : data_(values) {
        builtFrom_ = "initializer_list";
    }
    // A different constructor: n copies of a value.
    IntSet(std::size_t n, int value) : data_(n, value) {
        builtFrom_ = "(count, value)";
    }
    std::size_t size() const { return data_.size(); }
    const std::string& builtFrom() const { return builtFrom_; }
private:
    std::vector<int> data_;
    std::string builtFrom_;
};

int main() {
    // Braces with a list of ints -> the initializer_list constructor.
    IntSet a{1, 2, 3, 4};
    std::cout << "a: size=" << a.size() << " via " << a.builtFrom() << "\n";

    // Even {3, 0} prefers the initializer_list ctor (two elements 3 and 0),
    // NOT the (count, value) constructor.
    IntSet b{3, 0};
    std::cout << "b: size=" << b.size() << " via " << b.builtFrom() << "\n";

    // Parentheses do NOT consider initializer_list -- this is the (count, value)
    // constructor: three copies of 0.
    IntSet c(3, 0);
    std::cout << "c: size=" << c.size() << " via " << c.builtFrom() << "\n";

    // The same rule drives the standard containers (see the anti-pattern):
    std::vector<int> v{5, 5};   // two elements, both 5
    std::cout << "vector{5,5} size=" << v.size() << "\n";
    return 0;
}
