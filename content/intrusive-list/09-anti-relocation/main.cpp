#include <iostream>
#include <vector>

struct Node {
    int   v;
    Node* next = nullptr;
    explicit Node(int x) : v(x) {}
};

int main() {
    std::vector<Node> pool;
    pool.reserve(2);                 // room for 2 without reallocating
    pool.emplace_back(1);
    pool.emplace_back(2);

    // Thread node 0 -> node 1 by their CURRENT addresses.
    pool[0].next = &pool[1];
    const Node* before = &pool[1];
    std::cout << "link points at the live element? " << (pool[0].next == before) << '\n';  // 1

    // --- the trap: grow the vector past its capacity ---
    // Reallocation moves every element to a NEW address. pool[0].next was move-
    // constructed with the OLD address of node 1 -- it now dangles. (An intrusive
    // element must live in storage that never relocates.)
    pool.emplace_back(3);            // reallocation: all elements move

    const Node* after = &pool[1];    // node 1's new address
    std::cout << "elements moved to a new address: " << (before != after) << '\n';         // 1
    std::cout << "the stored link still points at the live element? "
              << (pool[0].next == after) << '\n';                                           // 0

    // --- the fix: keep intrusive elements in STABLE storage ---
    // reserve enough up front, or use std::deque / std::list / a pool whose
    // elements never move, so the links stay valid.
    std::cout << "fix: use stable storage (reserve, deque, or a pool) for the nodes\n";
    return 0;
}
