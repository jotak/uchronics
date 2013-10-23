#ifndef _WORLD_BUILDER_H
#define _WORLD_BUILDER_H

#include "../Utils/utils.h"
#include "Terrain.h"
#include "GameObject.h"

class WorldBuilder
{
public:
	WorldBuilder() {};
    virtual ~WorldBuilder() {};

    virtual void build(Terrain*, list<GameObject*>*) = 0;
};

#endif
