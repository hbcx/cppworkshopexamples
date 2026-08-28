#include <iostream>

// Protected inheritance: the base interface is hidden from outside code but stays
// available to classes that derive further. Compare with private, where even a
// grandchild would be locked out.

class Storage {
public:
    void put(int v) { value_ = v; }
    int  get() const { return value_; }
private:
    int value_ = 0;
};

class Cache : protected Storage {   // put/get become PROTECTED in Cache
    // Outside code cannot call put/get on a Cache, but a subclass still can.
};

class CountingCache : public Cache {
public:
    // A grandchild reaches the protected base interface -- this is what protected
    // inheritance allows and private would forbid.
    void store(int v) { put(v); ++writes_; }
    int  load() const { return get(); }
    int  writes() const { return writes_; }
private:
    int writes_ = 0;
};

int main() {
    CountingCache c;
    c.store(7);
    c.store(9);

    std::cout << "value: " << c.load() << "\n";     // 9
    std::cout << "writes: " << c.writes() << "\n";   // 2

    // c.put(1);  // would NOT compile: put() is protected, unreachable from outside
    return 0;
}
