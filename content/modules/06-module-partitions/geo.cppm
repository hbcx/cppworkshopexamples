// The PRIMARY interface unit of module geo. It pulls in the partition and
// re-exports it, so importers of geo see the partition's names too.
export module geo;

export import :trig;   // bring geo:trig into geo and re-export it

export double rectangleArea(double w, double h) {
    return w * h;
}
