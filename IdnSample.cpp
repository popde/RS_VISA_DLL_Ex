#include "VisaResourceManager.h"
#include "VisaSession.h"

VisaResourceManager rm;
VisaResourceManager::session_t mysession;
ViBoolean initOK = VI_FALSE;

bool VisaInit(const char* deviceName) {

	const std::string TestDevice = deviceName;
	ViStatus status = VI_FALSE;
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
	ViStatus status = VI_FALSE;
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

bool VisaSetAttr(ViAttr attrName, ViAttrState attrValue) {
	ViStatus status = VI_FALSE;
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

bool VisaWrite(const char* str) {
	ViStatus status = VI_FALSE;
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

const char* VisaRead() {
	ViStatus status = VI_FALSE;
	static std::string readValue;
	if (initOK)
	{
		try
		{
			readValue = mysession->read();
			status = VI_TRUE;
		}
		catch (VisaException ex)
		{
			status = VI_FALSE;
		}
	}
	
	return readValue.c_str();
}


