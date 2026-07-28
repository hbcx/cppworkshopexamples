#include <iostream>
#include <memory>
#include <string>
#include <utility>

static int g_bad = 0;    // live Bad objects
static int g_good = 0;   // live Good objects

// Two objects that point at each other with OWNING shared_ptrs form a cycle:
// each keeps the other's count at 1, so neither ever reaches 0 -> both leak.
struct Bad {
    std::string name;
    std::shared_ptr<Bad> other;
    explicit Bad(std::string n) : name(std::move(n)) { ++g_bad; }
    ~Bad() { --g_bad; std::cout << "Bad " << name << " destroyed\n"; }
};

// The fix: make the back-edge a weak_ptr. It observes without owning, so the
// count can reach 0 and the objects are destroyed.
struct Good {
    std::string name;
    std::shared_ptr<Good> owns;      // forward edge: owning
    std::weak_ptr<Good> observes;    // back edge: non-owning
    explicit Good(std::string n) : name(std::move(n)) { ++g_good; }
    ~Good() { --g_good; std::cout << "Good " << name << " destroyed\n"; }
};

int main() {
    std::cout << "-- the cycle (leaks) --\n";
    {
        auto a = std::make_shared<Bad>("A");
        auto b = std::make_shared<Bad>("B");
        a->other = b;     // A owns B
        b->other = a;     // B owns A  -> cycle
        std::cout << "A use_count = " << a.use_count() << '\n';   // 2
    }   // the a and b handles are gone, but the objects still own each other
    std::cout << "live Bad after scope = " << g_bad << " (leaked: no destructor ran)\n";  // 2

    std::cout << "-- the fix (weak_ptr back-edge) --\n";
    {
        auto a = std::make_shared<Good>("A");
        auto b = std::make_shared<Good>("B");
        a->owns = b;         // A owns B
        b->observes = a;     // B only observes A -> no cycle
        std::cout << "A use_count = " << a.use_count() << '\n';   // 1: only `a`
        if (std::shared_ptr<Good> locked = b->observes.lock()) {
            std::cout << "B can still reach " << locked->name << " while it lives\n";
        }
    }   // both are destroyed here
    std::cout << "live Good after scope = " << g_good << " (fixed: all freed)\n";  // 0
    return 0;
}
