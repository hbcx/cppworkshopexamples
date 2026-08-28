#include <iostream>
#include <compare>
#include <vector>
#include <set>
#include <algorithm>

// A point ordered by x, then y. The single defaulted <=> is all std::sort and
// std::set need -- no comparator, no hand-written operator<.

struct Point {
    int x;
    int y;
    auto operator<=>(const Point&) const = default;
    bool operator==(const Point&) const = default;
};

int main() {
    std::vector<Point> pts{{3, 1}, {1, 9}, {1, 2}, {3, 0}};

    // std::sort uses <, supplied by <=>:
    std::sort(pts.begin(), pts.end());
    std::cout << "sorted:";
    for (const Point& p : pts)
        std::cout << " (" << p.x << "," << p.y << ")";
    std::cout << "\n";   // (1,2) (1,9) (3,0) (3,1)

    // std::set orders its keys the same way, again with no comparator:
    std::set<Point> uniq{{2, 2}, {2, 2}, {1, 1}};
    std::cout << "set:  ";
    for (const Point& p : uniq)
        std::cout << " (" << p.x << "," << p.y << ")";
    std::cout << "\n";   // (1,1) (2,2)   -- the duplicate collapsed
    return 0;
}
