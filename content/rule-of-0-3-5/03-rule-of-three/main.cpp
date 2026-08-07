#include <cstddef>
#include <iostream>

// A class that owns a raw heap buffer. Because the destructor frees it, the
// compiler's shallow copy would double-free -- so the rule of three says: if
// you write one of destructor / copy ctor / copy assignment, write all three.
class IntArray {
public:
    explicit IntArray(std::size_t n) : size_(n), data_(new int[n]) {
        for (std::size_t i = 0; i < n; ++i) data_[i] = 0;
    }
    ~IntArray() { delete[] data_; }                              // (1) destructor

    IntArray(const IntArray& o) : size_(o.size_), data_(new int[o.size_]) {  // (2) copy ctor
        for (std::size_t i = 0; i < size_; ++i) data_[i] = o.data_[i];       //     deep copy
    }
    IntArray& operator=(const IntArray& o) {                     // (3) copy assignment
        if (this != &o) {                                        //     guard self-assignment
            int* fresh = new int[o.size_];                       //     allocate BEFORE freeing
            for (std::size_t i = 0; i < o.size_; ++i) fresh[i] = o.data_[i];
            delete[] data_;
            data_ = fresh;
            size_ = o.size_;
        }
        return *this;
    }

    std::size_t size() const { return size_; }
    void set(std::size_t i, int v) { data_[i] = v; }
    int get(std::size_t i) const { return data_[i]; }

private:
    std::size_t size_;
    int* data_;
};

int main() {
    IntArray a(3);
    a.set(0, 10); a.set(1, 20); a.set(2, 30);

    IntArray b = a;        // copy ctor: b gets its OWN buffer
    b.set(0, 99);          // changing b must not touch a

    IntArray c(1);
    c = a;                 // copy assignment: c releases its buffer, deep-copies a

    std::cout << "a[0]=" << a.get(0) << "  (unchanged by b)\n";
    std::cout << "b[0]=" << b.get(0) << "\n";
    std::cout << "c[0]=" << c.get(0) << " size=" << c.size() << "\n";
    // Each of the three buffers is freed exactly once at scope end.
    return 0;
}
