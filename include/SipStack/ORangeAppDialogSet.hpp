/***************************************************
 * OrangeAppDialogSet.hpp
 * Created on Sat, 31 Jan 2026 06:08:58 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <S2B/Command.hpp>
#include <resip/dum/AppDialog.hxx>
#include <resip/dum/AppDialogSet.hxx>
#include <resip/dum/AppDialogSetFactory.hxx>
#include <resip/dum/DialogUsageManager.hxx>

class SipStack;

class ORangeAppDialog : public resip::AppDialog
{
  CompletionToken m_sessionId;
  SipStack* m_sipStack;

public:
  ORangeAppDialog(SipStack* s,
                  resip::HandleManager& msg,
                  const CompletionToken& sid);
  ~ORangeAppDialog();

  const CompletionToken& getSessionToken() { return m_sessionId; }
};

class ORangeAppDialogSet : public resip::AppDialogSet
{
  CompletionToken m_sessionId;
  SipStack* m_sipStack;
  resip::DialogUsageManager& m_dum;

public:
  ORangeAppDialogSet(SipStack* s,
                     resip::DialogUsageManager& dum,
                     const CompletionToken& sid);
  ORangeAppDialogSet(SipStack* s, resip::DialogUsageManager& dum);

  resip::AppDialog* createAppDialog(const resip::SipMessage& msg) override;

  ~ORangeAppDialogSet();

  const CompletionToken& getSessionToken() { return m_sessionId; }
};

class ORangeAppDialogSetFactory : public resip::AppDialogSetFactory
{
  SipStack* m_sipStack;

public:
  ORangeAppDialogSetFactory(SipStack* s)
    : m_sipStack(s)
  {
  }

  virtual resip::AppDialogSet* createAppDialogSet(
    resip::DialogUsageManager& dum,
    const resip::SipMessage&)
  {
    return new ORangeAppDialogSet(m_sipStack, dum);
  }
};
