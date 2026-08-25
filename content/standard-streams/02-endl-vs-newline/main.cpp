#include <iostream>

int main() {
    // '\n' writes a newline and lets the stream buffer. std::endl writes a
    // newline AND flushes the buffer to the destination. Same visible text, but
    // endl forces a physical write every time.
    std::cout << "with newline\n";           // buffered
    std::cout << "with endl" << std::endl;   // newline + flush

    // The flush is the cost: in a loop that writes many lines, endl forces one
    // write per line, far slower than buffering and flushing once. Prefer '\n';
    // reach for endl or std::flush only when you truly need output out now.
    for (int i = 0; i < 3; ++i)
        std::cout << "line " << i << '\n';   // buffered, flushed at program exit
    std::cout << "done\n";
    return 0;
}
