#include <cstddef>
#include <iostream>
#include <new>

// A resource whose live count we track, to see whether an SBO holder cleans up.
struct Resource {
    Resource()                { ++live; }
    Resource(const Resource&) { ++live; }
    ~Resource()               { --live; }
    static int live;
};
int Resource::live = 0;

// The buggy holder: it placement-news a Resource into its inline buffer but its
// destructor does NOT call ~Resource(). Placement new registers no destructor --
// you must call it by hand -- so the Resource is never destroyed.
struct LeakyBox {
    alignas(Resource) unsigned char buf[sizeof(Resource)];
    LeakyBox() { ::new (buf) Resource(); }
    // ~LeakyBox() is missing the explicit ~Resource() call   <-- BUG
};

// The fix: destroy the object that was placement-new'd, explicitly.
struct FixedBox {
    alignas(Resource) unsigned char buf[sizeof(Resource)];
    FixedBox() { ::new (buf) Resource(); }
    ~FixedBox() { reinterpret_cast<Resource*>(buf)->~Resource(); }
};

int main() {
    { LeakyBox b; }
    std::cout << "after LeakyBox scope, live Resources: " << Resource::live << '\n';  // 1 (leaked)

    Resource::live = 0;
    { FixedBox b; }
    std::cout << "after FixedBox scope, live Resources: " << Resource::live << '\n';  // 0 (clean)
    return 0;
}
