#ifndef _JSON_UTIL_H
#define _JSON_UTIL_H

#include "../Utils/utils.h"
#include <JoSon.h>

class JSonUtil
{
public:
	static double getDouble(const JoS_Element&, double=0.0f);
	static string getString(const JoS_Element&, string="");
	static int getInt(const JoS_Element&, int=0);
	static int getCappedInt(const JoS_Element&, int, int, int=0);
};

#endif
