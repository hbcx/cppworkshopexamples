#include <atomic>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

// compare_exchange lets you build lock-free data structures whose nodes are
// shared_ptr -- and shared_ptr solves the hard part of lock-free structures, safe
// reclamation: a node is freed only when the last thread holding it lets go, so no
// thread ever reads a node another thread has already deleted. Here many threads push
// onto a singly linked stack with the classic compare-exchange loop: read the head,
// point the new node at it, and swap the new node in as the head only if the head has
// not changed in the meantime.
struct Node {
    int value;
    std::shared_ptr<Node> next;
};

int main() {
    std::atomic<std::shared_ptr<Node>> head{nullptr};

    const int THREADS = 8;
    const int PER_THREAD = 100;
    std::vector<std::jthread> workers;
    for (int t = 0; t < THREADS; ++t) {
        workers.emplace_back([&] {
            for (int k = 0; k < PER_THREAD; ++k) {
                auto node = std::make_shared<Node>();
                node->value = 1;
                node->next = head.load();
                // Install node as the new head; if another thread changed head first,
                // compare_exchange_weak refreshes node->next and we retry.
                while (!head.compare_exchange_weak(node->next, node)) {
                }
            }
        });
    }
    workers.clear();                    // join all pushers

    // Walk the finished stack and count the nodes that made it on.
    long count = 0;
    for (std::shared_ptr<Node> n = head.load(); n; n = n->next) count += n->value;
    std::cout << "pushed nodes on the stack = " << count << '\n';   // 800
    return 0;
}
