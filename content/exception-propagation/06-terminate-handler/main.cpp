// std::terminate is the runtime's last resort -- it is called for an uncaught
// exception, an exception escaping a noexcept function, or a throw during
// unwinding. std::set_terminate installs your handler (to log, flush, write a
// crash marker) before the program ends. A terminate handler must NOT return; it
// ends the program. Here it prints and calls std::exit(0) so the demo finishes
// cleanly instead of aborting.
#include <iostream>
#include <exception>
#include <cstdlib>

void on_terminate() {
    std::cout << "custom terminate handler: last-resort cleanup\n";
    std::exit(0);                 // end the program; a handler must not return
}

int main() {
    std::terminate_handler previous = std::set_terminate(on_terminate);
    (void)previous;               // set_terminate returns the old handler

    std::cout << "about to call std::terminate\n";
    std::terminate();             // triggers our handler; the program ends there

    return 0;                     // not reached
}
