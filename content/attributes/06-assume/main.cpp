#include <iostream>

// [[assume(cond)]] (C++23) hands the optimizer a fact it cannot prove but you can
// guarantee. It generates no code and performs no check -- it is a promise the
// compiler may use to drop checks and take shortcuts that are valid only when cond
// holds. The results are identical with or without it; only the code changes.
int roundDownToMultipleOf16(int n) {
    [[assume(n >= 0)]];        // every caller below passes a non-negative n
    return (n / 16) * 16;
}

// Promising that len is a multiple of 4 lets the optimizer treat the trip count as
// divisible by 4 and unroll or vectorize without a scalar tail.
long sumAll(const int* data, int len) {
    [[assume(len % 4 == 0)]];
    long total = 0;
    for (int i = 0; i < len; ++i)
        total += data[i];
    return total;
}

int main() {
    std::cout << "roundDownToMultipleOf16(70) = " << roundDownToMultipleOf16(70) << "\n";

    int data[] = {1, 2, 3, 4, 5, 6, 7, 8};
    std::cout << "sumAll(data, 8) = " << sumAll(data, 8) << "\n";
    return 0;
}
