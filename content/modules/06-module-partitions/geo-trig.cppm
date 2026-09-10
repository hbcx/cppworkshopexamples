// A module PARTITION: part of module geo, named geo:trig. Partitions let a large
// module be written across several files while still presenting one name to users.
export module geo:trig;

export double triangleArea(double base, double height) {
    return 0.5 * base * height;
}
