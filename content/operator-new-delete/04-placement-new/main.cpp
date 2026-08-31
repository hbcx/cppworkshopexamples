#include <iostream>
#include <new>
#include <string>

int main() {
    // A buffer with the size and alignment of a std::string, but no string
    // is alive in it yet -- it is just raw bytes.
    alignas(std::string) unsigned char storage[sizeof(std::string)];

    // Placement new constructs a std::string INSIDE the existing buffer.
    // It does not allocate; it only runs the constructor at that address.
    std::string* s = new (storage) std::string("built in place");
    std::cout << *s << " (size " << s->size() << ")\n";

    // Placement new has no matching delete: YOU must call the destructor to
    // end the object's life. A typedef avoids the ~std::string() syntax trap.
    using Str = std::string;
    s->~Str();

    std::cout << "string destroyed, buffer still ours\n";
}
