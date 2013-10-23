#ifndef _WORLD_BUILDER_TEST_H
#define _WORLD_BUILDER_TEST_H

#include "WorldBuilder.h"

class WorldBuilderTest : public WorldBuilder
{
public:
	WorldBuilderTest();
    ~WorldBuilderTest();

    void build(Terrain*, list<GameObject*>*);

private:
    void createAI(string, list<GameObject*>*);
};

#endif
