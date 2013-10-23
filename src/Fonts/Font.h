#ifndef _FONT_H
#define _FONT_H

#include "../Utils/utils.h"

class Texture;
class CharDescriptor
{
public:
    int x, y, width, height, xoffset, yoffset, xadvance;
    char c;
};

class Font
{
public:
    Font();

    bool load(string sFontName);
    string getFontName() { return m_sFontName; };
    int getStringLength(string sText);
    u8 getFontHeight() { return m_uFontHeight; };
    int putStringInBox(string sText, int iBoxWidth);
    CharDescriptor * findCharDescriptor(char c, bool checkAcute = true);
    CharDescriptor * getLastAcuteDescriptor() { return m_pLastAcute; };
    Texture * getTexture() { return m_pTex; };
    CoordsScreen getCharacterPosition(int iPos, string sText);
    int getCharacterPosition(CoordsScreen cs, string sText);

    static void initUnicodeTables();

private:
    void storeData(string, string, string);

    string m_sFontName;
    u8 m_uFontHeight;
    CharDescriptor m_AllChars[256];
    int m_iNbDescriptor;
    Texture * m_pTex;
    CharDescriptor * m_pLastAcute;

    static ch_hash m_hmUnicodeReplacementTable;
    static ch_hash m_hmUnicodeReplacementAcutesTable;
};

#endif
