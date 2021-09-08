/**
 @file VisaResourceManager.cpp
 @copyright (c) Rohde & Schwarz GmbH & Co. KG, Munich
 */

#include "VisaResourceManager.h"
#include <cstring>

VisaResourceManager::VisaResourceManager()
  : m_RM(openRM())
{
}

VisaResourceManager::~VisaResourceManager()
{
  disconnectAll();
  viClose(m_RM);
}

ViSession VisaResourceManager::openRM()
{
  ViSession result;
  ViStatus status = viOpenDefaultRM(&result);
  checkAndThrow(status, "Could not load Default Resource Manager");
  return result;
}

VisaResourceManager::devicelist_t VisaResourceManager::findResources(bool VXI11, bool mDNS) const
{
  ViStatus status;
#ifdef RSVISA_EXTENSION
  ViAttrState searchAttributes = VI_RS_FIND_MODE_CONFIG;
  if (VXI11)
  {
    searchAttributes |= VI_RS_FIND_MODE_VXI11;
  }

  if (mDNS)
  {
    searchAttributes |= VI_RS_FIND_MODE_MDNS;
  }

  status = viSetAttribute(m_RM, VI_RS_ATTR_TCPIP_FIND_RSRC_MODE, searchAttributes);
#endif
  ViUInt32 retCnt;
  ViFindList vi;
  ViChar desc[256];
  const std::string expr = "?*";
  status = viFindRsrc(m_RM, expr.c_str(), &vi, &retCnt, desc);
  checkAndThrow(status, "viFindRsrc failed");

  devicelist_t result;
  result.reserve(retCnt);

  addDeviceToFindList(vi, desc, result);

  for (ViInt16 i = 0; i < static_cast<ViInt16>(retCnt) - 1; ++i)
  {
    status = viFindNext(vi, desc);
    checkAndThrow(status, "viFindNext failed");
    addDeviceToFindList(vi, desc, result);
  }

  viClose(vi);

  return result;
}

void VisaResourceManager::addDeviceToFindList(ViFindList vi, const std::string& desc, devicelist_t& deviceList) const
{
  DeviceInformation deviceInformation;
  deviceInformation.VisaResourceString = desc;
#ifdef RSVISA_EXTENSION
  ViStatus status;
#define GETLXIATTR(_memberName, _attr) ViChar _memberName[512];\
  status = viGetAttribute(vi, _attr, _memberName);\
  checkAndThrow(status, "viGetAttribute(" + std::string(#_attr) + ") failed.");\
  deviceInformation._memberName = _memberName;

  GETLXIATTR(manufacturer, VI_RS_ATTR_LXI_MANF);
  GETLXIATTR(model, VI_RS_ATTR_LXI_MODEL);
  GETLXIATTR(serialNumber, VI_RS_ATTR_LXI_SERIAL);
  GETLXIATTR(version, VI_RS_ATTR_LXI_VERSION);
  GETLXIATTR(description, VI_RS_ATTR_LXI_DESCRIPTION);
  GETLXIATTR(hostname, VI_RS_ATTR_LXI_HOSTNAME);

#undef GETLXIATTR
#endif

  parseRsrcEx(desc, deviceInformation);
  deviceList.push_back(deviceInformation);
}

void VisaResourceManager::parseRsrcEx(const std::string& rsrcName, DeviceInformation& deviceInformation) const
{
  ViUInt16 intfType;
  ViUInt16 intfNum;
  ViChar rsrcClass[1024];
  ViChar expandedUnaliasedName[1024];
  ViChar aliasIfExists[1024];

  memset(rsrcClass, 0, sizeof(rsrcClass));
  memset(expandedUnaliasedName, 0, sizeof(expandedUnaliasedName));
  memset(aliasIfExists, 0, sizeof(aliasIfExists));

  viParseRsrcEx(m_RM, rsrcName.c_str(), &intfType, &intfNum, rsrcClass, expandedUnaliasedName, aliasIfExists);
  deviceInformation.intfType = intfType;
  deviceInformation.intfNum = intfNum;
  deviceInformation.rsrcClass = rsrcClass;
  deviceInformation.expandedUnaliasedName = expandedUnaliasedName;
  deviceInformation.aliasIfExists = aliasIfExists;
}

VisaResourceManager::session_t VisaResourceManager::connect(const std::string& device)
{
  std::lock_guard<decltype(m_mutex)> lock(m_mutex);
  ViSession vi;
  ViStatus status = viOpen(m_RM, device.c_str(), 0, 1000, &vi);
  checkAndThrow(status, "Could not connect to device");

  session_t result = new VisaSession(vi);
  m_connectedSessions[result] = vi;
  return result;
}

void VisaResourceManager::disconnect(session_t sesn)
{
  std::lock_guard<decltype(m_mutex)> lock(m_mutex);
  openSessionsmap_t::iterator iter = m_connectedSessions.find(sesn);
  if (iter != m_connectedSessions.end())
  {
    viClose(iter->second);
    delete iter->first;
    m_connectedSessions.erase(iter);
  }
}

void VisaResourceManager::disconnectAll()
{
  std::lock_guard<decltype(m_mutex)> lock(m_mutex);
  for (auto& i : m_connectedSessions)
  {
    viClose(i.second);
    delete i.first;
  }
  m_connectedSessions.clear();
}

/*static*/ std::string VisaResourceManager::statusDescription(ViStatus status)
{
  ViChar buf[256];
  memset(buf, 0, sizeof(buf));
  ViStatus error = viStatusDesc(VI_NULL, status, buf);
  // We do not throw an exception here, because the user might use this function to produce an
  // error message in the exception handler. If VisaResourceManager::statusDescription throws this
  // might result in an infinite loop
  if (error != VI_SUCCESS)
  {
    return "Error in statusDescription: " + std::string(buf) + std::to_string(error);
  }
  return buf;
}

void VisaResourceManager::checkAndThrow(ViStatus status, const std::string& msg)
{
  if (status < VI_SUCCESS)
  {
    throw VisaException(status, msg);
  }
}
