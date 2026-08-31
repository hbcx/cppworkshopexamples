#include <iostream>
#include <new>
#include <cstddef>

struct Item {
    Item()  { std::cout << "Item()\n"; }
    ~Item() { std::cout << "~Item()\n"; }

    // The array forms are distinct from the scalar operator new/delete.
    static void* operator new[](std::size_t n) {
        std::cout << "operator new[] called\n";     // exact byte count is
        return ::operator new(n);                    // implementation-defined
    }
    static void operator delete[](void* p) noexcept {
        std::cout << "operator delete[]\n";
        ::operator delete(p);
    }
};

int main() {
    // new[] calls operator new[] ONCE, then constructs each element in turn.
    Item* items = new Item[3];

    // delete[] destroys every element (in reverse) then calls operator
    // delete[]. It must be delete[], never delete -- the array allocation is
    // a separate pairing and mixing the two is undefined.
    delete[] items;
}
