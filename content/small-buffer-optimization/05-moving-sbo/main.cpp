#include <cstddef>
#include <iostream>
#include <new>
#include <utility>

// A type that reports when it is moved, so we can watch what an SBO move does.
struct Tracked {
    int v;
    Tracked(int x) : v(x) {}
    Tracked(Tracked&& o) noexcept : v(o.v) { o.v = -1; ++moves; }
    Tracked(const Tracked&) = default;
    static int moves;
};
int Tracked::moves = 0;

// A trimmed small_vector that stays within its inline capacity, focused on the
// move constructor. Real code also handles the heap case (steal the pointer);
// the comment marks where that branch would go.
template <class T, std::size_t N>
class SmallVec {
    alignas(T) unsigned char inline_[N * sizeof(T)];
    T*          data_;
    std::size_t size_ = 0;
    T* inline_ptr() { return reinterpret_cast<T*>(inline_); }
public:
    SmallVec() : data_(inline_ptr()) {}
    void push(T x) { ::new (data_ + size_) T(std::move(x)); ++size_; }  // assumes room

    // The move constructor. If the source were on the heap we could steal its
    // pointer in O(1). Because it is inline, we must move-construct each element
    // into THIS object's own inline buffer -- an O(N) operation.
    SmallVec(SmallVec&& o) noexcept : data_(inline_ptr()), size_(o.size_) {
        for (std::size_t i = 0; i < size_; ++i) {
            ::new (data_ + i) T(std::move(o.data_[i]));
            o.data_[i].~T();
        }
        o.size_ = 0;
    }
    ~SmallVec() { for (std::size_t i = 0; i < size_; ++i) data_[i].~T(); }

    const void* data_addr()             const { return data_; }
    const T&    operator[](std::size_t i) const { return data_[i]; }
};

int main() {
    SmallVec<Tracked, 4> a;
    a.push(Tracked{1});
    a.push(Tracked{2});
    a.push(Tracked{3});

    const void* before = a.data_addr();
    int movesBefore = Tracked::moves;

    SmallVec<Tracked, 4> b = std::move(a);   // inline move: elements move one by one
    const void* after = b.data_addr();

    std::cout << "inline data address changed on move: "
              << (before != after) << '\n';                                 // 1
    std::cout << "elements move-constructed during the move: "
              << (Tracked::moves - movesBefore) << '\n';                    // 3
    std::cout << "b now holds: " << b[0].v << ' ' << b[1].v << ' ' << b[2].v << '\n';  // 1 2 3
    return 0;
}
