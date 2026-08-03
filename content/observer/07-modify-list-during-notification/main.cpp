#include <functional>
#include <iostream>
#include <utility>
#include <vector>

// ANTI-PATTERN: an observer modifies the subject's observer list DURING
// notification. Iterating by index while a handler erases an entry shifts the
// remaining elements, so the observer AFTER the removed one is skipped. (With a
// range-for or stored iterators it is worse: erase invalidates them, which is
// undefined behaviour.) Either way, mutating the list mid-broadcast is a bug.
class SubjectBad {
public:
    void add(std::function<void(SubjectBad&)> h) { handlers_.push_back(std::move(h)); }
    void notify() {
        for (std::size_t i = 0; i < handlers_.size(); ++i) {
            handlers_[i](*this);           // a handler may call removeFirst() and shift the rest
        }
    }
    void removeFirst() {
        if (!handlers_.empty()) handlers_.erase(handlers_.begin());
    }
private:
    std::vector<std::function<void(SubjectBad&)>> handlers_;
};

// FIX: iterate over a COPY (a snapshot) of the handlers, so a removal during
// notification takes effect next round and no observer is skipped or double-called.
// Deferring the mutation until after the broadcast is the equivalent alternative.
class SubjectGood {
public:
    void add(std::function<void(SubjectGood&)> h) { handlers_.push_back(std::move(h)); }
    void notify() {
        auto snapshot = handlers_;         // iteration is stable even if handlers_ changes
        for (auto& h : snapshot) h(*this);
    }
    void removeFirst() {
        if (!handlers_.empty()) handlers_.erase(handlers_.begin());
    }
private:
    std::vector<std::function<void(SubjectGood&)>> handlers_;
};

int main() {
    std::cout << "== mutating the list during notify ==\n";
    SubjectBad bad;
    bad.add([](SubjectBad& s) { std::cout << "A (removes first)\n"; s.removeFirst(); });
    bad.add([](SubjectBad&) { std::cout << "B\n"; });
    bad.notify();      // A removes index 0; B shifts to index 0 and is SKIPPED

    std::cout << "== iterating a snapshot ==\n";
    SubjectGood good;
    good.add([](SubjectGood& s) { std::cout << "A (removes first)\n"; s.removeFirst(); });
    good.add([](SubjectGood&) { std::cout << "B\n"; });
    good.notify();     // both run; the removal applies on the next broadcast
    return 0;
}
