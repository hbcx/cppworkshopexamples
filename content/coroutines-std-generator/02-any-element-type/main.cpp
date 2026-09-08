#include <generator>
#include <iostream>
#include <string>

std::generator<std::string> words() {
    co_yield "alpha";
    co_yield "beta";
    co_yield "gamma";
}

struct Point {
    int x;
    int y;
};

std::generator<Point> corners() {
    co_yield Point{0, 0};
    co_yield Point{1, 0};
    co_yield Point{1, 1};
}

int main() {
    std::cout << "words:";
    for (const std::string& w : words())
        std::cout << " " << w;
    std::cout << "\n";

    std::cout << "corners:";
    for (const Point& p : corners())
        std::cout << " (" << p.x << "," << p.y << ")";
    std::cout << "\n";
    return 0;
}
