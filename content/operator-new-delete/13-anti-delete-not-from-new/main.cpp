#include <iostream>
#include <new>
#include <string>

int main() {
    // WRONG (undefined behavior, NOT run here):
    //   alignas(std::string) unsigned char buf[sizeof(std::string)];
    //   std::string* s = new (buf) std::string("hi");
    //   delete s;     // buf did NOT come from operator new; delete frees it
    // delete both destroys the object AND calls operator delete on the
    // pointer -- but this memory was never handed out by new, so freeing it
    // is undefined. The same applies to a pointer into the middle of a block.

    // RIGHT: placement new is undone by an explicit destructor call; the
    // buffer itself is released when its own scope ends -- no delete.
    alignas(std::string) unsigned char buf[sizeof(std::string)];
    std::string* s = new (buf) std::string("in a stack buffer");
    std::cout << *s << "\n";
    using Str = std::string;
    s->~Str();
    std::cout << "destroyed via destructor; buffer released with its scope\n";
}
