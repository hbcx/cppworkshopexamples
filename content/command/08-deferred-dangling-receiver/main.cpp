#include <functional>
#include <iostream>
#include <memory>
#include <vector>

class Worker {
public:
    void doWork() { std::cout << "worker did the job\n"; }
};

// ANTI-PATTERN (described, not run): a command captured now and run LATER holds a
// reference to a receiver destroyed before the queue drains. Executing it then
// touches a dead object -- undefined behaviour. It works while the receiver
// happens to outlive the queue and fails once it does not.
//
//   std::vector<std::function<void()>> queue;
//   {
//       Worker w;
//       queue.push_back([&w] { w.doWork(); });   // captures &w by reference
//   }                                            // w is destroyed here
//   for (auto& cmd : queue) cmd();               // runs over a dead Worker -- UB
//
// FIX: give the deferred command SHARED OWNERSHIP of its receiver, so the receiver
// lives as long as any queued command that still needs it.
int main() {
    std::vector<std::function<void()>> queue;
    {
        auto w = std::make_shared<Worker>();
        queue.push_back([w] { w->doWork(); });   // captures the shared_ptr BY VALUE
    }   // the local handle drops, but the queued command still owns the Worker

    std::cout << "running the queue after the creating scope ended:\n";
    for (auto& cmd : queue) cmd();               // safe: the Worker is still alive
    return 0;
}
