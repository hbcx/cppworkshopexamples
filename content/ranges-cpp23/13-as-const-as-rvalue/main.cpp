#include <iostream>
#include <ranges>
#include <utility>
#include <vector>

// A type that counts moves versus copies, to prove which one as_rvalue causes.
struct Movable {
    int id;
    explicit Movable(int i) : id(i) {}
    Movable(const Movable& o) : id(o.id) { ++copies; }
    Movable(Movable&& o) noexcept : id(o.id) { ++moves; }
    static int moves, copies;
};
int Movable::moves = 0;
int Movable::copies = 0;

int main() {
    // views::as_const -- elements are seen as const, so the pipeline cannot
    // modify the underlying range through the view.
    std::vector<int> nums{1, 2, 3};
    int sum = 0;
    for (const int& x : nums | std::views::as_const) sum += x;   // x is const int&
    std::cout << "as_const sum (read-only view): " << sum << '\n';   // 6
    // Writing x inside that loop would not compile: the view yields const int&.

    // views::as_rvalue -- each element is yielded as an rvalue, so consuming the
    // view MOVES elements out of the source instead of copying them.
    std::vector<Movable> src;
    src.emplace_back(10);
    src.emplace_back(20);
    src.emplace_back(30);
    Movable::moves = Movable::copies = 0;

    std::vector<Movable> dst;
    dst.reserve(src.size());
    for (Movable&& m : src | std::views::as_rvalue)
        dst.push_back(std::move(m));

    std::cout << "as_rvalue moved " << Movable::moves
              << ", copied " << Movable::copies << '\n';   // moved 3, copied 0
    std::cout << "dst first id = " << dst.front().id << '\n';   // 10
    return 0;
}
