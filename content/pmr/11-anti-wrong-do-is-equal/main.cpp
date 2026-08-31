// Anti-pattern: a custom memory_resource whose do_is_equal always returns true.
//
// do_is_equal means "can this resource free memory the other allocated". Two
// independent resources cannot, so claiming they are equal lets a container move
// a buffer between them and free it through the wrong one -- undefined.

#include <cstddef>
#include <iostream>
#include <memory_resource>

// A resource with a broken equality: it claims every instance is interchangeable.
class AlwaysEqualResource : public std::pmr::memory_resource {
    void* do_allocate(std::size_t b, std::size_t a) override {
        return std::pmr::new_delete_resource()->allocate(b, a);
    }
    void do_deallocate(void* p, std::size_t b, std::size_t a) override {
        std::pmr::new_delete_resource()->deallocate(p, b, a);
    }
    bool do_is_equal(const std::pmr::memory_resource&) const noexcept override {
        return true;   // WRONG: two separate instances are not interchangeable
    }
};

// The correct shape: identity.
class IdentityResource : public std::pmr::memory_resource {
    void* do_allocate(std::size_t b, std::size_t a) override {
        return std::pmr::new_delete_resource()->allocate(b, a);
    }
    void do_deallocate(void* p, std::size_t b, std::size_t a) override {
        std::pmr::new_delete_resource()->deallocate(p, b, a);
    }
    bool do_is_equal(const std::pmr::memory_resource& o) const noexcept override {
        return this == &o;   // only equal to itself
    }
};

int main() {
    AlwaysEqualResource wrongA, wrongB;
    IdentityResource goodA, goodB;

    std::cout << std::boolalpha;
    std::cout << "always-true resource: two separate instances equal? "
              << wrongA.is_equal(wrongB) << "  (they cannot free each other)\n";
    std::cout << "identity resource:    two separate instances equal? "
              << goodA.is_equal(goodB) << '\n';

    // With the always-true version, a container move would let one resource
    // deallocate a pointer the other owns -- undefined. Identity equality forces
    // an element-by-element move instead, which is safe. (Free is described.)
    std::cout << "identity equality keeps each resource freeing only its own memory\n";
}
