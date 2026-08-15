#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

int main() {
    // ANTI-PATTERN: a detached thread that keeps using data from the scope that
    // launched it. detach() lets the thread outlive that scope, so any local it
    // captured BY REFERENCE can be destroyed while the thread is still running -- a
    // use-after-free that often "works" in a quick test and corrupts memory later.
    //
    //   void launch() {
    //       std::string msg = "hello";
    //       std::thread t([&msg]{ slow(); use(msg); });  // captures msg by REFERENCE
    //       t.detach();
    //   }   // msg destroyed here; t may still run and read freed memory -> UB
    //
    // We do NOT run that version -- undefined behaviour is nondeterministic and can
    // crash. Below are two safe ways to launch fire-and-forget work.

    std::atomic<bool> done{false};

    // FIX 1: give the thread its OWN copy, so it depends on nothing on this stack.
    {
        std::string msg = "hello";
        std::thread t([msg, &done] {              // msg captured BY VALUE (copied in)
            std::cout << "copy:       " << msg << '\n';
            done.store(true);
        });
        t.detach();
        while (!done.load()) std::this_thread::yield();   // wait before msg dies
    }

    // FIX 2: keep the shared data alive past the scope with a shared_ptr; the thread
    // holds a copy of the pointer, so the payload lives as long as the thread does.
    done.store(false);
    {
        auto payload = std::make_shared<std::string>("world");
        std::thread t([payload, &done] {          // shares ownership of the string
            std::cout << "shared_ptr: " << *payload << '\n';
            done.store(true);
        });
        t.detach();
        while (!done.load()) std::this_thread::yield();
    }

    // The rule: a detached thread must only touch data guaranteed to outlive it --
    // its own copies, shared ownership, or true globals. If you cannot guarantee
    // that, do not detach; join instead, or use std::jthread.
    return 0;
}
