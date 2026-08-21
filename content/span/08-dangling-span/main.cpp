#include <iostream>
#include <span>
#include <vector>

std::vector<int> make_data() { return {1, 2, 3, 4}; }

int main() {
    // Anti-pattern: a span does not own or extend the lifetime of what it views.
    // Binding one to a temporary leaves it dangling the moment the statement ends.

    // WRONG (undefined -- shown as a comment, never run):
    //   std::span<int> s = make_data();  // the returned vector dies at the ;
    //   for (int x : s) ...              // reads freed memory -> UB
    //
    // The vector make_data() returns is a temporary; nothing keeps it alive for
    // the span, so s would dangle immediately.

    // RIGHT: give the container a name so it outlives the span.
    std::vector<int> owner = make_data(); // the vector lives as long as `owner`
    std::span<int> s = owner;             // views a named object that outlives s

    long total = 0;
    for (int x : s) total += x;
    std::cout << "sum over a live owner: " << total << '\n';
    return 0;
}
