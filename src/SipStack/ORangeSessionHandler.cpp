#include <ORangeSessionHandler.hpp>

#include <resip/dum/ServerInviteSession.hxx>

#include <iostream>

void ORangeSessionHandler::onNewSession(resip::ServerInviteSessionHandle h, resip::InviteSession::OfferAnswerType oat, const resip::SipMessage& msg)
{
    std::cout << "Received new session with OfferAnswerType: " << static_cast<int>(oat) << std::endl;
    std::cout << "SIP Message: " << msg.brief() << std::endl;

    h->provisional(100);
    h->reject(486);
    // Handle new incoming session here
}