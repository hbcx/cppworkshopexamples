#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

// Subject.
struct PriceFeed {
    virtual int priceOf(const std::string& sym) = 0;
    virtual ~PriceFeed() = default;
};

// Real subject: a slow source that counts how often it is actually queried.
class RemoteFeed : public PriceFeed {
public:
    int priceOf(const std::string& sym) override {
        ++hits_;
        return static_cast<int>(sym.size()) * 100;   // stand-in for a slow query
    }
    int hits() const { return hits_; }
private:
    int hits_ = 0;
};

// Caching proxy: the same interface, but it CONTROLS access to the real subject
// -- on a cache hit it does not call the real object at all. Deciding whether the
// real call happens is the defining trait of a proxy. Structurally this is
// identical to the caching decorator in the Decorator chapter; what differs is
// intent: a decorator ADDS behaviour to the result, a proxy STANDS IN for the
// subject and governs access to it.
class CachingFeed : public PriceFeed {
public:
    explicit CachingFeed(std::unique_ptr<PriceFeed> real) : real_(std::move(real)) {}
    int priceOf(const std::string& sym) override {
        auto it = cache_.find(sym);
        if (it != cache_.end()) {
            return it->second;                        // access controlled: no real call
        }
        int p = real_->priceOf(sym);
        cache_.emplace(sym, p);
        return p;
    }
private:
    std::unique_ptr<PriceFeed> real_;
    std::map<std::string, int> cache_;
};

int main() {
    auto backend = std::make_unique<RemoteFeed>();
    RemoteFeed* raw = backend.get();
    CachingFeed feed(std::move(backend));

    const std::string syms[] = {"AAA", "BB", "AAA", "AAA", "BB"};
    for (const std::string& s : syms) {
        std::cout << s << " -> " << feed.priceOf(s) << "\n";
    }
    std::cout << "real lookups: " << raw->hits() << "\n";   // 2 distinct, not 5
    return 0;
}
