#include <iostream>
#include <memory>
#include <utility>
#include <vector>

// Track live observers so a leak is observable.
struct Counted {
    Counted() { ++live(); }
    virtual ~Counted() { --live(); }
    static int& live() { static int n = 0; return n; }
};

struct Observer : Counted {
    virtual void update() = 0;
};

struct ConcreteObserver : Observer {
    void update() override {}
};

// ANTI-PATTERN: the subject keeps observers alive with shared_ptr and never
// unsubscribes them. An observer that is logically finished is still owned by the
// subject, so it is never freed -- the "lapsed listener" leak. The list only ever
// grows for the life of the subject.
class StrongSubject {
public:
    void subscribe(std::shared_ptr<Observer> o) { obs_.push_back(std::move(o)); }
    void notify() {
        for (auto& o : obs_) o->update();
    }
private:
    std::vector<std::shared_ptr<Observer>> obs_;   // strong -> owns observers forever
};

// FIX: the subject holds WEAK pointers. It does not own observers, so one is freed
// the moment its real owner drops it, and notify() skips (and could prune) any
// that have expired.
class WeakSubject {
public:
    void subscribe(const std::shared_ptr<Observer>& o) { obs_.push_back(o); }
    void notify() {
        for (auto& w : obs_) {
            if (auto o = w.lock()) o->update();
        }
    }
private:
    std::vector<std::weak_ptr<Observer>> obs_;
};

int main() {
    std::cout << "live at start: " << Counted::live() << "\n";   // 0

    {
        WeakSubject weak;
        auto o = std::make_shared<ConcreteObserver>();
        weak.subscribe(o);
    }   // o's owner drops it; the weak subject held no ownership
    std::cout << "after weak-subject scope: " << Counted::live() << "\n";   // 0 -- freed

    StrongSubject strong;
    {
        auto o = std::make_shared<ConcreteObserver>();
        strong.subscribe(o);
    }   // o's owner drops it, but the subject still owns it
    std::cout << "after strong subject kept it: " << Counted::live() << "\n";   // 1 -- leaked
    return 0;
}
