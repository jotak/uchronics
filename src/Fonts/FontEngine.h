#ifndef _FONT_ENGINE_H
#define _FONT_ENGINE_H

#include "Font.h"

#define MAX_FONTS                   15
#define IS_VALID_FONTID(i)          (i <= 0 && i > -MAX_FONTS)
#define INVALID_FONTID              1

class FontEngine
{
public:
    // Singleton / destructor
    ~FontEngine();
    static FontEngine * getInstance()
    {
        if (m_pInstance == NULL) m_pInstance = new FontEngine();
        return m_pInstance;
    };

    // Font loading funtions
    void resetAllFonts();
    int registerFont(string sFontName);

    // Member access functions
    Font * getFont(int iIndex);

    // Misc
    int getStringLength(string sText, int iIndex);
    int getStringHeight(string sText, int iIndex);
    int getFontHeight(int iIndex);
    int putStringInBox(string sText, int iBoxWidth, int iIndex);
    CoordsScreen getCharacterPosition(int iPos, string sText, int iIndex);
    int getCharacterPosition(CoordsScreen cs, string sText, int iIndex);

private:
    FontEngine();
    static FontEngine * m_pInstance;

    Font * m_pAllFonts[MAX_FONTS];
};

// Helper alias
#define _font FontEngine::getInstance()

#endif
