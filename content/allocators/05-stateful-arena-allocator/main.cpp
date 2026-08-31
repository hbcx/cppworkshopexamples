// A stateful allocator carries a pointer to an arena and slices storage out of
// its fixed buffer. deallocate is a no-op -- the arena frees everything when it
// dies. The vector's elements end up living inside the arena buffer.

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>   // std::align
#include <vector>

struct Arena {
    alignas(std::max_align_t) unsigned char buf[4096];
    std::size_t used = 0;

    void* take(std::size_t bytes, std::size_t al) {
        void* p = buf + used;
        std::size_t space = sizeof(buf) - used;
        void* a = std::align(al, bytes, p, space);
        used = (static_cast<unsigned char*>(a) - buf) + bytes;
        return a;
    }
    bool contains(const void* p) const {
        return p >= buf && p < buf + sizeof(buf);
    }
};

template <class T>
struct ArenaAlloc {
    using value_type = T;
    Arena* arena;

    ArenaAlloc(Arena* a) : arena(a) {}
    template <class U>
    ArenaAlloc(const ArenaAlloc<U>& o) : arena(o.arena) {}   // keep the arena

    T* allocate(std::size_t n) {
        return static_cast<T*>(arena->take(n * sizeof(T), alignof(T)));
    }
    void deallocate(T*, std::size_t) {}   // freed all at once with the arena
};

template <class T, class U>
bool operator==(const ArenaAlloc<T>& a, const ArenaAlloc<U>& b) {
    return a.arena == b.arena;   // equal only if backed by the same arena
}
template <class T, class U>
bool operator!=(const ArenaAlloc<T>& a, const ArenaAlloc<U>& b) { return !(a == b); }

int main() {
    Arena arena;
    std::vector<int, ArenaAlloc<int>> v{ArenaAlloc<int>(&arena)};
    v.reserve(10);
    for (int i = 0; i < 10; ++i) v.push_back(i * i);

    std::cout << "arena used " << arena.used << " bytes\n";
    std::cout << "vector storage inside the arena buffer? "
              << std::boolalpha << arena.contains(v.data()) << '\n';
    std::cout << "first and last: " << v.front() << " .. " << v.back() << '\n';
}
