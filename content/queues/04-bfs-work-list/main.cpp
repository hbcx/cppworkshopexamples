#include <iostream>
#include <queue>
#include <vector>

// A small unweighted graph:
//
//   0 --- 1 --- 3 --- 5
//   |     |
//   2 --- 4
//
static const std::vector<std::vector<int>> graph = {
    {1, 2},      // 0
    {0, 3},      // 1
    {0, 4},      // 2
    {1, 5},      // 3
    {2},         // 4
    {3}          // 5
};

int main() {
    const int start = 0;
    std::vector<int> distance(graph.size(), -1);   // -1 = not reached yet

    std::queue<int> pending;                       // the work list
    distance[start] = 0;
    pending.push(start);

    std::cout << "visit order:";
    while (!pending.empty()) {                     // the queue shrinks AND grows here
        const int node = pending.front();
        pending.pop();
        std::cout << ' ' << node;

        for (int next : graph[node]) {
            if (distance[next] == -1) {            // mark on PUSH, not on pop --
                distance[next] = distance[node] + 1;   // otherwise a node with two
                pending.push(next);                    // incoming edges is queued twice
            }
        }
    }
    std::cout << '\n';

    // FIFO means nodes come out level by level, so the first time we reach a node
    // is along a shortest path.
    for (std::size_t i = 0; i < distance.size(); ++i) {
        std::cout << "hops 0 -> " << i << " = " << distance[i] << '\n';
    }
    return 0;
}
