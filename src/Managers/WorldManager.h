#ifndef _WORLD_MANAGER_H
#define _WORLD_MANAGER_H

#include "../World/WorldBuilder.h"
#include "../World/Character.h"
#include "../Input/EventListener.h"
#include "../Input/KeyboardListener.h"
#include "../Physics/SpacePart.h"

#define AI_INTERACTION_RADIUS		1.5f

class WorldManager : public EventListener, public KeyboardListener
{
public:
    ~WorldManager();
    static WorldManager * getInstance()
    {
        if (m_pInstance == NULL) m_pInstance = new WorldManager();
        return m_pInstance;
    };

    // Init / Update / Display
    void init(WorldBuilder * pBuilder);
    void update(double delta);
    void display();

    // Input functions
    bool onCatchButtonEvent(ButtonAction * pEvent);
    bool onCursorMoveEvent(int xPxl, int yPxl);
    bool onKeyDown(unsigned char c);
    bool onSpecialKeyDown(int key);
    void pickObject(int xPxl, int yPxl);

    Character * getActiveCharacter() { return m_pActiveCharacter; };
    void setActiveCharacter(Character * c) { m_pActiveCharacter = c; };
    SpacePart * getSpacePartition() { return m_pSpacePartition; };
    Terrain * getTerrain() { return &m_Terrain; };

private:
    WorldManager();
    static WorldManager * m_pInstance;

    Terrain m_Terrain;
    list<GameObject*> m_pGameObjects;
    Character * m_pActiveCharacter;
    GameObject * m_pPickedObject;
    SpacePart * m_pSpacePartition;
};

// Helper alias
#define _world WorldManager::getInstance()

#endif
