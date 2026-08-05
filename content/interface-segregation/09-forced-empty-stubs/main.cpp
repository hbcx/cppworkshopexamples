// Anti-pattern: partial implementers forced to stub the rest.
//
// A fat WidgetListener declares a callback per event. FullPanel wants them all,
// but ClickCounter cares only about clicks and ScrollTracker only about
// scrolls -- yet each must write empty stubs for every other callback just to
// compile. The stubs are dead noise, and adding one event to the base breaks
// EVERY listener until each is stubbed again.
//
// The fix: one small listener interface per event; a listener implements only
// the ones it wants, and a new event type touches no one else.

#include <iostream>
#include <vector>

// ---------- one fat listener: every implementer defines every callback ----------
namespace bad {

class WidgetListener {
public:
    virtual ~WidgetListener() {}
    virtual void onClick() = 0;
    virtual void onScroll() = 0;
    virtual void onKey() = 0;
    // Add onResize() here and BOTH partial listeners below stop compiling.
};

class FullPanel : public WidgetListener {
public:
    void onClick() override { std::cout << "[bad] panel: click\n"; }
    void onScroll() override { std::cout << "[bad] panel: scroll\n"; }
    void onKey() override { std::cout << "[bad] panel: key\n"; }
};

// Cares only about clicks -- but must stub onScroll and onKey to compile.
class ClickCounter : public WidgetListener {
public:
    void onClick() override { ++clicks_; }
    void onScroll() override {}  // dead stub
    void onKey() override {}     // dead stub
    int clicks() const { return clicks_; }
private:
    int clicks_ = 0;
};

// Cares only about scrolls -- the other two are dead stubs again.
class ScrollTracker : public WidgetListener {
public:
    void onClick() override {}   // dead stub
    void onScroll() override { ++scrolls_; }
    void onKey() override {}      // dead stub
    int scrolls() const { return scrolls_; }
private:
    int scrolls_ = 0;
};

} // namespace bad

// ---------- one small interface per event; implement only what you want ------
namespace good {

class ClickListener {
public:
    virtual ~ClickListener() {}
    virtual void onClick() = 0;
};

class ScrollListener {
public:
    virtual ~ScrollListener() {}
    virtual void onScroll() = 0;
};

// The widget keeps a list per event and notifies only the interested listeners.
class Widget {
public:
    void addClickListener(ClickListener* l) { clickers_.push_back(l); }
    void addScrollListener(ScrollListener* l) { scrollers_.push_back(l); }
    void click() { for (ClickListener* l : clickers_) l->onClick(); }
    void scroll() { for (ScrollListener* l : scrollers_) l->onScroll(); }
private:
    std::vector<ClickListener*> clickers_;
    std::vector<ScrollListener*> scrollers_;
};

// Each listener implements ONLY the event it cares about -- no stubs.
class ClickCounter : public ClickListener {
public:
    void onClick() override { ++clicks_; }
    int clicks() const { return clicks_; }
private:
    int clicks_ = 0;
};

class ScrollTracker : public ScrollListener {
public:
    void onScroll() override { ++scrolls_; }
    int scrolls() const { return scrolls_; }
private:
    int scrolls_ = 0;
};

} // namespace good

int main() {
    // Bad: every listener carries stubs for events it ignores.
    bad::FullPanel panel;
    panel.onClick();
    bad::ClickCounter badClicks;
    bad::ScrollTracker badScrolls;
    badClicks.onClick();
    badClicks.onClick();
    badClicks.onScroll(); // stub: silently does nothing, no type error
    badScrolls.onScroll();
    std::cout << "[bad] clicks=" << badClicks.clicks()
              << " scrolls=" << badScrolls.scrolls() << "\n";

    // Good: each listener implements only its event; the Widget routes events.
    good::Widget w;
    good::ClickCounter clicks;
    good::ScrollTracker scrolls;
    w.addClickListener(&clicks);
    w.addScrollListener(&scrolls);
    w.click();
    w.click();
    w.click();
    w.scroll();
    w.scroll();
    std::cout << "[good] clicks=" << clicks.clicks()
              << " scrolls=" << scrolls.scrolls() << "\n";
}
