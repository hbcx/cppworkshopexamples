// uninitialized_copy: copy-construct a range of objects into raw storage.
//
// The bytes from ::operator new hold no objects. std::copy would ASSIGN into
// them -- operator= on a std::string that was never constructed, which reads a
// garbage pointer and length. uninitialized_copy runs the COPY CONSTRUCTOR in
// each slot instead, turning raw bytes into live objects.

#include <iostream>
#include <memory>   // std::uninitialized_copy
#include <string>
#include <vector>

int main() {
    std::vector<std::string> src = {"alpha", "beta", "gamma"};
    using String = std::string;

    // Raw, uninitialized storage for src.size() strings -- no object here yet.
    String* buf = static_cast<String*>(
        ::operator new(sizeof(String) * src.size()));

    // Copy-construct every element into the raw slots. Returns one past the
    // last constructed object.
    String* end = std::uninitialized_copy(src.begin(), src.end(), buf);

    std::cout << "constructed " << (end - buf) << " strings in raw storage\n";
    for (String* p = buf; p != end; ++p) {
        std::cout << "  " << *p << '\n';
    }

    // Tear down by hand: one destructor per live object, then free the bytes.
    for (String* p = buf; p != end; ++p) {
        p->~String();
    }
    ::operator delete(buf);
}
