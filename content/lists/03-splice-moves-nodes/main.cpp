#include <iostream>
#include <list>
#include <string>
#include <utility>

// A type that cannot be copied AND cannot be moved. Nothing can put this into a
// vector, or transfer it between containers -- except splice, which never
// touches the object at all.
struct Pinned {
    std::string name;
    explicit Pinned(std::string n) : name(std::move(n)) {}
    Pinned(const Pinned&) = delete;
    Pinned& operator=(const Pinned&) = delete;
    Pinned(Pinned&&) = delete;
    Pinned& operator=(Pinned&&) = delete;
};

static void print(const std::list<Pinned>& l, const char* label) {
    std::cout << label;
    for (const Pinned& p : l) {
        std::cout << ' ' << p.name;
    }
    std::cout << '\n';
}

int main() {
    std::list<Pinned> active;
    active.emplace_back("alpha");    // emplace: the element is built in the node
    active.emplace_back("beta");
    active.emplace_back("gamma");

    std::list<Pinned> retired;

    // Remember where 'beta' lives, and at what address.
    auto beta = ++active.begin();
    const Pinned* address = &*beta;

    // Move that ONE element to the other list. Four pointers are rewired; the
    // Pinned object is not copied, not moved, not destroyed -- which is why this
    // compiles at all for a type with every such operation deleted.
    retired.splice(retired.begin(), active, beta);

    print(active, "active: ");
    print(retired, "retired:");

    // The iterator survived the transfer and now refers into the other list.
    std::cout << "old iterator still reads: " << beta->name << '\n';
    std::cout << "same object, never moved: " << (&*beta == address ? "yes" : "no") << '\n';

    // Whole-list splice: 'retired' is emptied into 'active' in O(1), regardless
    // of how many elements it holds.
    active.splice(active.end(), retired);
    print(active, "after splicing everything back: ");
    std::cout << "retired is now empty: " << (retired.empty() ? "yes" : "no") << '\n';

    // Within one list, splice is a reorder: move the first element to the end.
    active.splice(active.end(), active, active.begin());
    print(active, "after moving the head to the tail: ");
    return 0;
}
