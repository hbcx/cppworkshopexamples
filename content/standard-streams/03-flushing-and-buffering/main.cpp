#include <iostream>

int main() {
    // cout buffers its output. When it is flushed depends on the destination:
    //   - to a TERMINAL it is typically line-buffered (flushes on newline),
    //   - to a FILE or PIPE it is fully buffered (flushes when the buffer fills
    //     or the program ends), so partial output may not appear until then.
    // std::flush forces the buffer out now, without writing a newline.
    std::cout << "computing..." << std::flush;   // appears before the work below
    // ... imagine a long computation here ...
    std::cout << " done\n";

    // std::unitbuf makes the stream flush after EVERY output operation (this is
    // how std::cerr behaves). Handy for debugging, expensive for throughput.
    std::cout << std::unitbuf;
    std::cout << "each";
    std::cout << " op";
    std::cout << " flushed\n";
    std::cout << std::nounitbuf;   // turn it back off
    return 0;
}
