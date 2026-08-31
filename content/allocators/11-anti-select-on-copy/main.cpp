// Anti-pattern: assuming a copied container gets a fresh allocator.
//
// select_on_container_copy_construction picks the copy's allocator, and its
// default returns a COPY of the source allocator. So a stateful allocator's
// state (here an arena id) is shared with the copy, not duplicated.

#include <cstddef>
#include <iostream>
#include <new>
#include <vector>

template <class T>
struct TaggedAlloc {
    using value_type = T;
    int arenaId;

    TaggedAlloc(int id) : arenaId(id) {}
    template <class U>
    TaggedAlloc(const TaggedAlloc<U>& o) : arenaId(o.arenaId) {}

    T* allocate(std::size_t n) { return static_cast<T*>(::operator new(n * sizeof(T))); }
    void deallocate(T* p, std::size_t) { ::operator delete(p); }

    // No select_on_container_copy_construction here, so the default is used:
    // it returns a copy of *this, keeping the same arenaId. To hand the copy
    // its own arena you would add:
    //   TaggedAlloc select_on_container_copy_construction() const {
    //       return TaggedAlloc(freshArenaId());
    //   }
};

template <class T, class U>
bool operator==(const TaggedAlloc<T>& a, const TaggedAlloc<U>& b) { return a.arenaId == b.arenaId; }
template <class T, class U>
bool operator!=(const TaggedAlloc<T>& a, const TaggedAlloc<U>& b) { return !(a == b); }

int main() {
    std::vector<int, TaggedAlloc<int>> orig({1, 2, 3}, TaggedAlloc<int>(7));
    auto copy = orig;   // copy-construction runs select_on_container_copy_construction

    std::cout << "original arena id: " << orig.get_allocator().arenaId << '\n';
    std::cout << "copy arena id:     " << copy.get_allocator().arenaId << '\n';
    std::cout << std::boolalpha
              << "copy shares the original's arena (default behaviour)? "
              << (copy.get_allocator() == orig.get_allocator()) << '\n';
}
