#pragma once

#include <memory>

namespace resip {
    class SipStack;
}

class SipStack {
public:
    static void createAndInitInstance();
    static SipStack* getInstance();
private:
    static SipStack* instance;
    std::shared_ptr<resip::SipStack> stack;
};