#ifndef _DISPLAY_MODE_H
#define _DISPLAY_MODE_H

class DisplayMode
{
public:
	DisplayMode(int w, int h, int d) {
		iWidth = w;
		iHeight = h;
		iBpp = d;
	};
	~DisplayMode() {};

	int iWidth;
	int iHeight;
	int iBpp;
};

#endif
