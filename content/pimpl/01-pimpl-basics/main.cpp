#include "widget.hpp"

int main() {
    Widget w("alpha");
    w.greet();
    w.greet();
    w.rename("beta");
    w.greet();
}
