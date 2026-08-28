#include <iostream>
#include <vector>
#include <initializer_list>

// Anti-pattern: a copy assignment that forgets to guard self-assignment. It wipes
// its own storage before copying from the source -- fine when they differ, but
// when the source IS the target (x = x) it copies from the storage it just
// cleared, and the data is gone. (Here the storage is a std::vector, so clearing
// then reading it is well defined but WRONG; with a raw new[]/delete[] buffer the
// same shape would be undefined behavior.)

class NaiveList {
public:
    NaiveList(std::initializer_list<int> xs) : data_(xs) {}

    // BAD: no "am I assigning to myself?" check.
    NaiveList& operator=(const NaiveList& other) {
        data_.clear();                          // step 1: wipe our data
        for (int x : other.data_)               // step 2: copy from other...
            data_.push_back(x);                 // ...but other may BE us, now empty
        return *this;
    }

    std::size_t size() const { return data_.size(); }

private:
    std::vector<int> data_;
};

class SafeList {
public:
    SafeList(std::initializer_list<int> xs) : data_(xs) {}

    // FIX: guard self-assignment first, and return *this so a = b = c chains.
    // (Return void here and that chained line would not compile.)
    SafeList& operator=(const SafeList& other) {
        if (this == &other) return *this;       // the guard that was missing above
        data_ = other.data_;
        return *this;
    }

    std::size_t size() const { return data_.size(); }

private:
    std::vector<int> data_;
};

int main() {
    NaiveList bad{1, 2, 3, 4};
    NaiveList& badAlias = bad;                  // an alias hides that this is self-assignment
    bad = badAlias;                             // ...exactly how it slips past review
    std::cout << "naive after x = x: size " << bad.size() << "\n";   // 0  (data lost)

    SafeList a{1, 2, 3}, b{9, 9}, c{7};
    a = b = c;                                  // chaining relies on returning *this
    std::cout << "safe after a = b = c: sizes "
              << a.size() << " " << b.size() << " " << c.size() << "\n";  // 1 1 1

    SafeList s{5, 6, 7, 8};
    SafeList& sAlias = s;                       // same aliased self-assignment...
    s = sAlias;                                 // ...but the guard makes it a no-op
    std::cout << "safe after x = x: size " << s.size() << "\n";       // 4  (kept)
    return 0;
}
