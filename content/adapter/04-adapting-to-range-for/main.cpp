#include <iostream>

// Adaptee: a legacy collection with an index-based interface only -- no
// iterators, so a range-for loop or an STL algorithm cannot touch it directly.
class NumberBag {
public:
    void add(int v) { data_[size_++] = v; }
    int count() const { return size_; }
    int at(int i) const { return data_[i]; }
private:
    int data_[8] = {};
    int size_ = 0;
};

// Adapter: give the legacy collection the iterator interface range-for expects.
// A minimal iterator over (bag, index) is enough; begin()/end() hand it out.
class BagRange {
public:
    explicit BagRange(const NumberBag& bag) : bag_(bag) {}

    class iterator {
    public:
        iterator(const NumberBag& bag, int i) : bag_(bag), i_(i) {}
        int operator*() const { return bag_.at(i_); }
        iterator& operator++() { ++i_; return *this; }
        bool operator!=(const iterator& other) const { return i_ != other.i_; }
    private:
        const NumberBag& bag_;
        int i_;
    };

    iterator begin() const { return iterator(bag_, 0); }
    iterator end() const { return iterator(bag_, bag_.count()); }

private:
    const NumberBag& bag_;
};

int main() {
    NumberBag bag;
    bag.add(3);
    bag.add(1);
    bag.add(4);

    // The legacy bag now works with a modern range-for through the adapter --
    // no change to NumberBag, and no copying its contents into a std::vector.
    int sum = 0;
    for (int v : BagRange(bag)) {
        std::cout << "got " << v << '\n';
        sum += v;
    }
    std::cout << "sum = " << sum << '\n';
    return 0;
}
