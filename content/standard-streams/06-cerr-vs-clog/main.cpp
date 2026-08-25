#include <iostream>
#include <sstream>

int main() {
    // Both cerr and clog write to stderr, but they buffer differently:
    //   - std::cerr has unitbuf SET, so it flushes after every operation --
    //     the message is out immediately, even if the program crashes next.
    //   - std::clog is buffered like cout, better for high-volume diagnostic
    //     logging where you do not need each line out instantly.
    bool cerrUnbuffered = (std::cerr.flags() & std::ios::unitbuf) != 0;
    bool clogBuffered   = (std::clog.flags() & std::ios::unitbuf) == 0;
    std::cout << "cerr flushes each op (unitbuf): " << cerrUnbuffered << '\n';   // 1
    std::cout << "clog is buffered:               " << clogBuffered << '\n';     // 1

    // Use cerr for errors that must appear even on a crash; clog for verbose
    // logs. Both stay off stdout so they do not pollute the program's data.
    // (Captured here so the text shows in the stdout the harness records.)
    std::ostringstream cap;
    std::streambuf* old = std::clog.rdbuf(cap.rdbuf());
    std::clog << "diagnostic: started\n";
    std::clog.rdbuf(old);
    std::cout << "captured clog: " << cap.str();
    return 0;
}
