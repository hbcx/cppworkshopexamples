// Anti-pattern: an accidental cycle in the chain.
//
// A chain is a linked list, and linking it by hand makes it easy to point a
// handler's next_ back at an earlier one, forming a cycle. A request that no
// handler accepts then travels the loop forever -- the traversal never reaches a
// null next_, so it never terminates. The unguarded loop below (in the comment)
// would hang; to make the bug observable without hanging, the runnable traversal
// carries a hop limit and reports a suspected cycle. The real fix is to not
// build the cycle: a well-formed chain ends in a null next_.

#include <iostream>
#include <string>

struct Node {
    std::string name;
    Node* next; // set explicitly at construction below
};

// An unguarded traversal would be:
//   while (n) { visit(n); n = n->next; }   // never ends if the chain has a cycle
//
// The hop limit here is a diagnostic, not the fix -- it stops the hang so the bug
// is visible, but a correct chain would not need it.
static void traverse(Node* head, int maxHops) {
    Node* n = head;
    int hops = 0;
    while (n) {
        std::cout << " " << n->name;
        if (++hops > maxHops) { std::cout << " ...aborted (cycle suspected)\n"; return; }
        n = n->next;
    }
    std::cout << " [end]\n";
}

int main() {
    Node a{ "a", nullptr }, b{ "b", nullptr }, c{ "c", nullptr };

    // ---- BAD: c links back to a -> a -> b -> c -> a -> ... forever ----
    a.next = &b; b.next = &c; c.next = &a; // the cycle
    std::cout << "bad (cyclic): ";
    traverse(&a, 6);

    // ---- GOOD: the chain terminates in a null next_ ----
    c.next = nullptr; // break the cycle
    std::cout << "good (acyclic):";
    traverse(&a, 6);
    return 0;
}
