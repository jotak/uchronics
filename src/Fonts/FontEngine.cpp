// -----------------------------------------------------------------
// Font Engine
// -----------------------------------------------------------------
#include "FontEngine.h"
#include "../Display/DisplayEngine.h"
#include "../Display/TextureEngine.h"
#include "../Managers/DebugManager.h"

FontEngine * FontEngine::m_pInstance = NULL;

// -----------------------------------------------------------------
// Name : FontEngine
//  Constructor
// -----------------------------------------------------------------
FontEngine::FontEngine()
{
	for (int i = 0; i < MAX_FONTS; i++) {
		m_pAllFonts[i] = NULL;
	}
}

// -----------------------------------------------------------------
// Name : ~FontEngine
//  Destructor
// -----------------------------------------------------------------
FontEngine::~FontEngine()
{
    for (int i = 0; i < MAX_FONTS; i++)
    {
        if (m_pAllFonts[i] != NULL) {
            delete m_pAllFonts[i];
        }
    }
}

// -----------------------------------------------------------------
// Name : resetAllFonts
// -----------------------------------------------------------------
void FontEngine::resetAllFonts()
{
    for (int i = 0; i < MAX_FONTS; i++)
    {
        if (m_pAllFonts[i] != NULL)
        {
            delete m_pAllFonts[i];
            m_pAllFonts[i] = NULL;
        }
    }
}

// -----------------------------------------------------------------
// Name : registerFont
//  return negative number if success, to avoid confusion with guiObject::FontId
// -----------------------------------------------------------------
int FontEngine::registerFont(string sFontName)
{
    int iFont = 0;
    while (m_pAllFonts[iFont] != NULL)
    {
        if (m_pAllFonts[iFont]->getFontName() == sFontName) {
            return -iFont;
        }
        iFont++;
        if (iFont == MAX_FONTS) {
            return 1;
        }
    }
    m_pAllFonts[iFont] = new Font();
    bool ok = m_pAllFonts[iFont]->load(sFontName);
    if (!ok)
    {
        _debug->error("Error when loading font " + sFontName);
        delete m_pAllFonts[iFont];
        m_pAllFonts[iFont] = NULL;
        return 1;
    } else {
        return -iFont;
    }
}

// -----------------------------------------------------------------
// Name : getFont
// -----------------------------------------------------------------
Font * FontEngine::getFont(int iIndex)
{
    assert(IS_VALID_FONTID(iIndex));
    return m_pAllFonts[-iIndex];
}

// ------------------------------------------------------------------
// Name : getStringLength
// ------------------------------------------------------------------
int FontEngine::getStringLength(string sText, int iIndex)
{
    assert(IS_VALID_FONTID(iIndex));
    assert(m_pAllFonts[-iIndex] != NULL);
    return m_pAllFonts[-iIndex]->getStringLength(sText);
}

// ------------------------------------------------------------------
// Name : getStringHeight
// ------------------------------------------------------------------
int FontEngine::getStringHeight(string sText, int iIndex)
{
    assert(IS_VALID_FONTID(iIndex));
    assert(m_pAllFonts[-iIndex] != NULL);
    int nbLines = 1;
    int i = 0;
    while (sText[i] != '\0')
    {
        if (sText[i++] == '\n') {
            nbLines++;
        }
    }
    return m_pAllFonts[-iIndex]->getFontHeight() * nbLines;
}

// ------------------------------------------------------------------
// Name : putStringInBox
// ------------------------------------------------------------------
int FontEngine::putStringInBox(string sText, int iBoxWidth, int iIndex)
{
    assert(IS_VALID_FONTID(iIndex));
    assert(m_pAllFonts[-iIndex] != NULL);
    return m_pAllFonts[-iIndex]->putStringInBox(sText, iBoxWidth);
}

// ------------------------------------------------------------------
// Name : getFontHeight
// ------------------------------------------------------------------
int FontEngine::getFontHeight(int iIndex)
{
    assert(IS_VALID_FONTID(iIndex));
    assert(m_pAllFonts[-iIndex] != NULL);
    return m_pAllFonts[-iIndex]->getFontHeight();
}

// ------------------------------------------------------------------
// Name : getCharacterPosition
// ------------------------------------------------------------------
CoordsScreen FontEngine::getCharacterPosition(int iPos, string sText, int iIndex)
{
    assert(IS_VALID_FONTID(iIndex));
    assert(m_pAllFonts[-iIndex] != NULL);
    return m_pAllFonts[-iIndex]->getCharacterPosition(iPos, sText);
}

// ------------------------------------------------------------------
// Name : getCharacterPosition
// ------------------------------------------------------------------
int FontEngine::getCharacterPosition(CoordsScreen cs, string sText, int iIndex)
{
    assert(IS_VALID_FONTID(iIndex));
    assert(m_pAllFonts[-iIndex] != NULL);
    return m_pAllFonts[-iIndex]->getCharacterPosition(cs, sText);
}
