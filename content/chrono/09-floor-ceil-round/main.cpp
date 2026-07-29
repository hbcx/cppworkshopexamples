#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono;
    using namespace std::chrono_literals;

    auto d = 2600ms;
    std::cout << "2600ms -> seconds\n";
    std::cout << "  duration_cast (toward zero): " << duration_cast<seconds>(d).count() << '\n';   // 2
    std::cout << "  floor: " << floor<seconds>(d).count() << '\n';   // 2
    std::cout << "  ceil:  " << ceil<seconds>(d).count() << '\n';    // 3
    std::cout << "  round: " << round<seconds>(d).count() << '\n';   // 3

    // The negative case, where truncation differs from floor.
    auto neg = -1500ms;
    std::cout << "-1500ms -> seconds\n";
    std::cout << "  duration_cast (toward zero): " << duration_cast<seconds>(neg).count() << '\n';  // -1
    std::cout << "  floor (toward -inf):         " << floor<seconds>(neg).count() << '\n';          // -2

    // round ties to even: 2500ms is exactly 2.5s -> the even neighbour, 2s.
    std::cout << "2500ms round to seconds (ties to even): "
              << round<seconds>(2500ms).count() << '\n';   // 2
    return 0;
}
