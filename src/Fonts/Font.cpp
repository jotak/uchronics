// -----------------------------------------------------------------
// FONT
// Contains all different classes of fonts
// -----------------------------------------------------------------
#include "Font.h"
#include "../Display/TextureEngine.h"


ch_hash Font::m_hmUnicodeReplacementTable;
ch_hash Font::m_hmUnicodeReplacementAcutesTable;

// -----------------------------------------------------------------
// Name : Font
//  Constructor
// -----------------------------------------------------------------
Font::Font()
{
    m_pTex = NULL;
    m_iNbDescriptor = 0;
    m_pLastAcute = NULL;
    m_uFontHeight = 0;
    if (m_hmUnicodeReplacementTable.size() == 0) {
        Font::initUnicodeTables();
    }
}

// -----------------------------------------------------------------
// Name : load
// -----------------------------------------------------------------
bool Font::load(string sFontName)
{
    m_sFontName = sFontName;

    string descFile = string(DATA_PATH) + "fonts/" + sFontName + ".fnt";

    ifstream file(descFile.c_str());
    if (!file.is_open()) {
        return false;
    }

    string sTitle;
    string sKey;
    string sValue;
    bool bGuillemets;

    while (file.good())
    {
        int c;
        do {
            c = file.get();
        } while (c == ' ' || c == '\t' || c == '\n' || c == '\r');

        do {
            sTitle += (char)c;
            c = file.get();
        } while (c != ' ' && c != '\t' && c != '\n' && c != '\r' && file.good());

        if (!file.good()) {
            break;
        }

        do {
        	c = file.get();
        } while (c == ' ' || c == '\t');

        while (c != '\n' && c != '\r') {
            do
            {
                sKey += (char)c;
            	c = file.get();
            } while (c != '=');

        	c = file.get();
            if (c == '\"') {
                bGuillemets = true;
            	c = file.get();
            } else {
                bGuillemets = false;
            }

            while ((bGuillemets && c != '\"') || (!bGuillemets && c != ' ' && c != '\t' && c != '\n' && c != '\r'))
            {
                sValue += (char)c;
            	c = file.get();
            }

            storeData(sTitle, sKey, sValue);
            sKey = "";
            sValue = "";
            if (bGuillemets) {
            	c = file.get();
            }

            while (c == ' ' || c == '\t') {
            	c = file.get();
            }
        }
        sTitle = "";
    }
    file.close();

    m_pTex = _tex->loadTexture(string("font/") + sFontName);

    return true;
}

// -----------------------------------------------------------------
// Name : storeData
// -----------------------------------------------------------------
void Font::storeData(string sTitle, string sKey, string sValue)
{
    if (sTitle == "common" && sKey == "lineHeight") {
        m_uFontHeight = (u8) atoi(sValue.c_str());
    } else if (sTitle == "char" && sKey == "id") {
        m_AllChars[m_iNbDescriptor++].c = (int) atoi(sValue.c_str());
    } else if (sTitle == "char" && sKey == "x") {
        m_AllChars[m_iNbDescriptor-1].x = atoi(sValue.c_str());
    } else if (sTitle == "char" && sKey == "y") {
        m_AllChars[m_iNbDescriptor-1].y = atoi(sValue.c_str());
    } else if (sTitle == "char" && sKey == "width") {
        m_AllChars[m_iNbDescriptor-1].width = atoi(sValue.c_str());
    } else if (sTitle == "char" && sKey == "height") {
        m_AllChars[m_iNbDescriptor-1].height = atoi(sValue.c_str());
    } else if (sTitle == "char" && sKey == "xoffset") {
        m_AllChars[m_iNbDescriptor-1].xoffset = atoi(sValue.c_str());
    } else if (sTitle == "char" && sKey == "yoffset") {
        m_AllChars[m_iNbDescriptor-1].yoffset = atoi(sValue.c_str());
    } else if (sTitle == "char" && sKey == "xadvance") {
        m_AllChars[m_iNbDescriptor-1].xadvance = atoi(sValue.c_str());
    }
}

// ------------------------------------------------------------------
// Name : getStringLength
// ------------------------------------------------------------------
int Font::getStringLength(string sText)
{
    assert(m_pTex != NULL);
    int iWidth = 0;
    int iMaxWidth = 0;
    int iLength = sText.length();
    for (int i = 0; i < iLength; i++)
    {
        if (sText[i] == '\n')
        {
            if (iWidth > iMaxWidth) {
                iMaxWidth = iWidth;
            }
            iWidth = 0;
            continue;
        }
        CharDescriptor * charDesc = findCharDescriptor(sText[i]);
        if (charDesc == NULL) {
            continue;
        }
        iWidth += charDesc->xadvance;
    }
    if (iWidth > iMaxWidth) {
        iMaxWidth = iWidth;
    }

    return iMaxWidth;
}

// ------------------------------------------------------------------
// Name : findCharDescriptor
// ------------------------------------------------------------------
CharDescriptor * Font::findCharDescriptor(char c, bool checkAcute)
{
    if (checkAcute) {
        m_pLastAcute = NULL;
    }
    for (int i = 0; i < m_iNbDescriptor; i++)
    {
        if (m_AllChars[i].c == c) {
            return &(m_AllChars[i]);
        }
    }
    if (checkAcute)
    {
        ch_hash::iterator it = Font::m_hmUnicodeReplacementTable.find(c);
        if (it != Font::m_hmUnicodeReplacementTable.end())
        {

            ch_hash::iterator itAcute = Font::m_hmUnicodeReplacementAcutesTable.find(c);
            if (itAcute != Font::m_hmUnicodeReplacementTable.end())
            {
                m_pLastAcute = findCharDescriptor(itAcute->second, false);
            }
            return findCharDescriptor(it->second, false);
        }
    }
    return NULL;
}

// ------------------------------------------------------------------
// Name : putStringInBox
// ------------------------------------------------------------------
int Font::putStringInBox(string sText, int iBoxWidth)
{
    assert(m_pTex != NULL);
    int iWidth = 0;
    int iHeight = m_uFontHeight;
    int iLength = sText.length();
    string sOldText = sText;
    int iNew = 0;
    int iLastSpace = -1;
    int iWidthSinceLastSpace = 0;
    for (int i = 0; i < iLength; i++)
    {
        sText[iNew] = sOldText[i];
        if (sOldText[i] == '\n')
        {
            iHeight += m_uFontHeight;
            iWidth = 0;
            iWidthSinceLastSpace = 0;
            iLastSpace = -1;
            iNew++;
            sText[iNew] = '\0';
            continue;
        }
        CharDescriptor * charDesc = findCharDescriptor(sOldText[i]);
        if (charDesc == NULL) {
            continue;
        }
        iWidth += charDesc->xadvance;
        iWidthSinceLastSpace += charDesc->xadvance;
        if (sOldText[i] == ' ' || sOldText[i] == '\t')
        {
            iLastSpace = iNew;
            iWidthSinceLastSpace = 0;
        }
        iNew++;
        sText[iNew] = '\0';
        if (iWidth > iBoxWidth)
        {
            if (iLastSpace >= 0) {
                sText[iLastSpace] = '\n';
            } else {
                sText[iNew++] = '\n';
            }
            iHeight += m_uFontHeight;
            iWidth = iWidthSinceLastSpace;
            iWidthSinceLastSpace = 0;
            iLastSpace = -1;
        }
    }
    return iHeight;
}

// ------------------------------------------------------------------
// Name : getCharacterPosition
// ------------------------------------------------------------------
CoordsScreen Font::getCharacterPosition(int iPos, string sText)
{
    assert(m_pTex != NULL);
    CoordsScreen cs(0,0);
    int iLength = sText.length();
    for (int i = 0; i < iPos; i++)
    {
        if (i >= iLength) {
            break;
        }
        if (sText[i] == '\n')
        {
            cs.x = 0;
            cs.y += m_uFontHeight;
            continue;
        }
        CharDescriptor * charDesc = findCharDescriptor(sText[i]);
        if (charDesc == NULL) {
            continue;
        }
        cs.x += charDesc->xadvance;
    }
    return cs;
}

// ------------------------------------------------------------------
// Name : getCharacterPosition
// ------------------------------------------------------------------
int Font::getCharacterPosition(CoordsScreen cs, string sText)
{
    assert(m_pTex != NULL);
    CoordsScreen cs2(0,m_uFontHeight);
    int iLength = sText.length();
    for (int i = 0; i < iLength; i++)
    {
        if (sText[i] == '\n')
        {
            if (cs.y <= cs2.y) {  // clicked on previous line
                return i;
            }
            cs2.x = 0;
            cs2.y += m_uFontHeight;
            continue;
        }
        CharDescriptor * charDesc = findCharDescriptor(sText[i]);
        if (charDesc == NULL) {
            continue;
        }
        int adv = charDesc->xadvance / 2;
        cs2.x += adv;
        if (cs.x <= cs2.x && cs.y <= cs2.y) {
            return i;
        }
        cs2.x += charDesc->xadvance - adv;
        if (cs.x <= cs2.x && cs.y <= cs2.y) {
            return i+1;
        }
    }
    return iLength;
}

// ------------------------------------------------------------------
// Name : initUnicodeTables
// ------------------------------------------------------------------
void Font::initUnicodeTables()
{
//ch_hash Font::m_hmUnicodeReplacementTable;
//ch_hash Font::m_hmUnicodeReplacementAcutesTable;

// See http://fr.wikipedia.org/wiki/Table_des_caract%C3%A8res_Unicode_%280000-0FFF%29
    Font::m_hmUnicodeReplacementTable[L'\u00C0'] = 'A';
    Font::m_hmUnicodeReplacementTable[L'\u00C1'] = 'A';
    Font::m_hmUnicodeReplacementTable[L'\u00C2'] = 'A';
    Font::m_hmUnicodeReplacementTable[L'\u00C3'] = 'A';
    Font::m_hmUnicodeReplacementTable[L'\u00C4'] = 'A';
    Font::m_hmUnicodeReplacementTable[L'\u00C5'] = 'A';
    Font::m_hmUnicodeReplacementTable[L'\u00C6'] = 'A';
    Font::m_hmUnicodeReplacementTable[L'\u00C7'] = 'C';
    Font::m_hmUnicodeReplacementTable[L'\u00C8'] = 'E';
    Font::m_hmUnicodeReplacementTable[L'\u00C9'] = 'E';
    Font::m_hmUnicodeReplacementTable[L'\u00CA'] = 'E';
    Font::m_hmUnicodeReplacementTable[L'\u00CB'] = 'E';
    Font::m_hmUnicodeReplacementTable[L'\u00CC'] = 'I';
    Font::m_hmUnicodeReplacementTable[L'\u00CD'] = 'I';
    Font::m_hmUnicodeReplacementTable[L'\u00CE'] = 'I';
    Font::m_hmUnicodeReplacementTable[L'\u00CF'] = 'I';
    Font::m_hmUnicodeReplacementTable[L'\u00D0'] = 'D';
    Font::m_hmUnicodeReplacementTable[L'\u00D1'] = 'N';
    Font::m_hmUnicodeReplacementTable[L'\u00D2'] = 'O';
    Font::m_hmUnicodeReplacementTable[L'\u00D3'] = 'O';
    Font::m_hmUnicodeReplacementTable[L'\u00D4'] = 'O';
    Font::m_hmUnicodeReplacementTable[L'\u00D5'] = 'O';
    Font::m_hmUnicodeReplacementTable[L'\u00D6'] = 'O';
    Font::m_hmUnicodeReplacementTable[L'\u00D7'] = 'X';
    Font::m_hmUnicodeReplacementTable[L'\u00D8'] = 'O';
    Font::m_hmUnicodeReplacementTable[L'\u00D9'] = 'U';
    Font::m_hmUnicodeReplacementTable[L'\u00DA'] = 'U';
    Font::m_hmUnicodeReplacementTable[L'\u00DB'] = 'U';
    Font::m_hmUnicodeReplacementTable[L'\u00DC'] = 'U';
    Font::m_hmUnicodeReplacementTable[L'\u00DD'] = 'Y';
    Font::m_hmUnicodeReplacementTable[L'\u00DE'] = 'D';
    Font::m_hmUnicodeReplacementTable[L'\u00DF'] = 's';
    Font::m_hmUnicodeReplacementTable[L'\u00E0'] = 'a';
    Font::m_hmUnicodeReplacementTable[L'\u00E1'] = 'a';
    Font::m_hmUnicodeReplacementTable[L'\u00E2'] = 'a';
    Font::m_hmUnicodeReplacementTable[L'\u00E3'] = 'a';
    Font::m_hmUnicodeReplacementTable[L'\u00E4'] = 'a';
    Font::m_hmUnicodeReplacementTable[L'\u00E5'] = 'a';
    Font::m_hmUnicodeReplacementTable[L'\u00E6'] = 'a';
    Font::m_hmUnicodeReplacementTable[L'\u00E7'] = 'c';
    Font::m_hmUnicodeReplacementTable[L'\u00E8'] = 'e';
    Font::m_hmUnicodeReplacementTable[L'\u00E9'] = 'e';
    Font::m_hmUnicodeReplacementTable[L'\u00EA'] = 'e';
    Font::m_hmUnicodeReplacementTable[L'\u00EB'] = 'e';
    Font::m_hmUnicodeReplacementTable[L'\u00EC'] = 'i';
    Font::m_hmUnicodeReplacementTable[L'\u00ED'] = 'i';
    Font::m_hmUnicodeReplacementTable[L'\u00EE'] = 'i';
    Font::m_hmUnicodeReplacementTable[L'\u00EF'] = 'i';
    Font::m_hmUnicodeReplacementTable[L'\u00F0'] = 'o';
    Font::m_hmUnicodeReplacementTable[L'\u00F1'] = 'n';
    Font::m_hmUnicodeReplacementTable[L'\u00F2'] = 'o';
    Font::m_hmUnicodeReplacementTable[L'\u00F3'] = 'o';
    Font::m_hmUnicodeReplacementTable[L'\u00F4'] = 'o';
    Font::m_hmUnicodeReplacementTable[L'\u00F5'] = 'o';
    Font::m_hmUnicodeReplacementTable[L'\u00F6'] = 'o';
    Font::m_hmUnicodeReplacementTable[L'\u00F7'] = '/';
    Font::m_hmUnicodeReplacementTable[L'\u00F8'] = 'o';
    Font::m_hmUnicodeReplacementTable[L'\u00F9'] = 'u';
    Font::m_hmUnicodeReplacementTable[L'\u00FA'] = 'u';
    Font::m_hmUnicodeReplacementTable[L'\u00FB'] = 'u';
    Font::m_hmUnicodeReplacementTable[L'\u00FC'] = 'u';
    Font::m_hmUnicodeReplacementTable[L'\u00FD'] = 'y';
    Font::m_hmUnicodeReplacementTable[L'\u00FE'] = 'b';
    Font::m_hmUnicodeReplacementTable[L'\u00FF'] = 'y';

    Font::m_hmUnicodeReplacementAcutesTable[L'\u00C0'] = L'\u0060';  // `
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00C1'] = 171;
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00C2'] = L'\u005E';  // ^
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00C3'] = L'\u007E';  // ~
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00C4'] = L'\u00A8';  // ¨
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00C8'] = L'\u0060';  // `
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00C9'] = 171;
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00CA'] = L'\u005E';  // ^
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00CB'] = L'\u00A8';  // ¨
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00CC'] = L'\u0060';  // `
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00CD'] = 171;
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00CE'] = L'\u005E';  // ^
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00CF'] = L'\u00A8';  // ¨
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00D2'] = L'\u0060';  // `
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00D3'] = 171;
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00D4'] = L'\u005E';  // ^
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00D5'] = L'\u007E';  // ~
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00D6'] = L'\u00A8';  // ¨
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00D9'] = L'\u0060';  // `
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00DA'] = 171;
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00DB'] = L'\u005E';  // ^
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00DC'] = L'\u00A8';  // ¨
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00E0'] = L'\u0060';  // `
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00E1'] = 171;
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00E2'] = L'\u005E';  // ^
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00E3'] = L'\u007E';  // ~
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00E4'] = L'\u00A8';  // ¨
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00E8'] = L'\u0060';  // `
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00E9'] = 171;
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00EA'] = L'\u005E';  // ^
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00EB'] = L'\u00A8';  // ¨
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00EC'] = L'\u0060';  // `
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00ED'] = 171;
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00EE'] = L'\u005E';  // ^
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00EF'] = L'\u00A8';  // ¨
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00F2'] = L'\u0060';  // `
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00F3'] = 171;
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00F4'] = L'\u005E';  // ^
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00F5'] = L'\u007E';  // ~
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00F6'] = L'\u00A8';  // ¨
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00F9'] = L'\u0060';  // `
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00FA'] = 171;
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00FB'] = L'\u005E';  // ^
    Font::m_hmUnicodeReplacementAcutesTable[L'\u00FC'] = L'\u00A8';  // ¨
}
