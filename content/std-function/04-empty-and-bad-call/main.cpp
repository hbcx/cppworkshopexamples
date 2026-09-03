#include <functional>
#include <iostream>

int main() {
    std::cout << std::boolalpha;

    // A default-constructed std::function is EMPTY -- it holds no callable.
    std::function<int(int)> f;
    std::cout << "empty f, bool(f) = " << static_cast<bool>(f) << "\n";   // false

    f = [](int x) { return x * 2; };
    std::cout << "assigned, bool(f) = " << static_cast<bool>(f) << "\n";  // true

    // Clearing it with nullptr makes it empty again.
    f = nullptr;
    std::cout << "cleared, bool(f) = " << static_cast<bool>(f) << "\n";   // false

    // Calling an empty std::function throws std::bad_function_call -- it does
    // not silently do nothing. Guard with the bool test before calling.
    try {
        f(10);
    } catch (const std::bad_function_call& e) {
        std::cout << "calling empty threw: " << e.what() << "\n";
    }

    // The safe pattern for an optional callback:
    if (f) f(10);
    else   std::cout << "no callback set, skipped\n";
    return 0;
}
