#ifndef _INVENTORY_OBJECT_H
#define _INVENTORY_OBJECT_H

#include "GameObject.h"

class InventoryObject : public GameObject
{
public:
	InventoryObject(int price, int weight, string name)
		{ this->price = price; this->weight = weight; this->name = name; };
    virtual ~InventoryObject() {};

    virtual bool isFood() { return false; };

    int getWeight() { return weight; };
    void setWeight(int weight) { this->weight = weight; };
    int getPrice() { return price; };
    void setPrice(int price) { this->price = price; };
    string getName() { return name; };

private:
    int weight;
    int price;
    string name;
};

#endif
