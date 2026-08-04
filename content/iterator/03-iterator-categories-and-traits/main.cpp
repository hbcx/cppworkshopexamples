// Iterator categories: what the category tag buys you.
//
// An iterator's iterator_category is a promise about which operations it
// supports, and algorithms dispatch on it. A forward iterator (previous example)
// supports ++ only, so std::distance must count step by step, O(n). A
// random-access iterator also supports it + n, it - it, it[n] and <, so
// std::distance is a single subtraction, O(1), and std::sort applies. Numbers
// below wraps a contiguous buffer with a random-access iterator, so the whole
// random-access algorithm family works on it.

#include <iostream>
#include <iterator>   // std::random_access_iterator_tag, std::distance, std::advance
#include <algorithm>  // std::sort
#include <cstddef>    // std::ptrdiff_t

class Numbers {
    int data_[8];
    std::size_t size_ = 0;
public:
    Numbers(std::initializer_list<int> xs) {
        for (int x : xs) data_[size_++] = x;
    }

    class iterator {
        int* p_;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = int;
        using difference_type   = std::ptrdiff_t;
        using pointer           = int*;
        using reference         = int&;

        explicit iterator(int* p) : p_(p) {}
        reference operator*() const { return *p_; }
        reference operator[](difference_type n) const { return p_[n]; }

        iterator& operator++() { ++p_; return *this; }
        iterator& operator--() { --p_; return *this; }
        iterator& operator+=(difference_type n) { p_ += n; return *this; }
        iterator& operator-=(difference_type n) { p_ -= n; return *this; }
        iterator operator+(difference_type n) const { return iterator(p_ + n); }
        iterator operator-(difference_type n) const { return iterator(p_ - n); }
        difference_type operator-(const iterator& o) const { return p_ - o.p_; }

        bool operator==(const iterator& o) const { return p_ == o.p_; }
        bool operator!=(const iterator& o) const { return p_ != o.p_; }
        bool operator<(const iterator& o) const { return p_ < o.p_; }
        bool operator>(const iterator& o) const { return p_ > o.p_; }
        bool operator<=(const iterator& o) const { return p_ <= o.p_; }
        bool operator>=(const iterator& o) const { return p_ >= o.p_; }
    };

    iterator begin() { return iterator(data_); }
    iterator end()   { return iterator(data_ + size_); }
};

int main() {
    Numbers n{ 5, 3, 8, 1, 9, 2 };

    // O(1): random-access, so std::distance is end - begin, one subtraction.
    std::cout << "count: " << std::distance(n.begin(), n.end()) << "\n";

    // std::sort REQUIRES a random-access iterator -- it would not compile against
    // the forward iterator of the previous example.
    std::sort(n.begin(), n.end());
    std::cout << "sorted:";
    for (int v : n) std::cout << " " << v;
    std::cout << "\n";

    // std::advance uses += for a random-access iterator: one jump, not n steps.
    auto it = n.begin();
    std::advance(it, 3);
    std::cout << "index 3 after sort: " << *it << "\n";
    return 0;
}
