// -----------------------------------------------------------------
// TEXTURE
// Contains all different classes of textures
// -----------------------------------------------------------------
#include "Texture.h"
#include "../Managers/DebugManager.h"
#include <stdio.h>

// -----------------------------------------------------------------
// Name : Texture
//  Constructor
// -----------------------------------------------------------------
Texture::Texture(string sFilename, bool bComposed, bool bMipMap)
{
	m_pComposedDescriptor = NULL;
    m_sFilename = sFilename;
    m_pTexels = NULL;
    m_bIsLoadedInVideo = false;
    m_bMipMap = bMipMap;
    m_Format = GL_RGB;
    m_iBpp = 3;

    if (bComposed) {
        // Build file name
        string jsonDesc = string(GAME_TEXTURES_PATH) + sFilename + ".json";
        string err;
        m_pComposedDescriptor = JoSon::fromFile(jsonDesc, &err);
        if (m_pComposedDescriptor == NULL) {
        	_debug->error(err);
        }
    }
}

// -----------------------------------------------------------------
// Name : ~Texture
//  Destructor
// -----------------------------------------------------------------
Texture::~Texture()
{
    if (m_bIsLoadedInVideo) {
        glDeleteTextures(1, &m_uGlId);
    }
    unload();
    FREE(m_pComposedDescriptor);
}

// -----------------------------------------------------------------
// Name : load
//  Load PNG files
// -----------------------------------------------------------------
bool Texture::load()
{
    unload();
    string sFilePath = string(GAME_TEXTURES_PATH) + m_sFilename + ".png";

    // Use C-style files here for PNG is C lib
    FILE * f = NULL;
    errno_t err = fopen_s(&f, sFilePath.c_str(), "rb");
    if (err != 0) {
    	_debug->error("Can't open texture for reading: " + sFilePath);
    	return false;
    }

    png_byte magic[8];
    // Ensure we've got a PNG file
    fread(magic, 1, sizeof(magic), f);
    if (!png_check_sig(magic, sizeof(magic))) {
    	fclose(f);
    	_debug->error("Not a valid PNG: " + sFilePath);
    	return false;
    }

    // Create PNG structures
    png_structp lpPng = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!lpPng) {
    	fclose(f);
    	_debug->error("Can't create PNG structure: " + sFilePath);
    	return false;
    }
    png_infop lpPngInfo = png_create_info_struct(lpPng);
    if (!lpPngInfo)
    {
    	fclose(f);
        png_destroy_read_struct(&lpPng, NULL, NULL);
    	_debug->error("Can't create PNG info structure: " + sFilePath);
    	return false;
    }

    // Set PNG's exception function
    if (setjmp(png_jmpbuf(lpPng)))
    {
    	fclose(f);
        png_destroy_read_struct(&lpPng, &lpPngInfo, NULL);
        unload();
    	_debug->error("PNG read exception: " + sFilePath);
    	return false;
    }

    // Start reading infos
    png_init_io(lpPng, f);
    png_set_sig_bytes(lpPng, sizeof(magic));
    png_read_info(lpPng, lpPngInfo);

    // Do some changes in PNG to comply with GL
    int iBitDepth = png_get_bit_depth(lpPng, lpPngInfo);
    int iColorType = png_get_color_type(lpPng, lpPngInfo);
    // Convert index color images to RGB images
    if (iColorType == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(lpPng);
    }
    // Convert 1-2-4 bits grayscale images to 8 bits grayscale.
    if (iColorType == PNG_COLOR_TYPE_GRAY && iBitDepth < 8) {
        png_set_gray_1_2_4_to_8(lpPng);
    }
    if (png_get_valid(lpPng, lpPngInfo, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(lpPng);
    }
    if (iBitDepth == 16) {
        png_set_strip_16(lpPng);
    } else if (iBitDepth < 8) {
        png_set_packing(lpPng);
    }
    png_read_update_info(lpPng, lpPngInfo); // Validate changes

    // Read width and height
    png_uint_32 w, h;
    png_get_IHDR(lpPng, lpPngInfo, &w, &h, &iBitDepth, &iColorType, NULL, NULL, NULL);
    m_iWidth = (GLuint)w;
    m_iHeight = (GLuint)h;

    // Read format
    switch (iColorType)
    {
    case PNG_COLOR_TYPE_GRAY:
        m_Format = GL_LUMINANCE;
        m_iBpp = 1;
        break;
    case PNG_COLOR_TYPE_GRAY_ALPHA:
        m_Format = GL_LUMINANCE_ALPHA;
        m_iBpp = 2;
        break;
    case PNG_COLOR_TYPE_RGB:
        m_Format = GL_RGB;
        m_iBpp = 3;
        break;
    case PNG_COLOR_TYPE_RGB_ALPHA:
        m_Format = GL_RGBA;
        m_iBpp = 4;
        break;
    default:
    {
    	fclose(f);
        png_destroy_read_struct(&lpPng, &lpPngInfo, NULL);
        unload();
        ostringstream os;
        os << "Unknown PNG color type " << iColorType << ": " << sFilePath;
    	_debug->error(os.str());
    	return false;
    }
    }

    // Read pixel data
    m_pTexels = new GLubyte[m_iWidth * m_iHeight * m_iBpp];
    png_bytep * lpRows = new png_bytep[m_iHeight];
    for (int i = 0; i < (int)m_iHeight; ++i)
    {
        // Make all rows pointing to m_pTexels, with vertical flip due to different coordinate systems between PNG and GL
        //  WITHOUT FLIP (m_iHeight - (i + 1))
        lpRows[i] = (png_bytep)(m_pTexels + (i * m_iWidth * m_iBpp));
    }
    png_read_image(lpPng, lpRows);

    // Clean up
    delete[] lpRows;
    png_read_end(lpPng, NULL);
    png_destroy_read_struct(&lpPng, &lpPngInfo, NULL);
    fclose(f);

    return true;
}

// -----------------------------------------------------------------
// Name : unload
// -----------------------------------------------------------------
void Texture::unload()
{
    if (m_pTexels != NULL)
    {
        delete[] m_pTexels;
        m_pTexels = NULL;
    }
}
