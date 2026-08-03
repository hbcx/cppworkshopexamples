#include <algorithm>
#include <iostream>
#include <vector>

struct Observer {
    virtual void update(int v) = 0;
    virtual ~Observer() = default;
};

class Subject {
public:
    void subscribe(Observer* o) { obs_.push_back(o); }
    void unsubscribe(Observer* o) {
        obs_.erase(std::remove(obs_.begin(), obs_.end(), o), obs_.end());
    }
    void notify(int v) {
        for (auto* o : obs_) o->update(v);
    }
private:
    std::vector<Observer*> obs_;
};

// ANTI-PATTERN: an observer that does NOT unsubscribe itself. If it is destroyed
// while still registered, the subject keeps a dangling pointer to it, and the next
// notify() calls a dead object -- undefined behaviour. Described below, not run.
struct LeakyObserver : Observer {
    void update(int v) override { std::cout << "leaky sees " << v << "\n"; }
    // No cleanup: destroying this while subscribed leaves a dangling pointer in
    // the subject's list.
};

// FIX: the observer unsubscribes itself in its destructor (RAII), so it is removed
// from the subject before it can ever be called dead. It holds the subject to do
// so, and registers itself on construction for symmetry.
class SafeObserver : public Observer {
public:
    explicit SafeObserver(Subject& s) : subject_(s) { subject_.subscribe(this); }
    ~SafeObserver() override { subject_.unsubscribe(this); }   // auto-remove on destruction
    void update(int v) override { std::cout << "safe sees " << v << "\n"; }
private:
    Subject& subject_;
};

int main() {
    Subject subject;

    // The undefined-behaviour path, described rather than executed:
    //   { LeakyObserver o; subject.subscribe(&o); }  // o dies, still subscribed
    //   subject.notify(1);                            // calls dangling &o -- UB
    std::cout << "a leaky observer would leave a dangling pointer (not run)\n";

    // The fix: a self-unsubscribing observer is safe across its own destruction.
    {
        SafeObserver a(subject);
        subject.notify(1);          // safe sees 1
    }   // a unsubscribes here
    subject.notify(2);              // 0 observers left -- nothing dangling, nothing printed
    std::cout << "after the safe observer left, notify(2) reached 0 observers\n";
    return 0;
}
