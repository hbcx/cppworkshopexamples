#include <iostream>
#include <unordered_set>
#include <vector>

// A node with non-owning child links, so a cycle is possible. A tree with
// unique_ptr children cannot form one, but a graph -- or a tree whose parent
// links are followed as if they were children -- can.
struct Node {
    int id;
    std::vector<Node*> children;
};

// ANTI-PATTERN (described, not run): a recursive walk with no record of what it
// has visited never terminates on a cycle -- it follows a -> b -> a -> b forever
// and overflows the stack. That is undefined behaviour, so it is not executed.
//
//   int countBad(const Node& n) {
//       int total = 1;
//       for (const Node* c : n.children) total += countBad(*c);   // loops on a cycle
//       return total;
//   }

// FIX: remember the nodes already seen, so each is counted once and a cycle
// terminates instead of recursing forever.
int countGood(const Node& n, std::unordered_set<const Node*>& seen) {
    if (!seen.insert(&n).second) return 0;   // already visited -- stop here
    int total = 1;
    for (const Node* c : n.children) total += countGood(*c, seen);
    return total;
}

int main() {
    Node a{1, {}};
    Node b{2, {}};
    a.children.push_back(&b);
    b.children.push_back(&a);        // cycle: a -> b -> a -> ...

    std::unordered_set<const Node*> seen;
    std::cout << "distinct nodes reachable: " << countGood(a, seen) << "\n";   // 2
    return 0;
}
