#include <iostream>
#include <ranges>

int main() {
    // views::iota(2) is INFINITE, and filtering it leaves it infinite.
    //
    // BAD (described, not run, because it never terminates):
    //   for (int x : std::views::iota(2) | std::views::filter(is_prime))
    //       std::cout << x;   // asks for primes forever -- the loop never ends
    //
    // GOOD: bound the infinite range with take BEFORE consuming it.
    auto firstFivePrimes = std::views::iota(2)
        | std::views::filter([](int n){
              for (int d = 2; d * d <= n; ++d) {
                  if (n % d == 0) return false;
              }
              return true;
          })
        | std::views::take(5);

    std::cout << "first 5 primes:";
    for (int x : firstFivePrimes) std::cout << " " << x;   // 2 3 5 7 11
    std::cout << "\n";
    return 0;
}
