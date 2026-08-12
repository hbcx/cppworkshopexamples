#include <iostream>
#include <functional>
#include <vector>

// Pass a lambda with ZERO overhead by making the function a template on the
// callable type: the lambda's real type flows through and the call inlines.
template <class F>
int applyTwice(F f, int x) { return f(f(x)); }

int main() {
    // Every lambda has its own unique, unnamable type. auto holds one exactly.
    auto plus1 = [](int x) { return x + 1; };
    std::cout << "auto + template: " << applyTwice(plus1, 10) << '\n';   // 12

    // std::function ERASES the type, so different lambdas -- even capturing ones --
    // fit one variable or one container, at the cost of an allocation and an
    // indirect call.
    std::vector<std::function<int(int)>> pipeline;
    pipeline.push_back([](int x) { return x + 1; });
    pipeline.push_back([](int x) { return x * x; });
    int factor = 3;
    pipeline.push_back([factor](int x) { return x * factor; });   // captures, still fits

    int val = 5;
    for (auto& step : pipeline) val = step(val);
    std::cout << "chained through std::function: " << val << '\n';   // ((5+1)^2)*3 = 108
    return 0;
}
