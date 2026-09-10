// A module INTERFACE unit. `export module math;` names the module; anything
// marked `export` becomes visible to code that imports it, and nothing else does.
export module math;

export int square(int x) {
    return x * x;
}

export int cube(int x) {
    return x * x * x;
}
