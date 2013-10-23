#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "../Utils/utils.h"
#include "../../../joson/JoSon.h"

class Texture
{
public:
	friend class TextureEngine;

    Texture(string sFilename, bool bComposed, bool bMipMap);
    virtual ~Texture();

    virtual bool load();
    void unload();

    string getFilename() { return m_sFilename; };
    GLuint getWidth() { return m_iWidth; };
    GLuint getHeight() { return m_iHeight; };
    GLfloat getU0() { return m_fU0; };
    GLfloat getU1() { return m_fU1; };
    GLfloat getV0() { return m_fV0; };
    GLfloat getV1() { return m_fV1; };
    GLenum getFormat() { return m_Format; };
    GLint getBpp() { return m_iBpp; };
    GLuint getGlid() { return m_uGlId; };
    GLubyte * getTexels() { return m_pTexels; };
    bool isLoadedInVideo() { return m_bIsLoadedInVideo; };
    bool isMipMap() { return m_bMipMap; };
    const JoSon& getComposedDescriptor() { return *m_pComposedDescriptor; };

protected:
    string m_sFilename;
    GLuint m_iWidth, m_iHeight;
    GLfloat m_fU0, m_fU1, m_fV0, m_fV1;
    GLenum m_Format;          // RVB, RVBA, Luminance, Luminance Alpha
    GLint m_iBpp;             // Bits per pixels
    GLuint m_uGlId;
    GLubyte * m_pTexels;      // data
    bool m_bIsLoadedInVideo;
    JoSon * m_pComposedDescriptor;
    bool m_bMipMap;
};

#endif
