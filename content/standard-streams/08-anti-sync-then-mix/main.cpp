#include <iostream>

int main() {
    // Anti-pattern: calling sync_with_stdio(false) and then still mixing C and
    // C++ I/O. With sync off, printf and cout have SEPARATE buffers that flush
    // independently, so their output can appear out of order relative to how the
    // code wrote it.
    //
    //   std::ios_base::sync_with_stdio(false);
    //   std::printf("A ");     // C buffer
    //   std::cout << "B ";     // C++ buffer  -> "B" may print before "A"
    //
    // We do NOT mix them here (the resulting order is unspecified). The rule:
    // once you disable sync, use ONE I/O system -- pick C++ streams -- and do not
    // call printf/scanf again. Also call sync_with_stdio(false) BEFORE any I/O.
    std::ios_base::sync_with_stdio(false);
    std::cout << "sync disabled; using only C++ streams from here\n";
    std::cout << "no printf mixed in, so the order is well-defined\n";
    return 0;
}
