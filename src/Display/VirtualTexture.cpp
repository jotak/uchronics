// -----------------------------------------------------------------
// VirtualTexture
// -----------------------------------------------------------------
#include "VirtualTexture.h"
#include "../Managers/DebugManager.h"

// -----------------------------------------------------------------
// Name : VirtualTexture
//  Constructor
// -----------------------------------------------------------------
VirtualTexture::VirtualTexture(Texture * pMaster, string sFilename, int ustart, int uend, int vstart, int vend)
		: Texture(sFilename, false/*composed*/, false/*mipmap*/)
{
    int width = 1 + uend - ustart;
    int height = 1 + vend - vstart;
    m_iWidth = width;
    m_iHeight = height;
    m_fU0 = (0.5f + (float) ustart) / (float) (pMaster->getWidth());
    m_fU1 = (0.5f + (float) uend) / (float) (pMaster->getWidth());
    m_fV0 = (0.5f + (float) vstart) / (float) (pMaster->getHeight());
    m_fV1 = (0.5f + (float) vend) / (float) (pMaster->getHeight());
    m_uGlId = pMaster->getGlid();
    m_Format = pMaster->getFormat();
    m_bIsLoadedInVideo = pMaster->isLoadedInVideo();
}

// -----------------------------------------------------------------
// Name : ~VirtualTexture
//  Destructor
// -----------------------------------------------------------------
VirtualTexture::~VirtualTexture()
{
	m_bIsLoadedInVideo = false;
	m_pTexels = NULL;
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
VirtualTexture * VirtualTexture::build(Texture * pMaster, string sFilename, string sTitle)
{
	const JoS_Element& texCoords = pMaster->getComposedDescriptor()[sTitle];
    int ustart = texCoords[0].toInt();
    int uend = texCoords[1].toInt();
    int vstart = texCoords[2].toInt();
    int vend = texCoords[3].toInt();

    int width = 1 + uend - ustart;
    int height = 1 + vend - vstart;
    if (width <= 0 || height <= 0) {
    	_debug->error(string("Error while building virtual texture ") + sFilename + ": invalid width or height.");
    	return NULL;
    }

    return new VirtualTexture(pMaster, sFilename, ustart, uend, vstart, vend);
}
