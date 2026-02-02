/***************************************************
 * ORangeAppDialogSet.cpp
 * Created on Sat, 31 Jan 2026 08:30:44 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "ORangeAppDialogSet.hpp"
#include "SipStack.hpp"
#include <S2B/SignalCommand.hpp>

ORangeAppDialog::ORangeAppDialog(SipStack* s,
                                 resip::HandleManager& hm,
                                 const CompletionToken& sid)
  : resip::AppDialog(hm)
  , m_sessionId(sid)
  , m_sipStack(s)
{
  m_sipStack->addDialog(this);
}

ORangeAppDialog::~ORangeAppDialog()
{
  m_sipStack->removeDialog(this);
}

ORangeAppDialogSet::ORangeAppDialogSet(SipStack* s,
                                       resip::DialogUsageManager& dum,
                                       const CompletionToken& sid)
  : resip::AppDialogSet(dum)
  , m_sessionId(sid)
  , m_sipStack(s)
  , m_dum(dum)
{
}

ORangeAppDialogSet::ORangeAppDialogSet(SipStack* s,
                                       resip::DialogUsageManager& dum)
  : resip::AppDialogSet(dum)
  , m_sessionId(std::make_shared<CompletionTokenTag>())
  , m_sipStack(s)
  , m_dum(dum)
{
}

ORangeAppDialogSet::~ORangeAppDialogSet() {}

resip::AppDialog* ORangeAppDialogSet::createAppDialog(
  const resip::SipMessage& msg)
{
  return new ORangeAppDialog(m_sipStack, m_dum, m_sessionId);
}
