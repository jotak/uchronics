#ifndef _GEOMETRY_CYLINDER_H
#define _GEOMETRY_CYLINDER_H

#include "Geometry.h"

class GLUQuadric;

class GeometryCylinder : public Geometry
{
public:
    GeometryCylinder(float fDiameter, float fHeight, u16 uSlices, Texture * pTex, VBType type);
    virtual ~GeometryCylinder();

    void display(CoordsScreen pos, Color color, Color borderColor);
    void display(Coords3D pos, Color color, Color borderColor);
    void modify(float fDiameter, float fHeight, u16 uSlices, Texture * pTex);
    Texture * getTexture();
    void setTexture(Texture * pTex);
    virtual void reload();

protected:
    Texture * m_pTex;
    Texture * m_pTopTex;
    Texture * m_pRoundTex;
    GLuint m_BordersVboId;
    GLUquadric * m_pQuadric;
    float m_fDiameter;
    float m_fHeight;
    u16 m_uSlices;
};

#endif
