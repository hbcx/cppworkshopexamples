#include <cstddef>
#include <cstdio>    // std::snprintf
#include <iostream>

// A plain, trivial struct with a C-compatible layout. This can cross a C boundary
// by pointer. std::string or std::vector cannot: C has no way to construct or
// destroy them, and there is no shared ABI for passing them by value.
struct Point {
    int x;
    int y;
};

// A C-style API: a pointer to the (trivial) struct, and a caller-provided buffer
// to fill C-string style. It reports success through a return code.
extern "C" int point_label(const Point* p, char* buf, std::size_t bufsize) {
    if (p == nullptr || buf == nullptr || bufsize < 16)
        return 1;
    int written = std::snprintf(buf, bufsize, "(%d,%d)", p->x, p->y);
    return (written < 0) ? 1 : 0;
}

int main() {
    Point p{3, 4};
    char label[32];
    if (point_label(&p, label, sizeof(label)) == 0)
        std::cout << "label = " << label << "\n";
    return 0;
}
