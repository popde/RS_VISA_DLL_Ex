/**
  @file VisaException.h
  @copyright (c) Rohde & Schwarz GmbH & Co. KG, Munich
 */

#pragma once
#include <string>
#include <exception>
#include "visa.h"

#if defined USECPP11 && (! defined(_MSC_VER) || _MSC_VER >= 1900 ) 
#define NOEXCEPT noexcept
#else 
#define NOEXCEPT
#endif

class VisaException : public std::exception
{
public:
  VisaException(ViStatus errorcode, const std::string & error = "")
    : m_errorcode(errorcode)
    , m_sError(error)
    , m_sErrorMsg("VISA error " + std::to_string(errorcode) + ": " + error)
  {
  }

  const char* what() const NOEXCEPT override { return m_sErrorMsg.c_str(); }

  ViStatus getErrorCode() const { return m_errorcode; }
  const std::string& getError() const { return m_sError; }

private:
  const ViStatus m_errorcode;
  const std::string m_sError;
  const std::string m_sErrorMsg;
};
