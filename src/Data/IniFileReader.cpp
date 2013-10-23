// -----------------------------------------------------------------
// INI FILE READER
// -----------------------------------------------------------------
#include "IniFileReader.h"

// -----------------------------------------------------------------
// Name : IniFileReader
// -----------------------------------------------------------------
IniFileReader::IniFileReader()
{
}

// -----------------------------------------------------------------
// Name : ~IniFileReader
// -----------------------------------------------------------------
IniFileReader::~IniFileReader()
{
    m_hmData.clear();
}

// -----------------------------------------------------------------
// Name : findValue
// -----------------------------------------------------------------
string * IniFileReader::findValue(string sKey)
{
    str_hash::iterator it = m_hmData.find(sKey);
    if (it == m_hmData.end())
    {
        return NULL;
    }
    return &(it->second);
}

// -----------------------------------------------------------------
// Name : findCharValue
// -----------------------------------------------------------------
const char * IniFileReader::findCharValue(string sKey, const char * sDefault)
{
    string * res = findValue(sKey);
    return (res == NULL ? sDefault : res->c_str());
}

// -----------------------------------------------------------------
// Name : findIntValue
// -----------------------------------------------------------------
int IniFileReader::findIntValue(string sKey, int iDefault)
{
    string * res = findValue(sKey);
    return (res == NULL ? iDefault : atoi(res->c_str()));
}

// -----------------------------------------------------------------
// Name : findBoolValue
// -----------------------------------------------------------------
bool IniFileReader::findBoolValue(string sKey, bool bDefault)
{
    string * res = findValue(sKey);
    return (res == NULL ? bDefault : (strcmp(res->c_str(), "1") == 0 || strcmp(res->c_str(), "true") == 0));
}

// -----------------------------------------------------------------
// Name : findFloatValue
// -----------------------------------------------------------------
float IniFileReader::findFloatValue(string sKey, float fDefault)
{
    string * res = findValue(sKey);
    return (res == NULL ? fDefault : atof(res->c_str()));
}

// -----------------------------------------------------------------
// Name : read
// -----------------------------------------------------------------
bool IniFileReader::read(string sFileName)
{
    // Open file
    ifstream file(sFileName.c_str());
    if (!file.is_open()) {
        return false;
    }

    int iChar = 0;
    bool bKey = true;
    char sBuf[1024];
    string sKey;

    while (file.good()) {
        char c = file.get();
        if (bKey) {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') {
                sBuf[iChar++] = c;
                sBuf[iChar] = '\0';
            } else if (c == '=') {
                sKey = string(sBuf);
                bKey = false;
                iChar = 0;
            }
        } else {
            if (iChar == 0 && (c == ' ' || c == '\t')) {
                continue;
            }
            if (c == '\r' || c == '\n') {
                m_hmData[sKey] = string(sBuf);
                bKey = true;
                iChar = 0;
            } else {
                sBuf[iChar++] = c;
                sBuf[iChar] = '\0';
            }
        }
    }

    file.close();
    return true;
}
