/**
 @file VisaSession.h
 @copyright (c) Rohde & Schwarz GmbH & Co. KG, Munich
 */

#pragma once
#include <vector>
#include <string>
#include "visa.h"
#include "VisaException.h"

/**
@class VisaSession
@brief Provides some basic VISA function calls

If a function fails a VisaException is thrown
*/
class VisaSession
{
public:
  enum class Events
  {
    IO_COMPLETION
    , EVENT_TRIG
    , SERVICE_REQ
    , CLEAR
    , EXCEPTION
    , GPIB_CIC
    , GPIB_TALK
    , GPIB_LISTEN
    , VXI_VME_SYSFAIL
    , VXI_VME_SYSRESET
    , VXI_SIGP
    , VXI_VME_INTR
    , PXI_INTR
    , TCPIP_CONNECT
    , USB_INTR
    , ALL_EVENTS
  };

  enum EventMechanism
  {
    QUEUE               = VI_QUEUE,
    HNDLR               = VI_HNDLR,
    SUSPEND_HNDLR       = VI_SUSPEND_HNDLR,
    QUEUE_HNDLR         = VI_QUEUE | VI_HNDLR,
    QUEUE_SUSPEND_HNDLR = VI_QUEUE | SUSPEND_HNDLR
  };

  explicit VisaSession(ViSession vi);
  virtual ~VisaSession();

  /// writes the string to the connected device and returns the number of written bytes
  ViUInt32 write(const std::string& msg);

  /// reads from the device
  std::string read();

  /// writes formatted output to the device
  void Printf(const std::string writeFmt, ...);

  /// reads formatted input from the device
  void Scanf(const std::string readFmt, ...);

  /// formatted in- and output
  void Queryf(const std::string& writeFmt, const std::string readFmt, ...);

  /// Enable events of the given type
  void enableEvent(Events eventType, EventMechanism mechanism);

  /// Disable events of the given type
  void disableEvent(Events eventType, EventMechanism mechanism);

  /// Discard events of the given type
  void discardEvents(Events eventType, EventMechanism mechanism);

  /// Installs an event handler
  void installHandler(Events eventType, ViHndlr handler, ViAddr userHandle);

  /// Uninstalls an event handler
  void uninstallHandler(Events eventType, ViHndlr handler, ViAddr userHandle);

  /// Read status byte
  ViUInt16 readSTB();
  
  /// Sets the value of an attribute
  void setAttribute(ViAttr attrName, ViAttrState attrValue);

  /// Clears the session
  void clear();

protected:
  ViSession getVi() const { return m_vi; }
  static void checkAndThrow(ViStatus status, const std::string& msg = "");

private:
  const ViSession m_vi;
  static ViEventType Event2EventType(Events eventType);
};
