#ifndef _GEOMETRY_TEXT_H
#define _GEOMETRY_TEXT_H

#include "Geometry.h"

class GeometryText : public Geometry
{
public:
    GeometryText(string sText, int iFontId, VBType type);
    GeometryText(string sText, int iFontId, float fFontHeight, VBType type);
    virtual ~GeometryText();

    void display(CoordsScreen pos, Color color);
    void display(Coords3D pos, Color color);
    void setText(string sText, int iFontId);
    virtual void reload();

protected:
    int m_iNbQuads;
    string m_sText;
    int m_iTextLength;
    int m_iFontId;
    float m_fScale;
};

#endif
