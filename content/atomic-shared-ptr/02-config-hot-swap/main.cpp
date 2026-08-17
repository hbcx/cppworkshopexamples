#include <atomic>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

// The signature use of atomic<shared_ptr>: a read-mostly value that is occasionally
// replaced wholesale. Readers take a snapshot with load() and use it; a writer builds
// a brand-new immutable object and publishes it with store(). No reader ever sees a
// half-updated object -- the fields always agree -- and no reader blocks a writer or
// another reader, because there is no mutex. Old snapshots are freed automatically
// when the last reader holding one lets it go.
struct Config {
    int limit;
    int limit_times_ten;                // an invariant: always limit * 10
};

int main() {
    std::atomic<std::shared_ptr<const Config>> config{
        std::make_shared<const Config>(Config{1, 10})};

    std::atomic<bool> stop{false};
    std::atomic<long> consistent_reads{0};
    std::atomic<long> total_reads{0};

    std::vector<std::jthread> readers;
    for (int i = 0; i < 4; ++i) {
        readers.emplace_back([&] {
            long ok = 0, n = 0;
            while (!stop.load(std::memory_order_relaxed)) {
                std::shared_ptr<const Config> c = config.load();   // a snapshot
                if (c->limit_times_ten == c->limit * 10) ++ok;     // invariant holds
                ++n;
            }
            consistent_reads.fetch_add(ok);
            total_reads.fetch_add(n);
        });
    }

    // The writer republishes the config many times, each a fresh consistent object.
    for (int k = 2; k <= 200; ++k) {
        config.store(std::make_shared<const Config>(Config{k, k * 10}));
    }
    stop.store(true, std::memory_order_relaxed);
    readers.clear();                    // join readers

    // Every read observed a self-consistent snapshot, so the two counts match.
    std::cout << "every snapshot was self-consistent = " << std::boolalpha
              << (consistent_reads.load() == total_reads.load()) << '\n';   // true
    return 0;
}
