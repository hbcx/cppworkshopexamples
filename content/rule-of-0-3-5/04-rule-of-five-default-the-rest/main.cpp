#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

// This class holds a std::vector, which manages itself, so rule of zero would
// give it correct copy AND move for free. But suppose we must declare a
// destructor -- here to log, in real code to close a handle or because this is
// a polymorphic base. Declaring it STOPS the compiler generating the move
// constructor and move assignment, so the class silently falls back to copying.
// The fix: = default the moves (and copies) back.
class Buffer {
public:
    explicit Buffer(std::size_t n) : data_(n, 0) {}
    ~Buffer() { std::cout << "~Buffer size=" << data_.size() << "\n"; }

    Buffer(Buffer&&) noexcept = default;             // brought back
    Buffer& operator=(Buffer&&) noexcept = default;  // brought back
    Buffer(const Buffer&) = default;                 // kept (copy)
    Buffer& operator=(const Buffer&) = default;      // kept (copy)

    std::size_t size() const { return data_.size(); }

private:
    std::vector<int> data_;
};

Buffer makeBuffer() { return Buffer(1000); }

int main() {
    Buffer a(1000);
    Buffer b = std::move(a);      // move ctor (defaulted): steals the buffer
    std::cout << "after move:   b.size=" << b.size()
              << " a.size=" << a.size() << " (moved-from)\n";

    b = makeBuffer();             // move assignment (defaulted)
    std::cout << "after assign: b.size=" << b.size() << "\n";

    // Without the = default lines, the destructor would have deleted the moves:
    // `Buffer b = std::move(a)` would COPY 1000 ints instead of stealing them,
    // and the class would look movable while never actually moving.
    return 0;
}
