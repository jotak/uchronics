#ifndef _GEOMETRY_ENGINE_H
#define _GEOMETRY_ENGINE_H

#include "../Utils/BaseObject.h"
#include "GeometryModifier.h"
#include "../Display/Texture.h"

#pragma pack(push, 1)
class Vertex
{
public:
    void set(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);

    GLfloat u, v;
    GLfloat x, y, z;
};
#pragma pack(pop)

enum VBType
{
    VB_Static = 0,
    VB_Stream,
    VB_Dynamic
};

class Geometry : public BaseObject
{
public:
    Geometry(VBType type);
    virtual ~Geometry();

    // Update function
    virtual void update(double delta);

    // Display functions
    virtual void display(CoordsScreen pos, Color color) {};
    virtual void display(Coords3D pos, Color color) {};

    // Mods functions
    void bindModifier(GeometryModifier * pMod);

    // Shaders
    bool bindShader(string sVertexShader, string sPixelShader);
    bool isShaderActive() { return m_bShaderEnabled; };
    void activateShader();
    void deactivateShader();
    int registerShaderVariable(string name);
    void beginSetShaderVariables();
    void endSetShaderVariables();
    void setShaderInt(int iVarId, int val);
    void setShaderFloat(int iVarId, float val);
    void setShaderIvec2(int iVarId, CoordsScreen val);
    void setShaderVec3(int iVarId, Coords3D val);

    // Misc.
    virtual void reload() = 0;

protected:
    void doModTransforms(Color * pColor);

    GLuint m_VboId;
    VBType m_Type;
    list<GeometryModifier*> m_lstMods;
    GLuint m_uShaderProgram;
    GLuint m_uPxShader;
    GLuint m_uVxShader;
    bool m_bShaderEnabled;
    bool m_bShaderLoaded;
};

#endif
