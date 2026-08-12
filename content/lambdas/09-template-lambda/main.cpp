#include <iostream>
#include <vector>
#include <concepts>

int main() {
    // A template lambda (C++20) names the type parameter, so you can USE the type
    // -- not just the value, which is all a plain auto gives you.
    auto firstOf = []<class T>(const std::vector<T>& v) -> T {
        return v.empty() ? T{} : v.front();
    };
    std::vector<int> nums{7, 8, 9};
    std::cout << "firstOf(nums) = " << firstOf(nums) << '\n';   // 7

    // Naming T lets you make a value of it, default-construct it, or match a shape
    // like vector<T> in the parameter -- impossible with a bare auto.
    auto zeroOf = []<class T>(const std::vector<T>&) { return T{}; };
    std::cout << "zeroOf(nums) = " << zeroOf(nums) << '\n';   // 0

    // Constrain the template lambda with a concept, exactly as you would a template.
    auto doubleIt = []<std::integral T>(T x) { return x * 2; };
    std::cout << "doubleIt(21) = " << doubleIt(21) << '\n';   // 42
    return 0;
}
