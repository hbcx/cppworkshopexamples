#include <iostream>

import stats;

int main() {
    // rms(1, 7) = sqrt((1 + 49) / 2) = sqrt(25) = 5
    std::cout << "rootMeanSquare(1, 7) = " << rootMeanSquare(1.0, 7.0) << "\n";

    // <cmath> was included only inside stats's global module fragment, so std::sqrt
    // is NOT visible here just because we imported stats.
    return 0;
}
