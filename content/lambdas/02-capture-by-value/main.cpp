#include <iostream>

int main() {
    int factor = 10;

    // [factor] copies factor INTO the closure at the moment the lambda is created.
    // The closure holds its own copy from then on.
    auto scale = [factor](int x) { return x * factor; };
    std::cout << "scale(3) with factor 10: " << scale(3) << '\n';   // 30

    // Changing factor afterwards does NOT affect the closure -- it kept the value
    // it saw at creation time.
    factor = 100;
    std::cout << "after factor = 100, scale(3): " << scale(3) << '\n';   // still 30
    return 0;
}
