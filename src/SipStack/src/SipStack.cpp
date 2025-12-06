#include <SipStack.hpp>

#include <resip/dum/InviteSessionHandler.hxx>
#include <resip/dum/DialogUsageManager.hxx>
#include <resip/stack/SipStack.hxx>

void SipStack::createAndInitInstance()
{
    instance = new SipStack();

    instance->stack = std::make_shared<resip::SipStack>();
}

SipStack* SipStack::getInstance()
{
    if(instance == nullptr) {
        throw std::runtime_error("SipStack instance is not created");
    }

    return instance;
}

SipStack* SipStack::instance = nullptr;