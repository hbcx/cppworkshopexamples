#include <functional>
#include <iostream>
#include <thread>

struct Tracer {
    int copies = 0;
    Tracer() = default;
    Tracer(const Tracer& o) : copies(o.copies + 1) {}   // count how often we are copied
};

void inspect_by_value(Tracer t) {                        // takes the argument BY VALUE
    std::cout << "inside thread, argument was copied: "
              << (t.copies > 0) << '\n';                 // true: it is a copy, not ours
}

void bump(int& counter) { counter += 1; }                // wants to modify the caller's var

int main() {
    std::cout << std::boolalpha;

    // FACT 1: std::thread COPIES each argument into its own storage before calling
    // the function. Pass a Tracer and, inside the thread, its copy count is non-zero
    // -- the function is looking at a copy, never your object.
    Tracer original;
    std::thread a(inspect_by_value, original);           // 'original' is copied in
    a.join();

    // FACT 2 (the trap): because the stored copy is handed to the function as an
    // RVALUE, a function that takes a NON-CONST REFERENCE cannot bind to it. On
    // modern standard libraries this is now a hard COMPILE error -- a static_assert
    // was added specifically to stop the old silent bug -- so the line below will
    // not build:
    //
    //   int n = 0;
    //   std::thread bad(bump, n);   // error: arguments not invocable as rvalues
    //
    // On OLDER libraries it compiled and bump modified the throwaway copy, leaving
    // your variable unchanged -- a silent do-nothing. Either way, the plain call is
    // wrong.

    // FIX: std::ref wraps a real reference that survives the copy, so bump binds to
    // your actual variable. (std::cref does the same for a const reference.)
    int n = 0;
    std::thread good(bump, std::ref(n));
    good.join();
    std::cout << "with std::ref, n = " << n << '\n';     // 1

    // Takeaway: a thread argument is copied, not referenced. Say std::ref when you
    // truly need a reference (and keep that object alive for the whole run); other-
    // wise pass by value on purpose and return the result through a slot or a future.
    return 0;
}
