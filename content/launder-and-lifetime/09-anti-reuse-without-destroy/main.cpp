// Anti-pattern: placement-new over an object that still owns a resource.
//
// Reusing storage ends the old object's lifetime but does NOT run its
// destructor. On a type that owns something, that resource leaks. Destroy the
// old object first. The live-object counter shows the difference.

#include <iostream>
#include <new>

struct Owner {
    int id;
    static int alive;
    explicit Owner(int i) : id(i) { ++alive; }   // stands in for acquiring a resource
    ~Owner() { --alive; }                        // stands in for releasing it
};
int Owner::alive = 0;

int main() {
    // BAD: construct over a live object without destroying it first.
    {
        alignas(Owner) unsigned char storage[sizeof(Owner)];
        new (storage) Owner(1);
        new (storage) Owner(2);   // object 1's destructor never ran -> leak
        // Clean up the current object (2). Object 1 can never be reclaimed.
        std::launder(reinterpret_cast<Owner*>(storage))->~Owner();
        std::cout << "bad  (no destroy first): alive = " << Owner::alive
                  << " (object 1 leaked)\n";
    }

    Owner::alive = 0;   // reset the counter for the second demo

    // GOOD: destroy the old object before reusing its storage.
    {
        alignas(Owner) unsigned char storage[sizeof(Owner)];
        Owner* o = new (storage) Owner(1);
        o->~Owner();                          // release before reuse
        Owner* o2 = new (storage) Owner(2);
        o2->~Owner();                         // and release the second one too
        std::cout << "good (destroy first):    alive = " << Owner::alive
                  << " (nothing leaked)\n";
    }
}
