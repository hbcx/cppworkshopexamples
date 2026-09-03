#include <functional>
#include <iostream>
#include <vector>

int freeFn(int x) { return x + 1; }

struct Functor {                          // a class with operator()
    int factor;
    int operator()(int x) const { return x * factor; }
};

int main() {
    int base = 10;

    // Four different callable KINDS, all stored in the same std::function type.
    // That is type erasure: the concrete type is hidden behind the signature.
    std::vector<std::function<int(int)>> fns;
    fns.push_back(freeFn);                            // free function
    fns.push_back([](int x) { return x * x; });      // captureless lambda
    fns.push_back([base](int x) { return x + base; });// capturing lambda
    fns.push_back(Functor{3});                       // functor object

    const char* names[] = { "freeFn", "square", "add base", "Functor x3" };
    for (std::size_t i = 0; i < fns.size(); ++i)
        std::cout << names[i] << "(5) = " << fns[i](5) << "\n";
    return 0;
}
