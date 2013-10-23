#ifndef _FOOD_OBJECT_H
#define _FOOD_OBJECT_H

#include "InventoryObject.h"

class FoodObject : public InventoryObject
{
public:
	FoodObject(int restore, int price, int weight, string name) : InventoryObject(price, weight, name)
		{ setRestore(restore); };
    virtual ~FoodObject() {};

    virtual bool isFood() { return true; };

    int getRestore() { return restore; };
    void setRestore(int restore) { this->restore = restore; };

private:
    int restore;
};

#endif
