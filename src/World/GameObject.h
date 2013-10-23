#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include "../Geometries/Geometry.h"
#include "../Physics/PartitionableItem.h"

class GameObject : public PartitionableItem
{
public:
	GameObject();
    virtual ~GameObject();

    friend ostream& operator<< (ostream& stream, const GameObject& obj);
    virtual string toString() const;
    virtual void update(double delta);
    virtual void display();

    virtual bool isCharacter() { return false; };
    virtual bool isAI() { return false; };

    void setPosition(f3d pos);
    f3d getPosition() const { return m_pos; };
    Geometry * getGeometry() { return m_pGeometry; };
    void setGeometry(Geometry * pGeo) { m_pGeometry = pGeo; };
    bool isCloseTo(f3d pos);

private:
    f3d m_pos;
    Geometry * m_pGeometry;
};

#endif
