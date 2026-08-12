#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> orders;   // empty: no orders in the batch at all

    // all_of on an EMPTY range is true by definition -- there is no element that
    // fails the predicate. none_of is also true; only any_of is false. So this
    // reads as "all orders delivered" even though there are NO orders.
    bool allDelivered = std::all_of(orders.begin(), orders.end(),
        [](int status){ return status == 2; });   // 2 == delivered
    std::cout << std::boolalpha;
    std::cout << "all delivered (empty batch)? " << allDelivered << '\n';   // true -- surprising

    // If "there is nothing to check" should NOT count as success, guard the
    // emptiness first: require at least one element before trusting all_of.
    bool reallyAllDelivered = !orders.empty()
        && std::all_of(orders.begin(), orders.end(),
                       [](int status){ return status == 2; });
    std::cout << "with empty treated as false:  " << reallyAllDelivered << '\n';   // false
    return 0;
}
