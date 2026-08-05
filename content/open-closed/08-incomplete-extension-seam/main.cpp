// Anti-pattern: an incomplete extension seam.
//
// Widget has a render() seam, but the layout needs each widget's height and the
// interface does not provide it -- so totalHeight() dynamic_casts to every
// concrete type. The seam is incomplete, so adding a widget STILL edits that
// function, and a forgotten case is silently sized as zero.
//
// The fix completes the interface with a virtual height(); the layout becomes a
// closed loop and a new widget touches nothing.

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ---------- the interface is missing height() ----------
namespace bad {

class Widget {
public:
    virtual ~Widget() {}
    virtual std::string render() const = 0;
};

class Button : public Widget {
public:
    std::string render() const override { return "[Button]"; }
};
class Label : public Widget {
public:
    std::string render() const override { return "Label"; }
};

// The seam leaks: the layout must know every concrete type to size it.
int totalHeight(const std::vector<Widget*>& ws) {
    int h = 0;
    for (Widget* w : ws) {
        if (dynamic_cast<const Button*>(w)) h += 30;
        else if (dynamic_cast<const Label*>(w)) h += 20;
        // a new widget type needs a new branch RIGHT HERE, or it counts as zero
    }
    return h;
}

} // namespace bad

// ---------- height() is part of the interface ----------
namespace good {

class Widget {
public:
    virtual ~Widget() {}
    virtual std::string render() const = 0;
    virtual int height() const = 0; // the seam now covers what the layout varies on
};

class Button : public Widget {
public:
    std::string render() const override { return "[Button]"; }
    int height() const override { return 30; }
};
class Label : public Widget {
public:
    std::string render() const override { return "Label"; }
    int height() const override { return 20; }
};
class Slider : public Widget { // new type: totalHeight below is untouched
public:
    std::string render() const override { return "---o---"; }
    int height() const override { return 15; }
};

int totalHeight(const std::vector<std::unique_ptr<Widget> >& ws) {
    int h = 0;
    for (const std::unique_ptr<Widget>& w : ws) h += w->height();
    return h;
}

} // namespace good

int main() {
    bad::Button bb;
    bad::Label bl;
    std::vector<bad::Widget*> badWs;
    badWs.push_back(&bb);
    badWs.push_back(&bl);
    std::cout << "[bad]  totalHeight=" << bad::totalHeight(badWs) << "\n";

    std::vector<std::unique_ptr<good::Widget> > ws;
    ws.push_back(std::unique_ptr<good::Widget>(new good::Button));
    ws.push_back(std::unique_ptr<good::Widget>(new good::Label));
    ws.push_back(std::unique_ptr<good::Widget>(new good::Slider)); // the new type
    std::cout << "[good] totalHeight=" << good::totalHeight(ws) << "\n";
    for (const std::unique_ptr<good::Widget>& w : ws)
        std::cout << "  " << w->render() << " h=" << w->height() << "\n";
}
