#include <iostream>
#include <iterator>
#include <list>
#include <string>

static void print(const std::list<std::string>& l, const char* label) {
    std::cout << label;
    for (const std::string& s : l) {   // bidirectional iterators: range-for is fine
        std::cout << ' ' << s;
    }
    std::cout << '\n';
}

int main() {
    std::list<std::string> stops{"berlin", "prague"};

    // Both ends are O(1) -- a vector has no push_front at all, because it would
    // have to shift every element.
    stops.push_back("vienna");
    stops.push_front("hamburg");
    print(stops, "stops: ");

    std::cout << "front = " << stops.front() << ", back = " << stops.back()
              << ", size = " << stops.size() << '\n';   // size() is O(1) since C++11

    // What a list does NOT have, because nothing is contiguous:
    //   stops[2];        // no operator[]
    //   stops.at(2);     // no at()
    //   stops.data();    // no data(): there is no array to point at
    // Reaching position k means following k pointers. std::next does the walk:
    auto third = std::next(stops.begin(), 2);
    std::cout << "third stop = " << *third << "  (found by walking, not indexing)\n";

    // The iterators are bidirectional: ++ and -- work, but + and - do not.
    auto it = stops.end();
    --it;                                   // step back from the past-the-end slot
    std::cout << "last stop  = " << *it << '\n';
    std::advance(it, -2);                   // std::advance walks; it cannot jump
    std::cout << "two back   = " << *it << '\n';

    // Insert happens BEFORE the given position, and the list needs no shifting:
    // it just relinks two pointers.
    stops.insert(third, "dresden");
    print(stops, "after inserting dresden before the third stop: ");
    return 0;
}
