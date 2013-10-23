#ifndef _TEXTURE_ENGINE_H
#define _TEXTURE_ENGINE_H

#include "Texture.h"

class TextureEngine
{
public:
    // Singleton / destructor
    ~TextureEngine();
    static TextureEngine * getInstance()
    {
        if (m_pInstance == NULL) m_pInstance = new TextureEngine();
        return m_pInstance;
    };

    // Texture loading funtions
    Texture * loadTexture(string sFilename, bool bComposed = false, bool bMipmap = false, int ustart = -1, int uend = -1, int vstart = -1, int vend = -1);
    Texture * findTexture(string sFilename);
    void reloadAllTextures();

protected:
    TextureEngine();
    static TextureEngine * m_pInstance;

    void textureLoaded(Texture * pTex);

    std::vector<Texture*> m_AllTextures;
};

// Helper alias
#define _tex TextureEngine::getInstance()

#endif
