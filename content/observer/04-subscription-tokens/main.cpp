#include <functional>
#include <iostream>
#include <map>
#include <utility>

// A signal that hands back a SUBSCRIPTION TOKEN when you subscribe. When the token
// is destroyed, it unsubscribes automatically (RAII), so an observer can never
// outlive its subscription -- the standard fix for the dangling-observer problem,
// and the idea behind boost::signals2's scoped connection.
class Signal {
public:
    // The token: a move-only handle that removes its handler on destruction.
    class Subscription {
    public:
        Subscription() = default;
        Subscription(Signal* sig, int id) : sig_(sig), id_(id) {}
        Subscription(Subscription&& other) noexcept : sig_(other.sig_), id_(other.id_) {
            other.sig_ = nullptr;
        }
        Subscription& operator=(Subscription&& other) noexcept {
            reset();
            sig_ = other.sig_;
            id_ = other.id_;
            other.sig_ = nullptr;
            return *this;
        }
        ~Subscription() { reset(); }
    private:
        void reset() {
            if (sig_) { sig_->handlers_.erase(id_); sig_ = nullptr; }   // auto-unsubscribe
        }
        Signal* sig_ = nullptr;
        int id_ = 0;
    };

    Subscription subscribe(std::function<void()> handler) {
        const int id = nextId_++;
        handlers_.emplace(id, std::move(handler));
        return Subscription(this, id);
    }
    void emit() {
        for (auto& kv : handlers_) kv.second();
    }
    std::size_t subscriberCount() const { return handlers_.size(); }
private:
    std::map<int, std::function<void()>> handlers_;
    int nextId_ = 0;
};

int main() {
    Signal signal;
    auto keep = signal.subscribe([] { std::cout << "always here\n"; });

    {
        auto temp = signal.subscribe([] { std::cout << "temporary\n"; });
        std::cout << "subscribers: " << signal.subscriberCount() << "\n";   // 2
        signal.emit();                                                      // both fire
    }   // temp's token is destroyed here -> its handler auto-unsubscribes

    std::cout << "subscribers: " << signal.subscriberCount() << "\n";       // 1
    signal.emit();                                                          // only the kept one
    return 0;
}
