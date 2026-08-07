#include <functional>
#include <iostream>
#include <vector>

// A plain function, a stateful functor, and a lambda: three unrelated
// types, all callable as int(int). std::function erases the type and
// keeps only the signature, so they share one element type.
int timesTwo(int n) { return n * 2; }

struct AddK {
    int k;
    int operator()(int n) const { return n + k; }
};

int main() {
    std::vector<std::function<int(int)>> ops;
    ops.push_back(timesTwo);                            // function pointer
    ops.push_back(AddK{10});                            // functor with state
    int factor = 3;
    ops.push_back([factor](int n) { return n * factor; });  // lambda with capture

    for (const std::function<int(int)>& op : ops)
        std::cout << op(5) << "\n";                     // 10, 15, 15

    // A template parameter F would bind ONE concrete callable type per
    // instantiation -- a function pointer and a lambda have different
    // types, so they could never share a std::vector<F>. std::function
    // gives them one type, at the cost of a virtual call per invocation.
    return 0;
}
