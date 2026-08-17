#include <atomic>
#include <iostream>
#include <memory>
#include <thread>

// Anti-pattern: reading the atomic<shared_ptr> repeatedly within one logical
// operation instead of taking a single snapshot.
//
// Each load() may observe a DIFFERENT object than the last, because a writer can store
// a new pointer between your loads. Code that loads the atomic several times in one
// operation can mix data from two different published versions:
//
//   if (cfg.load()->enabled)          // load #1: version A, enabled == true
//       use(cfg.load()->value);       // load #2: version B -- a different object!
//
// The fix is to load ONCE into a local shared_ptr and use that stable owning copy for
// the whole operation. It keeps the object alive and unchanging no matter what the
// writer does next.
struct Config { bool enabled; int value; };   // invariant: enabled => value == 20

int main() {
    std::atomic<std::shared_ptr<const Config>> cfg{
        std::make_shared<const Config>(Config{true, 20})};

    std::atomic<bool> stop{false};
    std::atomic<long> mismatches{0};

    std::jthread reader([&] {
        long bad = 0;
        while (!stop.load(std::memory_order_relaxed)) {
            std::shared_ptr<const Config> snap = cfg.load();   // ONE snapshot
            // Both fields come from the same object, so the invariant always holds.
            if (snap->enabled && snap->value != 20) ++bad;
        }
        mismatches.fetch_add(bad);
    });

    for (int k = 0; k < 500; ++k) {
        bool en = (k % 2 == 0);
        cfg.store(std::make_shared<const Config>(Config{en, en ? 20 : 99}));
    }
    stop.store(true, std::memory_order_relaxed);
    reader.join();

    std::cout << "fields always came from one snapshot, mismatches = "
              << mismatches.load() << '\n';   // 0
    return 0;
}
