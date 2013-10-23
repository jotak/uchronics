#ifndef _INIFILE_READER_H
#define _INIFILE_READER_H

#include "../Utils/utils.h"

class IniFileReader
{
public:
    // Constructor / destructor
	IniFileReader();
    ~IniFileReader();

    bool read(string sFileName);

    string * findValue(string sKey);
    const char * findCharValue(string sKey, const char * sDefault = "");
    bool findBoolValue(string sKey, bool bDefault = false);
    int findIntValue(string sKey, int iDefault = 0);
    float findFloatValue(string sKey, float fDefault = 0.0f);

private:
    str_hash m_hmData;
};

#endif
