// -----------------------------------------------------------------
// GeometryText
// -----------------------------------------------------------------
#include "GeometryText.h"
#include "../Display/DisplayEngine.h"
#include "../Display/TextureEngine.h"
#include "../Fonts/FontEngine.h"

// -----------------------------------------------------------------
// Name : GeometryText
//  Constructor
// -----------------------------------------------------------------
GeometryText::GeometryText(string sText, int iFontId, VBType type) : Geometry(type)
{
    m_VboId = 0;
    m_sText = string("");
    m_fScale = -1;
    setText(sText, iFontId);
}

// -----------------------------------------------------------------
// Name : GeometryText
//  Constructor
// -----------------------------------------------------------------
GeometryText::GeometryText(string sText, int iFontId, float fFontHeight, VBType type) : Geometry(type)
{
    m_VboId = 0;
    m_sText = string("");
    Font * pFont = _font->getFont(iFontId);
    if (pFont) {
        Coords3D p = _display->getGUI3D(CoordsScreen(0, pFont->getFontHeight()));
        m_fScale = fFontHeight / p.y;
    } else {
        m_fScale = -1;
    }
    setText(sText, iFontId);
}

// -----------------------------------------------------------------
// Name : ~GeometryText
//  Destructor
// -----------------------------------------------------------------
GeometryText::~GeometryText()
{
    if (m_VboId > 0) {
        glDeleteBuffers(1, &m_VboId);
    }
}

// -----------------------------------------------------------------
// Name : display
// -----------------------------------------------------------------
void GeometryText::display(CoordsScreen position, Color color)
{
    Coords3D d3Coords = _display->getGUI3D(position);
    display(d3Coords, color);
}

// -----------------------------------------------------------------
// Name : display
// -----------------------------------------------------------------
void GeometryText::display(Coords3D d3Coords, Color color)
{
    glEnable(GL_TEXTURE_2D);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboId);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(2 * sizeof(GLfloat)));
    glColor4f(color.r, color.g, color.b, color.a);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, _font->getFont(m_iFontId)->getTexture()->getGlid());

    if (m_bShaderEnabled) {
        glUseProgram(m_uShaderProgram);
    }

    glPushMatrix();
    {
        glTranslatef(d3Coords.x, d3Coords.y, FARPLANE - d3Coords.z);
        glDrawArrays(GL_QUADS, 0, 4*m_iNbQuads);
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
// Name : setText
// -----------------------------------------------------------------
void GeometryText::setText(string sText, int iFontId)
{
    m_iTextLength = sText.length() + 1;
    m_sText = sText;
    m_iFontId = iFontId;
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
    if (glIsBuffer(m_VboId)) {
        glDeleteBuffers(1, &m_VboId);
    }
    glGenBuffers(1, &m_VboId);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboId);

    Font * pFont = _font->getFont(iFontId);
    float fTexWidth = (float) pFont->getTexture()->getWidth();
    float fTexHeight = (float) pFont->getTexture()->getHeight();
    int iXPxl = 0;
    int iYPxl = 0;
    int iLength = sText.length();
    m_iNbQuads = 0;
    Vertex * vertices = new Vertex[iLength * 8];  // double number of vertice, because of acutes
    for (int i = 0; i < iLength; i++)
    {
        if (sText[i] == '\n')
        {
            iXPxl = 0;
            iYPxl += pFont->getFontHeight();
            continue;
        }
        CharDescriptor * charDesc = pFont->findCharDescriptor(sText[i]);
        if (charDesc == NULL)
            continue;
        CharDescriptor * acuteDesc = pFont->getLastAcuteDescriptor();

        Coords3D p0 = _display->getGUI3D(CoordsScreen(iXPxl + charDesc->xoffset, iYPxl + charDesc->yoffset));
        Coords3D p1 = _display->getGUI3D(CoordsScreen(iXPxl + charDesc->xoffset + charDesc->width, iYPxl + charDesc->yoffset + charDesc->height));
        if (m_fScale > 0)
        {
            p0.x *= m_fScale;
            p0.y *= m_fScale;
            p1.x *= m_fScale;
            p1.y *= m_fScale;
        }
        float fSrcLeft = (float)charDesc->x / fTexWidth;
        float fSrcRight = (float)(charDesc->x + charDesc->width) / fTexWidth;
        float fSrcTop = (float)charDesc->y / fTexHeight;
        float fSrcBottom = (float)(charDesc->y + charDesc->height) / fTexHeight;
        vertices[4*m_iNbQuads].set(p0.x, p0.y, 0.0f, fSrcLeft, fSrcTop);
        vertices[4*m_iNbQuads+1].set(p1.x, p0.y, 0.0f, fSrcRight, fSrcTop);
        vertices[4*m_iNbQuads+2].set(p1.x, p1.y, 0.0f, fSrcRight, fSrcBottom);
        vertices[4*m_iNbQuads+3].set(p0.x, p1.y, 0.0f, fSrcLeft, fSrcBottom);
        m_iNbQuads++;

        if (acuteDesc != NULL)  // acute?
        {
            Coords3D p0 = _display->getGUI3D(CoordsScreen(iXPxl + acuteDesc->xoffset, iYPxl + acuteDesc->yoffset));
            Coords3D p1 = _display->getGUI3D(CoordsScreen(iXPxl + acuteDesc->xoffset + acuteDesc->width, iYPxl + acuteDesc->yoffset + acuteDesc->height));
            float fSrcLeft = (float)acuteDesc->x / fTexWidth;
            float fSrcRight = (float)(acuteDesc->x + acuteDesc->width) / fTexWidth;
            float fSrcTop = (float)acuteDesc->y / fTexHeight;
            float fSrcBottom = (float)(acuteDesc->y + acuteDesc->height) / fTexHeight;
            vertices[4*m_iNbQuads].set(p0.x, p0.y, 0.0f, fSrcLeft, fSrcTop);
            vertices[4*m_iNbQuads+1].set(p1.x, p0.y, 0.0f, fSrcRight, fSrcTop);
            vertices[4*m_iNbQuads+2].set(p1.x, p1.y, 0.0f, fSrcRight, fSrcBottom);
            vertices[4*m_iNbQuads+3].set(p0.x, p1.y, 0.0f, fSrcLeft, fSrcBottom);
            m_iNbQuads++;
        }
        iXPxl += charDesc->xadvance;
    }
    glBufferData(GL_ARRAY_BUFFER, m_iNbQuads * 4 * sizeof(Vertex), vertices, glType);
    delete[] vertices;
}

// -----------------------------------------------------------------
// Name : reload
// -----------------------------------------------------------------
void GeometryText::reload()
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

    Font * pFont = _font->getFont(m_iFontId);
    float fTexWidth = (float) pFont->getTexture()->getWidth();
    float fTexHeight = (float) pFont->getTexture()->getHeight();
    int iXPxl = 0;
    int iYPxl = 0;
    int iLength = m_sText.length();
    m_iNbQuads = 0;
    Vertex * vertices = new Vertex[iLength * 8];  // double number of vertice, because of acutes
    for (int i = 0; i < iLength; i++)
    {
        if (m_sText[i] == '\n')
        {
            iXPxl = 0;
            iYPxl += pFont->getFontHeight();
            continue;
        }
        CharDescriptor * charDesc = pFont->findCharDescriptor(m_sText[i]);
        if (charDesc == NULL)
            continue;
        CharDescriptor * acuteDesc = pFont->getLastAcuteDescriptor();

        Coords3D p0 = _display->getGUI3D(CoordsScreen(iXPxl + charDesc->xoffset, iYPxl + charDesc->yoffset));
        Coords3D p1 = _display->getGUI3D(CoordsScreen(iXPxl + charDesc->xoffset + charDesc->width, iYPxl + charDesc->yoffset + charDesc->height));
        float fSrcLeft = (float)charDesc->x / fTexWidth;
        float fSrcRight = (float)(charDesc->x + charDesc->width) / fTexWidth;
        float fSrcTop = (float)charDesc->y / fTexHeight;
        float fSrcBottom = (float)(charDesc->y + charDesc->height) / fTexHeight;
        vertices[4*m_iNbQuads].set(p0.x, p0.y, 0.0f, fSrcLeft, fSrcTop);
        vertices[4*m_iNbQuads+1].set(p1.x, p0.y, 0.0f, fSrcRight, fSrcTop);
        vertices[4*m_iNbQuads+2].set(p1.x, p1.y, 0.0f, fSrcRight, fSrcBottom);
        vertices[4*m_iNbQuads+3].set(p0.x, p1.y, 0.0f, fSrcLeft, fSrcBottom);
        m_iNbQuads++;

        if (acuteDesc != NULL)  // acute?
        {
            Coords3D p0 = _display->getGUI3D(CoordsScreen(iXPxl + acuteDesc->xoffset, iYPxl + acuteDesc->yoffset));
            Coords3D p1 = _display->getGUI3D(CoordsScreen(iXPxl + acuteDesc->xoffset + acuteDesc->width, iYPxl + acuteDesc->yoffset + acuteDesc->height));
            float fSrcLeft = (float)acuteDesc->x / fTexWidth;
            float fSrcRight = (float)(acuteDesc->x + acuteDesc->width) / fTexWidth;
            float fSrcTop = (float)acuteDesc->y / fTexHeight;
            float fSrcBottom = (float)(acuteDesc->y + acuteDesc->height) / fTexHeight;
            vertices[4*m_iNbQuads].set(p0.x, p0.y, 0.0f, fSrcLeft, fSrcTop);
            vertices[4*m_iNbQuads+1].set(p1.x, p0.y, 0.0f, fSrcRight, fSrcTop);
            vertices[4*m_iNbQuads+2].set(p1.x, p1.y, 0.0f, fSrcRight, fSrcBottom);
            vertices[4*m_iNbQuads+3].set(p0.x, p1.y, 0.0f, fSrcLeft, fSrcBottom);
            m_iNbQuads++;
        }
        iXPxl += charDesc->xadvance;
    }
    glBufferData(GL_ARRAY_BUFFER, m_iNbQuads * 4 * sizeof(Vertex), vertices, glType);
    delete[] vertices;
}
