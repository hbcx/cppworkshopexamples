#include <array>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {
int moveCtors = 0;
int moveAssigns = 0;

void reset() {
    moveCtors = 0;
    moveAssigns = 0;
}
void report(const char* label) {
    std::cout << label << ": move-constructions=" << moveCtors
              << " move-assignments=" << moveAssigns << '\n';
    reset();
}
}  // namespace

struct Payload {
    std::string data;

    Payload() = default;
    explicit Payload(std::string d) : data(std::move(d)) {}
    Payload(const Payload&) = default;
    Payload& operator=(const Payload&) = default;
    Payload(Payload&& other) noexcept : data(std::move(other.data)) { ++moveCtors; }
    Payload& operator=(Payload&& other) noexcept {
        data = std::move(other.data);
        ++moveAssigns;
        return *this;
    }
};

int main() {
    // Anti-pattern: reaching for std::move on an array and expecting what a
    // vector gives you. The elements live INSIDE the array object, so there is
    // no buffer to hand over -- every element is moved, one at a time.
    std::array<Payload, 4> source{Payload("a"), Payload("b"), Payload("c"), Payload("d")};
    reset();
    std::array<Payload, 4> moved = std::move(source);
    report("array<Payload,4> move   ");   // 4 -- one per element, O(N)

    // The habit's origin: a vector move is three pointers changing hands. The
    // element count is irrelevant, and no element is touched at all.
    std::vector<Payload> vec;
    vec.reserve(4);
    for (const char* s : {"a", "b", "c", "d"}) {
        vec.emplace_back(std::string(s));
    }
    reset();
    std::vector<Payload> vecMoved = std::move(vec);
    report("vector<Payload> move    ");   // 0 -- O(1), whatever the size

    // swap follows the same rule, and this is why array::swap can throw while
    // vector::swap is noexcept.
    std::array<Payload, 4> other{Payload("w"), Payload("x"), Payload("y"), Payload("z")};
    reset();
    moved.swap(other);
    report("array<Payload,4> swap   ");   // 4 constructions + 8 assignments

    std::vector<Payload> a2(2);
    std::vector<Payload> b2(2);
    reset();
    a2.swap(b2);
    report("vector<Payload> swap    ");   // 0

    // And with a trivial element type there is no move constructor to call, so
    // "moving" is a plain copy of N objects, spelled differently.
    std::array<int, 4> ints{1, 2, 3, 4};
    std::array<int, 4> intsMoved = std::move(ints);
    std::cout << "array<int,4> after being moved from: " << ints[0] << ' ' << ints[1]
              << ' ' << ints[2] << ' ' << ints[3] << "   <- still there: it was copied\n";
    std::cout << "the copy holds: " << intsMoved[0] << ' ' << intsMoved[3] << '\n';
    return 0;
}
