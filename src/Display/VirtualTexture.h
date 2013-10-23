#ifndef _VIRTUAL_TEXTURE_H
#define _VIRTUAL_TEXTURE_H

#include "Texture.h"

class VirtualTexture : public Texture
{
public:
    ~VirtualTexture();
    static VirtualTexture * build(Texture * pMaster, string sFilename, string sTitle);

    bool load() { return false; };

private:
    VirtualTexture(Texture * pMaster, string sFilename, int ustart, int uend, int vstart, int vend);
};

#endif
