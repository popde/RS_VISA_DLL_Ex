/*******************************************************************************/
/**
@file           RsVisaLoader.h
@copyright      (c) Rohde & Schwarz GmbH & Co. KG, Munich
@version        $Workfile$
*
@author         Guido Kiener, 1CC3
@responsible         - " -
*
@language       C++ ANSI V3
*
@description
@see
*
*******************************************************************************/

#ifndef RsVisaLoader_H
#define RsVisaLoader_H

/* INCLUDE FILES **************************************************************/
#include "visa.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/* GLOBAL TYPE DECLARATIONS ***************************************************/

/* GLOBAL CONSTANT DECLARATIONS ***********************************************/

#define RSVISA_MANFID_DEFAULT ((ViUInt16)(0xFFFF))
#define RSVISA_MANFID_RS      ((ViUInt16)(0xFAF))
#define RSVISA_MANFID_NI      ((ViUInt16)(0xFF6))
#define RSVISA_MANFID_AG      ((ViUInt16)(0xFFF))

/* GLOBAL VARIABLE DECLARATIONS ***********************************************/

/* GLOBAL MACRO DEFINITIONS ***************************************************/

/* EXPORTED FUNCTIONS *********************************************************/

/* FUNCTION ******************************************************************/
/** RsViSetDefaultLibrary
**
Selects loading of the default VISA library. Call this function before any
other VISA library functions.
RSVISA_MANFID_DEFAULT: means that the primary visa32.dll is loaded.
RSVISA_MANFID_RS:      means that the rsvisa32.dll is loaded.
RSVISA_MANFID_AG:      means that the agvisa32.dll is loaded.
RSVISA_MANFID_NI:      for 32 bit applications same as RSVISA_MANFID_DEFAULT; for
                       64 bit applications nivisa64.dll is loaded

If the default library can not be loaded, the rsvisa32.dll is tried.
Note that the library is loaded with the first call of a VISA function.
*
@precondition  - must be called before viOpenDefaultRM(..) or other
                 vi... functions
@postcondition - none
@side_effects  - none
*
@param uManfID:  a value of RSVISA_MANFID_DEFAULT, RSVISA_MANFID_RS, etc.
*
@return - VI_TRUE: the default library value has changed.
        - VI_FALSE: uManfID is invalid
******************************************************************************/
ViBoolean _VI_FUNC RsViSetDefaultLibrary(ViUInt16 uManfID);

/* FUNCTION ******************************************************************/
/** RsViUnloadVisaLibrary
**
Unloads the loaded VISA library. Call this function only after calling
viClose( <session of defaultRM> ).
*
@precondition  - must be called after viClose(..)
@postcondition - none
@side_effects  - none
******************************************************************************/
void _VI_FUNC RsViUnloadVisaLibrary();

/* FUNCTION ******************************************************************/
/** RsViIsVisaLibraryInstalled
**
Checks if the VISA library of the given manufacturer uManfID is installed.
RSVISA_MANFID_DEFAULT: checks that the primary visa32.dll is in system32.
RSVISA_MANFID_RS:      checks that the rsvisa32.dll is in system32.
RSVISA_MANFID_AG:      checks that the agvisa32.dll is in system32.
RSVISA_MANFID_NI:      here same as RSVISA_MANFID_DEFAULT.
*
@precondition  - none
@postcondition - none
@side_effects  - none
*
@param uManfID:  a value of RSVISA_MANFID_DEFAULT, RSVISA_MANFID_RS, etc.
*
@return - VI_TRUE: the VISA library is installed.
        - VI_FALSE: not installed or uManfID is isvalid.
******************************************************************************/
ViBoolean _VI_FUNC RsViIsVisaLibraryInstalled(ViUInt16 uManfID);


#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // RsVisaLoader_H

/******************************************************************************/
/**
@history
// $Log$
*/