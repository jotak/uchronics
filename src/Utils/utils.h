#ifndef _UTILS_H
#define _UTILS_H

#include "../_Linux/SystemHeaders.h"

using namespace std;

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned long   u32;
typedef char            s8;
typedef short           s16;
typedef long            s32;

#include <algorithm>
#include <queue>
#include <list>
#include <tr1/unordered_map>
#include <tr1/memory>
#include <tr1/functional>
#include <tuple>

// TODO: remove this
#define max(x,y) ((x)>=(y)?(x):(y))
#define min(x,y) ((x)<=(y)?(x):(y))
extern errno_t fopen_s(FILE ** pFile, const char * sFilename, const char * sMode);

typedef tr1::unordered_map<string, long> long_hash;
typedef tr1::unordered_map<string, string> str_hash;
typedef tr1::unordered_map<char, char> ch_hash;

#define FREE(m)               	{ if (m != NULL) { delete m; m = NULL; } }
#define FREEARR(m)            	{ if (m != NULL) { delete[] m; m = NULL; } }
#define FREEVEC(v)            	{ while(!v.empty()) delete v.back(), v.pop_back(); }
#define FREESTACK(v)           { while(!v.empty()) delete v.top(), v.pop(); }
#define EPSILON               	0.000001f
#define PI                    	3.141592f
#define FRAND100(max)			(((double)(rand()%100))*(double)(max)/100.0f)

#include "paths.h"
#include "Coords.h"
#include "Color.h"
#include "DisplayMode.h"

extern void chop(char * str);
extern void add_long_to_wstr(long iVal, int precision, char * sDst, int * iDst);
extern void add_double_to_wstr(double fVal, int precision, char * sDst, int * iDst);
extern void wsafecpy(char * dst, unsigned int size, const char * src);
extern void wsafecat(char * dst, unsigned int size, const char * src);

// The following is defined in platform-specific cpp files
extern bool md5folder(const char * sFolder, char * sDigest);
extern bool copyStringToClipboard(const char * wsource);
extern char * getStringFromClipboard(char * sBuffer, int iBufSize);
extern int getEditions(char ** sEditionsList, unsigned int iListSize, int iEditionNameSize);
extern int getSkills(char ** sSkillsList, unsigned int iListSize, int iSkillNameSize, const char * sEdition);
extern int getProfiles(char ** sProfilesList, unsigned int iListSize, int iProfileNameSize);
extern int getSavedGames(char ** sSavesList, unsigned int iListSize, int iSavesNameSize);
extern int getMaps(char ** sMapsList, unsigned int iListSize, int iMapNameSize);
extern bool getAvailableDisplayModes(list<DisplayMode>*);

template <typename From, typename To>
struct static_caster
{
    To operator()(From p) { return static_cast<To>(p); }
};

#endif
