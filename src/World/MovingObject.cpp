// -----------------------------------------------------------------
// MOVING OBJECT
// -----------------------------------------------------------------
#include "MovingObject.h"

// -----------------------------------------------------------------
// Name : MovingObject
// -----------------------------------------------------------------
MovingObject::MovingObject()
{
	radius = 0;
}

// -----------------------------------------------------------------
// Name : ~MovingObject
// -----------------------------------------------------------------
MovingObject::~MovingObject()
{
	FREEVEC(m_Movements);
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void MovingObject::update(double delta)
{
	f3d oldPos = getPosition();
	f3d newPos = oldPos;
	GameObject::update(delta);
	for (list<Movement*>::iterator it = m_Movements.begin(); it != m_Movements.end(); ++it) {
		newPos = (*it)->apply(delta, newPos);
		if ((*it)->getState() == Finished) {
			delete *it;
			it = m_Movements.erase(it);
		}
	}
	if (oldPos != newPos) {
		setPosition(newPos);
	}
}
