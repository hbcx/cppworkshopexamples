// The table-driven alternative: transitions as data, not classes.
//
// When a state machine is mostly about WHICH state comes next -- and the states
// do little else -- a transition table can be clearer than a class per state:
// every edge is one row, and the whole machine is visible in one place. This
// example models the same turnstile as a map from (state, event) to next state.
// The trade-off against state objects is in the notes: the table wins for pure
// transition logic; state objects win when each state has real behaviour.

#include <iostream>
#include <map>
#include <utility>

enum class St { Locked, Unlocked };
enum class Ev { Coin, Push };

const char* nameOf(St s) { return s == St::Locked ? "Locked" : "Unlocked"; }

// The entire machine, as data. A missing (state, event) pair means "no change".
const std::map<std::pair<St, Ev>, St>& transitions() {
    static const std::map<std::pair<St, Ev>, St> table = {
        { { St::Locked,   Ev::Coin }, St::Unlocked },
        { { St::Unlocked, Ev::Push }, St::Locked   },
    };
    return table;
}

St step(St current, Ev event) {
    auto it = transitions().find({ current, event });
    return it == transitions().end() ? current : it->second; // no edge: stay
}

int main() {
    St s = St::Locked;
    const Ev sequence[] = { Ev::Push, Ev::Coin, Ev::Push, Ev::Coin, Ev::Coin };
    const char* evName[] = { "coin", "push" };

    std::cout << "start: " << nameOf(s) << "\n";
    for (Ev e : sequence) {
        St next = step(s, e);
        std::cout << evName[static_cast<int>(e)] << "  " << nameOf(s)
                  << (next == s ? " (stay)" : " -> ") << (next == s ? "" : nameOf(next))
                  << "\n";
        s = next;
    }
    std::cout << "end:   " << nameOf(s) << "\n";
    return 0;
}
