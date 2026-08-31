// A mini growable vector that uses the whole toolkit:
//   ::operator new        -- raw capacity, no objects
//   placement new         -- construct one element in the next raw slot
//   uninitialized_move    -- relocate live objects into a bigger block on grow
//   std::destroy          -- run destructors on the live prefix
// Capacity is separate from size, so most of the buffer is raw at any moment.

#include <iostream>
#include <memory>       // std::uninitialized_move, std::destroy
#include <string>
#include <utility>

template <class T>
class MiniVec {
    T* data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t cap_ = 0;

    void grow() {
        std::size_t newCap = cap_ ? cap_ * 2 : 1;
        T* nd = static_cast<T*>(::operator new(sizeof(T) * newCap));
        // Relocate the live objects into the new block by moving.
        std::uninitialized_move(data_, data_ + size_, nd);
        // Old objects are moved-from but alive: destroy them, free old bytes.
        std::destroy(data_, data_ + size_);
        ::operator delete(data_);
        data_ = nd;
        cap_ = newCap;
    }

public:
    void push_back(const T& value) {
        if (size_ == cap_) grow();
        new (data_ + size_) T(value);   // construct in the next raw slot
        ++size_;
    }
    std::size_t size() const { return size_; }
    std::size_t capacity() const { return cap_; }
    const T& operator[](std::size_t i) const { return data_[i]; }

    ~MiniVec() {
        std::destroy(data_, data_ + size_);
        ::operator delete(data_);
    }
};

int main() {
    MiniVec<std::string> v;
    const char* parts[] = {"one", "two", "three", "four", "five"};
    for (const char* p : parts) {
        v.push_back(p);
        std::cout << "pushed " << p << " -> size " << v.size()
                  << ", capacity " << v.capacity() << '\n';
    }

    std::cout << "contents:";
    for (std::size_t i = 0; i < v.size(); ++i) std::cout << ' ' << v[i];
    std::cout << '\n';
}
