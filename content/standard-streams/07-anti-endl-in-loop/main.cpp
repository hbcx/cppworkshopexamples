#include <iostream>

int main() {
    // Anti-pattern: std::endl on every line of output. endl flushes the stream
    // each time, and to a file or pipe that is a physical write per line, which
    // can be dramatically slower than buffering and flushing once at the end.
    //
    //   for (...) std::cout << data << std::endl;   // flushes every iteration
    //
    // Fix: write '\n' and let the stream flush when its buffer fills or at
    // program exit. Flush explicitly only where you truly need output out now.
    for (int i = 0; i < 5; ++i)
        std::cout << "row " << i << '\n';   // buffered
    std::cout << std::flush;                // one explicit flush, if needed at all
    std::cout << "wrote 5 rows with a single flush\n";
    return 0;
}
