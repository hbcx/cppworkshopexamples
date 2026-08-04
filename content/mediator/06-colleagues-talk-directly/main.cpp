// Anti-pattern: colleagues that still talk to each other directly.
//
// A mediator only helps if the colleagues actually route through it. When they
// keep direct references to one another "for convenience", you get the worst of
// both worlds: the mesh of coupling the mediator was meant to remove, AND the
// mediator. Here two fields hold pointers to each other and to the button, and
// each duplicates the enable rule. The fix is the mediator form: fields know
// only the mediator, and the rule lives once. The output is identical; the
// coupling is not.

#include <iostream>
#include <string>

class Button {
    bool enabled_ = false;
public:
    void setEnabled(bool e) {
        if (e != enabled_) {
            enabled_ = e;
            std::cout << "  submit " << (e ? "enabled" : "disabled") << "\n";
        }
    }
};

// ---- BAD: each field references the OTHER field and the button ----
class FieldBad {
    std::string text_;
    FieldBad* other_ = nullptr; // must know the other field...
    Button* button_ = nullptr;  // ...and the button
public:
    void wire(FieldBad* other, Button* button) { other_ = other; button_ = button; }
    bool filled() const { return !text_.empty(); }
    void setText(const std::string& t) {
        text_ = t;
        // the rule is duplicated in every field that can change:
        button_->setEnabled(filled() && other_->filled());
    }
};

// ---- GOOD: fields know only the mediator; the rule lives once ----
class FormMediator;

class FieldGood {
    std::string text_;
    FormMediator* mediator_ = nullptr;
public:
    void setMediator(FormMediator* m) { mediator_ = m; }
    bool filled() const { return !text_.empty(); }
    void setText(const std::string& t);
};

class FormMediator {
    FieldGood& user_;
    FieldGood& pass_;
    Button& submit_;
public:
    FormMediator(FieldGood& u, FieldGood& p, Button& s) : user_(u), pass_(p), submit_(s) {}
    void changed() { submit_.setEnabled(user_.filled() && pass_.filled()); } // one place
};

void FieldGood::setText(const std::string& t) {
    text_ = t;
    if (mediator_) mediator_->changed();
}

int main() {
    std::cout << "-- bad (mesh) --\n";
    Button b1;
    FieldBad user1, pass1;
    user1.wire(&pass1, &b1);
    pass1.wire(&user1, &b1);
    user1.setText("alice");
    pass1.setText("secret");
    pass1.setText("");

    std::cout << "-- good (hub) --\n";
    Button b2;
    FieldGood user2, pass2;
    FormMediator form(user2, pass2, b2);
    user2.setMediator(&form);
    pass2.setMediator(&form);
    user2.setText("alice");
    pass2.setText("secret");
    pass2.setText("");
    return 0;
}
