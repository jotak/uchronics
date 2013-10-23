#ifdef linux
#include "../Utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <dirent.h>
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>

errno_t fopen_s(FILE ** pFile, const char * sFilename, const char * sMode)
{
    *pFile = fopen(sFilename, sMode);
    if (*pFile == NULL) {
        return -1;
    }
    return 0;
}

// type: 0=any, 0x4=folder, 0x8=file
int getDirectoryContent(string dir, vector<string> &files, unsigned char type)
{
    DIR * dp;
    struct dirent * dirp;
    if ((dp  = opendir(dir.c_str())) == NULL)
    {
        return -1;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        if (type == 0 || type == dirp->d_type)
        {
            files.push_back(string(dirp->d_name));
        }
    }
    closedir(dp);
    return 0;
}

char sClipboard[1024];

bool copyStringToClipboard(const char * wsource)
{
    // TODO: real clipboard on linux??
    wsafecpy(sClipboard, 1024, wsource);
    return true;
}

char * getStringFromClipboard(char * sBuffer, int iBufSize)
{
    // TODO: real clipboard on linux??
    wsafecpy(sBuffer, iBufSize, sClipboard);
    return sBuffer;
}

bool getAvailableDisplayModes(list<DisplayMode> * lstDisplayModes)
{
	Display * display;
	XF86VidModeModeInfo ** modes;
	int nbModes;

	display = XOpenDisplay(0);
	int screen = DefaultScreen(display);
	int bpp = XDefaultDepth(display, screen);
	XF86VidModeGetAllModeLines(display, screen, &nbModes, &modes);
	for (int i = 0; i < nbModes; i++) {
		// Check it's not already added
		bool skip = false;
		for (DisplayMode &mode : (*lstDisplayModes)) {
			if (mode.iWidth == modes[i]->hdisplay && mode.iHeight == modes[i]->vdisplay) {
				skip = true;
				break;
			}
		}
		if (!skip) {
			lstDisplayModes->push_back(DisplayMode(modes[i]->hdisplay, modes[i]->vdisplay, bpp));
		}
	}
	XFree(modes);
	return true;
}

#endif
