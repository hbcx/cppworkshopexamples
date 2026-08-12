#include <numeric>
#include <vector>
#include <iostream>

int main() {
    std::vector<double> prices{9.99, 19.99, 4.50, 100.00};

    // BAD: the init value's TYPE decides the accumulator type. Passing int 0 makes
    // accumulate sum in int, so every price is truncated to a whole number as it is
    // added -- the total is wrong.
    int badTotal = std::accumulate(prices.begin(), prices.end(), 0);   // int accumulator
    std::cout << "sum with init 0 (int):      " << badTotal << '\n';   // 132, not 134.48

    // GOOD: match the init type to the element type -- 0.0 keeps a double accumulator.
    double goodTotal = std::accumulate(prices.begin(), prices.end(), 0.0);
    std::cout << "sum with init 0.0 (double): " << goodTotal << '\n';  // 134.48

    // The same rule bites with OVERFLOW: summing many ints into an int 0 can
    // overflow, so use a wide init like 0LL to accumulate in a 64-bit type.
    std::vector<int> big(1000, 5000000);   // 1000 * 5,000,000 = 5,000,000,000 -> past 32-bit int
    long long wideSum = std::accumulate(big.begin(), big.end(), 0LL);   // 64-bit accumulator
    std::cout << "wide sum with init 0LL:     " << wideSum << '\n';     // 5000000000
    return 0;
}
