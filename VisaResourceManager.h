/**
 @file VisaResourceManager.h
 @copyright (c) Rohde & Schwarz GmbH & Co. KG, Munich
 */

#pragma once
#include <vector>
#include <map>
#include <mutex>
#include <string>
#include "visa.h"
#include "VisaException.h"
#include "VisaSession.h"

struct DeviceInformation
{
  std::string VisaResourceString;
  std::string manufacturer;
  std::string model;
  std::string serialNumber;
  std::string version;
  std::string description;
  std::string hostname;
  ViUInt16 intfType;
  ViUInt16 intfNum;
  std::string rsrcClass;
  std::string expandedUnaliasedName;
  std::string aliasIfExists;
};

/**
@class VisaResourceManager
@brief Representation of the Resource Manager

If a function encounters a VISA error a VisaException is thrown.

*/
class VisaResourceManager final
{
public:
  typedef std::vector <DeviceInformation>  devicelist_t;
  typedef VisaSession*                     session_t;

  VisaResourceManager();
  ~VisaResourceManager();

#ifdef USECPP11
  VisaResourceManager(VisaResourceManager const&) = delete;
  VisaResourceManager& operator =(VisaResourceManager const&) = delete;
#endif 

  /// returns a list of available VISA resources; if VXI11 == true devices are found over
  /// the network via VXI11 and if mDNS == true an mDNS search is invoked
  devicelist_t findResources() const;

  /// connects to the device with the given VISA resource String and returns a session object
  session_t connect(const std::string& device);

  /// disconnects the given session
  void disconnect(session_t sesn);

  /// Returns a description for the given status code
  static std::string statusDescription(ViStatus status);

  /// Fetches information about device \a rsrcName and stores these in \a deviceInformation
  void parseRsrcEx(const std::string& rsrcName, DeviceInformation& deviceInformation) const;

private:
  typedef std::map<session_t, ViSession> openSessionsmap_t;
  const ViSession m_RM;
  openSessionsmap_t m_connectedSessions;
  std::recursive_mutex m_mutex;
  /// disconnects all sessions
  void disconnectAll();

  static void checkAndThrow(ViStatus status, const std::string& msg = "");
  static ViSession openRM();

  /// Adds the device with VisaResourceString \a desc to \a deviceList
  void addDeviceToFindList(ViFindList vi, const std::string& desc, devicelist_t& deviceList) const;

#ifndef USECPP11
  VisaResourceManager(VisaResourceManager const&);
  VisaResourceManager& operator =(VisaResourceManager const&);
#endif 

};
