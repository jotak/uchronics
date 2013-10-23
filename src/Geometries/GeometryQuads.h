#ifndef _GEOMETRY_QUADS_H
#define _GEOMETRY_QUADS_H

#include "Geometry.h"

class QuadData;

class GeometryQuads : public Geometry
{
public:
    GeometryQuads(int nQuads, QuadData ** pAllQuads, VBType type);
    GeometryQuads(QuadData * pQuad, VBType type);
    GeometryQuads(VBType type);
    virtual ~GeometryQuads();

    void display(CoordsScreen pos, Color color);
    void display(Coords3D pos, Color color);
    void modify(int nQuads, QuadData ** pAllQuads);
    void modify(QuadData * pQuad) { modify(1, &pQuad); };
    Texture * getTexture(int iQuad = 0);
    void setTexture(Texture * pTex, int iQuad = 0);
    void setOffset(CoordsScreen offset2D);
    virtual void reload();

protected:
    int m_iNbQuads;
    QuadData ** m_pAllQuads;
    Coords3D m_Offset;
};


class QuadData
{
    friend class GeometryQuads;
public:
    QuadData(int xstart, int xend, int ystart, int yend, string texture);
    QuadData(int xstart, int xend, int ystart, int yend, Texture * texture);
    QuadData(int xstart, int xend, int ystart, int yend, int ustart, int uend, int vstart, int vend, string texture);
    QuadData(float xstart, float xend, float ystart, float yend, string texture);
    QuadData(float xstart, float xend, float ystart, float yend, int ustart, int uend, int vstart, int vend, string texture);
    QuadData(float xstart, float xend, float ystart, float yend, float ustart, float uend, float vstart, float vend, Texture * texture);

    static void releaseQuads(int nQuads, QuadData ** pQuads);
    QuadData * clone();

    float m_fXStart;
    float m_fXEnd;
    float m_fYStart;
    float m_fYEnd;
    float m_fUStart;
    float m_fUEnd;
    float m_fVStart;
    float m_fVEnd;
    Texture * m_pTex;

private:
    void init(Coords3D d3start, Coords3D d3end, Texture * texture);
};

#endif
