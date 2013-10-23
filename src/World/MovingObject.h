#ifndef _MOVING_OBJECT_H
#define _MOVING_OBJECT_H

#include "GameObject.h"
#include "Collidable.h"
#include "../Physics/Movement.h"

class MovingObject : public GameObject
{
public:
	MovingObject();
    virtual ~MovingObject();

    virtual void update(double delta);

    int getRadius() { return radius; };
    void setRadius(int radius) { this->radius = radius; };

    virtual void collide(Collidable * pCollidable) {};
    void addMovement(Movement * pMvt) { m_Movements.push_back(pMvt); };
    virtual bool isMoving() { return !m_Movements.empty(); };

private:
    int radius;
    list<Movement*> m_Movements;
};

#endif
