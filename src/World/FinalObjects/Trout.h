#ifndef _TROUT_H
#define _TROUT_H

#include "../FoodObject.h"

class Trout : public FoodObject
{
public:
	Trout() : FoodObject(10/*restore*/, 5/*price*/, 2/*weight*/, "trout") {};
    virtual ~Trout() {};
};

#endif
