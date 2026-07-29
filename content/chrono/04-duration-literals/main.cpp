#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono;
    using namespace std::chrono_literals;   // switch on the suffixes

    auto timeout = 100ms;                    // std::chrono::milliseconds(100)
    auto window  = 2s;                       // std::chrono::seconds(2)
    std::cout << "timeout = " << timeout.count() << " ms\n";
    std::cout << "window  = " << window.count() << " s\n";

    // They mix and convert exactly like the constructor forms.
    auto combined = 1s + 500ms;
    std::cout << "1s + 500ms = " << combined.count() << " ms\n";   // 1500

    // A floating-point literal yields a floating-point duration -- a fraction
    // with no cast needed.
    auto oneAndHalf = 1.5s;
    std::cout << "1.5s in seconds = " << oneAndHalf.count() << '\n';   // 1.5
    std::cout << "5min = " << (5min).count() << " min\n";
    return 0;
}
