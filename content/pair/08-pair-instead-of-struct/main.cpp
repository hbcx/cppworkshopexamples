#include <iostream>
#include <string>
#include <utility>

// Anti-pattern: pair (and nested pair) used as a domain type. The members carry
// no names, so the meaning lives only in the author's head, and every access
// site has to remember what .first.second stood for.
std::pair<std::pair<double, double>, std::string> makeWaypointBad() {
    return {{52.23, 21.01}, "Warsaw"};   // is that {lat, lon} or {lon, lat}?
}

// Fix: a tiny struct with named members. Same size and cost, but every field
// says what it is -- and it still supports aggregate init and, in C++17,
// structured bindings.
struct Waypoint {
    double lat;
    double lon;
    std::string label;
};

Waypoint makeWaypointGood() {
    return {52.23, 21.01, "Warsaw"};
}

int main() {
    std::pair<std::pair<double, double>, std::string> bad = makeWaypointBad();
    // What is being printed? You have to go read makeWaypointBad to be sure.
    std::cout << bad.first.second << ", " << bad.first.first
              << " (" << bad.second << ")\n";

    Waypoint good = makeWaypointGood();
    // Self-explanatory at the point of use.
    std::cout << good.lon << ", " << good.lat << " (" << good.label << ")\n";
    return 0;
}
