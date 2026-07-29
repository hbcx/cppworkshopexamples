#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono;

    // Construct time_points a known distance from the epoch (no live clock, so the
    // output is exact and reproducible).
    steady_clock::time_point epoch;                 // default: the epoch itself
    steady_clock::time_point at5s = epoch + seconds(5);

    std::cout << "at5s since epoch = "
              << duration_cast<seconds>(at5s.time_since_epoch()).count() << " s\n";   // 5

    // time_point - time_point = duration (how far apart).
    auto start = epoch + seconds(2);
    auto finish = epoch + seconds(7);
    std::cout << "finish - start = "
              << duration_cast<seconds>(finish - start).count() << " s\n";   // 5

    // time_point + duration = a deadline; comparisons order instants.
    auto deadline = start + seconds(3);
    std::cout << "start before deadline? " << (start < deadline ? "yes" : "no") << '\n';
    std::cout << "finish past deadline?  " << (finish > deadline ? "yes" : "no") << '\n';   // 7s > 5s
    return 0;
}
