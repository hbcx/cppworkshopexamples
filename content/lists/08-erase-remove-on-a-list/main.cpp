#include <algorithm>
#include <iostream>
#include <list>

namespace {
int move_assignments = 0;
}

// An element whose move is not free -- a string, a buffer, anything with a
// resource. The counter reports how often it gets dragged around.
struct Item {
    int id;

    Item(int i) : id(i) {}                                   // NOLINT: implicit on purpose
    Item(const Item&) = default;
    Item& operator=(const Item&) = default;
    Item(Item&&) = default;
    Item& operator=(Item&& other) noexcept {
        id = other.id;
        ++move_assignments;
        return *this;
    }
};

static bool operator==(const Item& a, const Item& b) { return a.id == b.id; }

static void print(const std::list<Item>& l, const char* label) {
    std::cout << label;
    for (const Item& x : l) {
        std::cout << ' ' << x.id;
    }
    std::cout << '\n';
}

int main() {
    // Anti-pattern: the idiom a vector genuinely needs, applied to a list.
    // std::remove has iterators and nothing else, so it cannot unlink a node --
    // it move-assigns every survivor towards the front and returns a new end.
    std::list<Item> viaAlgorithm{1, 9, 2, 9, 3, 4};
    move_assignments = 0;
    viaAlgorithm.erase(std::remove(viaAlgorithm.begin(), viaAlgorithm.end(), Item(9)),
                       viaAlgorithm.end());
    print(viaAlgorithm, "erase-remove:   ");
    std::cout << "  move-assignments: " << move_assignments
              << "  <- values dragged from node to node\n";

    // The member knows about the nodes: it unlinks the matching ones and
    // destroys them. Nothing else in the list is touched.
    std::list<Item> viaMember{1, 9, 2, 9, 3, 4};
    move_assignments = 0;
    viaMember.remove(Item(9));
    print(viaMember, "member remove:  ");
    std::cout << "  move-assignments: " << move_assignments
              << "  <- nodes unlinked, values never touched\n";

    return 0;
}
