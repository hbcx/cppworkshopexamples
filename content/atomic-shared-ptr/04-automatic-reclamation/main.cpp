#include <atomic>
#include <iostream>
#include <memory>

// A quiet but essential guarantee: when you store() a new pointer, the object the
// atomic used to hold is destroyed as soon as the LAST shared_ptr snapshot of it goes
// away -- the reference count does the reclamation for you. That is what makes the
// hot-swap pattern safe: a reader can keep using an old version while a writer
// publishes a new one, and the old version lives exactly until its last reader lets
// go. (The atomic also offers wait()/notify_one()/notify_all() to block on a change,
// like any atomic.)
struct Tracked {
    static inline std::atomic<int> alive{0};
    Tracked()  { alive.fetch_add(1); }
    ~Tracked() { alive.fetch_sub(1); }
};

int main() {
    std::atomic<std::shared_ptr<Tracked>> slot{std::make_shared<Tracked>()};
    std::cout << "after first publish, alive = " << Tracked::alive.load() << '\n';   // 1

    // A reader takes a snapshot and holds on to it.
    std::shared_ptr<Tracked> reader_snapshot = slot.load();

    // The writer publishes a new version. The old object is NOT freed yet, because the
    // reader still holds it -- so two are alive at once.
    slot.store(std::make_shared<Tracked>());
    std::cout << "reader still holds the old one, alive = " << Tracked::alive.load() << '\n';  // 2

    // The reader releases its snapshot: the old version is reclaimed now, leaving one.
    reader_snapshot.reset();
    std::cout << "after reader releases, alive = " << Tracked::alive.load() << '\n';   // 1

    return 0;
}
