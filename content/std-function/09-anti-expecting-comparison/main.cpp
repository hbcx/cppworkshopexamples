#include <functional>
#include <iostream>
#include <vector>
#include <cstddef>

// TRAP: trying to remove a handler by comparing std::functions for equality.
// std::function has NO operator== against another std::function -- only against
// nullptr. The line
//     handlers.erase(find(handlers.begin(), handlers.end(), someHandler));
// does not compile, because two std::functions cannot be compared. Even
// conceptually there is no meaningful equality for two arbitrary callables.
//
// FIX: hand out a token (an id) when registering, and remove by that token.
class Signal {
public:
    using Handler = std::function<void()>;

    std::size_t connect(Handler h) {
        slots_.push_back({nextId_, std::move(h)});
        return nextId_++;                 // the caller keeps this token
    }
    void disconnect(std::size_t id) {
        for (std::size_t i = 0; i < slots_.size(); ++i)
            if (slots_[i].id == id) { slots_.erase(slots_.begin() + i); return; }
    }
    void emit() const { for (const auto& s : slots_) s.fn(); }

private:
    struct Slot { std::size_t id; Handler fn; };
    std::vector<Slot> slots_;
    std::size_t nextId_ = 1;
};

int main() {
    Signal sig;
    std::size_t a = sig.connect([] { std::cout << "A "; });
    sig.connect([] { std::cout << "B "; });

    std::cout << "before: "; sig.emit(); std::cout << "\n";
    sig.disconnect(a);                    // remove by token, not by comparison
    std::cout << "after : "; sig.emit(); std::cout << "\n";
    return 0;
}
