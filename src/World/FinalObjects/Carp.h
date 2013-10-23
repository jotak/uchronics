#ifndef _CARP_H
#define _CARP_H

#include "../FoodObject.h"

class Carp : public FoodObject
{
public:
	Carp() : FoodObject(12/*restore*/, 5/*price*/, 3/*weight*/, "carp") {};
    virtual ~Carp() {};
};

#endif
