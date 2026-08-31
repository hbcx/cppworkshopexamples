#include <iostream>
#include <new>

struct Tracked {
    static int alive;
    Tracked()  { ++alive; }
    ~Tracked() { --alive; }
};
int Tracked::alive = 0;

int main() {
    // WRONG (undefined behavior, NOT run here):
    //   Tracked* arr = new Tracked[3];
    //   delete arr;            // single delete on an array allocation
    // new[] and delete[] are a separate pairing from new/delete. Plain delete
    // runs at most one destructor and frees with the scalar operator delete,
    // typically corrupting the heap. The count cookie new[] wrote is ignored.

    // RIGHT: array new pairs with array delete, which destroys every element.
    Tracked* arr = new Tracked[3];
    std::cout << "after new[]:    alive = " << Tracked::alive << "\n";
    delete[] arr;
    std::cout << "after delete[]: alive = " << Tracked::alive << "\n";
}
