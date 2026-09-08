#include <generator>
#include <iostream>
#include <ranges>
#include <vector>

struct Node {
    int value;
    std::vector<Node> children;
};

// A recursive generator. co_yield elements_of(subgenerator) yields the whole
// subtree without a manual stack and without paying a forwarding cost at each
// level -- the standard splices the child generator straight into this one.
std::generator<int> preorder(const Node& n) {
    co_yield n.value;
    for (const Node& child : n.children)
        co_yield std::ranges::elements_of(preorder(child));
}

int main() {
    // tree:  1 has children [2, 3]; 2 has children [4, 5]
    //   preorder visits a node, then each child subtree left to right: 1 2 4 5 3
    Node tree{1, {Node{2, {Node{4, {}}, Node{5, {}}}}, Node{3, {}}}};

    std::cout << "preorder:";
    for (int v : preorder(tree))
        std::cout << " " << v;
    std::cout << "\n";
    return 0;
}
