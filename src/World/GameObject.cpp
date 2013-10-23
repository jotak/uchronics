// -----------------------------------------------------------------
// GAME OBJECT
// -----------------------------------------------------------------
#include "GameObject.h"
#include "../Display/DisplayEngine.h"
#include "../Managers/WorldManager.h"

#define CLOSE_TO_RADIUS		0.2f

// -----------------------------------------------------------------
// Name : GameObject
// -----------------------------------------------------------------
GameObject::GameObject()
{
	m_pGeometry = NULL;
}

// -----------------------------------------------------------------
// Name : ~GameObject
// -----------------------------------------------------------------
GameObject::~GameObject()
{
	FREE(m_pGeometry);
}

// -----------------------------------------------------------------
// Name : operator<< (~toString)
// -----------------------------------------------------------------
ostream& operator<< (ostream& stream, const GameObject& obj) {
	stream << obj.toString();
	return stream;
}

// -----------------------------------------------------------------
// Name : toString
// -----------------------------------------------------------------
string GameObject::toString() const {
	std::ostringstream oss;
	oss << "x: " << m_pos.x << " | y: " << m_pos.y;
	return oss.str();
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void GameObject::update(double delta)
{
	if (m_pGeometry != NULL) {
		m_pGeometry->update(delta);
	}
}

// -----------------------------------------------------------------
// Name : display
// -----------------------------------------------------------------
void GameObject::display()
{
	if (m_pGeometry != NULL) {
		m_pGeometry->display(m_pos, Color::white);
	}
}

// -----------------------------------------------------------------
// Name : setPosition
// -----------------------------------------------------------------
void GameObject::setPosition(f3d newPos)
{
	if (newPos != m_pos) {
		m_pos = newPos;
		SpacePart * pSpacePartition = _world->getSpacePartition();
		if (pSpacePartition != NULL) {
			pSpacePartition->put(this);
		}
	}
}

// -----------------------------------------------------------------
// Name : isCloseTo
// -----------------------------------------------------------------
bool GameObject::isCloseTo(f3d pos)
{
	return abs(pos.x-m_pos.x) < CLOSE_TO_RADIUS && abs(pos.y-m_pos.y) < CLOSE_TO_RADIUS && abs(pos.z-m_pos.z) < CLOSE_TO_RADIUS;
}
