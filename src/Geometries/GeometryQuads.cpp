// -----------------------------------------------------------------
// GeometryQuads
// -----------------------------------------------------------------
#include "GeometryQuads.h"
#include "../Display/DisplayEngine.h"
#include "../Display/TextureEngine.h"

// -----------------------------------------------------------------
// Name : GeometryQuads
//  Constructor
// -----------------------------------------------------------------
GeometryQuads::GeometryQuads(int nQuads, QuadData ** pAllQuads, VBType type) : Geometry(type)
{
    m_VboId = 0;
    m_iNbQuads = 0;
    modify(nQuads, pAllQuads);
}

// -----------------------------------------------------------------
// Name : GeometryQuads
//  Constructor
// -----------------------------------------------------------------
GeometryQuads::GeometryQuads(QuadData * pQuad, VBType type) : Geometry(type)
{
    m_VboId = 0;
    m_iNbQuads = 0;
    modify(1, &pQuad);
}

// -----------------------------------------------------------------
// Name : GeometryQuads
//  Constructor
// -----------------------------------------------------------------
GeometryQuads::GeometryQuads(VBType type) : Geometry(type)
{
    m_VboId = 0;
    m_iNbQuads = 0;
}

// -----------------------------------------------------------------
// Name : ~GeometryQuads
//  Destructor
// -----------------------------------------------------------------
GeometryQuads::~GeometryQuads()
{
    if (m_VboId > 0) {
        glDeleteBuffers(1, &m_VboId);
    }
    if (m_iNbQuads > 0)
    {
        for (int i = 0; i < m_iNbQuads; i++) {
            delete m_pAllQuads[i];
        }
        delete[] m_pAllQuads;
    }
}

// -----------------------------------------------------------------
// Name : display
// -----------------------------------------------------------------
void GeometryQuads::display(CoordsScreen pos, Color color)
{
    if (m_iNbQuads == 0) {
        return;
    }

    Coords3D d3Coords = _display->getGUI3D(pos);
    display(d3Coords, color);
}

// -----------------------------------------------------------------
// Name : display
// -----------------------------------------------------------------
void GeometryQuads::display(Coords3D pos, Color color)
{
    if (m_iNbQuads == 0) {
        return;
    }

    pos = pos - m_Offset;

    glEnable(GL_TEXTURE_2D);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(2 * sizeof(GLfloat)));
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glPushMatrix();
    glTranslatef(pos.x, pos.y, FARPLANE - pos.z);
    doModTransforms(&color);

    if (m_bShaderEnabled) {
        glUseProgram(m_uShaderProgram);
    }

    glColor4f(color.r, color.g, color.b, color.a);
    for (int i = 0; i < m_iNbQuads; i++) {
        glBindTexture(GL_TEXTURE_2D, m_pAllQuads[i]->m_pTex->getGlid());
        glDrawArrays(GL_QUADS, 4*i, 4);
    }

    if (m_bShaderEnabled) {
        glUseProgram(0);
    }

	glPopMatrix();
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_TEXTURE_2D);
}

// -----------------------------------------------------------------
// Name : modify
// -----------------------------------------------------------------
void GeometryQuads::modify(int nQuads, QuadData ** pAllQuads)
{
    GLenum glType;
    switch (m_Type)
    {
    case VB_Static:
        glType = GL_STATIC_DRAW;
        break;
    case VB_Dynamic:
        glType = GL_DYNAMIC_DRAW;
        break;
    case VB_Stream:
    default:
        glType = GL_STREAM_DRAW;
        break;
    }
    if (m_iNbQuads > 0)
    {
        for (int i = 0; i < m_iNbQuads; i++) {
            delete m_pAllQuads[i];
        }
        delete[] m_pAllQuads;
    }
    if (!glIsBuffer(m_VboId)) {
        glGenBuffers(1, &m_VboId);
    } else if (m_iNbQuads != nQuads) {
        glDeleteBuffers(1, &m_VboId);
        glGenBuffers(1, &m_VboId);
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
    m_iNbQuads = nQuads;
    if (m_iNbQuads == 0)
    {
        m_pAllQuads = NULL;
        return;
    }

    m_pAllQuads = new QuadData*[m_iNbQuads];
    Vertex * vertices = new Vertex[m_iNbQuads*4];
    for (int i = 0; i < m_iNbQuads; i++)
    {
        // QuadData
        m_pAllQuads[i] = pAllQuads[i]->clone();

        // Vertex
        vertices[4*i+0].set(pAllQuads[i]->m_fXStart, pAllQuads[i]->m_fYEnd  , 0.0f, pAllQuads[i]->m_fUStart, pAllQuads[i]->m_fVStart);
        vertices[4*i+1].set(pAllQuads[i]->m_fXEnd  , pAllQuads[i]->m_fYEnd  , 0.0f, pAllQuads[i]->m_fUEnd  , pAllQuads[i]->m_fVStart);
        vertices[4*i+2].set(pAllQuads[i]->m_fXEnd  , pAllQuads[i]->m_fYStart, 0.0f, pAllQuads[i]->m_fUEnd  , pAllQuads[i]->m_fVEnd  );
        vertices[4*i+3].set(pAllQuads[i]->m_fXStart, pAllQuads[i]->m_fYStart, 0.0f, pAllQuads[i]->m_fUStart, pAllQuads[i]->m_fVEnd  );
    }
    glBufferData(GL_ARRAY_BUFFER, m_iNbQuads * 4 * sizeof(Vertex), vertices, glType);
    delete[] vertices;
}

// -----------------------------------------------------------------
// Name : getTexture
// -----------------------------------------------------------------
Texture * GeometryQuads::getTexture(int iQuad)
{
    return m_pAllQuads[iQuad]->m_pTex;
}

// -----------------------------------------------------------------
// Name : setTexture
// -----------------------------------------------------------------
void GeometryQuads::setTexture(Texture * pTex, int iQuad)
{
    m_pAllQuads[iQuad]->m_pTex = pTex;
}

// -----------------------------------------------------------------
// Name : reload
// -----------------------------------------------------------------
void GeometryQuads::reload()
{
    GLenum glType;
    switch (m_Type)
    {
    case VB_Static:
        glType = GL_STATIC_DRAW;
        break;
    case VB_Dynamic:
        glType = GL_DYNAMIC_DRAW;
        break;
    case VB_Stream:
    default:
        glType = GL_STREAM_DRAW;
        break;
    }
    glGenBuffers(1, &m_VboId);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
    if (m_iNbQuads == 0) {
        return;
    }

    Vertex * vertices = new Vertex[m_iNbQuads*4];
    for (int i = 0; i < m_iNbQuads; i++)
    {
        // Vertex
        vertices[4*i+0].set(m_pAllQuads[i]->m_fXStart, m_pAllQuads[i]->m_fYEnd  , 0.0f, m_pAllQuads[i]->m_fUStart, m_pAllQuads[i]->m_fVStart);
        vertices[4*i+1].set(m_pAllQuads[i]->m_fXEnd  , m_pAllQuads[i]->m_fYEnd  , 0.0f, m_pAllQuads[i]->m_fUEnd  , m_pAllQuads[i]->m_fVStart);
        vertices[4*i+2].set(m_pAllQuads[i]->m_fXEnd  , m_pAllQuads[i]->m_fYStart, 0.0f, m_pAllQuads[i]->m_fUEnd  , m_pAllQuads[i]->m_fVEnd  );
        vertices[4*i+3].set(m_pAllQuads[i]->m_fXStart, m_pAllQuads[i]->m_fYStart, 0.0f, m_pAllQuads[i]->m_fUStart, m_pAllQuads[i]->m_fVEnd  );
    }
    glBufferData(GL_ARRAY_BUFFER, m_iNbQuads * 4 * sizeof(Vertex), vertices, glType);
    delete[] vertices;
}

// -----------------------------------------------------------------
// Name : setOffset
// -----------------------------------------------------------------
void GeometryQuads::setOffset(CoordsScreen offset2D)
{
	// Works only for a single quad
	if (m_iNbQuads == 1) {
		m_Offset.x = ((float)offset2D.x / (float)m_pAllQuads[0]->m_pTex->getWidth()) * (m_pAllQuads[0]->m_fXEnd - m_pAllQuads[0]->m_fXStart);
		m_Offset.y = ((float)offset2D.y / (float)m_pAllQuads[0]->m_pTex->getHeight()) * (m_pAllQuads[0]->m_fYEnd - m_pAllQuads[0]->m_fYStart);
	}
}


// -----------------------------------------------------------------
// Name : QuadData
//  Constructor
// -----------------------------------------------------------------
QuadData::QuadData(int xstart, int xend, int ystart, int yend, string texture)
{
	init(
			_display->getGUI3D(CoordsScreen(xstart, ystart)),
			_display->getGUI3D(CoordsScreen(xend, yend)),
			_tex->loadTexture(texture));
}

// -----------------------------------------------------------------
// Name : QuadData
//  Constructor
// -----------------------------------------------------------------
QuadData::QuadData(int xstart, int xend, int ystart, int yend, Texture * texture)
{
	init(
			_display->getGUI3D(CoordsScreen(xstart, ystart)),
			_display->getGUI3D(CoordsScreen(xend, yend)),
			texture);
}

// -----------------------------------------------------------------
// Name : QuadData
//  Constructor
// -----------------------------------------------------------------
QuadData::QuadData(int xstart, int xend, int ystart, int yend, int ustart, int uend, int vstart, int vend, string texture)
{
	init(
			_display->getGUI3D(CoordsScreen(xstart, ystart)),
			_display->getGUI3D(CoordsScreen(xend, yend)),
			_tex->loadTexture(texture, false, false, ustart, uend, vstart, vend));
}

// -----------------------------------------------------------------
// Name : QuadData
//  Constructor
// -----------------------------------------------------------------
QuadData::QuadData(float xstart, float xend, float ystart, float yend, string texture)
{
	init(
			Coords3D(xstart, ystart),
			Coords3D(xend, yend),
			_tex->loadTexture(texture));
}

// -----------------------------------------------------------------
// Name : QuadData
//  Constructor
// -----------------------------------------------------------------
QuadData::QuadData(float xstart, float xend, float ystart, float yend, int ustart, int uend, int vstart, int vend, string texture)
{
	init(
			Coords3D(xstart, ystart),
			Coords3D(xend, yend),
			_tex->loadTexture(texture, false, false, ustart, uend, vstart, vend));
}

// -----------------------------------------------------------------
// Name : QuadData
//  Constructor
// -----------------------------------------------------------------
QuadData::QuadData(float xstart, float xend, float ystart, float yend, float ustart, float uend, float vstart, float vend, Texture * texture)
{
    m_fXStart = xstart;
    m_fYStart = ystart;
    m_fXEnd = xend;
    m_fYEnd = yend;
    m_pTex = texture;
    m_fUStart = ustart;
    m_fUEnd = uend;
    m_fVStart = vstart;
    m_fVEnd = vend;
}

// -----------------------------------------------------------------
// Name : init
// -----------------------------------------------------------------
void QuadData::init(Coords3D d3start, Coords3D d3end, Texture * texture)
{
    m_fXStart = d3start.x;
    m_fYStart = d3start.y;
    m_fXEnd = d3end.x;
    m_fYEnd = d3end.y;
    m_pTex = texture;
    m_fUStart = m_pTex->getU0();
    m_fUEnd = m_pTex->getU1();
    m_fVStart = m_pTex->getV0();
    m_fVEnd = m_pTex->getV1();
}

// -----------------------------------------------------------------
// Name : clone
// -----------------------------------------------------------------
QuadData * QuadData::clone()
{
    return new QuadData(m_fXStart, m_fXEnd, m_fYStart, m_fYEnd, m_fUStart, m_fUEnd, m_fVStart, m_fVEnd, m_pTex);
}

// -----------------------------------------------------------------
// Name : releaseQuads
//  Static function
// -----------------------------------------------------------------
void QuadData::releaseQuads(int nQuads, QuadData ** pQuads)
{
    for (int i = 0; i < nQuads; i++) {
        delete pQuads[i];
    }
    delete[] pQuads;
}
