#include "VisaResourceManager.h"
#include "VisaSession.h"

VisaResourceManager rm;
VisaResourceManager::session_t mysession;
VisaResourceManager::devicelist_t availableRsrc;
ViBoolean initOK = VI_FALSE;

int32_t VisaRsrc() {
	availableRsrc = rm.findResources();
	return availableRsrc.size();
}

const char* VisaGetRsrcString(ViAttr index) {
	if (index<availableRsrc.size())
	{
		return availableRsrc.at(index).VisaResourceString.c_str();
	}
	return NULL;
}

bool VisaInit(const char* deviceName) {

	const std::string TestDevice = deviceName;
	bool status = VI_FALSE;
	try
	{
		mysession = rm.connect(TestDevice);
		status = VI_TRUE;
		initOK = VI_TRUE;
	}
	catch (VisaException ex)
	{
		status = VI_FALSE;
	}

	return status;
}

bool VisaClose() {
	bool status = VI_FALSE;
	if (initOK)
	{
		try
		{
			rm.disconnect(mysession);
			status = VI_TRUE;
			initOK = VI_FALSE;
		}
		catch (VisaException ex)
		{
			status = VI_FALSE;
		}
	}
	return status;
}

bool VisaClear() {
	bool status = VI_FALSE;
	if (initOK)
	{
		try
		{
			mysession->clear();
			status = VI_TRUE;
		}
		catch (VisaException ex)
		{
			status = VI_FALSE;
		}
	}
	return status;
}

bool VisaSetAttr(ViAttr attrName, uint32_t attrValue) {
	bool status = VI_FALSE;
	if (initOK)
	{
		try
		{
			mysession->setAttribute(attrName, attrValue);
			status = VI_TRUE;
		}
		catch (VisaException ex)
		{
			status = VI_FALSE;
		}
	}
	return status;
}

uint32_t VisaGetAttr(ViAttr attrName) {
	uint32_t attrValue = 0;
	if (initOK)
	{
		try
		{
			attrValue = mysession->getAttribute(attrName);
		}
		catch (VisaException ex)
		{
			
		};
	}
	return attrValue;
}

bool VisaWrite(const char* str) {
	bool status = VI_FALSE;
	if (initOK)
	{
		try
		{
			mysession->write(str);
			status = VI_TRUE;
		}
		catch (VisaException ex)
		{
			status = VI_FALSE;
		}
	}

	return status;
}

const char* VisaRead(uint16_t num) {
	static std::string readValue;
	if (initOK)
	{
		readValue = mysession->read(num);
	}

	return readValue.c_str();
}


