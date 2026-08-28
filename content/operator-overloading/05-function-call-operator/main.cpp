#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>   // std::ref

// A predicate that remembers a threshold set at construction, and also counts how
// many times it matched. State like this is exactly what a plain function cannot
// hold and a function pointer cannot carry.

class AboveThreshold {
public:
    explicit AboveThreshold(int limit) : limit_(limit) {}

    // The call operator. It takes an int and answers a question, so it reads like
    // a function: pred(value). Mutable member -> we count the matches.
    bool operator()(int value) {
        if (value > limit_) { ++matched_; return true; }
        return false;
    }

    int matched() const { return matched_; }

private:
    int limit_;
    int matched_ = 0;
};

int main() {
    std::vector<int> v{3, 12, 7, 20, 1, 15};

    AboveThreshold over10{10};
    int n = std::count_if(v.begin(), v.end(), std::ref(over10));  // pass by ref to keep the count

    std::cout << "values over 10: " << n << "\n";                 // 3
    std::cout << "functor counted: " << over10.matched() << "\n"; // 3

    // Called directly, too -- it is just an object with operator():
    std::cout << "is 42 over 10? " << over10(42) << "\n";         // 1
    return 0;
}
