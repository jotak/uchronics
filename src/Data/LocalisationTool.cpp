#include "LocalisationTool.h"
#include "../Managers/DebugManager.h"
#include "Parameters.h"

LocalisationTool * LocalisationTool::m_pInstance = NULL;

// -----------------------------------------------------------------
// Name : LocalisationTool
// -----------------------------------------------------------------
LocalisationTool::LocalisationTool()
{
}

// -----------------------------------------------------------------
// Name : ~LocalisationTool
// -----------------------------------------------------------------
LocalisationTool::~LocalisationTool()
{
}

// -----------------------------------------------------------------
// Name : init
// -----------------------------------------------------------------
vector<string> LocalisationTool::readCsvLine(string sLine)
{
	vector<string> ret;
	stringstream lineBuf(sLine);
	stringstream wordBuf;

	bool quoted = false;
	bool readUntilComa = false;
	if (lineBuf.peek() == '\"') {
		quoted = true;
		lineBuf.get();
	}

	while (lineBuf.good()) {
		char c = lineBuf.get();
		if (readUntilComa) {
			if (c == ',') {
				readUntilComa = false;
			}
		} else if (c == '\n' || c == '\r') {
			// end line
			break;
		} else if (quoted && c == '\"') {
			ret.push_back(wordBuf.str());
			wordBuf.str(string());	// clear wordBuf
			readUntilComa = true;
			quoted = false;
		} else if (!quoted && c == ',') {
			ret.push_back(wordBuf.str());
			wordBuf.str(string());	// clear wordBuf
		} else {
			wordBuf << c;
		}
	}
	ret.push_back(wordBuf.str());

	return ret;
}

// -----------------------------------------------------------------
// Name : init
// -----------------------------------------------------------------
void LocalisationTool::init()
{
    // Free current data, if any
    m_sAllTexts.clear();

    string sLang = _params->getLanguage();
    string sPath = string(DATA_PATH) + "langs.csv";

    // Open file
    ifstream file(sPath.c_str());
    if (!file.is_open()) {
        _debug->error(string("Could not open language file. Check out file ") + sPath + ".");
        return;
    }

    // Get first line (row header) ; it contains "key" and langcodes.
    int langCol = -1;
    string sLine;
    getline(file, sLine);
    vector<string> vLine = readCsvLine(sLine);
    for (int iCol = 0; iCol < (int) vLine.size(); iCol++) {
    	if (vLine[iCol] == sLang) {
    		langCol = iCol;
    		break;
    	}
    }
    if (langCol < 0) {
    	_debug->error(string("Can't find column for language ") + sLang + " in CSV file.");
    	langCol = 0;
    }

    // Read file (CSV)
    while (file.good()) {
        // Get line by line
        getline(file, sLine);
        vLine = readCsvLine(sLine);
        if ((int)vLine.size() > langCol) {
        	string key = vLine[0];
        	string value = vLine[langCol];
        	if (value != "") {
        		m_sAllTexts[key] = value;
        	}
        }
    }

    file.close();
}

// -----------------------------------------------------------------
// Name : getText
//  Fields order is managed (%$1x)
//  pArgs must contain all the variables to replace in model
// -----------------------------------------------------------------
string LocalisationTool::getText(string sKey, void ** pArgs)
{
    // Get text in hashmap
    string ret = m_sAllTexts[sKey];
    if (ret == "") {
        _debug->error("l12n key not found: " + sKey);
        ret = sKey;
    }

    // If pArgs is null we can skip all variable replacement stuff
    if (pArgs != NULL)
    {
        string sModel = string(ret);
        ret = string("");

        // If text contains variable fields, fill them
        int iSrc = 0;
        int iVarPos = 0;
        while (sModel[iSrc] != '\0')
        {
            if (sModel[iSrc] == '%')
            {
                iSrc++;
                // is '%' character?
                if (sModel[iSrc] == '%')
                {
                    ret += sModel[iSrc++];
                }
                else
                {
                    // ordered element?
                    if (sModel[iSrc] == '$')
                    {
                        iVarPos = (int) (sModel[++iSrc] - '1');
                        iSrc++;
                    }
                    //int precision = -1;
                    // precision?
                    if (sModel[iSrc] == '.')
                    {
                    	++iSrc;
                        //precision = (int) (sModel[++iSrc] - '0');
                        iSrc++;
                    }
                    // length?
                    if (sModel[iSrc] == 'h')
                    {
                        iSrc++;
                    }
                    else if (sModel[iSrc] == 'L' || sModel[iSrc] == 'l')
                    {
                        iSrc++;
                    }
                    // specifier
                    switch (sModel[iSrc])
                    {
                    case 'u':
                    {
                        unsigned uVal = *(unsigned*)(pArgs[iVarPos]);
                        ret += uVal;
                        iSrc++;
                        iVarPos++;
                        break;
                    }
                    case 'd':
                    {
                        long iVal = *(long*)(pArgs[iVarPos]);
                        ret += iVal;
                        iSrc++;
                        iVarPos++;
                        break;
                    }
                    case 'f':
                    {
                        double fVal = *(double*)(pArgs[iVarPos]);
                        ret += fVal;
                        iSrc++;
                        iVarPos++;
                        break;
                    }
                    case 'c':
                    {
                        int cVal = *(int*)(pArgs[iVarPos]);
                        ret += (char) cVal;
                        iSrc++;
                        iVarPos++;
                        break;
                    }
                    case 's':
                    {
                        char * sVal = (char*)(pArgs[iVarPos]);
                        ret += sVal;
                        iSrc++;
                        iVarPos++;
                        break;
                    }
                    }
                }
            }
            else
            {
                ret += sModel[iSrc++];
            }
        }
    }

    return ret;
}

// -----------------------------------------------------------------
// Name : getTextUCFirst
//  Upper case for 1st letter
// -----------------------------------------------------------------
string LocalisationTool::getTextUCFirst(string sKey, void ** pArgs)
{
    string str = getText(sKey, pArgs);
    transform(str.begin(), str.begin(), str.begin(), ::toupper);
    return str;
}

// -----------------------------------------------------------------
// Name : getTextUC
//  Upper case
// -----------------------------------------------------------------
string LocalisationTool::getTextUC(string sKey, void ** pArgs)
{
    string str = getText(sKey, pArgs);
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// -----------------------------------------------------------------
// Name : getTextLC
//  Lower case
// -----------------------------------------------------------------
string LocalisationTool::getTextLC(string sKey, void ** pArgs)
{
    string str = getText(sKey, pArgs);
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}
