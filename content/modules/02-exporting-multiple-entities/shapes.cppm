export module shapes;

// Export a type, a function, and a constant individually.
export struct Point {
    int x;
    int y;
};

export int manhattan(Point a, Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy);
}

export inline constexpr int dimensions = 2;

// An export BLOCK exports every declaration inside it at once.
export {
    int originDistance(Point p) { return manhattan(p, Point{0, 0}); }
    const char* library_name() { return "shapes"; }
}
