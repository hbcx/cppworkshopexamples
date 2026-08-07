// Anti-pattern: a resource-owning class that stays copyable.
//
// If a class releases a resource in its destructor but does not control copying,
// the compiler-provided copy duplicates the HANDLE, not the resource. Now two
// objects believe they own the same resource, and both destructors release it.
// Releasing the same resource twice is a double free / double close: real
// undefined behaviour with a raw resource.
//
// We do NOT trigger that UB. Instead each handle counts how many times it was
// released; a correct program releases each handle exactly once, and the bug
// shows up as a release count of 2.

#include <iostream>

int releaseCount[8] = {0};

// BROKEN: copy is left enabled. The copy shares handle_, so two owners release
// the same handle. (Writing it = default compiles cleanly; leaving copy
// implicit does the same shallow copy -- and is deprecated for exactly this.)
struct BadOwner {
    int handle_;
    explicit BadOwner(int h) : handle_(h) { std::cout << "open  " << handle_ << "\n"; }
    BadOwner(const BadOwner&) = default;              // shallow copy of the handle -- the bug
    ~BadOwner() {
        ++releaseCount[handle_];
        std::cout << "close " << handle_ << " (release #" << releaseCount[handle_] << ")\n";
    }
};

// FIXED: move-only. Copy is deleted; move transfers the handle and nulls the
// source, so the moved-from object releases nothing.
struct GoodOwner {
    int handle_;
    explicit GoodOwner(int h) : handle_(h) { std::cout << "open  " << handle_ << "\n"; }
    GoodOwner(const GoodOwner&) = delete;
    GoodOwner& operator=(const GoodOwner&) = delete;
    GoodOwner(GoodOwner&& o) noexcept : handle_(o.handle_) { o.handle_ = -1; }
    ~GoodOwner() {
        if (handle_ == -1) return;                    // moved-from: owns nothing
        ++releaseCount[handle_];
        std::cout << "close " << handle_ << " (release #" << releaseCount[handle_] << ")\n";
    }
};

int main() {
    std::cout << "-- copyable owner --\n";
    {
        BadOwner a(1);
        BadOwner b = a;              // shallow copy: b.handle_ == a.handle_ == 1
        (void)b;                     // both destructors will close handle 1
    }
    std::cout << "handle 1 released " << releaseCount[1] << " times  <- double release\n\n";

    std::cout << "-- move-only owner --\n";
    {
        GoodOwner a(2);
        GoodOwner b = std::move(a);  // ownership moves to b; a now holds -1
        (void)b;
    }
    std::cout << "handle 2 released " << releaseCount[2] << " times  <- exactly once\n";
}
