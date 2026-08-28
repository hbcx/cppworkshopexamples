#include <iostream>
#include <vector>
#include <cstddef>

// Anti-pattern: a Stack that inherits std::vector to reuse its storage. Because
// the inheritance is public, callers get the ENTIRE vector interface, and can
// break the last-in-first-out rule the Stack was supposed to guarantee.

class BadStack : public std::vector<int> {
public:
    void push(int x) { push_back(x); }
    int  pop() { int v = back(); pop_back(); return v; }
};

// FIX: composition. The vector is private, and only stack operations are exposed,
// so nothing outside can reach past the invariant.
class GoodStack {
public:
    void push(int x) { data_.push_back(x); }
    int  pop() { int v = data_.back(); data_.pop_back(); return v; }
    std::size_t size() const { return data_.size(); }
private:
    std::vector<int> data_;
};

int main() {
    BadStack bad;
    bad.push(1);
    bad.push(2);
    bad.push(3);

    // All of these compile -- and none of them are stack operations:
    bad[0] = 99;        // random write through the leaked operator[]
    bad.clear();        // wipe the whole stack from outside
    std::cout << "bad stack size after outside clear(): " << bad.size() << "\n";  // 0

    GoodStack good;
    good.push(1);
    good.push(2);
    // good.clear();  and  good[0]  do NOT compile -- there is no way in.
    std::cout << "good stack popped: " << good.pop()
              << ", size now " << good.size() << "\n";   // 2, size now 1
    return 0;
}
