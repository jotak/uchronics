// -----------------------------------------------------------------
// Texture Engine
// -----------------------------------------------------------------
#include "TextureEngine.h"
#include "VirtualTexture.h"
#include "../Managers/DebugManager.h"

TextureEngine * TextureEngine::m_pInstance = NULL;

// -----------------------------------------------------------------
// Name : TextureEngine
//  Constructor
// -----------------------------------------------------------------
TextureEngine::TextureEngine()
{
}

// -----------------------------------------------------------------
// Name : ~TextureEngine
//  Destructor
// -----------------------------------------------------------------
TextureEngine::~TextureEngine()
{
    for (u16 i = 0; i < m_AllTextures.size(); i++) {
        delete m_AllTextures[i];
    }
    m_AllTextures.clear();
}

// -----------------------------------------------------------------
// Name : loadTexture
// -----------------------------------------------------------------
Texture * TextureEngine::loadTexture(string sFilename, bool bComposed, bool bMipmap, int ustart, int uend, int vstart, int vend)
{
    if (sFilename == "") {
        return NULL;
    }

    Texture * pTex = findTexture(sFilename);
    if (pTex != NULL) { // Texture already loaded
        return pTex;
    }

    pTex = new Texture(sFilename, bComposed, bMipmap);
    m_AllTextures.push_back(pTex);
    if (!pTex->load()) {
        return pTex;
    } else {
        textureLoaded(pTex);
    }

    pTex->m_bIsLoadedInVideo = true;

    // Set u/v coords
    if (ustart >= 0)
    {
        pTex->m_fU0 = (GLfloat)ustart / (GLfloat)pTex->getWidth();
        pTex->m_fU1 = (GLfloat)uend / (GLfloat)pTex->getWidth();
    }
    else
    {
        pTex->m_fU0 = 0.0f;
        pTex->m_fU1 = 1.0f;
    }
    if (vstart >= 0)
    {
        pTex->m_fV0 = (GLfloat)vstart / (GLfloat)pTex->getHeight();
        pTex->m_fV1 = (GLfloat)vend / (GLfloat)pTex->getHeight();
    }
    else
    {
        pTex->m_fV0 = 0.0f;
        pTex->m_fV1 = 1.0f;
    }
    return pTex;
}

// -----------------------------------------------------------------
// Name : textureLoaded
// -----------------------------------------------------------------
void TextureEngine::textureLoaded(Texture * pTex)
{
    // Register texture in Gl's engine
    glGenTextures(1, &(pTex->m_uGlId));
    glBindTexture(GL_TEXTURE_2D, pTex->m_uGlId);

    // Setup texture filters
    if (pTex->m_bMipMap) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//        glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_ALPHA);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    if (pTex->m_bMipMap) {
        gluBuild2DMipmaps(GL_TEXTURE_2D, pTex->m_iBpp, pTex->getWidth(), pTex->getHeight(), pTex->m_Format, GL_UNSIGNED_BYTE, pTex->m_pTexels);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, pTex->m_iBpp, pTex->getWidth(), pTex->getHeight(), 0, pTex->m_Format, GL_UNSIGNED_BYTE, pTex->m_pTexels);
    }

    pTex->unload();  // We don't need to keep it in main memory
}

// -----------------------------------------------------------------
// Name : findTexture
// -----------------------------------------------------------------
Texture * TextureEngine::findTexture(string sFilename)
{
    if (sFilename ==  "") {
        return NULL;
    }

    for (u16 i = 0; i < m_AllTextures.size(); i++) {
        if (m_AllTextures[i]->m_sFilename == sFilename) {
            return m_AllTextures[i];
        }
    }

    // Not found => might be a virtual texture that we need to create from master texture
    if (sFilename.find(":") != string::npos) {
    	size_t pos = sFilename.find_first_of(':', 0);
    	string rootFile = sFilename.substr(0, pos);
    	string title = sFilename.substr(pos+1);
    	Texture * pMaster = findTexture(rootFile);
    	if (pMaster != NULL) {
    		VirtualTexture * vTex = VirtualTexture::build(pMaster, sFilename, title);
    		if (vTex != NULL) {
    			m_AllTextures.push_back(vTex);
    			return vTex;
    		}
    	}
        _debug->error(string("Texture not found: ") + sFilename);
    }

    return NULL;
}

// -----------------------------------------------------------------
// Name : reloadAllTextures
// -----------------------------------------------------------------
void TextureEngine::reloadAllTextures()
{
    for (u16 i = 0; i < m_AllTextures.size(); i++)
    {
        Texture * pTex = m_AllTextures[i];
        if (pTex->load()) {
        	textureLoaded(pTex);
        }
    }
}
