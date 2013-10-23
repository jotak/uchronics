// -----------------------------------------------------------------
// Geometry and Vertex
// -----------------------------------------------------------------
#include "Geometry.h"
#include "../Display/DisplayEngine.h"

// -----------------------------------------------------------------
// Name : Vertex::set
// -----------------------------------------------------------------
void Vertex::set(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->u = u;
    this->v = v;
}

// -----------------------------------------------------------------
// Name : Geometry
//  Constructor
// -----------------------------------------------------------------
Geometry::Geometry(VBType type)
{
    m_Type = type;
    _display->registerGeometry(this);
    m_uShaderProgram = 0;
    m_uPxShader = m_uVxShader = 0;
    m_bShaderEnabled = false;
    m_bShaderLoaded = false;
}

// -----------------------------------------------------------------
// Name : ~Geometry
//  Destructor
// -----------------------------------------------------------------
Geometry::~Geometry()
{
    if (m_uShaderProgram != 0) {
        glDeleteProgram(m_uShaderProgram);
    }
    if (m_uPxShader != 0) {
        glDeleteShader(m_uPxShader);
    }
    if (m_uVxShader != 0) {
        glDeleteShader(m_uVxShader);
    }
    _display->unregisterGeometry(this);
    FREEVEC(m_lstMods);
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void Geometry::update(double delta)
{
    // Update display mods
	for (GeometryModifier* &pMod : m_lstMods) {
        if (pMod->isActive() && pMod->isRunning()) {
        	pMod->update(delta);
        }
    }
}

// -----------------------------------------------------------------
// Name : bindModifier
// -----------------------------------------------------------------
void Geometry::bindModifier(GeometryModifier * pMod)
{
	m_lstMods.push_front(pMod);
}

// -----------------------------------------------------------------
// Name : doModTransforms
// -----------------------------------------------------------------
void Geometry::doModTransforms(Color * pColor)
{
	for (GeometryModifier* &pMod : m_lstMods) {
        if (pMod->isActive()) {
        	pMod->doTransforms(pColor);
        }
    }
}

// -----------------------------------------------------------------
// Name : bindShader
// -----------------------------------------------------------------
bool Geometry::bindShader(string sVertexShader, string sPixelShader)
{
    m_bShaderLoaded = false;
    m_bShaderEnabled = false;
    if (m_uShaderProgram != 0)
    {
        glDeleteProgram(m_uShaderProgram);
        m_uShaderProgram = 0;
    }
    if (m_uVxShader != 0)
    {
        glDeleteShader(m_uVxShader);
        m_uVxShader = 0;
    }
    if (m_uPxShader != 0)
    {
        glDeleteShader(m_uPxShader);
        m_uPxShader = 0;
    }
    if (sVertexShader != "")
    {
        if (!_display->loadShader(&m_uVxShader, GL_VERTEX_SHADER, sVertexShader)) {
            return false;
        }
    }
    if (sPixelShader != "")
    {
        if (!_display->loadShader(&m_uPxShader, GL_FRAGMENT_SHADER, sPixelShader)) {
            return false;
        }
    }
    m_bShaderLoaded = _display->linkShaders(&m_uShaderProgram, m_uVxShader, m_uPxShader);
    return m_bShaderLoaded;
}

// -----------------------------------------------------------------
// Name : activateShader
// -----------------------------------------------------------------
void Geometry::activateShader()
{
    if (m_bShaderLoaded) {
        m_bShaderEnabled = true;
    }
}

// -----------------------------------------------------------------
// Name : deactivateShader
// -----------------------------------------------------------------
void Geometry::deactivateShader()
{
    m_bShaderEnabled = false;
}

// -----------------------------------------------------------------
// Name : registerShaderVariable
// -----------------------------------------------------------------
int Geometry::registerShaderVariable(string name)
{
    if (m_bShaderLoaded) {
        return (int) glGetUniformLocation(m_uShaderProgram, name.c_str());
    }
    return 0;
}

// -----------------------------------------------------------------
// Name : beginSetShaderVariables
// -----------------------------------------------------------------
void Geometry::beginSetShaderVariables()
{
    if (m_bShaderLoaded) {
        glUseProgram(m_uShaderProgram);
    }
}

// -----------------------------------------------------------------
// Name : endSetShaderVariables
// -----------------------------------------------------------------
void Geometry::endSetShaderVariables()
{
    if (m_bShaderLoaded) {
        glUseProgram(0);
    }
}

// -----------------------------------------------------------------
// Name : setShaderInt
// -----------------------------------------------------------------
void Geometry::setShaderInt(int iVarId, int val)
{
    if (m_bShaderLoaded) {
        glUniform1i((GLint) iVarId, val);
    }
}

// -----------------------------------------------------------------
// Name : setShaderFloat
// -----------------------------------------------------------------
void Geometry::setShaderFloat(int iVarId, float val)
{
    if (m_bShaderLoaded) {
        glUniform1f((GLint) iVarId, val);
    }
}

// -----------------------------------------------------------------
// Name : setShaderIvec2
// -----------------------------------------------------------------
void Geometry::setShaderIvec2(int iVarId, CoordsScreen val)
{
    if (m_bShaderLoaded) {
        glUniform2i((GLint) iVarId, val.x, val.y);
    }
}

// -----------------------------------------------------------------
// Name : setShaderVec3
// -----------------------------------------------------------------
void Geometry::setShaderVec3(int iVarId, Coords3D val)
{
    if (m_bShaderLoaded) {
        glUniform3f((GLint) iVarId, val.x, val.y, val.z);
    }
}
