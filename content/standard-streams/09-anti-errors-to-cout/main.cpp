#include <iostream>
#include <sstream>

int main() {
    // Anti-pattern: writing errors, warnings or logs to std::cout. They land in
    // the program's DATA stream, so `prog > result.txt` captures your error text
    // into the result file, and a downstream pipe (`prog | parser`) is fed
    // diagnostics it will choke on.
    //
    // Imagine the program's real output is a single number, but a warning was
    // mistakenly sent to cout too. We capture cout to stand in for `> file`.
    std::ostringstream stdoutCapture;
    std::streambuf* old = std::cout.rdbuf(stdoutCapture.rdbuf());

    std::cout << "WARNING: using defaults\n";   // WRONG: goes into the data
    std::cout << "42\n";                         // the real result

    std::cout.rdbuf(old);   // restore the real stdout

    std::cout << "what a `> file` redirection would capture:\n" << stdoutCapture.str();
    std::cout << "-- the warning polluted the data. Send diagnostics to std::cerr.\n";
    return 0;
}
