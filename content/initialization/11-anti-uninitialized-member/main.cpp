#include <iostream>
#include <string>
#include <utility>

// Anti-pattern: a constructor that forgets to initialize a built-in member. The
// members with their own constructor are fine, but the raw int/bool/pointer
// members hold indeterminate values -- reading one is undefined behaviour, and
// the bug hides until that path runs.

struct AccountBad {
    std::string owner;   // has a default constructor -> "", always fine
    int         balance; // built-in -> INDETERMINATE if the ctor forgets it
    bool        active;  // same

    // Forgets balance and active: they are left with garbage. Reading them would
    // be undefined behaviour, so this example never prints them.
    explicit AccountBad(std::string o) : owner(std::move(o)) {}
};

struct AccountGood {
    std::string owner;
    int         balance{0};      // default member initializer -> always defined
    bool        active{false};

    // Even a constructor that forgets balance/active now gets defined values.
    explicit AccountGood(std::string o) : owner(std::move(o)) {}
};

int main() {
    AccountBad bad{"alice"};
    std::cout << "bad.owner   = " << bad.owner
              << "  (the string member is fine)\n";
    // std::cout << bad.balance;   // UB: indeterminate value -- not run

    AccountGood good{"bob"};
    std::cout << "good.owner  = " << good.owner << "\n";
    std::cout << "good.balance= " << good.balance << "\n";                // 0
    std::cout << "good.active = " << (good.active ? "yes" : "no") << "\n"; // no
    return 0;
}
