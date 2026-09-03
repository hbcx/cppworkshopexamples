#include <functional>
#include <iostream>
#include <typeinfo>

int freeFn(int x) { return x + 1; }

struct Doubler {
    int operator()(int x) const { return x * 2; }
};

int main() {
    std::cout << std::boolalpha;

    std::function<int(int)> f = freeFn;

    // target_type() reports the typeid of the stored callable; target<T>()
    // returns a T* if the stored callable is exactly a T, else nullptr. This is
    // the escape hatch out of type erasure -- rarely needed, but it shows the
    // wrapper really does remember what it holds.
    std::cout << "holds Doubler? " << (f.target<Doubler>() != nullptr) << "\n";      // false
    std::cout << "holds freeFn?  " << (f.target<int(*)(int)>() != nullptr) << "\n";  // true

    // Recover the concrete callable and use it directly.
    if (auto p = f.target<int(*)(int)>())
        std::cout << "recovered pointer, (*p)(41) = " << (*p)(41) << "\n";           // 42

    f = Doubler{};
    std::cout << "after reassigning, holds Doubler? "
              << (f.target<Doubler>() != nullptr) << "\n";                           // true
    return 0;
}
