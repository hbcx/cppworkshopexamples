#include <iostream>

int main() {
    // Anti-pattern: debugging a crash with std::cout prints. cout is buffered, so
    // if the program aborts (segfault, std::abort, an uncaught exception calling
    // terminate) before the buffer is flushed, the LAST prints -- often the ones
    // right before the crash, the ones you most need -- are lost, and it looks
    // like the code never reached them.
    //
    //   std::cout << "about to touch the pointer\n";   // buffered, may be lost
    //   crash();                                        // buffer never flushed
    //
    // Fix: send debug traces to std::cerr (unbuffered -- each line is out before
    // the next statement runs), or flush cout explicitly with std::flush / endl
    // right before the risky operation.
    std::cerr << "trace via cerr: out immediately, survives a crash\n";  // to stderr
    std::cout << "trace via cout, flushed now: " << std::flush;
    std::cout << "reached the checkpoint\n";
    return 0;
}
