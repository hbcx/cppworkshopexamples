// Anti-pattern: holding a pointer to an object whose storage is later reused.
//
// std::launder reaches the NEW object; nothing reaches the OLD one, because its
// lifetime ended. A cached pointer to the old object is a handle to something
// gone, and the bytes now belong to a different object.

#include <iostream>
#include <new>

struct Tracked {
    int id;
    static int alive;
    explicit Tracked(int i) : id(i) { ++alive; }
    ~Tracked() { --alive; }
};
int Tracked::alive = 0;

int main() {
    alignas(Tracked) unsigned char storage[sizeof(Tracked)];

    Tracked* first = new (storage) Tracked(1);
    Tracked* cached = first;              // some cache keeps this handle
    std::cout << "built object id = " << cached->id << '\n';

    first->~Tracked();                    // object 1's lifetime ends
    new (storage) Tracked(2);             // storage now holds a DIFFERENT object

    // BAD (undefined -- described, not run):
    //   use cached->id as if it were still the id-1 object -- that object is
    //   gone; launder would only ever give you object 2, never object 1 back.

    std::cout << "objects alive: " << Tracked::alive << " (only the new one)\n";
    Tracked* now = std::launder(cached);  // this is object 2, not object 1
    std::cout << "storage now holds id = " << now->id << '\n';

    now->~Tracked();
}
