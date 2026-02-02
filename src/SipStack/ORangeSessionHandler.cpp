#include <ORangeSessionHandler.hpp>
#include <S2B/CommandBus.hpp>
#include <S2B/SignalCommand.hpp>
#include <SipStack.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <resip/dum/ClientInviteSession.hxx>
#include <resip/dum/ServerInviteSession.hxx>

#include "Formatters.hpp"
#include "Logger/Logger.hpp"

void ORangeSessionHandler::onNewSession(
  resip::ServerInviteSessionHandle h,
  resip::InviteSession::OfferAnswerType oat,
  const resip::SipMessage& msg)
{
  auto dialogSetHandle = h->getAppDialogSet();
  auto orangeAppDialogSet =
    dynamic_cast<ORangeAppDialogSet*>(dialogSetHandle.get());

  auto cmd = std::make_unique<SignalCommand>(SignalEventType::CREATED);
  cmd->setCompletionToken(orangeAppDialogSet->getSessionToken());
  const resip::NameAddr& from = msg.header(resip::h_From);
  const resip::NameAddr& to = msg.header(resip::h_To);

  cmd->source() = from.uri().getAorNoReally().c_str();
  cmd->destination() = to.uri().getAorNoReally().c_str();
  CommandBus::instance().publish(std::move(cmd));
}

void ORangeSessionHandler::onNewSession(
  resip::ClientInviteSessionHandle h,
  resip::InviteSession::OfferAnswerType oat,
  const resip::SipMessage& msg)
{
  LOG_INFO("Starting outgoing Session").show();
}

void ORangeSessionHandler::onProvisional(resip::ClientInviteSessionHandle h,
                                         const resip::SipMessage& msg)
{
  LOG_INFO("Got provisional {}", msg).show();
  auto dialogSetHandle = h->getAppDialogSet();
  auto orangeAppDialogSet =
    dynamic_cast<ORangeAppDialogSet*>(dialogSetHandle.get());

  auto statusLine = msg.header(resip::h_StatusLine);

  SignalEventType type = SignalEventType::INVALID;

  switch (statusLine.statusCode()) {
  case 180:
    type = SignalEventType::RINGING;
    break;
  }

  if (type != SignalEventType::INVALID) {
    auto cmd = std::make_unique<SignalCommand>(type);
    cmd->setCompletionToken(orangeAppDialogSet->getSessionToken());
    CommandBus::instance().publish(std::move(cmd));
  }
}

void ORangeSessionHandler::onOfferRequired(resip::InviteSessionHandle h,
                                           const resip::SipMessage& msg)
{
  LOG_INFO("Offer required for dialog ").show();
}

void ORangeSessionHandler::onTerminated(
  resip::InviteSessionHandle h,
  InviteSessionHandler::TerminatedReason reason,
  const resip::SipMessage* related)
{
  LOG_INFO("Session terminated ").show();
}
void ORangeSessionHandler::onFailure(resip::ClientInviteSessionHandle h,
                                     const resip::SipMessage& msg)
{
  LOG_INFO("Session rejected").show();
  auto dialogSetHandle = h->getAppDialogSet();
  auto orangeAppDialogSet =
    dynamic_cast<ORangeAppDialogSet*>(dialogSetHandle.get());

  auto statusLine = msg.header(resip::h_StatusLine);

  SignalEventType type = SignalEventType::REJECTED;
  SignalCommandRejectReason reason = SignalCommandRejectReason::INTERNAL_ERROR;

  switch (statusLine.statusCode()) {
  case 486:
    reason = SignalCommandRejectReason::BUSY;
    break;
  }

  auto cmd = std::make_unique<SignalCommand>(type);
  cmd->rejectReason() = reason;
  cmd->setCompletionToken(orangeAppDialogSet->getSessionToken());
  CommandBus::instance().publish(std::move(cmd));
}
