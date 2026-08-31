// A custom memory_resource is three virtual functions: do_allocate,
// do_deallocate, do_is_equal. Forward the work to an upstream resource and wrap
// your own logic around it. Here it counts allocations and bytes, then plugs
// into a pmr::vector.

#include <cstddef>
#include <iostream>
#include <memory_resource>
#include <vector>

class CountingResource : public std::pmr::memory_resource {
public:
    explicit CountingResource(std::pmr::memory_resource* up) : upstream_(up) {}
    std::size_t allocations = 0;
    std::size_t bytes = 0;

private:
    void* do_allocate(std::size_t bytesIn, std::size_t align) override {
        ++allocations;
        bytes += bytesIn;
        return upstream_->allocate(bytesIn, align);
    }
    void do_deallocate(void* p, std::size_t bytesIn, std::size_t align) override {
        upstream_->deallocate(p, bytesIn, align);
    }
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }
    std::pmr::memory_resource* upstream_;
};

int main() {
    CountingResource res{std::pmr::new_delete_resource()};

    std::pmr::vector<int> v{&res};
    v.reserve(100);                       // one allocation of 100 ints
    for (int i = 0; i < 10; ++i) v.push_back(i);

    std::cout << "allocations counted: " << res.allocations << '\n';
    std::cout << "bytes counted:       " << res.bytes << '\n';
    std::cout << "vector size:         " << v.size() << '\n';
}
