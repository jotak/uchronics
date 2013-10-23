// -----------------------------------------------------------------
// Some utils functions...
// -----------------------------------------------------------------
#include "utils.h"

const Color Color::white(1,1,1,1);
const Color Color::red(1,0,0,1);
const Color Color::green(0,1,0,1);
const Color Color::blue(0,0,1,1);
const Color Color::black(0,0,0,1);

void wchop(char * str)
{
    int len = (int) strlen(str) - 1;
    while (len >= 0 && (str[len] == '\n' || str[len] == '\r' || str[len] == '\t' || str[len] == ' '))
    {
        str[len] = '\0';
        len--;
    }
}

void chop(char * str)
{
    int len = (int) strlen(str) - 1;
    while (len >= 0 && (str[len] == '\n' || str[len] == '\r' || str[len] == '\t' || str[len] == ' '))
    {
        str[len] = '\0';
        len--;
    }
}

void add_long_to_wstr(long iVal, int precision, char * sDst, int * iDst)
{
    if (iVal < 0)
    {
        sDst[(*iDst)++] = '-';
        precision--;
        iVal = -iVal;
    }
    char sBuf[128];
    int iBuf = 0;
    do
    {
        u8 digit = (u8) (iVal % 10);
        sBuf[iBuf++] = '0' + digit;
        iVal /= 10;
    }
    while (iVal != 0);
    while (precision-iBuf > 0)
    {
        sDst[(*iDst)++] = '0';
        precision--;
    }
    while (iBuf > 0)
        sDst[(*iDst)++] = sBuf[--iBuf];
}

void add_double_to_wstr(double fVal, int precision, char * sDst, int * iDst)
{
    if (fVal < 0)
    {
        sDst[(*iDst)++] = '-';
        fVal = -fVal;
    }
    char sBuf[128];
    int iBuf = 0;
    long tronc = (long) fVal;
    fVal = fVal - (double) tronc;
    do
    {
        u8 digit = (u8) (tronc % 10);
        sBuf[iBuf++] = '0' + digit;
        tronc /= 10;
    }
    while (tronc != 0);
    while (iBuf > 0)
        sDst[(*iDst)++] = sBuf[--iBuf];
    if ((fVal == 0.0f && precision < 0) || precision == 0)
        return;
    sDst[(*iDst)++] = '.';
    while (fVal != 0.0f)
    {
        fVal *= 10;
        u8 digit = (u8) fVal;
        sDst[(*iDst)++] = '0' + digit;
        fVal = fVal - (double) digit;
        precision--;
        if (precision == 0 || precision == -5)  // 4 decimals by default if precision was -1
            return;
    }
}

void wsafecpy(char * dst, unsigned int size, const char * src)
{
    if (size < strlen(src)+1)
    {
        unsigned int i = 0;
        while (i < size - 4)
        {
            dst[i] = src[i];
            i++;
        }
        while (i < size - 1)
            dst[i++] = '.';
        dst[i] = '\0';
    }
    else
        strncpy(dst, src, size);
}

void wsafecat(char * dst, unsigned int size, const char * src)
{
    unsigned int idest = strlen(dst);
    if (size < idest + strlen(src)+1)
    {
        unsigned int i = 0;
        while (idest < size - 4)
            dst[idest++] = src[i++];
        while (idest < size - 1)
            dst[idest++] = '.';
        dst[idest] = '\0';
    }
    else
        strncat(dst, src, size);
}

f3d sin(const f3d &d3) {
	return f3d(sin(d3.x), sin(d3.y), sin(d3.z));
}
f3d cos(const f3d &d3) {
	return f3d(cos(d3.x), cos(d3.y), cos(d3.z));
}
f3d tan(const f3d &d3) {
	return f3d(tan(d3.x), tan(d3.y), tan(d3.z));
}
f3d atan(const f3d &d3) {
	return f3d(atan(d3.x), atan(d3.y), atan(d3.z));
}
f3d sqrt(const f3d &d3) {
	return f3d(sqrt(d3.x), sqrt(d3.y), sqrt(d3.z));
}

f3d closestPoint(f3d &p, f3d &a, f3d &b, bool segment) {
    f3d ap = p - a;
    f3d ab = b - a;
    double ab2 = ab.x*ab.x + ab.y*ab.y;
    double ap_ab = ap.x*ab.x + ap.y*ab.y;
    double t = ap_ab / ab2;
    if (segment) {
         if (t < 0.0f) {
        	 t = 0.0f;
         } else if (t > 1.0f) {
        	 t = 1.0f;
         }
    }
    return a + ab * t;
}
