#include <functional>
#include <iostream>
#include <vector>

// A template parameter can have a DEFAULT, used when the caller omits it. Here Cmp
// defaults to std::less<T>, and the default may depend on an earlier parameter (T).
template <class T, class Cmp = std::less<T>>
T pickBest(const std::vector<T>& xs, Cmp cmp = Cmp{}) {
    T best = xs.front();
    for (const T& x : xs) {
        if (cmp(best, x)) best = x;   // less -> keep the larger; greater -> keep the smaller
    }
    return best;
}

int main() {
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2};
    std::cout << "max (default less): " << pickBest(v) << "\n";                       // 9
    std::cout << "min (greater):      " << pickBest(v, std::greater<int>{}) << "\n";  // 1
    return 0;
}
