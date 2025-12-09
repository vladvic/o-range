#pragma once

#include <resip/dum/InviteSessionHandler.hxx>

class ORangeSessionHandler : public resip::InviteSessionHandler {
 public:
  ORangeSessionHandler() = default;
  ~ORangeSessionHandler() override = default;

  void onNewSession(resip::ServerInviteSessionHandle,
                    resip::InviteSession::OfferAnswerType oat,
                    const resip::SipMessage& msg) override;

  void onNewSession(resip::ClientInviteSessionHandle,
                    resip::InviteSession::OfferAnswerType oat,
                    const resip::SipMessage& msg) override {}
  void onFailure(resip::ClientInviteSessionHandle,
                 const resip::SipMessage& msg) override {};
  void onEarlyMedia(resip::ClientInviteSessionHandle, const resip::SipMessage&,
                    const resip::SdpContents&) override {}
  void onEarlyMedia(resip::ClientInviteSessionHandle, const resip::SipMessage&,
                    const resip::Contents&) override {}
  void onProvisional(resip::ClientInviteSessionHandle,
                     const resip::SipMessage&) override {}
  void onConnected(resip::ClientInviteSessionHandle,
                   const resip::SipMessage& msg) override {}
  void onConnected(resip::InviteSessionHandle,
                   const resip::SipMessage& msg) override {}
  void onTerminated(resip::InviteSessionHandle,
                    InviteSessionHandler::TerminatedReason reason,
                    const resip::SipMessage* related = 0) override {}
  void onForkDestroyed(resip::ClientInviteSessionHandle) override {}
  void onRedirected(resip::ClientInviteSessionHandle,
                    const resip::SipMessage& msg) override {}
  void onAnswer(resip::InviteSessionHandle, const resip::SipMessage& msg,
                const resip::SdpContents&) override {}
  void onOffer(resip::InviteSessionHandle, const resip::SipMessage& msg,
               const resip::SdpContents&) override {}
  void onOfferRequired(resip::InviteSessionHandle,
                       const resip::SipMessage& msg) override {}
  void onOfferRejected(resip::InviteSessionHandle,
                       const resip::SipMessage* msg) override {}
  void onInfo(resip::InviteSessionHandle,
              const resip::SipMessage& msg) override {}
  void onInfoSuccess(resip::InviteSessionHandle,
                     const resip::SipMessage& msg) override {}
  void onInfoFailure(resip::InviteSessionHandle,
                     const resip::SipMessage& msg) override {}
  void onMessage(resip::InviteSessionHandle,
                 const resip::SipMessage& msg) override {}
  void onMessageSuccess(resip::InviteSessionHandle,
                        const resip::SipMessage& msg) override {}
  void onMessageFailure(resip::InviteSessionHandle,
                        const resip::SipMessage& msg) override {}
  void onRefer(resip::InviteSessionHandle, resip::ServerSubscriptionHandle,
               const resip::SipMessage& msg) override {}
  void onReferNoSub(resip::InviteSessionHandle,
                    const resip::SipMessage& msg) override {}
  void onReferRejected(resip::InviteSessionHandle,
                       const resip::SipMessage& msg) override {}
  void onReferAccepted(resip::InviteSessionHandle,
                       resip::ClientSubscriptionHandle,
                       const resip::SipMessage& msg) override {}

};