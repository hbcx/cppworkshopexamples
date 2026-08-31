// unsynchronized_pool_resource serves many small same-size allocations out of
// large chunks it takes from an upstream resource. A pmr::list of 100 ints makes
// 100 node allocations, but the upstream sees only a few big requests. We count
// the upstream to show the difference.

#include <cstddef>
#include <iostream>
#include <list>
#include <memory_resource>

class CountingResource : public std::pmr::memory_resource {
public:
    explicit CountingResource(std::pmr::memory_resource* up) : upstream_(up) {}
    std::size_t allocations = 0;

private:
    void* do_allocate(std::size_t b, std::size_t a) override {
        ++allocations;
        return upstream_->allocate(b, a);
    }
    void do_deallocate(void* p, std::size_t b, std::size_t a) override {
        upstream_->deallocate(p, b, a);
    }
    bool do_is_equal(const std::pmr::memory_resource& o) const noexcept override {
        return this == &o;
    }
    std::pmr::memory_resource* upstream_;
};

int main() {
    CountingResource counter{std::pmr::new_delete_resource()};
    std::pmr::unsynchronized_pool_resource pool{&counter};

    const int nodes = 100;
    {
        std::pmr::list<int> lst{&pool};
        for (int i = 0; i < nodes; ++i) lst.push_back(i);
        std::cout << "list holds " << lst.size() << " nodes\n";
    }   // list gone; the pool still owns its chunks until it is destroyed

    std::cout << "upstream allocations for " << nodes << " nodes: fewer than "
              << nodes << "? " << std::boolalpha
              << (counter.allocations < nodes) << '\n';
}
