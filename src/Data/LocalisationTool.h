#ifndef _LOCALISATION_TOOL_H
#define _LOCALISATION_TOOL_H

#include "../Utils/utils.h"

// This is a singleton
class LocalisationTool
{
public:
    ~LocalisationTool();
    static LocalisationTool * getInstance()
    {
        if (m_pInstance == NULL) m_pInstance = new LocalisationTool();
        return m_pInstance;
    };

    void init();
    string getText(string sKey, void ** pArgs = NULL);
    string getTextUCFirst(string sKey, void ** pArgs = NULL);
    string getTextUC(string sKey, void ** pArgs = NULL);
    string getTextLC(string sKey, void ** pArgs = NULL);
    char * long_hashToString(char * sBuf, int iBufSize, const char * sSeparator, long_hash * hm, int nArgs, ...);

private:
    LocalisationTool();
    vector<string> readCsvLine(string sLine);

    static LocalisationTool * m_pInstance;
    str_hash m_sAllTexts;
};

// Helper alias
#define i18n LocalisationTool::getInstance()

#endif
