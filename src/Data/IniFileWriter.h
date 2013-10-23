#ifndef _INIFILE_WRITER_H
#define _INIFILE_WRITER_H

#include "../Utils/utils.h"

class IniFileWriter
{
public:
    // Constructor / destructor
	IniFileWriter();
    ~IniFileWriter();

    bool write(string sFileName);

    void set(string sKey, string value);
    void set(string sKey, bool value);
    void set(string sKey, int value);
    void set(string sKey, float value);

private:
    stringstream stream;
};

#endif
