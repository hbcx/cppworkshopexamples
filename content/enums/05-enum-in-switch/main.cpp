#include <iostream>

// A switch over an enum is the natural way to handle each case, and the compiler
// helps: with -Wall (-Wswitch) it warns when a scoped enum's switch is missing a
// case and has no default. That turns "did I handle every state?" into a build
// check -- as long as you do NOT add a catch-all default (see the anti-pattern).

enum class Direction { North, East, South, West };

const char* name(Direction d) {
    switch (d) {
        case Direction::North: return "North";
        case Direction::East:  return "East";
        case Direction::South: return "South";
        case Direction::West:  return "West";
    }
    return "unknown";   // for a value cast in from outside the enum
}

int main() {
    for (Direction d : {Direction::North, Direction::East,
                        Direction::South, Direction::West}) {
        std::cout << static_cast<int>(d) << " = " << name(d) << "\n";
    }
    // If a fifth direction were added to the enum, the switch above would draw a
    // -Wswitch warning because it no longer covers every enumerator.
    return 0;
}
