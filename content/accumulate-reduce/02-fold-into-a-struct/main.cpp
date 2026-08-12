#include <numeric>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

struct Order { std::string id; double total; };

struct Stats { double sum = 0.0; int count = 0; double max = 0.0; };

int main() {
    std::vector<Order> orders{
        {"A", 19.99}, {"B", 5.50}, {"C", 120.00},
    };

    // accumulate is a general fold, not just for numbers -- the accumulator can be
    // ANY type. Fold the orders into a summary struct in one pass: sum, count, and
    // the largest, all together.
    Stats s = std::accumulate(orders.begin(), orders.end(), Stats{},
        [](Stats acc, const Order& o){
            acc.sum += o.total;
            acc.count += 1;
            acc.max = std::max(acc.max, o.total);
            return acc;
        });

    std::cout << "count: " << s.count << ", sum: " << s.sum
              << ", max: " << s.max << '\n';
    std::cout << "average: " << (s.sum / s.count) << '\n';
    return 0;
}
