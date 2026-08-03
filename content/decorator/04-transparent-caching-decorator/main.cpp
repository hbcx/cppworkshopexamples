#include <iostream>
#include <map>
#include <memory>
#include <utility>

// Component: a read service. The client depends only on this.
struct PriceService {
    virtual int priceOf(int itemId) = 0;
    virtual ~PriceService() = default;
};

// Concrete component: the "expensive" backend -- imagine a network round-trip.
// It counts how many times it is really hit, so the effect of caching is visible.
class RemotePriceService : public PriceService {
public:
    int priceOf(int itemId) override {
        ++calls_;
        return itemId * 10;   // pretend this is slow
    }
    int calls() const { return calls_; }
private:
    int calls_ = 0;
};

// Decorator: adds caching AROUND any PriceService. Same interface in, same
// interface out -- a cross-cutting concern (caching) bolted on without the
// backend or the client knowing it is there. Neither had to change.
class CachingPriceService : public PriceService {
public:
    explicit CachingPriceService(std::unique_ptr<PriceService> inner)
        : inner_(std::move(inner)) {}
    int priceOf(int itemId) override {
        auto it = cache_.find(itemId);
        if (it != cache_.end()) {
            return it->second;          // served from cache -- backend untouched
        }
        int price = inner_->priceOf(itemId);
        cache_.emplace(itemId, price);
        return price;
    }
private:
    std::unique_ptr<PriceService> inner_;
    std::map<int, int> cache_;
};

int main() {
    auto backend = std::make_unique<RemotePriceService>();
    RemotePriceService* raw = backend.get();       // observe the real hit count
    CachingPriceService service(std::move(backend));

    const int ids[] = {1, 2, 1, 1, 2, 3};
    for (int id : ids) {
        std::cout << "price(" << id << ") = " << service.priceOf(id) << '\n';
    }
    // Six queries, but only three distinct ids -> the backend ran three times.
    std::cout << "backend calls: " << raw->calls() << '\n';
    return 0;
}
