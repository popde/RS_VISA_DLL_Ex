/**
 @file VisaSession.cpp
 @copyright (c) Rohde & Schwarz GmbH & Co. KG, Munich
 */

#include "VisaSession.h"

#include <stdarg.h>

VisaSession::VisaSession(ViSession vi)
  : m_vi(vi)
{
}

VisaSession::~VisaSession()
{
}

ViUInt32 VisaSession::write(const std::string& msg)
{
  ViUInt32 retCnt;
  const ViStatus status = viWrite(getVi(), reinterpret_cast<ViConstBuf>(msg.c_str()), static_cast<ViUInt32>(msg.size()), &retCnt);
  checkAndThrow(status, "viWrite failed");
  return retCnt;
}

std::string VisaSession::read()
{
  static const ViUInt32 buflen = 1024;
 
  std::string result;

  ViByte buf[buflen];
  ViUInt32 retCnt;
  ViStatus status = VI_SUCCESS_MAX_CNT;

  while (status == VI_SUCCESS_MAX_CNT)
  {
    status = viRead(getVi(), buf, buflen, &retCnt);
    checkAndThrow(status, "viRead failed");
    result.append(reinterpret_cast<char*>(buf), retCnt);
  }

  return result;
}

void VisaSession::Printf(const std::string writeFmt, ...)
{
  va_list args;
  va_start(args, writeFmt);
  const ViStatus status = viVPrintf(getVi(), writeFmt.c_str(), args);
  va_end(args);
  checkAndThrow(status, "viVPrintf failed");
}

void VisaSession::Scanf(const std::string readFmt, ...)
{
  va_list args;
  va_start(args, readFmt);
  const ViStatus status = viVScanf(getVi(), readFmt.c_str(), args);
  va_end(args);
  checkAndThrow(status, "viVScanf failed");
}

void VisaSession::Queryf(const std::string& writeFmt, const std::string readFmt, ...)
{
  va_list args;
  va_start(args, readFmt);
  const ViStatus status = viVQueryf(getVi(), writeFmt.c_str(), readFmt.c_str(), args);
  va_end(args);
  checkAndThrow(status, "viVScanf failed");
}

void VisaSession::enableEvent(Events eventType, EventMechanism mechanism)
{
  const ViStatus status = viEnableEvent(getVi(), Event2EventType(eventType), static_cast<ViUInt16>(mechanism), VI_NULL);
  checkAndThrow(status, "viEnableEvent failed");
}

void VisaSession::disableEvent(Events eventType, EventMechanism mechanism)
{
  const ViStatus status = viDisableEvent(getVi(), Event2EventType(eventType), static_cast<ViUInt16>(mechanism));
  checkAndThrow(status, "viDisableEvent failed");
}

void VisaSession::discardEvents(Events eventType, EventMechanism mechanism)
{
  const ViStatus status = viDiscardEvents(getVi(), Event2EventType(eventType), static_cast<ViUInt16>(mechanism));
  checkAndThrow(status, "viDiscardEvents failed");
}

void VisaSession::installHandler(Events eventType, ViHndlr handler, ViAddr userHandle)
{
  const ViStatus status = viInstallHandler(getVi(), Event2EventType(eventType), handler, userHandle);
  checkAndThrow(status, "viInstallHandler failed");
}

void VisaSession::uninstallHandler(Events eventType, ViHndlr handler, ViAddr userHandle)
{
  const ViStatus status = viUninstallHandler(getVi(), Event2EventType(eventType), handler, userHandle);
  checkAndThrow(status, "viUninstallHandler failed");
}

ViUInt16 VisaSession::readSTB()
{
  ViUInt16 stb = 0;
  ViStatus status = viReadSTB(getVi(), &stb);
  checkAndThrow(status, "viReadSTB failed.");

  return stb;
}

void VisaSession::setAttribute(ViAttr attrName, ViAttrState attrValue)
{
  const ViStatus status = viSetAttribute(getVi(), attrName, attrValue);
  checkAndThrow(status, "viSetAttribute failed");
}

void VisaSession::clear()
{
  ViStatus status = viClear(getVi());
  checkAndThrow(status, "viClear failed.");
}

ViEventType VisaSession::Event2EventType(Events eventType)
{
  ViEventType result = 0;
  switch (eventType)
  {
    case Events::IO_COMPLETION:
      result = VI_EVENT_IO_COMPLETION;
      break;
    case Events::EVENT_TRIG:
      result = VI_EVENT_TRIG;
      break;
    case Events::SERVICE_REQ:
      result = VI_EVENT_SERVICE_REQ;
      break;
    case Events::CLEAR:
      result = VI_EVENT_CLEAR;
      break;
    case Events::EXCEPTION:
      result = VI_EVENT_EXCEPTION;
      break;
    case Events::GPIB_CIC:
      result = VI_EVENT_GPIB_CIC;
      break;
    case Events::GPIB_TALK:
      result = VI_EVENT_GPIB_TALK;
      break;
    case Events::GPIB_LISTEN:
      result = VI_EVENT_GPIB_LISTEN;
      break;
    case Events::VXI_VME_SYSFAIL:
      result = VI_EVENT_VXI_VME_SYSFAIL;
      break;
    case Events::VXI_VME_SYSRESET:
      result = VI_EVENT_VXI_VME_SYSRESET;
      break;
    case Events::VXI_SIGP:
      result = VI_EVENT_VXI_SIGP;
      break;
    case Events::VXI_VME_INTR:
      result = VI_EVENT_VXI_VME_INTR;
      break;
    case Events::PXI_INTR:
      result = VI_EVENT_PXI_INTR;
      break;
    case Events::TCPIP_CONNECT:
      result = VI_EVENT_TCPIP_CONNECT;
      break;
    case Events::USB_INTR:
      result = VI_EVENT_USB_INTR;
      break;
    case Events::ALL_EVENTS:
      result = VI_ALL_ENABLED_EVENTS;
      break;
  }
  return result;
}

void VisaSession::checkAndThrow(ViStatus status, const std::string& msg)
{
  if (status < VI_SUCCESS)
  {
    throw VisaException(status, msg);
  }
}
