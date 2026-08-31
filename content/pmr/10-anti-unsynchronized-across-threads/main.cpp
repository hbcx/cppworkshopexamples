// Anti-pattern: sharing an unsynchronized_pool_resource (or a monotonic one)
// between threads.
//
// These resources do no locking. Two threads allocating from one instance race
// on its free lists -- fine in a light test, corruption under load. Use a
// synchronized_pool_resource (locks internally) or one resource per thread.

#include <iostream>
#include <memory_resource>
#include <vector>

int main() {
    // BAD (data race -- described, not run):
    //   std::pmr::unsynchronized_pool_resource shared{...};
    //   // thread A: std::pmr::vector<int> va{&shared}; va.push_back(...);
    //   // thread B: std::pmr::vector<int> vb{&shared}; vb.push_back(...);
    //   // both touch shared's free lists with no lock -> race, corruption

    // GOOD: synchronized_pool_resource locks internally, so many threads may
    // share one instance safely. (Run single-threaded here to stay
    // deterministic; the point is the type choice.)
    std::pmr::synchronized_pool_resource shared{std::pmr::new_delete_resource()};

    std::pmr::vector<int> v{&shared};
    for (int i = 0; i < 8; ++i) v.push_back(i);

    std::cout << "synchronized_pool_resource is safe to share across threads\n";
    std::cout << "vector built on it holds " << v.size() << " elements\n";
}
