#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

struct Order {
    std::string id;
    std::string status;   // "paid", "pending", "cancelled"
    int items;
};

int main() {
    std::vector<Order> orders{
        {"A-1", "paid", 3},
        {"A-2", "pending", 1},
        {"A-3", "paid", 5},
        {"A-4", "cancelled", 2},
    };

    // ranges::count (C++20) with a projection counts by a member value directly --
    // no lambda to reach into the struct.
    auto paid = std::ranges::count(orders, "paid", &Order::status);
    std::cout << "paid orders: " << paid << '\n';   // 2

    // ranges::count_if with a projection: the predicate sees the projected field.
    auto bulky = std::ranges::count_if(orders, [](int n){ return n >= 3; }, &Order::items);
    std::cout << "orders with 3+ items: " << bulky << '\n';   // 2 (3, 5)
    return 0;
}
