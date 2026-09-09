#include <iostream>

// A namespace groups related declarations under a name.
namespace geometry {
    int area(int w, int h) { return w * h; }
}

// A namespace can be reopened and added to across several declarations -- the
// standard library does exactly this across its headers.
namespace geometry {
    int perimeter(int w, int h) { return 2 * (w + h); }
}

// C++17 compact form for a nested namespace (same as namespace app { namespace ui {...} }).
namespace app::ui {
    const char* title() { return "Editor"; }
}

int main() {
    // A qualified name reaches into the namespace with ::.
    std::cout << "area = " << geometry::area(3, 4) << "\n";
    std::cout << "perimeter = " << geometry::perimeter(3, 4) << "\n";
    std::cout << "title = " << app::ui::title() << "\n";
    return 0;
}
